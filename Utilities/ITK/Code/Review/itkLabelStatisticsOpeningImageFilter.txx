/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkLabelStatisticsOpeningImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2009-08-07 12:26:30 $
  Version:   $Revision: 1.1 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkLabelStatisticsOpeningImageFilter_txx
#define __itkLabelStatisticsOpeningImageFilter_txx

#include "itkLabelStatisticsOpeningImageFilter.h"
#include "itkProgressAccumulator.h"


namespace itk {

template<class TInputImage, class TFeatureImage>
LabelStatisticsOpeningImageFilter<TInputImage, TFeatureImage>
::LabelStatisticsOpeningImageFilter()
{
  m_BackgroundValue = NumericTraits<OutputImagePixelType>::NonpositiveMin();
  m_Lambda = NumericTraits< double >::Zero;
  m_ReverseOrdering = false;
  m_Attribute = LabelObjectType::MEAN;
  this->SetNumberOfRequiredInputs(2);
}

template<class TInputImage, class TFeatureImage>
void 
LabelStatisticsOpeningImageFilter<TInputImage, TFeatureImage>
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


template<class TInputImage, class TFeatureImage>
void 
LabelStatisticsOpeningImageFilter<TInputImage, TFeatureImage>
::EnlargeOutputRequestedRegion(DataObject *)
{
  this->GetOutput()
    ->SetRequestedRegion( this->GetOutput()->GetLargestPossibleRegion() );
}


template<class TInputImage, class TFeatureImage>
void
LabelStatisticsOpeningImageFilter<TInputImage, TFeatureImage>
::GenerateData()
{
  // Create a process accumulator for tracking the progress of this minipipeline
  ProgressAccumulator::Pointer progress = ProgressAccumulator::New();
  progress->SetMiniPipelineFilter(this);

  // Allocate the output
  this->AllocateOutputs();
  
  typename LabelizerType::Pointer labelizer = LabelizerType::New();
  labelizer->SetInput( this->GetInput() );
  labelizer->SetBackgroundValue( m_BackgroundValue );
  labelizer->SetNumberOfThreads( this->GetNumberOfThreads() );
  progress->RegisterInternalFilter(labelizer, .3f);
  
  typename LabelObjectValuatorType::Pointer valuator = LabelObjectValuatorType::New();
  valuator->SetInput( labelizer->GetOutput() );
  valuator->SetFeatureImage( this->GetFeatureImage() );
  valuator->SetLabelImage( this->GetInput() );
  valuator->SetNumberOfThreads( this->GetNumberOfThreads() );
  valuator->SetComputeHistogram( false );
  if( m_Attribute == LabelObjectType::PERIMETER || m_Attribute == LabelObjectType::ROUNDNESS )
    {
    valuator->SetComputePerimeter( true );
    }
  if( m_Attribute == LabelObjectType::FERET_DIAMETER )
    {
    valuator->SetComputeFeretDiameter( true );
    }
  progress->RegisterInternalFilter(valuator, .3f);
  
  typename OpeningType::Pointer opening = OpeningType::New();
  opening->SetInput( valuator->GetOutput() );
  opening->SetLambda( m_Lambda );
  opening->SetReverseOrdering( m_ReverseOrdering );
  opening->SetAttribute( m_Attribute );
  opening->SetNumberOfThreads( this->GetNumberOfThreads() );
  progress->RegisterInternalFilter(opening, .2f);
  
  typename BinarizerType::Pointer binarizer = BinarizerType::New();
  binarizer->SetInput( opening->GetOutput() );
  binarizer->SetNumberOfThreads( this->GetNumberOfThreads() );
  progress->RegisterInternalFilter(binarizer, .2f);  

  binarizer->GraftOutput( this->GetOutput() );
  binarizer->Update();
  this->GraftOutput( binarizer->GetOutput() );
}


template<class TInputImage, class TFeatureImage>
void
LabelStatisticsOpeningImageFilter<TInputImage, TFeatureImage>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "BackgroundValue: "  << static_cast<typename NumericTraits<OutputImagePixelType>::PrintType>(m_BackgroundValue) << std::endl;
  os << indent << "Lambda: "  << m_Lambda << std::endl;
  os << indent << "ReverseOrdering: "  << m_ReverseOrdering << std::endl;
  os << indent << "Attribute: "  << LabelObjectType::GetNameFromAttribute(m_Attribute) << " (" << m_Attribute << ")" << std::endl;
}
  
}// end namespace itk
#endif
