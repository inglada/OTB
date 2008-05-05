/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkSubsample.txx,v $
  Language:  C++
  Date:      $Date: 2003-12-15 01:00:46 $
  Version:   $Revision: 1.18 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkSubsample_txx
#define __itkSubsample_txx

#include "itkObject.h"

namespace itk{ 
namespace Statistics{

template< class TSample >
Subsample< TSample >
::Subsample()
{
  m_Sample = 0 ;
  m_TotalFrequency = NumericTraits< FrequencyType >::Zero ;
  m_ActiveDimension = 0 ;
}

template< class TSample >
void
Subsample< TSample >
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);

  os << indent << "Sample: " ;
  if ( m_Sample != 0 )
    {
    os << m_Sample << std::endl ;
    }
  else
    {
    os << "not set." << std::endl ;
    }

  os << indent << "TotalFrequency: " << m_TotalFrequency << std::endl ;
  os << indent << "ActiveDimension: " << m_ActiveDimension << std::endl ;
  os << indent << "InstanceIdentifierHolder : " << &m_IdHolder << std::endl ;
}

template< class TSample >
inline typename Subsample< TSample >::MeasurementVectorType
Subsample< TSample >
::GetMeasurementVectorByIndex(int index) const
{
  return m_Sample->GetMeasurementVector(m_IdHolder[index]) ;
}

template< class TSample >
inline typename Subsample< TSample >::FrequencyType
Subsample< TSample >
::GetFrequencyByIndex(int index) const
{
  return m_Sample->GetFrequency(m_IdHolder[index]) ;
}

template< class TSample >
inline typename Subsample< TSample >::InstanceIdentifier
Subsample< TSample >
::GetInstanceIdentifier(int index) const
{
  return m_IdHolder[index] ;
}

template< class TSample >
inline void
Subsample< TSample >
::Swap(int index1, int index2)
{
  InstanceIdentifier temp = m_IdHolder[index1] ;
  m_IdHolder[index1] = m_IdHolder[index2] ;
  m_IdHolder[index2] = temp ;
}

} // end of namespace Statistics 
} // end of namespace itk

#endif







