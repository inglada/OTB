/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkFEMElement2DC0LinearTriangularStrain.h,v $
  Language:  C++
  Date:      $Date: 2003/09/10 14:29:40 $
  Version:   $Revision: 1.2 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkFEMElement2DC0LinearTriangularStrain_h
#define __itkFEMElement2DC0LinearTriangularStrain_h

#include "itkFEMElement2DC0LinearTriangular.h"
#include "itkFEMElement2DStrain.h"

namespace itk {
namespace fem {




/**
 * \class Element2DC0LinearTriangularStrain
 * \brief 3-noded finite element class in 2D space for linear elasticity problem.
 *
 * This element is combined from Element2DC0LinearTriangular and Element2DStrain.
 */
class Element2DC0LinearTriangularStrain : public Element2DStrain<Element2DC0LinearTriangular>
{
FEM_CLASS(Element2DC0LinearTriangularStrain,Element2DStrain<Element2DC0LinearTriangular>)
public:

  HANDLE_ELEMENT_LOADS();

  /**
   * Default constructor only clears the internal storage
   */
  Element2DC0LinearTriangularStrain();

  /**
   * Construct an element by specifying pointers to
   * 3 points and a material.
   */
  Element2DC0LinearTriangularStrain(
      NodeIDType n1_, 
      NodeIDType n2_,
      NodeIDType n3_,
      Material::ConstPointer p_ );

}; // class Element2DC0LinearTriangularStrain 

FEM_CLASS_INIT(Element2DC0LinearTriangularStrain)




}} // end namespace itk::fem

#endif  // #ifndef __itkFEMElement2DC0LinearTriangularStrain_h
