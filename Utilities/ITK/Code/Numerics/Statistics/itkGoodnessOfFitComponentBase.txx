/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkGoodnessOfFitComponentBase.txx,v $
  Language:  C++
  Date:      $Date: 2009-03-04 15:23:49 $
  Version:   $Revision: 1.18 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkGoodnessOfFitComponentBase_txx
#define __itkGoodnessOfFitComponentBase_txx

#include "itkGoodnessOfFitComponentBase.h"
#include "itkNumericTraits.h"

namespace itk { 
namespace Statistics {

template< class TInputSample >
GoodnessOfFitComponentBase< TInputSample >
::GoodnessOfFitComponentBase()
  : m_MeasurementVectorSize( 0 )
{
  m_InputSample = 0;
  m_Resampler = ResamplerType::New();

  m_HistogramNumberOfBins = 0;
  m_HistogramBinOverlap = 0.75;
  m_HistogramExtent = 1.5;
  m_HistogramMean = 0.0;
  m_HistogramStandardDeviation = 0.0;
  m_HistogramUseEquiProbableBins = true;
  m_UseExpectedHistogram = true;
  m_HistogramSizeChanged = true;

  m_Projector = ProjectorType::New();
  m_Projector->SetInputSample(m_Resampler->GetOutput());
  m_Projector->SetHistogramBinOverlap(m_HistogramBinOverlap);

  m_ObservedHistogram = 0;
  m_ExpectedHistogram = 0;

  m_Proportion = 0.5;
  //ProjectionAxisType tempAxis;
  //tempAxis.Fill(0.0);
  //m_ProjectionAxes.Fill(tempAxis);
  m_TotalObservedScale = 0.0;
}

template< class TInputSample >
GoodnessOfFitComponentBase< TInputSample >
::~GoodnessOfFitComponentBase()
{
}

template< class TInputSample >
void
GoodnessOfFitComponentBase< TInputSample >
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);

  os << indent << "Input Sample: ";
  if ( m_InputSample != 0 )
    {
     os << m_InputSample << std::endl;
     os << indent << "Length of each measurement vector: " << m_MeasurementVectorSize << std::endl;
    }
  else
    {
    os << "not set." << std::endl;
    }

  if(!m_Parameters.empty())
    {
    os << indent << "Parameters : " << m_Parameters << std::endl;
    }
  os << indent << "Proportion: " << m_Proportion << std::endl;
  os << indent << "Resampler: " << m_Resampler << std::endl;
  os << indent << "Projector: " << m_Projector << std::endl;
  os << indent << "Projection Axes: " << m_ProjectionAxes << std::endl;
  os << indent << "Total Observed Scale: " << m_TotalObservedScale << std::endl;

  os << indent << "Observed Histogram: ";
  if ( m_ObservedHistogram.IsNotNull() )
    {
    os << m_ObservedHistogram << std::endl;
    }
  else
    {
    os << "not allocated." << std::endl;
    }

  os << indent << "Expected Histogram: ";
  if ( m_ExpectedHistogram.IsNotNull() )
    {
    os << m_ExpectedHistogram << std::endl;
    }
  else
    {
    os << "not allocated yet." << std::endl;
    }

  os << indent << "Histogram Number of Bins: " 
     << m_HistogramNumberOfBins << std::endl;
  os << indent << "Histogram Use Equiprobable Bins: " 
     << m_HistogramUseEquiProbableBins << std::endl;
  os << indent << "Histogram Extent: " 
     << m_HistogramExtent << std::endl;
  os << indent << "Histogram Bin Overlap: " 
     << m_HistogramBinOverlap << std::endl;
  os << indent << "Histogram Size Changed: " 
     << m_HistogramSizeChanged << std::endl;
  os << indent << "Histogram Mean: " 
     << m_HistogramMean << std::endl;
  os << indent << "Histogram Standard Deviation: " 
     << m_HistogramStandardDeviation << std::endl;
}

template< class TInputSample >
void
GoodnessOfFitComponentBase< TInputSample >
::SetInputSample(const TInputSample* sample)
{
  if ( m_InputSample != sample )
    {
    m_InputSample = sample;

    // Get length of measurement vector from the sample and set the 
    // length of the arrays.
    this->m_MeasurementVectorSize = m_InputSample->GetMeasurementVectorSize();
    this->m_ProjectionAxes.SetSize( 
        this->m_MeasurementVectorSize, this->m_MeasurementVectorSize );
    m_ProjectionAxes.Fill(0.0);
    
    m_Resampler->SetInputSample(m_InputSample);
    this->Modified();
    }
}

template< class TInputSample >
const TInputSample*
GoodnessOfFitComponentBase< TInputSample >
::GetInputSample() const
{
  return m_InputSample;
}

template< class TInputSample >
void
GoodnessOfFitComponentBase< TInputSample >
::SetParameters(const ParametersType &parameters)
{
  if ( m_Parameters != parameters )
    {
    m_Parameters = parameters;
    this->Modified();
    }
}

template< class TInputSample >
void
GoodnessOfFitComponentBase< TInputSample >
::SetUseExpectedHistogram(bool flag)
{
  if ( m_UseExpectedHistogram != flag )
    {
    m_UseExpectedHistogram = flag;
    this->Modified();
    }
}

template< class TInputSample >
void
GoodnessOfFitComponentBase< TInputSample >
::SetHistogramNumberOfBins(int numberOfBins)
{
  if ( m_HistogramNumberOfBins != (unsigned int)numberOfBins )
    {
    m_HistogramNumberOfBins = numberOfBins;
    m_HistogramSizeChanged = true;
    this->Modified();
    }
}

template< class TInputSample >
void
GoodnessOfFitComponentBase< TInputSample >
::SetHistogramBinOverlap(double histogramBinOverlap)
{
  if ( m_HistogramBinOverlap != histogramBinOverlap )
    {
    m_HistogramBinOverlap = histogramBinOverlap;
    m_Projector->SetHistogramBinOverlap(histogramBinOverlap);
    this->Modified();
    }
}

template< class TInputSample >
void
GoodnessOfFitComponentBase< TInputSample >
::SetHistogramExtent(double histogramExtent)
{
  if ( m_HistogramExtent != histogramExtent )
    {
    m_HistogramExtent = histogramExtent;
    this->Modified();
    }
}

template< class TInputSample >
void
GoodnessOfFitComponentBase< TInputSample >
::SetHistogramUseEquiProbableBins(bool flag)
{
  if ( m_HistogramUseEquiProbableBins != flag )
    {
    m_HistogramUseEquiProbableBins = flag;
    this->Modified();
    }
}

template< class TInputSample >
void
GoodnessOfFitComponentBase< TInputSample >
::CreateEquiProbableBins()
{
  if (m_HistogramNumberOfBins < 1)
    {
    return;
    }

  HistogramType::SizeType size;
  
  size[0] = m_HistogramNumberOfBins;

  m_ObservedHistogram->Initialize(size);

  m_ObservedHistogram->SetBinMin(0, 0, -m_HistogramExtent);

  double binMinProbability = 
    this->GetCumulativeProbability(-m_HistogramExtent);
  
  double step = 
    (1.0 - 2.0 * binMinProbability) / (double)m_HistogramNumberOfBins;
  
  HistogramType::InstanceIdentifier binId;
  double rangeExtent;
  MeasurementType tempMin;
  MeasurementType tempMax = NumericTraits< MeasurementType >::Zero;
  double binMaxProbability = 0.0;

  for(binId = 0; binId < m_HistogramNumberOfBins; binId++)
    {
    tempMin = m_ObservedHistogram->GetBinMin(0, binId);
   
    for (rangeExtent = 0.00001; rangeExtent < 2.0; rangeExtent += 0.00001)
      {
      tempMax = tempMin + rangeExtent;
      binMaxProbability = 
        this->GetCumulativeProbability(tempMax);

      if ( (binMaxProbability - binMinProbability) > step )
        {
        break;
        }
      }

    m_ObservedHistogram->SetBinMax(0, binId, tempMax);
  
    binMinProbability = binMaxProbability;
  
    if ( binId < (m_HistogramNumberOfBins - 1) )
      {
      m_ObservedHistogram->SetBinMin(0, binId + 1, tempMax);
      }
    }
  
  m_ObservedHistogram->
    SetBinMax(0, m_HistogramNumberOfBins - 1, m_HistogramExtent);
}

template< class TInputSample >
void
GoodnessOfFitComponentBase< TInputSample >
::CreateEquiRangeBins()
{
  if (m_HistogramNumberOfBins < 1)
    {
    return;
    }

  HistogramType::SizeType size;
  HistogramType::MeasurementVectorType lowerBound;
  HistogramType::MeasurementVectorType upperBound;

  size[0] = m_HistogramNumberOfBins;
  lowerBound[0] = -m_HistogramExtent;
  upperBound[0] = m_HistogramExtent;

  m_ObservedHistogram->Initialize(size, lowerBound, upperBound);
}

template< class TInputHistogram >
void
GoodnessOfFitComponentBase< TInputHistogram >
::CreateHistograms()
{
  m_TotalObservedScale = this->GetCumulativeProbability(m_HistogramExtent);
  m_TotalObservedScale = m_TotalObservedScale - (1.0f - m_TotalObservedScale);
  m_HistogramMean = (double(m_HistogramNumberOfBins) - 1.0) / 2.0;
  m_HistogramStandardDeviation = m_HistogramMean / m_HistogramExtent;

  m_ObservedHistogram = HistogramType::New();

  if ( m_HistogramUseEquiProbableBins )
    {
    this->CreateEquiProbableBins();
    }
  else
    {
    this->CreateEquiRangeBins();
    }


  m_Projector->SetHistogram(m_ObservedHistogram.GetPointer());

  if ( m_UseExpectedHistogram )
    {
    m_ExpectedHistogram = HistogramType::New();
    HistogramType::SizeType size;
    size[0] = m_HistogramNumberOfBins;
    m_ExpectedHistogram->Initialize(size);
    }
  
  m_HistogramSizeChanged = false;
}

template< class TInputSample >
void
GoodnessOfFitComponentBase< TInputSample >
::Resample()
{
  m_Resampler->SetCenter(this->GetCenter());
  m_Resampler->SetRadius(this->GetRadius());
  m_Resampler->Update();

}

template< class TInputSample >
unsigned int
GoodnessOfFitComponentBase< TInputSample >
::GetResampledSampleSize()
{
  return m_Resampler->GetOutput()->Size();
}


template< class TInputSample >
void
GoodnessOfFitComponentBase< TInputSample >
::Project(int projectionAxisIndex)
{
  m_Projector->SetMean(this->GetMean());
  m_Projector->SetStandardDeviation(this->GetStandardDeviation());
  
  Array< double > projectionAxis( m_MeasurementVectorSize );
  for( unsigned int i=0; i< m_MeasurementVectorSize; i++ )
    {
    projectionAxis[i] = m_ProjectionAxes(projectionAxisIndex, i );
    }
  m_Projector->SetProjectionAxis( &projectionAxis );
  m_Projector->Update();
}

template< class TInputHistogram >
void
GoodnessOfFitComponentBase< TInputHistogram >
::UpdateExpectedHistogram()
{
  float totalObservedFrequency = m_ObservedHistogram->GetTotalFrequency();
  HistogramType::Iterator e_iter = m_ExpectedHistogram->Begin();
  HistogramType::Iterator e_last = m_ExpectedHistogram->End();
  int i = 0;
  float frequency;
  while (e_iter != e_last)
    {
    frequency = 
      this->GetCumulativeProbability(m_ObservedHistogram->GetBinMax(0, i)) - 
      this->GetCumulativeProbability(m_ObservedHistogram->GetBinMin(0, i));
    frequency = (frequency / m_TotalObservedScale) * 
      totalObservedFrequency;
    
    e_iter.SetFrequency(frequency);
    ++e_iter;
    ++i;
    }
}

template< class TInputSample >
typename GoodnessOfFitComponentBase< TInputSample >::HistogramType *
GoodnessOfFitComponentBase< TInputSample >
::GetObservedHistogram() 
{
  return m_ObservedHistogram;
}

template< class TInputSample >
typename GoodnessOfFitComponentBase< TInputSample >::HistogramType *
GoodnessOfFitComponentBase< TInputSample >
::GetExpectedHistogram() 
{
  return m_ExpectedHistogram;
}

} // end of namespace Statistics 
} // end of namespace itk

#endif
