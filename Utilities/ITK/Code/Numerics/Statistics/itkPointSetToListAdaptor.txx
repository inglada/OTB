/*=========================================================================

Program:   Insight Segmentation & Registration Toolkit
Module:    $RCSfile: itkPointSetToListAdaptor.txx,v $
Language:  C++
Date:      $Date: 2005-09-30 14:14:18 $
Version:   $Revision: 1.12 $

Copyright (c) Insight Software Consortium. All rights reserved.
See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

This software is distributed WITHOUT ANY WARRANTY; without even 
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itkPointSetToListAdaptor_txx
#define _itkPointSetToListAdaptor_txx

#include "itkPointSetToListAdaptor.h"

namespace itk{ 
namespace Statistics{

template < class TPointSet >
PointSetToListAdaptor< TPointSet >
::PointSetToListAdaptor() : m_PointSet( 0 )
{
  // Same as the dimension of the point set
  Superclass::SetMeasurementVectorSize( MeasurementVectorSize );
}

template < class TPointSet >
void
PointSetToListAdaptor< TPointSet >
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);

  os << indent << "PointSet: " ;
  if ( m_PointSet.IsNotNull() )
    {
    os << m_PointSet << std::endl;
    }
  else
    {
    os << "not set." << std::endl ;
    }
}

template < class TPointSet >
void
PointSetToListAdaptor< TPointSet >
::SetPointSet(TPointSet* pointSet) 
{ 
  m_PointSet = pointSet ; 
  m_PointsContainer = pointSet->GetPoints() ;
}

template < class TPointSet >
TPointSet*
PointSetToListAdaptor< TPointSet >
::GetPointSet() 
{
  return m_PointSet.GetPointer() ; 
}  

/** returns the number of measurement vectors in this container*/
template < class TPointSet >
unsigned int
PointSetToListAdaptor< TPointSet >
::Size() const
{
  return m_PointsContainer->Size() ;
}

template < class TPointSet >
inline const typename PointSetToListAdaptor< TPointSet >::MeasurementVectorType & 
PointSetToListAdaptor< TPointSet >
::GetMeasurementVector(const InstanceIdentifier &id) const
{
  m_PointSet->GetPoint(id, &m_TempPoint) ;
  return (MeasurementVectorType&) m_TempPoint ;
}

template < class TPointSet >
inline typename PointSetToListAdaptor< TPointSet >::FrequencyType
PointSetToListAdaptor< TPointSet >
::GetFrequency(const InstanceIdentifier &) const 
{
  return 1 ;
}

template < class TPointSet >
typename PointSetToListAdaptor< TPointSet >::TotalFrequencyType
PointSetToListAdaptor< TPointSet >
::GetTotalFrequency() const
{ 
  return this->Size() ; 
}

} // end of namespace Statistics 
} // end of namespace itk

#endif



