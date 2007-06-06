/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkQuadEdgeMeshTopologyChecker.txx,v $
  Language:  C++
  Date:      $Date: 2007/02/25 15:07:14 $
  Version:   $Revision: 1.4 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


#ifndef __itkQuadEdgeMeshTopologyChecker_txx
#define __itkQuadEdgeMeshTopologyChecker_txx

#include "itkQuadEdgeMeshTopologyChecker.h"

namespace itk
{

template< class TMesh >
QuadEdgeMeshTopologyChecker< TMesh >
::QuadEdgeMeshTopologyChecker()
{
}


template< class TMesh >
bool
QuadEdgeMeshTopologyChecker< TMesh >
::ValidateEulerCharacteristic() const
{
  // FIXME move implementation here
  return true;
}

template< class TMesh >
void
QuadEdgeMeshTopologyChecker< TMesh >
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);

  os << indent << "ExpectedNumberOfPoints: "
    << static_cast<long>(m_ExpectedNumberOfPoints) << std::endl;
  os << indent << "ExpectedNumberOfEdges: "
    << static_cast<long>(m_ExpectedNumberOfEdges) << std::endl;
  os << indent << "ExpectedNumberOfFaces: "
    << static_cast<long>(m_ExpectedNumberOfFaces) << std::endl;
  os << indent << "ExpectedNumberOfBoundaries: "
    << static_cast<long>(m_ExpectedNumberOfBoundaries) << std::endl;
  os << indent << "ExpectedGenus: "
    << static_cast<long>(m_ExpectedGenus) << std::endl;
  os << indent << "Mesh: " << m_Mesh << std::endl;
}

}
#endif 
