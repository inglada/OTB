/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkNormalizeImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2008-10-16 19:33:45 $
  Version:   $Revision: 1.15 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkNormalizeImageFilter_txx
#define __itkNormalizeImageFilter_txx

#include "itkNormalizeImageFilter.h"
#include "itkImageRegionIterator.h"
#include "itkStatisticsImageFilter.h"
#include "itkShiftScaleImageFilter.h"
#include "itkProgressAccumulator.h"
#include "itkCommand.h"

namespace itk
{

template <class TInputImage, class TOutputImage>
NormalizeImageFilter<TInputImage, TOutputImage>
::NormalizeImageFilter()
{
  m_StatisticsFilter = 0;
  m_StatisticsFilter = StatisticsImageFilter<TInputImage>::New();
  m_ShiftScaleFilter = ShiftScaleImageFilter<TInputImage,TOutputImage>::New();
}

template <class TInputImage, class TOutputImage>
void
NormalizeImageFilter<TInputImage, TOutputImage>
::GenerateInputRequestedRegion()
{
  Superclass::GenerateInputRequestedRegion();
  if ( this->GetInput() )
    {
    InputImagePointer image =
      const_cast< typename Superclass::InputImageType * >( this->GetInput() );
    image->SetRequestedRegionToLargestPossibleRegion();
    }
}

template <class TInputImage, class TOutputImage>
void 
NormalizeImageFilter<TInputImage, TOutputImage>
::Modified() const
{
  Superclass::Modified();
  m_StatisticsFilter->Modified();
  m_ShiftScaleFilter->Modified();
}

template <class TInputImage, class TOutputImage>
void 
NormalizeImageFilter<TInputImage, TOutputImage>
::GenerateData()
{
  ProgressAccumulator::Pointer progress = ProgressAccumulator::New();
  progress->SetMiniPipelineFilter(this);

  progress->RegisterInternalFilter(m_StatisticsFilter,.5f);
  progress->RegisterInternalFilter(m_ShiftScaleFilter,.5f);

  // Gather statistics
  
  m_StatisticsFilter->SetInput(this->GetInput());
  m_StatisticsFilter->GetOutput()->SetRequestedRegion(this->GetOutput()->GetRequestedRegion());
  m_StatisticsFilter->Update();

  // Set the parameters for Shift
  m_ShiftScaleFilter->SetShift(-m_StatisticsFilter->GetMean());
  m_ShiftScaleFilter->SetScale(NumericTraits<ITK_TYPENAME StatisticsImageFilter<TInputImage>::RealType>::One
                               / m_StatisticsFilter->GetSigma());
  m_ShiftScaleFilter->SetInput(this->GetInput());
  
  m_ShiftScaleFilter->GetOutput()->SetRequestedRegion(this->GetOutput()->GetRequestedRegion());
  m_ShiftScaleFilter->Update();

  // Graft the mini pipeline output to this filters output
  this->GraftOutput(m_ShiftScaleFilter->GetOutput());
}

} // end namespace itk

#endif
