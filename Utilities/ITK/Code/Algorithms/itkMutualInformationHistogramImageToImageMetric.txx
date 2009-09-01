/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkMutualInformationHistogramImageToImageMetric.txx,v $
  Language:  C++
  Date:      $Date: 2009-05-04 18:13:52 $
  Version:   $Revision: 1.10 $

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
typename MutualInformationHistogramImageToImageMetric<TFixedImage,TMovingImage>::MeasureType
MutualInformationHistogramImageToImageMetric<TFixedImage,TMovingImage>
::EvaluateMeasure(HistogramType& histogram) const
{
  MeasureType entropyX = NumericTraits<MeasureType>::Zero;
  MeasureType entropyY = NumericTraits<MeasureType>::Zero;
  MeasureType jointEntropy = NumericTraits<MeasureType>::Zero;

  typedef typename NumericTraits< HistogramFrequencyType >::RealType  HistogramFrequencyRealType;

  HistogramFrequencyRealType totalFreq = 
    static_cast< HistogramFrequencyRealType >( histogram.GetTotalFrequency() );

  for (unsigned int i = 0; i < this->GetHistogramSize()[0]; i++)
    {
    HistogramFrequencyRealType freq = 
      static_cast< HistogramFrequencyRealType >( histogram.GetFrequency(i, 0) );
    if (freq > 0)
      {
      entropyX += freq*vcl_log(freq);
      }
    }

  entropyX = -entropyX/static_cast<MeasureType>(totalFreq) + vcl_log(totalFreq);

  for (unsigned int i = 0; i < this->GetHistogramSize()[1]; i++)
    {
    HistogramFrequencyRealType freq =
      static_cast< HistogramFrequencyRealType >( histogram.GetFrequency(i, 1) );
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
    HistogramFrequencyRealType freq = 
      static_cast< HistogramFrequencyRealType >( it.GetFrequency() );
    if (freq > 0)
      {
      jointEntropy += freq*vcl_log(freq);
      }
    ++it;
    }
  
  jointEntropy = -jointEntropy/
    static_cast<MeasureType>(totalFreq) + vcl_log(totalFreq );
  
  return entropyX + entropyY - jointEntropy;
}
} // End namespace itk

#endif // itkMutualInformationHistogramImageToImageMetric_txx
