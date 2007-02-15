/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkGradientImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2006/01/11 19:43:31 $
  Version:   $Revision: 1.21 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itkGradientImageFilter_txx
#define _itkGradientImageFilter_txx
#include "itkGradientImageFilter.h"

#include "itkConstNeighborhoodIterator.h"
#include "itkNeighborhoodInnerProduct.h"
#include "itkImageRegionIterator.h"
#include "itkDerivativeOperator.h"
#include "itkNeighborhoodAlgorithm.h"
#include "itkZeroFluxNeumannBoundaryCondition.h"
#include "itkOffset.h"
#include "itkProgressReporter.h"

namespace itk
{

template <class TInputImage, class TOperatorValueType, class TOutputValueType>
void 
GradientImageFilter<TInputImage, TOperatorValueType, TOutputValueType>
::GenerateInputRequestedRegion() throw (InvalidRequestedRegionError)
{
  // call the superclass' implementation of this method
  Superclass::GenerateInputRequestedRegion();
  
  // get pointers to the input and output
  InputImagePointer  inputPtr = 
    const_cast< InputImageType * >( this->GetInput());
  OutputImagePointer outputPtr = this->GetOutput();
  
  if ( !inputPtr || !outputPtr )
    {
    return;
    }

  // Build an operator so that we can determine the kernel size
  DerivativeOperator<OperatorValueType, InputImageDimension> oper;
  oper.SetDirection(0);
  oper.SetOrder(1);
  oper.CreateDirectional();
  unsigned long radius = oper.GetRadius()[0];
  
  // get a copy of the input requested region (should equal the output
  // requested region)
  typename TInputImage::RegionType inputRequestedRegion;
  inputRequestedRegion = inputPtr->GetRequestedRegion();

  // pad the input requested region by the operator radius
  inputRequestedRegion.PadByRadius( radius );

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
    e.SetDescription("Requested region is (at least partially) outside the largest possible region.");
    e.SetDataObject(inputPtr);
    throw e;
    }
}


template< class TInputImage, class TOperatorValueType, class TOutputValueType>
void
GradientImageFilter< TInputImage, TOperatorValueType, TOutputValueType >
::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                       int threadId)
{
  unsigned int i;
  OutputPixelType a;
  ZeroFluxNeumannBoundaryCondition<InputImageType> nbc;

  ConstNeighborhoodIterator<InputImageType> nit;
  ImageRegionIterator<OutputImageType> it;

  NeighborhoodInnerProduct<InputImageType, OperatorValueType,
    OutputValueType> SIP;

  // Get the input and output
  typename OutputImageType::Pointer       output = this->GetOutput();
  typename  InputImageType::ConstPointer  input  = this->GetInput();
  
  // Set up operators
  DerivativeOperator<OperatorValueType,InputImageDimension> op[InputImageDimension];

  for (i = 0; i< InputImageDimension; i++)
    {
    op[i].SetDirection(0);
    op[i].SetOrder(1);
    op[i].CreateDirectional();
    
    // Reverse order of coefficients for the convolution with the image to
    // follow.
    op[i].FlipAxes();

    // Take into account the pixel spacing if necessary
    if (m_UseImageSpacing == true)
      {
      if ( this->GetInput()->GetSpacing()[i] == 0.0 )
        {
        itkExceptionMacro(<< "Image spacing cannot be zero.");
        }
      else
        {
        op[i].ScaleCoefficients( 1.0 / this->GetInput()->GetSpacing()[i] );
        }
      }
    }
  
  // Calculate iterator radius
  Size<InputImageDimension> radius;
  for (i = 0; i < InputImageDimension; ++i)
    {
    radius[i]  = op[0].GetRadius()[0];
    }

  // Find the data-set boundary "faces"
  typename NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<InputImageType>::FaceListType faceList;
  NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<InputImageType> bC;
  faceList = bC(input, outputRegionForThread, radius);

  typename NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<InputImageType>::FaceListType::iterator fit;
  fit = faceList.begin();

  // support progress methods/callbacks
  ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());
  
  // Initialize the x_slice array
  nit = ConstNeighborhoodIterator<InputImageType>(radius, input, *fit);

  std::slice x_slice[InputImageDimension];
  const unsigned long center = nit.Size() / 2;
  for (i = 0; i < InputImageDimension; ++i)
    {
    x_slice[i] = std::slice( center - nit.GetStride(i) * radius[i],
                             op[i].GetSize()[0], nit.GetStride(i));
    }


  // Process non-boundary face and then each of the boundary faces.  
  // These are N-d regions which border the edge of the buffer.
  for (fit=faceList.begin(); fit != faceList.end(); ++fit)
    { 
    nit = ConstNeighborhoodIterator<InputImageType>(radius,
                                                    input, *fit);
    it = ImageRegionIterator<OutputImageType>(output, *fit);
    nit.OverrideBoundaryCondition(&nbc);
    nit.GoToBegin();
    
    while ( ! nit.IsAtEnd() )
      {
      for (i = 0; i < InputImageDimension; ++i)
        {
        a[i] = SIP(x_slice[i], nit, op[i]);
        }
      it.Value() = a;          
      ++nit;
      ++it;
      progress.CompletedPixel();
      }
    }
}

/**
 * Standard "PrintSelf" method
 */
template< class TInputImage, class TOperatorValueType, class TOutputValueType>
void
GradientImageFilter< TInputImage, TOperatorValueType, TOutputValueType >
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf( os, indent );

  os << indent << "UseImageSpacing: " 
     << (m_UseImageSpacing ? "On" : "Off") << std::endl;
}

} // end namespace itk


#endif
