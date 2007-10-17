/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkSampleClassifier.txx,v $
  Language:  C++
  Date:      $Date: 2003/12/15 01:00:46 $
  Version:   $Revision: 1.16 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkSampleClassifier_txx
#define __itkSampleClassifier_txx

#include "itkSampleClassifier.h"

namespace itk{ 
namespace Statistics{

template< class TSample >
SampleClassifier< TSample >
::SampleClassifier()
{
  m_Sample = 0 ;
  m_Output = OutputType::New() ;
}

template< class TSample >
void
SampleClassifier< TSample >
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);

  os << indent << "Sample: " ;
  if ( m_Sample != 0 )
    {
    os << m_Sample << std::endl;
    }
  else
    {
    os << "not set." << std::endl ;
    }

  os << indent << "Output: " << m_Output << std::endl;
}

template< class TSample >
void
SampleClassifier< TSample >
::SetSample(const TSample* sample)
{
  if ( m_Sample != sample )
    {
    m_Sample = sample ;
    m_Output->SetSample(sample) ;
    }
}

template< class TSample >
const TSample*
SampleClassifier< TSample >
::GetSample() const
{
  return m_Sample ;
}

template< class TSample >
void
SampleClassifier< TSample >
::SetMembershipFunctionClassLabels(ClassLabelVectorType& labels)
{
  m_ClassLabels = labels ;
}

template< class TSample >
void
SampleClassifier< TSample >
::GenerateData()
{
  unsigned int i ;
  typename TSample::ConstIterator iter = this->GetSample()->Begin() ;
  typename TSample::ConstIterator end  = this->GetSample()->End() ;
  typename TSample::MeasurementVectorType measurements ;

  m_Output->Resize( this->GetSample()->Size() ) ;
  std::vector< double > discriminantScores ;
  unsigned int numberOfClasses = this->GetNumberOfClasses() ;
  discriminantScores.resize(numberOfClasses) ;
  unsigned int classLabel ;
  m_Output->SetNumberOfClasses(numberOfClasses) ;
  typename Superclass::DecisionRuleType::Pointer rule = 
    this->GetDecisionRule() ;

  if ( m_ClassLabels.size() != this->GetNumberOfMembershipFunctions() )
    {
    while (iter != end)
      {
      measurements = iter.GetMeasurementVector() ;
      for (i = 0 ; i < numberOfClasses ; i++)
        {
        discriminantScores[i] =
          (this->GetMembershipFunction(i))->Evaluate(measurements) ;
        }
      classLabel = rule->Evaluate(discriminantScores) ;
      m_Output->AddInstance(classLabel, iter.GetInstanceIdentifier()) ;
      ++iter ;
      }
    }
  else
    {
    while (iter != end)
      {
      measurements = iter.GetMeasurementVector() ;
      for (i = 0 ; i < numberOfClasses ; i++)
        {
        discriminantScores[i] = 
          (this->GetMembershipFunction(i))->Evaluate(measurements) ;
        }
      classLabel = rule->Evaluate(discriminantScores) ;
      m_Output->AddInstance(m_ClassLabels[classLabel], 
                            iter.GetInstanceIdentifier()) ;
      ++iter ;
      }
    }
}

template< class TSample >
typename SampleClassifier< TSample >::OutputType*
SampleClassifier< TSample >
::GetOutput() 
{
  return m_Output ;
}

} // end of namespace Statistics 
} // end of namespace itk

#endif








