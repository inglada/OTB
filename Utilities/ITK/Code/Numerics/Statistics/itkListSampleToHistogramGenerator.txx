/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkListSampleToHistogramGenerator.txx,v $
  Language:  C++
  Date:      $Date: 2009-03-04 15:23:55 $
  Version:   $Revision: 1.16 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkListSampleToHistogramGenerator_txx
#define __itkListSampleToHistogramGenerator_txx

#include <exception>

namespace itk {
namespace Statistics {

template< class TListSample, 
          class THistogramMeasurement, 
          class TFrequencyContainer,
          unsigned int TMeasurementVectorLength >
ListSampleToHistogramGenerator< TListSample, 
                                THistogramMeasurement, 
                                TFrequencyContainer,
                                TMeasurementVectorLength >
::ListSampleToHistogramGenerator()
{
  m_Sizes.Fill(0);
  m_Histogram = HistogramType::New();
  m_MarginalScale = 100;
  m_HistogramMin.Fill(0);
  m_HistogramMax.Fill(0);
  m_AutoMinMax = true;
}  


template< class TListSample, 
          class THistogramMeasurement, 
          class TFrequencyContainer,
          unsigned int TMeasurementVectorLength >
void
ListSampleToHistogramGenerator< TListSample, 
                                THistogramMeasurement, 
                                TFrequencyContainer,
                                TMeasurementVectorLength >
::GenerateData()
{
  typename TListSample::MeasurementVectorType lower;
  typename TListSample::MeasurementVectorType upper;

  typename HistogramType::MeasurementVectorType h_upper  = m_HistogramMax;
  typename HistogramType::MeasurementVectorType h_lower = m_HistogramMin;

  // must test for the list size to avoid making FindSampleBound() segfault.
  // Also, the min and max can't be found automatically in that case. We can
  // only return an empty histogram
  if( m_AutoMinMax && m_List->Size() != 0 )
    {
    FindSampleBound(m_List, m_List->Begin(),
                    m_List->End(), lower, upper);
    
    float margin;

    for ( unsigned int i = 0; i < MeasurementVectorSize; i++ )
      {
      if ( !NumericTraits< THistogramMeasurement >::is_integer )
        {
        margin = 
            ( (THistogramMeasurement)(upper[i] - lower[i]) / 
              (THistogramMeasurement) m_Sizes[i] ) / 
            (THistogramMeasurement) m_MarginalScale;
        h_upper[i] = (THistogramMeasurement) (upper[i] + margin);
        if(h_upper[i] <= upper[i])
          { 
          // an overflow has occurred therefore set upper to upper
          h_upper[i] = upper[i];
          // Histogram measurement type would force the clipping the max value.
          // Therefore we must call the following to include the max value:
          m_Histogram->SetClipBinsAtEnds(false);
          // The above function is okay since here we are within the autoMinMax 
          // computation and clearly the user intended to include min and max.
          }
        }
      else
        {
        h_upper[i] = ((THistogramMeasurement) upper[i]) + 
          NumericTraits< THistogramMeasurement >::One;
        if(h_upper[i] <= upper[i])
          { 
          // an overflow has occurred therefore set upper to upper
          h_upper[i] = upper[i];
          // Histogram measurement type would force the clipping the max value.
          // Therefore we must call the following to include the max value:
          m_Histogram->SetClipBinsAtEnds(false);
          // The above function is okay since here we are within the autoMinMax 
          // computation and clearly the user intended to include min and max.
          }
        }
      h_lower[i] = ( THistogramMeasurement) lower[i];
      }
    }

  // initialize the Histogram object using the sizes and
  // the upper and lower bound from the FindSampleBound function
  m_Histogram->Initialize(m_Sizes, h_lower, h_upper);

  typename TListSample::ConstIterator iter = m_List->Begin();
  typename TListSample::ConstIterator last = m_List->End();
  typename HistogramType::IndexType index;
  typename TListSample::MeasurementVectorType lvector;
  typename HistogramType::MeasurementVectorType hvector;
  unsigned int i;
  while (iter != last)
    {
    lvector = iter.GetMeasurementVector();
    for ( i = 0; i < HistogramType::MeasurementVectorSize; i++)
      {
      hvector[i] = (THistogramMeasurement) lvector[i];
      }

    m_Histogram->GetIndex(hvector,index);
    if (!m_Histogram->IsIndexOutOfBounds(index))
      {
      // if the measurement vector is out of bound then
      // the GetIndex method has returned an index set to the max size of
      // the invalid dimension - even if the hvector is less than the minimum
      // bin value.
      // If the index isn't valid, we don't increase the frequency.
      // See the comments in Histogram->GetIndex() for more info.
      m_Histogram->IncreaseFrequency(index, 1);
      }
    ++iter;
    }
}

template< class TListSample, 
          class THistogramMeasurement, 
          class TFrequencyContainer,
          unsigned int TMeasurementVectorLength >
void
ListSampleToHistogramGenerator< TListSample, 
                                THistogramMeasurement, 
                                TFrequencyContainer,
                                TMeasurementVectorLength >
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);
  os << indent << "AutoMinMax: " << m_AutoMinMax << std::endl;
  os << indent << "Sizes: " << m_Sizes << std::endl;
  os << indent << "MarginalScale: "<< m_MarginalScale << std::endl;
  os << indent << "HistogramMin: "<< m_HistogramMin << std::endl;
  os << indent << "HistogramMax: "<< m_HistogramMax << std::endl;
}

} // end of namespace Statistics 
} // end of namespace itk 

#endif
