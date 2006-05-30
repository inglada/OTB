/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkFEMElement3DC0LinearHexahedronStrain.h,v $
  Language:  C++
  Date:      $Date: 2003/09/10 14:29:41 $
  Version:   $Revision: 1.5 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkFEMElement3DC0LinearHexahedronStrain_h
#define __itkFEMElement3DC0LinearHexahedronStrain_h

#include "itkFEMElement3DC0LinearHexahedron.h"
#include "itkFEMElement3DStrain.h"

namespace itk {
namespace fem {




/**
 * \class Element3DC0LinearHexahedronStrain
 * \brief 8-noded finite element class in 3D space for linear elasticity problem
 */
class Element3DC0LinearHexahedronStrain : public Element3DStrain<Element3DC0LinearHexahedron>
{
FEM_CLASS(Element3DC0LinearHexahedronStrain,Element3DStrain<Element3DC0LinearHexahedron>)
public:

  HANDLE_ELEMENT_LOADS();

  /**
   * Default constructor only clears the internal storage
   */
  Element3DC0LinearHexahedronStrain();

  /**
   * Construct an element by specifying pointers to
   * an array of 8 points and a material.
   */
  Element3DC0LinearHexahedronStrain(
      NodeIDType ns_[], 
      Material::ConstPointer p_ );

}; // class Element3DC0LinearHexahedronStrain

FEM_CLASS_INIT(Element3DC0LinearHexahedronStrain)




}} // end namespace itk::fem

#endif  // #ifndef __itkFEMElement3DC0LinearHexahedronStrain_h
