/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkGrayscaleGrindPeakImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2005/08/23 15:09:02 $
  Version:   $Revision: 1.4 $

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkGrayscaleGrindPeakImageFilter_txx
#define __itkGrayscaleGrindPeakImageFilter_txx

#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkGrayscaleGrindPeakImageFilter.h"
//#include "itkGrayscaleGeodesicDilateImageFilter.h"
#include "itkMinimumMaximumImageCalculator.h"
#include "itkImageRegionExclusionConstIteratorWithIndex.h"
#include "itkImageRegionExclusionIteratorWithIndex.h"
#include "itkReconstructionByDilationImageFilter.h"

namespace itk {

template <class TInputImage, class TOutputImage>
GrayscaleGrindPeakImageFilter<TInputImage, TOutputImage>
::GrayscaleGrindPeakImageFilter()
  : m_NumberOfIterationsUsed( 1 )
{
  m_FullyConnected = false;
}

template <class TInputImage, class TOutputImage>
void 
GrayscaleGrindPeakImageFilter<TInputImage, TOutputImage>
::GenerateInputRequestedRegion()
{
  // call the superclass' implementation of this method
  Superclass::GenerateInputRequestedRegion();
  
  // We need all the input.
  InputImagePointer input = const_cast<InputImageType *>(this->GetInput());
  
  input->SetRequestedRegion( input->GetLargestPossibleRegion() );
}


template <class TInputImage, class TOutputImage>
void 
GrayscaleGrindPeakImageFilter<TInputImage, TOutputImage>
::EnlargeOutputRequestedRegion(DataObject *)
{
  this->GetOutput()
    ->SetRequestedRegion( this->GetOutput()->GetLargestPossibleRegion() );
}


template<class TInputImage, class TOutputImage>
void
GrayscaleGrindPeakImageFilter<TInputImage, TOutputImage>
::GenerateData()
{
  // Allocate the output
  this->AllocateOutputs();
  
  // construct a marker image to manipulate using reconstruction by
  // dilation. the marker image will have the same pixel values as the
  // input image on the boundary of the image and will have the
  // minimum pixel value from the input image for all the pixels in
  // the interior
  //

  // compute the minimum pixel value in the input
  typename MinimumMaximumImageCalculator<TInputImage>::Pointer calculator
    = MinimumMaximumImageCalculator<TInputImage>::New();
  calculator->SetImage( this->GetInput() );
  calculator->ComputeMinimum();

  InputImagePixelType minValue;
  minValue = calculator->GetMinimum();
  
  // allocate a marker image
  InputImagePointer markerPtr = InputImageType::New();
  markerPtr->SetRequestedRegion( this->GetInput()->GetRequestedRegion() );
  markerPtr->SetBufferedRegion( this->GetInput()->GetBufferedRegion() );
  markerPtr
    ->SetLargestPossibleRegion( this->GetInput()->GetLargestPossibleRegion() );
  markerPtr->Allocate();

  // fill the marker image with the maximum value from the input
  markerPtr->FillBuffer( minValue );

  // copy the borders of the input image to the marker image
  //
  ImageRegionExclusionConstIteratorWithIndex<TInputImage>
    inputBoundaryIt( this->GetInput(), this->GetInput()->GetRequestedRegion());
  inputBoundaryIt.SetExclusionRegionToInsetRegion();

  ImageRegionExclusionIteratorWithIndex<TInputImage>
    markerBoundaryIt( markerPtr, this->GetInput()->GetRequestedRegion() );
  markerBoundaryIt.SetExclusionRegionToInsetRegion();

  // copy the boundary pixels
  inputBoundaryIt.GoToBegin();
  markerBoundaryIt.GoToBegin();
  while ( !inputBoundaryIt.IsAtEnd() )
    {
    markerBoundaryIt.Set( inputBoundaryIt.Get() );
    ++markerBoundaryIt;
    ++inputBoundaryIt;
    }
    
  
  // Delegate to a geodesic dilation filter.
  //
  //
  typename ReconstructionByDilationImageFilter<TInputImage, TInputImage>::Pointer
    dilate
       = ReconstructionByDilationImageFilter<TInputImage, TInputImage>::New();

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
GrayscaleGrindPeakImageFilter<TInputImage, TOutputImage>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "Number of iterations used to produce current output: "
     << m_NumberOfIterationsUsed << std::endl;
  os << indent << "FullyConnected: "  << m_FullyConnected << std::endl;
}
  
}// end namespace itk
#endif
