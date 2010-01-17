/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkCorrelationCoefficientHistogramImageToImageMetric.txx,v $
  Language:  C++
  Date:      $Date: 2009-05-05 17:51:18 $
  Version:   $Revision: 1.8 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkCorrelationCoefficientHistogramImageToImageMetric_txx
#define __itkCorrelationCoefficientHistogramImageToImageMetric_txx

#include "itkCorrelationCoefficientHistogramImageToImageMetric.h"

namespace itk
{
template <class TFixedImage, class TMovingImage>
typename CorrelationCoefficientHistogramImageToImageMetric<TFixedImage, \
                                                           TMovingImage>::MeasureType
CorrelationCoefficientHistogramImageToImageMetric<TFixedImage, TMovingImage>
::EvaluateMeasure(HistogramType& histogram) const
{
  const MeasureType varianceX  = this->VarianceX(  histogram );
  const MeasureType varianceY  = this->VarianceY(  histogram );
  const MeasureType covariance = this->Covariance( histogram );
  return vcl_fabs(covariance /( vcl_sqrt(varianceX )* vcl_sqrt(varianceY )));
}

template <class TFixedImage, class TMovingImage>
typename CorrelationCoefficientHistogramImageToImageMetric<TFixedImage, \
                                                           TMovingImage>::MeasureType
CorrelationCoefficientHistogramImageToImageMetric<TFixedImage, TMovingImage>
::MeanX(HistogramType& histogram) const
{
  MeasureType meanX = NumericTraits<MeasureType>::Zero;

  for (unsigned int i = 0; i < this->m_HistogramSize[0]; i++)
    {
    MeasureType valX = histogram.GetMeasurement(i, 0);
    HistogramFrequencyType freq = histogram.GetFrequency(i, 0);
    meanX += valX*freq;
    }

  meanX /= histogram.GetTotalFrequency();

  return meanX;
}

template <class TFixedImage, class TMovingImage>
typename CorrelationCoefficientHistogramImageToImageMetric<TFixedImage, \
                                                           TMovingImage>::MeasureType
CorrelationCoefficientHistogramImageToImageMetric<TFixedImage, TMovingImage>
::MeanY(HistogramType& histogram) const
{
  MeasureType meanY = NumericTraits<MeasureType>::Zero;

  for (unsigned int i = 0; i < this->m_HistogramSize[1]; i++)
    {
    MeasureType valY = histogram.GetMeasurement(i, 1);
    HistogramFrequencyType freq = histogram.GetFrequency(i, 1);
    meanY += valY*freq;
    }

  meanY /= histogram.GetTotalFrequency();

  return meanY;
}

template <class TFixedImage, class TMovingImage>
typename CorrelationCoefficientHistogramImageToImageMetric<TFixedImage, \
                                                           TMovingImage>::MeasureType
CorrelationCoefficientHistogramImageToImageMetric<TFixedImage, TMovingImage>
::VarianceX(HistogramType& histogram) const
{
  MeasureType varX = NumericTraits<MeasureType>::Zero;

  for (unsigned int i = 0; i < this->m_HistogramSize[0]; i++)
    {
    varX += static_cast<double>(histogram.GetFrequency(i, 0))/
      histogram.GetTotalFrequency()*
      vcl_pow(histogram.GetMeasurement(i, 0), 2);
    }

  return varX - vcl_pow(MeanX(histogram), 2);
}

template <class TFixedImage, class TMovingImage>
typename CorrelationCoefficientHistogramImageToImageMetric<TFixedImage, \
                                                           TMovingImage>::MeasureType
CorrelationCoefficientHistogramImageToImageMetric<TFixedImage, TMovingImage>
::VarianceY(HistogramType& histogram) const
{
  MeasureType varY = NumericTraits<MeasureType>::Zero;

  for (unsigned int i = 0; i < this->m_HistogramSize[1]; i++)
    {
    varY += static_cast<double>(histogram.GetFrequency(i, 1))/
      histogram.GetTotalFrequency()*
      vcl_pow(histogram.GetMeasurement(i, 1), 2);
    }

  return varY - vcl_pow(MeanY(histogram), 2);
}

template <class TFixedImage, class TMovingImage>
typename CorrelationCoefficientHistogramImageToImageMetric<TFixedImage, \
                                                           TMovingImage>::MeasureType
CorrelationCoefficientHistogramImageToImageMetric<TFixedImage, TMovingImage>
::Covariance(HistogramType& histogram) const
{
  MeasureType var = NumericTraits<MeasureType>::Zero;
  MeasureType meanX = MeanX(histogram);
  MeasureType meanY = MeanY(histogram);

  for (unsigned int j = 0; j < this->m_HistogramSize[1]; j++)
    {
    for (unsigned int i = 0; i < this->m_HistogramSize[0]; i++)
      {
      typename HistogramType::IndexType index;
#ifdef ITK_USE_REVIEW_STATISTICS
      index.SetSize(2);
#endif
      index[0] = i;
      index[1] = j;

      var += histogram.GetFrequency(index)*
        (histogram.GetMeasurement(i, 0) - meanX)*
        (histogram.GetMeasurement(j, 1) - meanY);
      }
    }
    
  var /= histogram.GetTotalFrequency();

  return var;
}
}

#endif // itkCorrelationCoefficientHistogramImageToImageMetric_txx
