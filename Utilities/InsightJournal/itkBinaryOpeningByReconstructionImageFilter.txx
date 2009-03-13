/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkBinaryOpeningByReconstructionImageFilter.txx,v $
  Language:  C++

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkBinaryOpeningByReconstructionImageFilter_txx
#define __itkBinaryOpeningByReconstructionImageFilter_txx

#include "itkBinaryOpeningByReconstructionImageFilter.h"
#include "itkBinaryErodeImageFilter.h"
#include "itkBinaryReconstructionByDilationImageFilter.h"
#include "itkProgressAccumulator.h"

namespace itk {

template<class TInputImage, class TKernel>
BinaryOpeningByReconstructionImageFilter<TInputImage, TKernel>
::BinaryOpeningByReconstructionImageFilter()
  : m_Kernel()
{
  m_ForegroundValue = NumericTraits<PixelType>::max();
  m_BackgroundValue = NumericTraits<PixelType>::Zero;
  m_FullyConnected = false;
}

template <class TInputImage, class TKernel>
void 
BinaryOpeningByReconstructionImageFilter<TInputImage, TKernel>
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

template<class TInputImage, class TKernel>
void
BinaryOpeningByReconstructionImageFilter<TInputImage, TKernel>
::GenerateData()
{
  // Allocate the outputs
  this->AllocateOutputs();

  /** set up erosion and dilation methods */
  typename BinaryErodeImageFilter<InputImageType, OutputImageType, TKernel>::Pointer
    erode = BinaryErodeImageFilter<InputImageType, OutputImageType, TKernel>::New();
  erode->SetErodeValue( m_ForegroundValue );
  erode->SetBackgroundValue( m_BackgroundValue );
  erode->SetKernel( this->GetKernel() );
  erode->SetInput( this->GetInput() );
  erode->ReleaseDataFlagOn();
  erode->SetNumberOfThreads( this->GetNumberOfThreads() );

  typename BinaryReconstructionByDilationImageFilter<OutputImageType>::Pointer
    dilate = BinaryReconstructionByDilationImageFilter<OutputImageType>::New();
  dilate->SetForegroundValue( m_ForegroundValue );
  dilate->SetBackgroundValue( m_BackgroundValue );
  dilate->SetMarkerImage( erode->GetOutput() );
  dilate->SetMaskImage( this->GetInput() );
  dilate->SetFullyConnected( m_FullyConnected );
  dilate->ReleaseDataFlagOn();
  dilate->SetNumberOfThreads( this->GetNumberOfThreads() );
    
  /** set up the minipipeline */
  ProgressAccumulator::Pointer progress = ProgressAccumulator::New();
  progress->SetMiniPipelineFilter(this);
  progress->RegisterInternalFilter(erode, .8f);
  progress->RegisterInternalFilter(dilate, .2f);

  /** execute the minipipeline */
  dilate->GraftOutput( this->GetOutput() );
  dilate->Update();
  this->GraftOutput( dilate->GetOutput() );
}

template<class TInputImage, class TKernel>
void
BinaryOpeningByReconstructionImageFilter<TInputImage, TKernel>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "Kernel: " << m_Kernel << std::endl;
  os << indent << "ForegroundValue: " << static_cast<typename NumericTraits<PixelType>::PrintType>(m_ForegroundValue) << std::endl;
  os << indent << "BackgroundValue: " << static_cast<typename NumericTraits<PixelType>::PrintType>(m_BackgroundValue) << std::endl;
  os << indent << "FullyConnected: "  << m_FullyConnected << std::endl;
}

}// end namespace itk
#endif
