/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkSymmetricEllipsoidInteriorExteriorSpatialFunction.txx,v $
  Language:  C++
  Date:      $Date: 2009-03-03 15:09:43 $
  Version:   $Revision: 1.18 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkSymmetricEllipsoidInteriorExteriorSpatialFunction_txx
#define __itkSymmetricEllipsoidInteriorExteriorSpatialFunction_txx

#include "itkSymmetricEllipsoidInteriorExteriorSpatialFunction.h"
#include <math.h>

namespace itk
{

template <unsigned int VDimension, typename TInput>
SymmetricEllipsoidInteriorExteriorSpatialFunction<VDimension,TInput>
::SymmetricEllipsoidInteriorExteriorSpatialFunction()
{
  m_Center.Fill(0.0); // Origin of ellipsoid
  m_Orientation.Fill(1.0);  // Orientation of unique axis
  m_UniqueAxis = 10;  // Length of unique axis
  m_SymmetricAxes = 5; // Length of symmetric axes
}

template <unsigned int VDimension, typename TInput>
SymmetricEllipsoidInteriorExteriorSpatialFunction<VDimension,TInput>
::~SymmetricEllipsoidInteriorExteriorSpatialFunction()
{

}

template <unsigned int VDimension,typename TInput>
typename SymmetricEllipsoidInteriorExteriorSpatialFunction<VDimension, TInput>::OutputType 
SymmetricEllipsoidInteriorExteriorSpatialFunction<VDimension, TInput>
::Evaluate(const InputType& position) const
{
  double uniqueTerm;     // Term in ellipsoid equation for unique axis
  double symmetricTerm;  // Term in ellipsoid equation for symmetric axes
  Vector<double, VDimension> pointVector;
  Vector<double, VDimension> symmetricVector;
  
  // Project the position onto the major axis, normalize by axis length, 
  // and determine whether position is inside ellipsoid.
  for(unsigned int i = 0; i < VDimension; i++)
    {
    pointVector[i] = position[i] - m_Center[i];
    }

  uniqueTerm = vcl_pow(static_cast<double>(((pointVector * m_Orientation)/(.5*m_UniqueAxis))),static_cast<double>(2));
  symmetricVector = pointVector - (m_Orientation * (pointVector * m_Orientation));
  symmetricTerm = vcl_pow(static_cast<double>(((symmetricVector.GetNorm())/(.5*m_SymmetricAxes))),static_cast<double>(2));

  if((uniqueTerm + symmetricTerm) >= 0 && (uniqueTerm + symmetricTerm) <= 1)
    {
    return 1; // Inside the ellipsoid.
    }
  //Default return value assumes outside the ellipsoid 
  return 0; // Outside the ellipsoid.
}

template <unsigned int VDimension,typename TInput>
void SymmetricEllipsoidInteriorExteriorSpatialFunction<VDimension,TInput>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "Origin of Ellipsoid: ";
  os << m_Center << std::endl;
  os << indent << "Unique Axis Orientation: ";
  os << m_Orientation << std::endl;
  os << indent << "Unique Axis Length: ";
  os << m_UniqueAxis << std::endl;
  os << indent << "Symmetric Axis Length: ";
  os << m_SymmetricAxes << std::endl;
}

template <unsigned int VDimension,typename TInput>
void SymmetricEllipsoidInteriorExteriorSpatialFunction<VDimension, TInput>
::SetOrientation(VectorType orientation, double uniqueAxis, double symmetricAxes)
{
  m_Orientation = orientation;  // Orientation of unique axis of ellipsoid
  m_SymmetricAxes = symmetricAxes;  // Length of symmetric axes
  m_UniqueAxis = uniqueAxis;  // Length of unique axis
}

} // end namespace itk

#endif
