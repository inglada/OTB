/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkMutualInformationHistogramImageToImageMetric.txx,v $
  Language:  C++
  Date:      $Date: 2005/03/05 21:30:25 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkMutualInformationHistogramImageToImageMetric_txx
#define __itkMutualInformationHistogramImageToImageMetric_txx

#include "itkMutualInformationHistogramImageToImageMetric.h"
#include "itkHistogram.h"

namespace itk
{
  template <class TFixedImage, class TMovingImage>
  typename MutualInformationHistogramImageToImageMetric<TFixedImage, \
  TMovingImage>::MeasureType
  MutualInformationHistogramImageToImageMetric<TFixedImage, \
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
        entropyX += freq*log(freq);
        }
      }

    entropyX = -entropyX/static_cast<MeasureType>(totalFreq) + log(totalFreq);

    for (unsigned int i = 0; i < this->GetHistogramSize()[1]; i++)
      {
      HistogramFrequencyType freq = histogram.GetFrequency(i, 1);
      if (freq > 0) 
        {
        entropyY += freq*log(freq);
        }
      }
    
    entropyY = -entropyY/static_cast<MeasureType>(totalFreq) + log(totalFreq);

    HistogramIteratorType it = histogram.Begin();
    HistogramIteratorType end = histogram.End();
    while (it != end) 
      {
      HistogramFrequencyType freq = it.GetFrequency();
      if (freq > 0)
        {
        jointEntropy += freq*log(freq);
        }
      ++it;
      }

    jointEntropy = -jointEntropy/
        static_cast<MeasureType>(totalFreq) + log( totalFreq );

    return entropyX + entropyY - jointEntropy;
  }
} // End namespace itk

#endif // itkMutualInformationHistogramImageToImageMetric_txx
