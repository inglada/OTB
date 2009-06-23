/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkZeroCrossingBasedEdgeDetectionImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2008-10-07 17:31:02 $
  Version:   $Revision: 1.16 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkZeroCrossingBasedEdgeDetectionImageFilter_txx
#define __itkZeroCrossingBasedEdgeDetectionImageFilter_txx

#include "itkZeroCrossingBasedEdgeDetectionImageFilter.h"
#include "itkDiscreteGaussianImageFilter.h"
#include "itkLaplacianImageFilter.h"
#include "itkZeroCrossingImageFilter.h"
#include "itkProgressAccumulator.h"

namespace itk
{
template< class TInputImage, class TOutputImage >
void
ZeroCrossingBasedEdgeDetectionImageFilter< TInputImage, TOutputImage >
::GenerateData( )
{
  typename  InputImageType::ConstPointer input  = this->GetInput();
  
  // Create the filters that are needed
  typename DiscreteGaussianImageFilter<TInputImage, TOutputImage>::Pointer
    gaussianFilter
    = DiscreteGaussianImageFilter<TInputImage, TOutputImage>::New();
  typename LaplacianImageFilter<TInputImage, TOutputImage>::Pointer  laplacianFilter 
    =  LaplacianImageFilter<TInputImage, TOutputImage>::New();
  typename ZeroCrossingImageFilter<TInputImage, TOutputImage>:: Pointer
    zerocrossingFilter =
    ZeroCrossingImageFilter<TInputImage,TOutputImage>::New(); 
  
  // Create a process accumulator for tracking the progress of this minipipeline
  ProgressAccumulator::Pointer progress = ProgressAccumulator::New();
  progress->SetMiniPipelineFilter(this);

  //Construct the mini-pipeline
  
  // Apply the Gaussian filter
  gaussianFilter->SetVariance(m_Variance);
  gaussianFilter->SetMaximumError(m_MaximumError);
  gaussianFilter->SetInput(input);
  progress->RegisterInternalFilter(gaussianFilter, 1.0f/3.0f);

  // Calculate the laplacian of the smoothed image
  laplacianFilter->SetInput(gaussianFilter->GetOutput());
  progress->RegisterInternalFilter(laplacianFilter, 1.0f/3.0f);

  // Find the zero-crossings of the laplacian
  zerocrossingFilter->SetInput(laplacianFilter->GetOutput());
  zerocrossingFilter->SetBackgroundValue( m_BackgroundValue );
  zerocrossingFilter->SetForegroundValue( m_ForegroundValue );
  zerocrossingFilter->GraftOutput( this->GetOutput() );
  progress->RegisterInternalFilter(zerocrossingFilter, 1.0f/3.0f);
  zerocrossingFilter->Update();

  // Graft the output of the mini-pipeline back onto the filter's output.
  // This action copies back the region ivars and meta-data 
  this->GraftOutput(zerocrossingFilter->GetOutput());
}

template< class TInputImage, class TOutputImage >
void
ZeroCrossingBasedEdgeDetectionImageFilter< TInputImage, TOutputImage >
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);
  os << indent << "Variance: " << m_Variance << std::endl;
  os << indent << "MaximumError: " << m_MaximumError << std::endl;
  os << indent << "ForegroundValue: "
     << static_cast<typename NumericTraits<OutputImagePixelType>::PrintType>(m_ForegroundValue)
     << std::endl;
  os << indent << "BackgroundValue: "
     << static_cast<typename NumericTraits<OutputImagePixelType>::PrintType>(m_BackgroundValue)
     << std::endl;
}

}//end of itk namespace

#endif
