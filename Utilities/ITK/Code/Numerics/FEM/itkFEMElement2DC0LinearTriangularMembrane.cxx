/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkFEMElement2DC0LinearTriangularMembrane.cxx,v $
  Language:  C++
  Date:      $Date: 2003/09/10 14:29:39 $
  Version:   $Revision: 1.2 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

// disable debug warnings in MS compiler
#ifdef _MSC_VER
#pragma warning(disable: 4786)
#endif

#include "itkFEMElement2DC0LinearTriangularMembrane.h"

namespace itk {
namespace fem {


Element2DC0LinearTriangularMembrane
::Element2DC0LinearTriangularMembrane() : Superclass() {}

Element2DC0LinearTriangularMembrane
::Element2DC0LinearTriangularMembrane(
      NodeIDType n1_,
      NodeIDType n2_,
      NodeIDType n3_,
      Material::ConstPointer m_) : Superclass()
{
  // Set the geometrical points
  this->SetNode( 0, n1_ );
  this->SetNode( 1, n2_ );
  this->SetNode( 2, n3_ );

  /*
   * Initialize the pointer to material object and check that
   * we were given the pointer to the right class.
   * If the material class was incorrect an exception is thrown.
   */
  if( (m_mat=dynamic_cast<const MaterialLinearElasticity*>(&*m_)) == 0 )
  {
    throw FEMExceptionWrongClass(__FILE__,__LINE__,"Element2DC0LinearTriangularMembrane::Element2DC0LinearTriangularMembrane()");
  }
}




FEM_CLASS_REGISTER(Element2DC0LinearTriangularMembrane)




}} // end namespace itk::fem
