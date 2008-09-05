/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkTetrahedronCellTopology.h,v $
  Language:  C++
  Date:      $Date: 2003-09-10 14:29:27 $
  Version:   $Revision: 1.3 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkTetrahedronCellTopology_h
#define __itkTetrahedronCellTopology_h

#include "itkWin32Header.h"

namespace itk
{

/** \class TetrahedronCellTopology
 * TetrahedronCellTopology holds data defining the topological 
 * connections of the vertices and edges of an Tetrahedron Cell.
 *
 * This class is used to localize static variables out of .txx 
 * files. This prevents multiple definition of static variables.
 *
 * \ingroup MeshObjects
 */

class ITKCommon_EXPORT TetrahedronCellTopology
{
protected:
   
  /** Tetrahedron topology data. */
  static const int m_Edges[6][2];
  static const int m_Faces[4][3];
    
public:
  TetrahedronCellTopology();
  ~TetrahedronCellTopology();
  
};

} // end namespace itk


#endif
