/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkNormalizedMutualInformationHistogramImageToImageMetric.txx,v $
  Language:  C++
  Date:      $Date: 2006-03-19 04:36:55 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkNormalizedMutualInformationHistogramImageToImageMetric_txx
#define __itkNormalizedMutualInformationHistogramImageToImageMetric_txx

#include "itkNormalizedMutualInformationHistogramImageToImageMetric.h"
#include "itkHistogram.h"

namespace itk
{
  template <class TFixedImage, class TMovingImage>
  typename NormalizedMutualInformationHistogramImageToImageMetric<TFixedImage, \
  TMovingImage>::MeasureType
  NormalizedMutualInformationHistogramImageToImageMetric<TFixedImage, \
  TMovingImage>
  ::EvaluateMeasure(HistogramType& histogram) const
  {
    MeasureType entropyX = NumericTraits<MeasureType>::Zero;
    MeasureType entropyY = NumericTraits<MeasureType>::Zero;
    MeasureType jointEntropy = NumericTraits<MeasureType>::Zero;
    HistogramFrequencyType totalFreq = histogram.GetTotalFrequency();

    for (unsigned int i = 0; i < this->GetHistogramSize()[0]; i++)
    {
      HistogramFrequencyType freq = histogram.GetFrequency(i, 0);
      if (freq > 0)
      {
        entropyX += freq*vcl_log(freq);
      }
    }

    entropyX = -entropyX/static_cast<MeasureType>(totalFreq) + vcl_log(totalFreq);

    for (unsigned int i = 0; i < this->GetHistogramSize()[1]; i++)
    {
      HistogramFrequencyType freq = histogram.GetFrequency(i, 1);
      if (freq > 0) 
      {
        entropyY += freq*vcl_log(freq);
      }
    }
    
    entropyY = -entropyY/static_cast<MeasureType>(totalFreq) + vcl_log(totalFreq);

    HistogramIteratorType it = histogram.Begin();
    HistogramIteratorType end = histogram.End();
    while (it != end) 
    {
      HistogramFrequencyType freq = it.GetFrequency();
      if (freq > 0)
      {
        jointEntropy += freq*vcl_log(freq);
      }
      ++it;
    }

    jointEntropy = -jointEntropy/static_cast<MeasureType>(totalFreq) +
      vcl_log(totalFreq);

    return (entropyX + entropyY)/jointEntropy;
  }
} // End namespace itk

#endif // itkNormalizedMutualInformationHistogramImageToImageMetric_txx
