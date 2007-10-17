/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkObjectMorphologyImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2006/03/19 04:36:56 $
  Version:   $Revision: 1.15 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkObjectMorphologyImageFilter_txx
#define __itkObjectMorphologyImageFilter_txx

#include <limits.h>

#include "itkConstantBoundaryCondition.h"
#include "itkNumericTraits.h"
#include "itkObjectMorphologyImageFilter.h"
#include "itkNeighborhoodAlgorithm.h"
#include "itkProgressReporter.h"
#include "itkImageRegionConstIterator.h"

namespace itk {

template<class TInputImage, class TOutputImage, class TKernel>
ObjectMorphologyImageFilter<TInputImage, TOutputImage, TKernel>
::ObjectMorphologyImageFilter()
  : m_Kernel()
{
  m_DefaultBoundaryCondition.SetConstant( NumericTraits<PixelType>::Zero );
  m_BoundaryCondition = &m_DefaultBoundaryCondition;

  m_UseBoundaryCondition = false;

  m_ObjectValue = NumericTraits<PixelType>::One;
  //this->SetNumberOfThreads(1);
}
  
template <class TInputImage, class TOutputImage, class TKernel>
void 
ObjectMorphologyImageFilter<TInputImage, TOutputImage, TKernel>
::GenerateInputRequestedRegion()
{
  // call the superclass' implementation of this method
  Superclass::GenerateInputRequestedRegion();
  
  // get pointers to the input and output
  typename Superclass::InputImagePointer  inputPtr = 
    const_cast< TInputImage * >( this->GetInput() );
  
  if ( !inputPtr )
    {
    return;
    }

  // get a copy of the input requested region (should equal the output
  // requested region)
  typename TInputImage::RegionType inputRequestedRegion;
  inputRequestedRegion = inputPtr->GetRequestedRegion();

  // pad the input requested region by the operator radius
  inputRequestedRegion.PadByRadius( m_Kernel.GetRadius() );

  // crop the input requested region at the input's largest possible region
  if ( inputRequestedRegion.Crop(inputPtr->GetLargestPossibleRegion()) )
    {
    inputPtr->SetRequestedRegion( inputRequestedRegion );
    return;
    }
  else
    {
    // Couldn't crop the region (requested region is outside the largest
    // possible region).  Throw an exception.

    // store what we tried to request (prior to trying to crop)
    inputPtr->SetRequestedRegion( inputRequestedRegion );
    
    // build an exception
    InvalidRequestedRegionError e(__FILE__, __LINE__);
    e.SetLocation(ITK_LOCATION);
    e.SetDescription("Requested region is outside largest possible region.");
    e.SetDataObject(inputPtr);
    throw e;
    }
}

template<class TInputImage, class TOutputImage, class TKernel>
void
ObjectMorphologyImageFilter<TInputImage, TOutputImage, TKernel>
::BeforeThreadedGenerateData()
{
  if(m_ObjectValue == 0)
    {
    this->GetOutput()->FillBuffer(1);
    }
  else
    {
    this->GetOutput()->FillBuffer(0);
    }
}



template<class TInputImage, class TOutputImage, class TKernel>
void
ObjectMorphologyImageFilter<TInputImage, TOutputImage, TKernel>
::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                       int threadId) 
{

  ImageRegionConstIterator<TInputImage> iRegIter;
  ImageRegionIterator<TOutputImage> oRegIter;
  iRegIter = ImageRegionConstIterator<InputImageType>(this->GetInput(),
                                                      outputRegionForThread);
  oRegIter = ImageRegionIterator<OutputImageType>(this->GetOutput(),
                                                  outputRegionForThread);
  /* Copy the input image to the output image - then only boundary pixels
   * need to be changed in the output image */
  iRegIter.GoToBegin();
  oRegIter.GoToBegin();
  while(!oRegIter.IsAtEnd())
    {
    if(oRegIter.Get()!=m_ObjectValue)
      {
       oRegIter.Set(iRegIter.Get());
      }
    ++oRegIter;
    ++iRegIter;
    }

  // Find the boundary "faces"
  typename NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<InputImageType>
    ::FaceListType faceList;
  NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<InputImageType> fC;
  faceList = fC(this->GetInput(), outputRegionForThread, m_Kernel.GetRadius());

  typename NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<InputImageType>
    ::FaceListType::iterator fit;

  // Setup the kernel that spans the immediate neighbors of the current
  // input pixel - used to determine if that pixel abuts a non-object
  // pixel, i.e., is a boundary pixel
  RadiusType bKernelSize;
  bKernelSize.Fill(1);
 
  ProgressReporter progress(this, threadId,
                            outputRegionForThread.GetNumberOfPixels());

  OutputNeighborhoodIteratorType oSNIter;
  InputNeighborhoodIteratorType iSNIter;
  for (fit = faceList.begin(); fit != faceList.end(); ++fit)
    { 
    oSNIter = OutputNeighborhoodIteratorType(m_Kernel.GetRadius(),
                                             this->GetOutput(), *fit);
    oSNIter.OverrideBoundaryCondition(m_BoundaryCondition);
    oSNIter.GoToBegin();

    iSNIter = InputNeighborhoodIteratorType(bKernelSize,
                                            this->GetInput(), *fit);
    iSNIter.OverrideBoundaryCondition(m_BoundaryCondition);
    iSNIter.GoToBegin();
    
    while ( ! iSNIter.IsAtEnd() )
      {
      if (iSNIter.GetCenterPixel() == m_ObjectValue)
        {
        if(this->IsObjectPixelOnBoundary(iSNIter))
          {
          this->Evaluate(oSNIter, m_Kernel);    
          }
        }
      ++iSNIter;
      ++oSNIter;
      progress.CompletedPixel();
      }
    }
}

// Use neighborhood iter to determine if pixel touches a non-object pixel
template<class TInputImage, class TOutputImage, class TKernel>
bool
ObjectMorphologyImageFilter<TInputImage, TOutputImage, TKernel>
::IsObjectPixelOnBoundary(const InputNeighborhoodIteratorType &iNIter)
{

  static const unsigned int s =
    (unsigned int)vcl_pow((double)3.0,
                      (double)(ImageDimension));

  PixelType tf;
  unsigned int i;
  bool isInside = true;
  if(m_UseBoundaryCondition)
    {
    for(i=0; i<s; i++)
      {
      tf = iNIter.GetPixel(i);
      if(tf != m_ObjectValue)
        {
        return true;
        }
      }
    }
  else
    {
    for(i=0; i<s; i++)
      {
      tf = iNIter.GetPixel(i, isInside);
      if(tf != m_ObjectValue && isInside)
        {
        return true;
        }
      }
    }

  return false;
}

template<class TInputImage, class TOutputImage, class TKernel>
void
ObjectMorphologyImageFilter<TInputImage, TOutputImage, TKernel>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "Boundary condition: " 
               << typeid( *m_BoundaryCondition ).name() << std::endl;
  os << indent << "Use boundary condition: " 
               << m_UseBoundaryCondition << std::endl;

  os << indent << "ObjectValue: " << m_ObjectValue << std::endl;
  os << indent << "Kernel: " << m_Kernel << std::endl;
}

}// end namespace itk
#endif
