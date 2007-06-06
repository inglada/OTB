/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkGrayscaleConnectedOpeningImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2006/12/15 21:40:38 $
  Version:   $Revision: 1.12 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkGrayscaleConnectedOpeningImageFilter_txx
#define __itkGrayscaleConnectedOpeningImageFilter_txx

#include "itkNumericTraits.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkGrayscaleConnectedOpeningImageFilter.h"
#include "itkReconstructionByDilationImageFilter.h"
#include "itkMinimumMaximumImageCalculator.h"
#include "itkProgressAccumulator.h"

namespace itk {

template <class TInputImage, class TOutputImage>
GrayscaleConnectedOpeningImageFilter<TInputImage, TOutputImage>
::GrayscaleConnectedOpeningImageFilter()
  : m_NumberOfIterationsUsed( 1 )
{
  m_Seed.Fill( NumericTraits<ITK_TYPENAME InputImageIndexType::OffsetValueType>::Zero );
  m_FullyConnected = false;
}

template <class TInputImage, class TOutputImage>
void 
GrayscaleConnectedOpeningImageFilter<TInputImage, TOutputImage>
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
GrayscaleConnectedOpeningImageFilter<TInputImage, TOutputImage>
::EnlargeOutputRequestedRegion(DataObject *)
{
  this->GetOutput()
    ->SetRequestedRegion( this->GetOutput()->GetLargestPossibleRegion() );
}


template<class TInputImage, class TOutputImage>
void
GrayscaleConnectedOpeningImageFilter<TInputImage, TOutputImage>
::GenerateData()
{
  // Allocate the output
  this->AllocateOutputs();
  
  // construct a marker image to manipulate using reconstruction by
  // dilation. the marker image will have the same pixel values as the
  // input image at the seed pixel and will have a minimum everywhere
  // else. 
  //

  // compute the minimum pixel value in the input
  typename MinimumMaximumImageCalculator<TInputImage>::Pointer calculator
    = MinimumMaximumImageCalculator<TInputImage>::New();
  calculator->SetImage( this->GetInput() );
  calculator->ComputeMinimum();

  InputImagePixelType minValue;
  minValue = calculator->GetMinimum();

  // compare this minimum value to the value at the seed pixel.
  InputImagePixelType seedValue;
  seedValue = this->GetInput()->GetPixel( m_Seed );

  if (minValue == seedValue)
    {
    itkWarningMacro(<< "GrayscaleConnectedClosingImageFilter: pixel value at seed point matches minimum value in image.  Resulting image will have a constant value.");
    this->GetOutput()->FillBuffer( minValue );
    return;
    }
  
  // allocate a marker image
  InputImagePointer markerPtr = InputImageType::New();
  markerPtr->SetRegions( this->GetInput()->GetRequestedRegion() );
  markerPtr->CopyInformation( this->GetInput() );
  markerPtr->Allocate();

  // fill the marker image with the maximum value from the input
  markerPtr->FillBuffer( minValue );

  // mark the seed point
  markerPtr->SetPixel( m_Seed, seedValue );

  // Delegate to a geodesic dilation filter.
  //
  //
  typename ReconstructionByDilationImageFilter<TInputImage, TInputImage>::Pointer
    dilate
    = ReconstructionByDilationImageFilter<TInputImage, TInputImage>::New();

  // Create a process accumulator for tracking the progress of this minipipeline
  ProgressAccumulator::Pointer progress = ProgressAccumulator::New();
  progress->SetMiniPipelineFilter(this);
  progress->RegisterInternalFilter(dilate,1.0f);

  // set up the dilate filter
  //dilate->RunOneIterationOff();             // run to convergence
  dilate->SetMarkerImage( markerPtr );
  dilate->SetMaskImage( this->GetInput() );
  dilate->SetFullyConnected( m_FullyConnected );

  // graft our output to the dilate filter to force the proper regions
  // to be generated
  dilate->GraftOutput( this->GetOutput() );

  // reconstruction by dilation
  dilate->Update();

  // graft the output of the dilate filter back onto this filter's
  // output. this is needed to get the appropriate regions passed
  // back.
  this->GraftOutput( dilate->GetOutput() );

}


template<class TInputImage, class TOutputImage>
void
GrayscaleConnectedOpeningImageFilter<TInputImage, TOutputImage>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "Seed point: " << m_Seed << std::endl;
  os << indent << "Number of iterations used to produce current output: "
     << m_NumberOfIterationsUsed << std::endl;
  os << indent << "FullyConnected: "  << m_FullyConnected << std::endl;
}
  
}// end namespace itk
#endif
