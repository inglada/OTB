/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkHexahedronCellTopology.h,v $
  Language:  C++
  Date:      $Date: 2008-10-17 01:21:45 $
  Version:   $Revision: 1.4 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkHexahedronCellTopology_h
#define __itkHexahedronCellTopology_h

#include "itkWin32Header.h"

namespace itk
{

/** \class HexahedronCellTopology
 * HexahedronCellTopology holds data defining the topological 
 * connections of the vertices and edges of an Hexahedron Cell.
 *
 * This class is used to localize static variables out of .txx 
 * files. This prevents multiple definition of static variables.
 *
 * \ingroup MeshObjects
 */

class ITKCommon_EXPORT HexahedronCellTopology
{
public:
  HexahedronCellTopology();
  ~HexahedronCellTopology();
 
protected:
   
  /** Hexahedron topology data. */
  static const int m_Edges[12][2];
  static const int m_Faces[6][4];
};

} // end namespace itk


#endif
