/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkOtsuMultipleThresholdsCalculator.txx,v $
  Language:  C++
  Date:      $Date: 2005-01-13 15:36:46 $
  Version:   $Revision: 1.4 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itkOtsuMultipleThresholdsCalculator_txx
#define _itkOtsuMultipleThresholdsCalculator_txx

#include "itkOtsuMultipleThresholdsCalculator.h"

namespace itk
{

template<class TInputHistogram>
OtsuMultipleThresholdsCalculator<TInputHistogram>
::OtsuMultipleThresholdsCalculator()
{
  m_NumberOfThresholds = 1;
  m_Output.resize(m_NumberOfThresholds);
  std::fill(m_Output.begin(),m_Output.end(),NumericTraits<MeasurementType>::Zero);
}
                                                    
template<class TInputHistogram>
const typename OtsuMultipleThresholdsCalculator<TInputHistogram>::OutputType&
OtsuMultipleThresholdsCalculator< TInputHistogram >
::GetOutput()
{
  return m_Output ;
}

/*
 * Increment the thresholds of one position along the histogram
 */
template<class TInputHistogram>
bool 
OtsuMultipleThresholdsCalculator<TInputHistogram>
::IncrementThresholds(InstanceIdentifierVectorType& thresholdIndexes, MeanType globalMean, MeanVectorType& classMean, FrequencyVectorType& classFrequency)
{
  typename TInputHistogram::ConstPointer histogram = this->GetInputHistogram();

  unsigned long numberOfHistogramBins = histogram->Size();
  unsigned long numberOfClasses = classMean.size();

  MeanType meanOld;
  FrequencyType freqOld;

  unsigned int k;
  int j;

  // from the upper threshold down
  for(j=static_cast<int>(m_NumberOfThresholds-1); j>=0; j--)
    {
    // if this threshold can be incremented (i.e. we're not at the end of the histogram)
    if (thresholdIndexes[j] < numberOfHistogramBins - 2 - (m_NumberOfThresholds-1 - j) )
      {
      // increment it and update mean and frequency of the class bounded by the threshold
      ++thresholdIndexes[j];

      meanOld = classMean[j];
      freqOld = classFrequency[j];
      
      classFrequency[j] += histogram->GetFrequency(thresholdIndexes[j]);
      
      if (NumericTraits<FrequencyType>::IsPositive(classFrequency[j]))
        {
        classMean[j] = (meanOld * static_cast<MeanType>(freqOld) + static_cast<MeanType>(histogram->GetMeasurementVector(thresholdIndexes[j])[0]) * static_cast<MeanType>(histogram->GetFrequency(thresholdIndexes[j]))) / static_cast<MeanType>(classFrequency[j]);
        }
      else
        {
        classMean[j] = NumericTraits<MeanType>::Zero;
        }
      
      // set higher thresholds adjacent to their previous ones, and update mean and frequency of the respective classes
      for (k=j+1; k<m_NumberOfThresholds; k++)
        {
        thresholdIndexes[k] = thresholdIndexes[k-1] + 1;
        classFrequency[k] = histogram->GetFrequency(thresholdIndexes[k]);
        if (NumericTraits<FrequencyType>::IsPositive(classFrequency[k]))
          {
          classMean[k] = static_cast<MeanType>(histogram->GetMeasurementVector(thresholdIndexes[k])[0]);
          }
        else
          {
          classMean[k] = NumericTraits<MeanType>::Zero;
          }
        }
      
      // update mean and frequency of the highest class
      classFrequency[numberOfClasses-1] = histogram->GetTotalFrequency();
      classMean[numberOfClasses-1] = globalMean * histogram->GetTotalFrequency();

      for(k=0; k<numberOfClasses-1; k++)
        {
        classFrequency[numberOfClasses-1] -= classFrequency[k];
        classMean[numberOfClasses-1] -= classMean[k] * static_cast<MeanType>(classFrequency[k]);
        }

      if (NumericTraits<FrequencyType>::IsPositive(classFrequency[numberOfClasses-1]))
        {
        classMean[numberOfClasses-1] /= static_cast<MeanType>(classFrequency[numberOfClasses-1]);
        }
      else
        {
        classMean[numberOfClasses-1] = NumericTraits<MeanType>::Zero;
        }

      // exit the for loop if a threshold has been incremented
      break;
      }
    else  // if this threshold can't be incremented
      {
      // if it's the lowest threshold
      if (j==0)
        {
        // we couldn't increment because we're done
        return false;
        }
      }
    }
  // we incremented
  return true;
}

/*
 * Compute Otsu's thresholds
 */                    
template<class TInputHistogram>
void
OtsuMultipleThresholdsCalculator<TInputHistogram>
::GenerateData()
{
  typename TInputHistogram::ConstPointer histogram = this->GetInputHistogram();

  // TODO: as an improvement, the class could accept multi-dimensional histograms
  // and the user could specify the dimension to apply the algorithm to.
  if (histogram->GetSize().GetSizeDimension() != 1)
    {
    itkExceptionMacro(<<"Histogram must be 1-dimensional.");
    }

  // compute global mean
  typename TInputHistogram::ConstIterator iter = histogram->Begin() ;
  typename TInputHistogram::ConstIterator end = histogram->End() ;

  MeanType globalMean = NumericTraits<MeanType>::Zero;
  FrequencyType globalFrequency = histogram->GetTotalFrequency();
  while (iter != end)
    {
    globalMean += static_cast<MeanType>(iter.GetMeasurementVector()[0]) * static_cast<MeanType>(iter.GetFrequency());
    ++iter ;
    }
  globalMean /= static_cast<MeanType>(globalFrequency);

  unsigned long numberOfClasses = m_NumberOfThresholds + 1;

  // initialize thresholds
  InstanceIdentifierVectorType thresholdIndexes(m_NumberOfThresholds);

  unsigned long j;
  for(j=0; j<m_NumberOfThresholds; j++)
    {
    thresholdIndexes[j] = j;
    }

  InstanceIdentifierVectorType maxVarThresholdIndexes = thresholdIndexes;

  // compute frequency and mean of initial classes
  FrequencyType freqSum = NumericTraits<FrequencyType>::Zero;
  FrequencyVectorType classFrequency(numberOfClasses);
  for (j=0; j<numberOfClasses-1; j++)
    {
    classFrequency[j] = histogram->GetFrequency(thresholdIndexes[j]);
    freqSum += classFrequency[j];
    }
  classFrequency[numberOfClasses-1] = globalFrequency - freqSum;
  
  MeanType meanSum = NumericTraits<MeanType>::Zero;
  MeanVectorType classMean(numberOfClasses);
  for (j=0; j < numberOfClasses-1; j++)
    {
    if (NumericTraits<FrequencyType>::IsPositive(classFrequency[j]))
      {
      classMean[j] = static_cast<MeanType>(histogram->GetMeasurementVector(j)[0]);
      }
    else
      {
      classMean[j] = NumericTraits<MeanType>::Zero;
      }
    meanSum += classMean[j] * static_cast<MeanType>(classFrequency[j]);
    }

  if (NumericTraits<FrequencyType>::IsPositive(classFrequency[numberOfClasses-1]))
    {
    classMean[numberOfClasses-1] = (globalMean * static_cast<MeanType>(globalFrequency) - meanSum) / static_cast<MeanType>(classFrequency[numberOfClasses-1]);
    }
  else
    {
    classMean[numberOfClasses-1] = NumericTraits<MeanType>::Zero;
    }
  
  VarianceType maxVarBetween = NumericTraits<VarianceType>::Zero;
  for (j=0; j<numberOfClasses; j++)
    {
    maxVarBetween += static_cast<VarianceType>(classFrequency[j]) * static_cast<VarianceType>((globalMean - classMean[j]) * (globalMean - classMean[j]));
    }

  // explore all possible threshold configurations and choose the one that yields maximum between-class variance
  while (Self::IncrementThresholds(thresholdIndexes, globalMean, classMean, classFrequency))
    {
    VarianceType varBetween = NumericTraits<VarianceType>::Zero;
    for (j=0; j<numberOfClasses; j++)
      {
      varBetween += static_cast<VarianceType>(classFrequency[j]) * static_cast<VarianceType>((globalMean - classMean[j]) * (globalMean - classMean[j]));
      }

    if (varBetween > maxVarBetween)
      {
      maxVarBetween = varBetween;
      maxVarThresholdIndexes = thresholdIndexes;
      }
    }

  // copy corresponding bin max to threshold vector
  m_Output.resize(m_NumberOfThresholds);

  for (j=0; j<m_NumberOfThresholds; j++)
    {
    m_Output[j] = histogram->GetBinMax(0,maxVarThresholdIndexes[j]);
    }
}

template<class TInputHistogram>
void
OtsuMultipleThresholdsCalculator<TInputHistogram>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);

  os << indent << "NumberOfThresholds: " << m_NumberOfThresholds;

  os << indent << "Output: ";
  for (unsigned long j=0; j<m_NumberOfThresholds; j++)
    {
    os << m_Output[j] << " ";
    }
  os << std::endl;
}

} // end namespace itk

#endif
