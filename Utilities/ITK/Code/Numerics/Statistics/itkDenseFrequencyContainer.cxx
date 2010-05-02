/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkDenseFrequencyContainer.cxx,v $
  Language:  C++
  Date:      $Date: 2009-08-08 20:19:11 $
  Version:   $Revision: 1.7 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkDenseFrequencyContainer.h"

namespace itk{ 
namespace Statistics{

DenseFrequencyContainer
::DenseFrequencyContainer()
{
  m_FrequencyContainer = FrequencyContainerType::New();
  m_TotalFrequency = NumericTraits< TotalFrequencyType >::Zero;
} 

void
DenseFrequencyContainer
::Initialize(unsigned long length) 
{   
  m_FrequencyContainer->Reserve(length);
  this->SetToZero();
}

void
DenseFrequencyContainer
::SetToZero() 
{   
  m_FrequencyContainer->Fill( NumericTraits< FrequencyType >::Zero );
  m_TotalFrequency = NumericTraits< TotalFrequencyType >::Zero;
}

bool
DenseFrequencyContainer
::SetFrequency(const InstanceIdentifier id, const FrequencyType value)
{ 
  if( id >= m_FrequencyContainer->Size() )
    {
    return false;
    }
  FrequencyType frequency = this->GetFrequency(id);
  (*m_FrequencyContainer)[id] = value; 
  m_TotalFrequency += (value - frequency);
  return true;
}

DenseFrequencyContainer::FrequencyType
DenseFrequencyContainer
::GetFrequency(const InstanceIdentifier id) const
{
  if( id >= m_FrequencyContainer->Size() )
    {
    return NumericTraits< FrequencyType >::Zero;
    }
  return (*m_FrequencyContainer)[id];
}

bool
DenseFrequencyContainer 
::IncreaseFrequency(const InstanceIdentifier id, const FrequencyType value)
{
  if( id >= m_FrequencyContainer->Size() )
    {
    return false;
    }
  FrequencyType frequency = this->GetFrequency(id);

  const FrequencyType largestIntegerThatFitsInFloat = 16777216;

  if( largestIntegerThatFitsInFloat - frequency < value )
    {
    itkExceptionMacro("Frequency container saturated for Instance ");
    }
  else
    {
    (*m_FrequencyContainer)[id] = frequency + value; 
    }

  m_TotalFrequency += value;

  return true;
}

void
DenseFrequencyContainer
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);
}

} // end of namespace Statistics
} // end of namespace itk 
