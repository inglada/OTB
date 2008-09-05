/*=========================================================================

Program:   Insight Segmentation & Registration Toolkit
Module:    $RCSfile: itkSampleToHistogramProjectionFilter.txx,v $
Language:  C++
Date:      $Date: 2007-01-26 12:11:36 $
Version:   $Revision: 1.15 $

Copyright (c) Insight Software Consortium. All rights reserved.
See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

This software is distributed WITHOUT ANY WARRANTY; without even 
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkSampleToHistogramProjectionFilter_txx
#define __itkSampleToHistogramProjectionFilter_txx

#include "itkSampleToHistogramProjectionFilter.h"

#include "vnl/vnl_math.h"
#include "itkNumericTraits.h"

namespace itk{ 
namespace Statistics{

template< class TInputSample, class THistogramMeasurement >
SampleToHistogramProjectionFilter< TInputSample, THistogramMeasurement >
::SampleToHistogramProjectionFilter()
{
  m_Mean = 0 ;
  m_StandardDeviation = 0 ;
  m_Histogram = 0 ;
  m_ProjectionAxis = 0 ;
  m_HistogramBinOverlap = 0.0 ;
  m_MinimumFrequency = NumericTraits< FrequencyType >::Zero ;
}

template< class TInputSample, class THistogramMeasurement >
void
SampleToHistogramProjectionFilter< TInputSample, THistogramMeasurement >
::PrintSelf(std::ostream& os, Indent indent) const 
{
  Superclass::PrintSelf(os,indent) ;

  os << indent << "Histogram: " ;
  if ( m_Histogram != 0 )
    {
    os << m_Histogram << std::endl;
    }
  else
    {
    os << "not set." << std::endl ;
    }

  os << indent << "Mean: " ;
  if ( m_Mean != 0 )
    {
    os << (*m_Mean) << std::endl;
    }
  else
    {
    os << "not set." << std::endl ;
    }

  os << indent << "Standard Deviation: " ;
  if ( m_StandardDeviation != 0 )
    {
    os << (*m_StandardDeviation) << std::endl;
    }
  else
    {
    os << "not set" << std::endl ;
    }

  os << indent << "ProjectionAxis: " ;
  if ( m_ProjectionAxis != 0 )
    { 
    os << (*m_ProjectionAxis) << std::endl ;
    }
  else
    {
    os << "not set." << std::endl ;
    }

  os << indent << "HistogramBinOverlap: "  << m_HistogramBinOverlap << std::endl ;
  os << indent << "Minimum Frequency   " << m_MinimumFrequency << std::endl;
}

template< class TInputSample, class THistogramMeasurement >
void
SampleToHistogramProjectionFilter< TInputSample, THistogramMeasurement >
::SetHistogram(HistogramType* histogram) 
{
  if ( m_Histogram != histogram )
    {
    m_Histogram = histogram ;
    this->Modified() ;
    }
}

template< class TInputSample, class THistogramMeasurement >
void
SampleToHistogramProjectionFilter< TInputSample, THistogramMeasurement >
::SetMean(MeanType* mean)
{
  if( this->GetMeasurementVectorSize() )
    {
    if( mean->size() != this->GetMeasurementVectorSize() )
      {
      itkExceptionMacro( << "Size of measurement vectors in the sample must be"
         << " the same as the size of the mean." );
      }
    }
  else
    {
    this->SetMeasurementVectorSize( mean->size() );
    }

  
  if ( m_Mean != mean )
    {
    m_Mean = mean ;
    this->Modified() ;
    }
}

template< class TInputSample, class THistogramMeasurement >
typename SampleToHistogramProjectionFilter< TInputSample, THistogramMeasurement >::MeanType*
SampleToHistogramProjectionFilter< TInputSample, THistogramMeasurement >
::GetMean()
{
  return m_Mean ; 
}

template< class TInputSample, class THistogramMeasurement >
void
SampleToHistogramProjectionFilter< TInputSample, THistogramMeasurement >
::SetStandardDeviation(double* value)
{
  if ( m_StandardDeviation != value )
    {
    m_StandardDeviation = value ;
    this->Modified() ;
    }
}

template< class TInputSample, class THistogramMeasurement >
double*
SampleToHistogramProjectionFilter< TInputSample, THistogramMeasurement >
::GetStandardDeviation()
{ 
  return m_StandardDeviation ; 
}

template< class TInputSample, class THistogramMeasurement >
void
SampleToHistogramProjectionFilter< TInputSample, THistogramMeasurement >
::SetProjectionAxis(ArrayType* axis)
{ 
  if( this->GetMeasurementVectorSize() )
    {
    if( axis->GetSize() != this->GetMeasurementVectorSize() )
      {
      itkExceptionMacro( << "Size of measurement vectors in the sample must be"
         << " the same as the size of the projection axis." );
      }
    }
  else
    {
    this->SetMeasurementVectorSize( axis->GetSize() );
    }
  
  if ( m_ProjectionAxis != axis )
    {
    m_ProjectionAxis = axis ;
    this->Modified() ;
    }
}

template< class TInputSample, class THistogramMeasurement >
typename SampleToHistogramProjectionFilter< TInputSample, THistogramMeasurement >::ArrayType*
SampleToHistogramProjectionFilter< TInputSample, THistogramMeasurement >
::GetProjectionAxis()
{
  return m_ProjectionAxis ; 
}

template< class TInputSample, class THistogramMeasurement >
void
SampleToHistogramProjectionFilter< TInputSample, THistogramMeasurement >
::SetHistogramBinOverlap(double overlap)
{ 
  if ( m_HistogramBinOverlap != overlap )
    {
    m_HistogramBinOverlap = overlap ;
    this->Modified() ;
    }
}

template< class TInputSample, class THistogramMeasurement >
inline float
SampleToHistogramProjectionFilter< TInputSample, THistogramMeasurement >
::CalculateOverlap(int binId,
                   float dotProduct, float scale,
                   float marginalDistance,
                   bool firstHalf)
{
  float minWeight ;
  float maxWeight ;

  float binMin = m_Histogram->GetBinMin(0, binId) ;
  float binMax = m_Histogram->GetBinMax(0, binId) ;
  
  if (firstHalf)
    {
    minWeight = (dotProduct - binMin) / ((binMax - binMin) / 2.0) ;
    }
  else
    {
    minWeight = (dotProduct - binMin) / 
      ((float(m_Histogram->GetBinMax(0, binId - 1)) - 
        float(m_Histogram->GetBinMin(0, binId - 1))) / 2.0) ;
    }

  if (minWeight > -1.0)
    {
    if (firstHalf)
      {
      maxWeight = (binMax - dotProduct) /
        ((float(m_Histogram->GetBinMax(0, binId + 1)) - 
          float(m_Histogram->GetBinMin(0, binId + 1))) / 2.0) ;
      }
    else
      {
      maxWeight = 
        (binMax - dotProduct) / ((binMax - binMin) / 2.0) ;
      }
      
    if (maxWeight > -1.0)
      {
      if (minWeight < 1.0)
        {
        minWeight = 1.0 / (1.0 + vcl_exp(-minWeight / scale)) ;
        }
      else
        {
        minWeight = 1.0 ;
        }
  
      if (maxWeight < 1.0)
        {
        maxWeight = 1.0 / (1.0 + vcl_exp(-maxWeight / scale)) ;
        }
      else
        {
        maxWeight = 1.0 ;
        }
  
      return minWeight * maxWeight * marginalDistance ;
      }
    }

  return m_MinimumFrequency ;
}

template< class TInputSample, class THistogramMeasurement >
void
SampleToHistogramProjectionFilter< TInputSample, THistogramMeasurement >
::GenerateData()
{
  // Assert at run time that the given mean and ProjectionAxis have the same length as 
  // measurement vectors in the sample and that the size is non-zero.
  const MeasurementVectorSizeType measurementVectorSize = 
                            this->GetMeasurementVectorSize();
  if( !measurementVectorSize || 
      ( MeasurementVectorTraits::GetLength( *m_Mean ) != measurementVectorSize ) ||
      ( m_ProjectionAxis->Size() != measurementVectorSize ) )
    {
    itkExceptionMacro( << "Size of measurement vectors in the sample must be "
        << "the same as the size of the mean and the ProjectionAxis.");
    }
  
  
  typename HistogramType::Iterator h_iter = m_Histogram->Begin() ;
  typename HistogramType::Iterator h_last = m_Histogram->End() ;
  while (h_iter != h_last)
    {
    h_iter.SetFrequency(0.0) ;
    ++h_iter ;
    }

  float scale = 1 ;

  if (m_HistogramBinOverlap)
    {
    scale = vcl_log(1.0 + m_HistogramBinOverlap / 10.0 ) ;
    }

  typename HistogramType::InstanceIdentifier binId ;
  unsigned int dimension ;
  float coordinateDistance ;
  float squaredDistance ;
  float marginalDistance ;
  float dotProduct ;
  FrequencyType tempFrequency ;
  FrequencyType frequency ;

  MeasurementVectorType tempMeasurementVector ;

  typename TInputSample::ConstIterator s_iter = this->GetInputSample()->Begin();
  typename TInputSample::ConstIterator s_last = this->GetInputSample()->End();

  unsigned long numberOfBins = (unsigned long) m_Histogram->Size() ;
  double extent = 
    vnl_math_abs( m_Histogram->GetBinMax(0, numberOfBins - 1UL) - 
                  m_Histogram->GetBinMin(0, 0) ) / 2.0 ;

  while (s_iter != s_last)
    {
    tempMeasurementVector = s_iter.GetMeasurementVector() ;
    squaredDistance = 0.0 ;
    dotProduct = 0.0 ;
    frequency = s_iter.GetFrequency() ;
    for (dimension = 0 ; dimension < measurementVectorSize ; dimension++)
      {
      coordinateDistance = 
        tempMeasurementVector[dimension] - (*m_Mean)[dimension] ;
      squaredDistance += coordinateDistance * coordinateDistance ;
      dotProduct += coordinateDistance * (*m_ProjectionAxis)[dimension] ;
      }

    marginalDistance = 
      vcl_sqrt(vnl_math_abs(squaredDistance - dotProduct * dotProduct)) /
      ((*m_StandardDeviation) * extent) ;

    dotProduct /= (*m_StandardDeviation) ;

    if (m_HistogramBinOverlap < 0.001)
      {
      if ( (marginalDistance <= 1) && 
           (dotProduct >= m_Histogram->GetBinMin(0, 0)) &&
           (dotProduct <= 
            m_Histogram->
            GetBinMax(0, numberOfBins - 1UL)) )
        {
        binId = 0 ;
        while ( (dotProduct > m_Histogram->GetBinMax(0, binId)) &&
                (binId < (numberOfBins - 1UL)) )
          {
          binId++ ;
          }
        m_Histogram->IncreaseFrequency(binId, frequency) ;
        }
      }
    else
      {
      marginalDistance = 1.0 - marginalDistance ;
      if ( marginalDistance > -1.0 )
        {
        marginalDistance = 
          1.0 / (1.0 + vcl_exp(-marginalDistance / scale)) ;
              
        for (binId = 0 ; binId <= (numberOfBins / 2UL) ; 
             binId++)
          {
          tempFrequency = 
            this->CalculateOverlap(binId, 
                                   dotProduct,
                                   scale,
                                   marginalDistance,
                                   true) ;
          if ( tempFrequency > m_MinimumFrequency )
            {
            m_Histogram->IncreaseFrequency(binId, tempFrequency * frequency) ;
            }
          } // end of for

        for (binId = numberOfBins / 2UL + 1UL ; 
             binId < numberOfBins ;
             binId++)
          {
          tempFrequency = 
            this->CalculateOverlap(binId, 
                                   dotProduct,
                                   scale,
                                   marginalDistance,
                                   false) ;

          if ( tempFrequency > m_MinimumFrequency )
            {
            m_Histogram->IncreaseFrequency(binId, tempFrequency * frequency) ;
            }
          } // end of for
        } // end of if (marginalDistance ...
      } // end of if (m_HistogramBinOverlap  ...
    ++s_iter ;
    } // end of while
}

} // end of namespace Statistics 
} // end of namespace itk

#endif
