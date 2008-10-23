/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkDistanceMetric.txx,v $
  Language:  C++
  Date:      $Date: 2005-11-21 02:40:48 $
  Version:   $Revision: 1.8 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.
  
     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkDistanceMetric_txx
#define __itkDistanceMetric_txx

#include "itkDistanceMetric.h"

namespace itk{ 
namespace Statistics{

template< class TVector >
DistanceMetric< TVector >
::DistanceMetric() 
{
  this->m_MeasurementVectorSize = 0;
}

template< class TVector >
void 
DistanceMetric< TVector >
::SetOrigin(const OriginType &x)
{
  if( this->m_MeasurementVectorSize != 0 )
    {  
    if( x.Size() != this->m_MeasurementVectorSize )
      {
      itkExceptionMacro( << "Size of the origin must be same as the length of"
          << " each measurement vector.");
      }
    }
  m_Origin = x ;
  this->m_MeasurementVectorSize = x.Size();
  this->Modified();
}


template< class TVector >
void 
DistanceMetric< TVector >
::SetMeasurementVectorSize( const MeasurementVectorSizeType s )
{
  if( s == this->m_MeasurementVectorSize )
    {
    return;
    }
  
  if( this->m_MeasurementVectorSize != 0 )
    {  
    itkWarningMacro( << "Destructively resizing paramters of the DistanceMetric." );
    }
  this->m_MeasurementVectorSize = s;
  m_Origin.SetSize( s );
  this->Modified();
}  


template< class TVector >
void 
DistanceMetric< TVector >
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);
  os << indent << "Origin: " << m_Origin << std::endl;
}

} // end of namespace Statistics 
} // end of namespace itk

#endif







