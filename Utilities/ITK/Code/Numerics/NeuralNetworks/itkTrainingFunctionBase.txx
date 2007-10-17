/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkTrainingFunctionBase.txx,v $
  Language:  C++
  Date:      $Date: 2007/08/17 13:10:57 $
  Version:   $Revision: 1.3 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkTrainingFunctionBase_txx
#define __itkTrainingFunctionBase_txx

#include "itkTrainingFunctionBase.h"

namespace itk
{
namespace Statistics
{

template<class TSample, class TTargetVector, class ScalarType>
TrainingFunctionBase<TSample,TTargetVector,ScalarType>
::TrainingFunctionBase()
{
  m_PerformanceFunction = DefaultPerformanceType::New();
  m_Iterations = 0;
  m_TrainingSamples = NULL;
  m_SampleTargets = NULL;
  m_LearningRate = 1.0;
}

template<class TSample, class TTargetVector, class ScalarType>
void TrainingFunctionBase<TSample,TTargetVector,ScalarType>
::SetTrainingSamples(TSample* samples)
{
  m_TrainingSamples = samples;
  std::cout << "Training functionSample Size=" << samples->Size() << std::endl;
  typename TSample::ConstIterator iter = samples->Begin();
  while (iter != samples->End())
    {
    //m_InputSamples.push_back(defaultconverter(iter.GetMeasurementVector()));
    m_InputSamples.push_back(iter.GetMeasurementVector());
    
    ++iter;
    }
}

template<class TSample, class TTargetVector, class ScalarType>
void TrainingFunctionBase<TSample,TTargetVector,ScalarType>
::SetTargetValues(TTargetVector* targets)
{
  typename TTargetVector::ConstIterator iter = targets->Begin();
  while (iter != targets->End())
    {
    //m_Targets.push_back(targetconverter(iter.GetMeasurementVector()));
    m_Targets.push_back(iter.GetMeasurementVector());
    ++iter;
    }
  std::cout << "Num of Sample Targets converted= " << m_Targets.size()
            << std::endl;
  this->Modified();
}
template<class TSample, class TTargetVector, class ScalarType>
void
TrainingFunctionBase<TSample,TTargetVector,ScalarType>
::SetLearningRate(ValueType lr)
{
  m_LearningRate = lr;
  this->Modified();
}

template<class TSample, class TTargetVector, class ScalarType>
typename TrainingFunctionBase<TSample,TTargetVector,ScalarType>::ValueType
TrainingFunctionBase<TSample,TTargetVector,ScalarType>
::GetLearningRate()
{
  return m_LearningRate;
}

template<class TSample, class TTargetVector, class ScalarType>
void TrainingFunctionBase<TSample,TTargetVector,ScalarType>
::SetPerformanceFunction(PerformanceFunctionType* f)
{
  m_PerformanceFunction=f;
  this->Modified();
}


/** Print the object */
template<class TSample, class TTargetVector, class ScalarType>
void  
TrainingFunctionBase<TSample,TTargetVector,ScalarType>
::PrintSelf( std::ostream& os, Indent indent ) const 
{ 
  os << indent << "TrainingFunctionBase(" << this << ")" << std::endl;

  os << indent << "m_PerformanceFunction = " << m_PerformanceFunction << std::endl;
  os << indent << "m_Iterations = " << m_Iterations << std::endl;
  if(m_TrainingSamples)
    {
    os << indent << "m_TrainingSamples = " << m_TrainingSamples << std::endl;
    }

  if(m_SampleTargets)
    {
    os << indent << "m_SampleTargets = " << m_SampleTargets << std::endl;
    }
  //os << indent << "m_InputSamples = " << m_InputSamples << std::endl;
  //os << indent << "m_Targets = " << m_Targets << std::endl;
  os << indent << "m_LearningRate = " << m_LearningRate << std::endl;
  Superclass::PrintSelf( os, indent ); 
} 

} // end namespace Statistics
} // end namespace itk

#endif
