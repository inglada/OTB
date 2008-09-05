/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkHConcaveImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2006-08-01 19:16:18 $
  Version:   $Revision: 1.8 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkHConcaveImageFilter_txx
#define __itkHConcaveImageFilter_txx

#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkHConcaveImageFilter.h"
#include "itkHMinimaImageFilter.h"
#include "itkSubtractImageFilter.h"

namespace itk {

template <class TInputImage, class TOutputImage>
HConcaveImageFilter<TInputImage, TOutputImage>
::HConcaveImageFilter()
{
  m_Height = 2;
  m_NumberOfIterationsUsed = 1;
  m_FullyConnected = false;
}

template <class TInputImage, class TOutputImage>
void 
HConcaveImageFilter<TInputImage, TOutputImage>
::GenerateInputRequestedRegion()
{
  // call the superclass' implementation of this method
  Superclass::GenerateInputRequestedRegion();
  
  // We need all the input.
  InputImagePointer input = const_cast<InputImageType *>(this->GetInput());
  if( input )
    {
    input->SetRequestedRegion( input->GetLargestPossibleRegion() );
    }
}


template <class TInputImage, class TOutputImage>
void 
HConcaveImageFilter<TInputImage, TOutputImage>
::EnlargeOutputRequestedRegion(DataObject *)
{
  this->GetOutput()
    ->SetRequestedRegion( this->GetOutput()->GetLargestPossibleRegion() );
}


template<class TInputImage, class TOutputImage>
void
HConcaveImageFilter<TInputImage, TOutputImage>
::GenerateData()
{
  // Allocate the output
  this->AllocateOutputs();
  
  // Create a process accumulator for tracking the progress of this minipipeline
  ProgressAccumulator::Pointer progress = ProgressAccumulator::New();
  progress->SetMiniPipelineFilter(this);

  // Delegate to a H-Minima filter.
  //
  //
  typename HMinimaImageFilter<TInputImage, TInputImage>::Pointer
    hmin = HMinimaImageFilter<TInputImage, TInputImage>::New();

  hmin->SetInput( this->GetInput() );
  hmin->SetHeight( m_Height );
  hmin->SetFullyConnected( m_FullyConnected );


  // Need to subtract the input from the H-Minima image
  typename SubtractImageFilter<TInputImage, TInputImage, TOutputImage>::Pointer
    subtract=SubtractImageFilter<TInputImage,TInputImage,TOutputImage>::New();

  subtract->SetInput1( hmin->GetOutput() );
  subtract->SetInput2( this->GetInput() );

  // graft our output to the subtract filter to force the proper regions
  // to be generated
  subtract->GraftOutput( this->GetOutput() );

  // run the algorithm
  progress->RegisterInternalFilter(hmin,.9f);
  progress->RegisterInternalFilter(subtract,.1f);

  subtract->Update();

  // graft the output of the subtract filter back onto this filter's
  // output. this is needed to get the appropriate regions passed
  // back.
  this->GraftOutput( subtract->GetOutput() );
}


template<class TInputImage, class TOutputImage>
void
HConcaveImageFilter<TInputImage, TOutputImage>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "Depth of local minima (contrast): "
     << static_cast<typename NumericTraits<InputImagePixelType>::PrintType>(m_Height)
     << std::endl;
  os << indent << "Number of iterations used to produce current output: "
     << m_NumberOfIterationsUsed << std::endl;
  os << indent << "FullyConnected: "  << m_FullyConnected << std::endl;
}
  
}// end namespace itk
#endif
