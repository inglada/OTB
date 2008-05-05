/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkListSample.txx,v $
  Language:  C++
  Date:      $Date: 2007-08-17 18:04:42 $
  Version:   $Revision: 1.14 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkListSample_txx
#define __itkListSample_txx

#include "itkListSample.h"

namespace itk{ 
namespace Statistics{

template< class TMeasurementVector >
ListSample< TMeasurementVector >
::ListSample()
{
}


template< class TMeasurementVector >
const typename ListSample< TMeasurementVector >::MeasurementVectorType &
ListSample< TMeasurementVector >
::GetMeasurementVector(const InstanceIdentifier &identifier) const 
{
  if ( identifier < m_InternalContainer.size() )
    {
    return m_InternalContainer[identifier] ;
    }
  throw ExceptionObject(__FILE__,__LINE__);  
}

template< class TMeasurementVector >
void 
ListSample< TMeasurementVector >
::SetMeasurement(const InstanceIdentifier &identifier, 
                 const unsigned int &dim,
                 const MeasurementType &value)
{
  if ( identifier < m_InternalContainer.size() )
    {
    m_InternalContainer[identifier][dim] = value ;
    }
}

template< class TMeasurementVector >
void
ListSample< TMeasurementVector >
::SetMeasurementVector(const InstanceIdentifier &identifier, 
                       const MeasurementVectorType &mv)
{
  if ( identifier < m_InternalContainer.size() )
    {
    m_InternalContainer[identifier] = mv ;
    }
}

template< class TMeasurementVector >
typename ListSample< TMeasurementVector >::FrequencyType 
ListSample< TMeasurementVector >
::GetFrequency(const InstanceIdentifier &identifier) const
{
  if ( identifier < m_InternalContainer.size() )
    {
    return 1.0 ;
    }
  else
    {
    return 0.0 ;
    }
}

template< class TMeasurementVector >
void 
ListSample< TMeasurementVector >
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);

  os << indent << "Internal Data Container: "
     << &m_InternalContainer << std::endl;
  os << indent << "Number of samples: " 
     << this->m_InternalContainer.size() << std::endl;
}
} // end of namespace Statistics
} // end of namespace itk 

#endif
