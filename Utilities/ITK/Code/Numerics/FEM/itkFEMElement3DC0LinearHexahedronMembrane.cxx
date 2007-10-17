/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkFEMElement3DC0LinearHexahedronMembrane.cxx,v $
  Language:  C++
  Date:      $Date: 2003/09/10 14:29:40 $
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

#include "itkFEMElement3DC0LinearHexahedronMembrane.h"

namespace itk {
namespace fem {


Element3DC0LinearHexahedronMembrane
::Element3DC0LinearHexahedronMembrane() : Superclass()
{
}

Element3DC0LinearHexahedronMembrane
::Element3DC0LinearHexahedronMembrane(
      NodeIDType ns_[],
      Material::ConstPointer m_) : Superclass()
{
  // Set the geometrical points
  for (int k=0; k<8; k++) { 
    this->SetNode( k, ns_[k] ); 
  }

  /*
   * Initialize the pointer to material object and check that
   * we were given the pointer to the right class.
   * If the material class was incorrect an exception is thrown.
   */
  if( (m_mat=dynamic_cast<const MaterialLinearElasticity*>(&*m_)) == 0 )
  {
    throw FEMExceptionWrongClass(__FILE__,__LINE__,"Element3DC0LinearHexahedronMembrane::Element3DC0LinearHexahedronMembrane()");
  }
}




FEM_CLASS_REGISTER(Element3DC0LinearHexahedronMembrane)




}} // end namespace itk::fem
