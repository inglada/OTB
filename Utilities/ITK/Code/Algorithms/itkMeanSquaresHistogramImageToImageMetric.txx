/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkMeanSquaresHistogramImageToImageMetric.txx,v $
  Language:  C++
  Date:      $Date: 2009-01-24 20:03:00 $
  Version:   $Revision: 1.5 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkMeanSquaresHistogramImageToImageMetric_txx
#define __itkMeanSquaresHistogramImageToImageMetric_txx

#include "itkMeanSquaresHistogramImageToImageMetric.h"

namespace itk
{
template <class TFixedImage, class TMovingImage>
typename MeanSquaresHistogramImageToImageMetric<TFixedImage, TMovingImage>
::MeasureType
MeanSquaresHistogramImageToImageMetric<TFixedImage, TMovingImage>
::EvaluateMeasure(HistogramType& histogram) const
{
  MeasureType measure = NumericTraits<MeasureType>::Zero;
  HistogramIteratorType it = histogram.Begin();
  HistogramIteratorType end = histogram.End();
  HistogramFrequencyType totalNumberOfSamples =
    NumericTraits<HistogramFrequencyType>::Zero;

  while (it != end)
    {
    HistogramFrequencyType freq = it.GetFrequency();
    if (freq > 0)
      {
      HistogramMeasurementVectorType value = it.GetMeasurementVector();
      measure += (value[0] - value[1])*(value[0] - value[1])*freq;
      totalNumberOfSamples += freq;
      }
    ++it;
    }
  
  measure /= totalNumberOfSamples;
  
  return measure;
}
} // End namespace itk

#endif // itkMeanSquaresHistogramImageToImageMetric_txx
