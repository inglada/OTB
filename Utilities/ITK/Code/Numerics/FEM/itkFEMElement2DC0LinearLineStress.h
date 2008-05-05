/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkFEMElement2DC0LinearLineStress.h,v $
  Language:  C++
  Date:      $Date: 2003-09-10 14:29:39 $
  Version:   $Revision: 1.4 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkFEMElement2DC0LinearLineStress_h
#define __itkFEMElement2DC0LinearLineStress_h

#include "itkFEMElement2DC0LinearLine.h"
#include "itkFEMElement1DStress.h"

namespace itk {
namespace fem {




/**
 * \class Element2DC0LinearLineStress
 * \brief 2-noded finite element class in 2D space for linear elasticity problem.
 */
class Element2DC0LinearLineStress : public Element1DStress<Element2DC0LinearLine>
{
FEM_CLASS(Element2DC0LinearLineStress,Element1DStress<Element2DC0LinearLine>)
public:

  HANDLE_ELEMENT_LOADS();

  /**
   * Default constructor only clears the internal storage
   */
  Element2DC0LinearLineStress();

  /**
   * Construct an element by specifying pointers to
   * 4 points and a material.
   */
  Element2DC0LinearLineStress(
      NodeIDType n1_, 
      NodeIDType n2_,
      Material::ConstPointer p_ );

  /**
   * Mass matrix
   */
  // FIMXE: this implementation should be made general and moved to base class
  virtual void GetMassMatrix( MatrixType& Me ) const;

}; // class Element2DC0LinearLineStress

FEM_CLASS_INIT(Element2DC0LinearLineStress)




}} // end namespace itk::fem

#endif  // #ifndef __itkFEMElement2DC0LinearLineStress_h
