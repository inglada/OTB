/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkTriangleCellTopology.cxx,v $
  Language:  C++
  Date:      $Date: 2009-03-03 15:11:07 $
  Version:   $Revision: 1.3 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "itkTriangleCellTopology.h"

namespace itk
{

/**
 * The triangle's topology data: Edges
 */
const int
TriangleCellTopology
::m_Edges[3][2] = { {0,1}, {1,2}, {2,0} };


TriangleCellTopology
::TriangleCellTopology()
{
}

TriangleCellTopology
::~TriangleCellTopology()
{
}

} // end namespace itk
