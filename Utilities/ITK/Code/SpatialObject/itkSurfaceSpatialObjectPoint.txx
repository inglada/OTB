/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkSurfaceSpatialObjectPoint.txx,v $
  Language:  C++
  Date:      $Date: 2004/09/09 22:29:20 $
  Version:   $Revision: 1.8 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkSurfaceSpatialObjectPoint_txx
#define __itkSurfaceSpatialObjectPoint_txx

#include "itkSurfaceSpatialObjectPoint.h"

namespace itk 
{

/** Constructor */
template< unsigned int TPointDimension >
SurfaceSpatialObjectPoint< TPointDimension >
::SurfaceSpatialObjectPoint( void ) 
{ 
  this->m_ID = 0;
  m_Normal.Fill(0);
}

/** Destructor */
template< unsigned int TPointDimension >
SurfaceSpatialObjectPoint< TPointDimension >
::~SurfaceSpatialObjectPoint( void ) 
{
}

/** Set the normal : N-D case */
template< unsigned int TPointDimension >
void 
SurfaceSpatialObjectPoint< TPointDimension >
::SetNormal( const VectorType & normal ) 
{
  m_Normal = normal;
}

/** Set the normal : 2D case */
template< unsigned int TPointDimension >
void 
SurfaceSpatialObjectPoint< TPointDimension >
::SetNormal(const double normalx, const double normaly)
{
  m_Normal[0] = normalx;
  m_Normal[1] = normaly;
}

/** Set the normal : 3D case */
template< unsigned int TPointDimension >
void 
SurfaceSpatialObjectPoint< TPointDimension >
::SetNormal(const double normalx, const double normaly, const double normalz)
{
  m_Normal[0] = normalx;
  m_Normal[1] = normaly;
  m_Normal[2] = normalz;
}

/** Get the normal at one point */
template< unsigned int TPointDimension >
const typename SurfaceSpatialObjectPoint< TPointDimension >::VectorType &
SurfaceSpatialObjectPoint< TPointDimension >
::GetNormal( void ) const
{
  return m_Normal;
}
  
/** Print the object */
template< unsigned int TPointDimension >
void 
SurfaceSpatialObjectPoint< TPointDimension >
::PrintSelf( std::ostream& os, Indent indent ) const 
{ 
  Superclass::PrintSelf(os,indent);
  os << indent << "SurfaceSpatialObjectPoint(" << this << ")" << std::endl; 
  os << indent << "Normal definition: ";
  os << indent <<  m_Normal << std::endl;
}

/** Copy a surface point to another */
template< unsigned int TPointDimension >
typename SurfaceSpatialObjectPoint< TPointDimension >::Self & 
SurfaceSpatialObjectPoint< TPointDimension >
::operator=(const SurfaceSpatialObjectPoint & rhs) 
{
  this->m_ID = rhs.m_ID;
  this->m_X = rhs.m_X;
  return * this;
}

} // end namespace itk

#endif
