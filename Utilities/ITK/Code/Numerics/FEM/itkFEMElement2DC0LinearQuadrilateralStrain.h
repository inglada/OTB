/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkFEMElement2DC0LinearQuadrilateralStrain.h,v $
  Language:  C++
  Date:      $Date: 2009-01-28 21:25:49 $
  Version:   $Revision: 1.3 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkFEMElement2DC0LinearQuadrilateralStrain_h
#define __itkFEMElement2DC0LinearQuadrilateralStrain_h

#include "itkFEMElement2DC0LinearQuadrilateral.h"
#include "itkFEMElement2DStrain.h"

namespace itk {
namespace fem {

/**
 * \class Element2DC0LinearQuadrilateralStrain
 * \brief 4-noded finite element class in 2D space for linear elasticity problem
 */
class Element2DC0LinearQuadrilateralStrain : public Element2DStrain<Element2DC0LinearQuadrilateral>
{
FEM_CLASS(Element2DC0LinearQuadrilateralStrain,Element2DStrain<Element2DC0LinearQuadrilateral>)
public:

  HANDLE_ELEMENT_LOADS();

  /**
   * Default constructor only clears the internal storage
   */
  Element2DC0LinearQuadrilateralStrain();

  /**
   * Construct an element by specifying pointers to
   * 4 points and a material.
   */
  Element2DC0LinearQuadrilateralStrain(
      NodeIDType n1_, 
      NodeIDType n2_,
      NodeIDType n3_,
      NodeIDType n4_,
      Material::ConstPointer p_ );

}; // class Element2DC0LinearQuadrilateralStrain

FEM_CLASS_INIT(Element2DC0LinearQuadrilateralStrain)
}} // end namespace itk::fem

#endif  // #ifndef __itkFEMElement2DC0LinearQuadrilateralStrain_h
