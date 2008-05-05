/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkFEMLoadImplementationGenericBodyLoad.cxx,v $
  Language:  C++
  Date:      $Date: 2003-12-15 14:13:20 $
  Version:   $Revision: 1.8 $

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

#include "itkFEMLoadImplementationGenericBodyLoad.h"

namespace itk {
namespace fem {




/*
 * Handle LoadGrav in element by integrating over the element domain.
 * This implementation requires that the element has the shape functions
 * and integration routines defined.
 *
 * It is also assumed, that element's local DOFs are numbered with respect
 * to node ID. If this is not the case, you should not use this function.
 */
void
LoadImplementationGenericBodyLoad
::Implementation(Element::ConstPointer element, LoadGrav::Pointer load, Element::VectorType& Fe)
{
  // Order of integration
  // FIXME: Allow changing the order of integration by setting a 
  //        static member within an element base class.
  unsigned int order=0;

  const unsigned int Nip=element->GetNumberOfIntegrationPoints(order);
  const unsigned int Ndofs=element->GetNumberOfDegreesOfFreedomPerNode();
  const unsigned int Nnodes=element->GetNumberOfNodes();

  Element::VectorType force(Ndofs,0.0),
                      ip,gip,force_tmp,shapeF;

  Fe.set_size(element->GetNumberOfDegreesOfFreedom());
  Fe.fill(0.0);

  Element::Float w,detJ;

  for(unsigned int i=0; i<Nip; i++)
  {
    element->GetIntegrationPointAndWeight(i,ip,w,order);
    gip=element->GetGlobalFromLocalCoordinates(ip);

    shapeF=element->ShapeFunctions(ip);
    detJ=element->JacobianDeterminant(ip);

    // Adjust the size of a force vector returned from the load object so
    // that it is equal to the number of DOFs per node. If the Fg returned
    // a vector with less dimensions, we add zero elements. If the Fg
    // returned a vector with more dimensions, we remove the extra dimensions.
    force.fill(0.0);
    // FIXME: Maybe Fg function should be declared as const in LoadGrav.
    force_tmp=load->Fg(gip);
    unsigned int Nd=Ndofs;
    if(force_tmp.size()<Nd) { Nd=force_tmp.size(); }
    for(unsigned int d=0; d<Nd; d++) { force[d] = force_tmp[d]; }

    // Claculate the equivalent nodal loads
    for(unsigned int n=0; n<Nnodes; n++)
    {
      for(unsigned int d=0; d<Ndofs; d++)
      {
        Fe[n*Ndofs+d]+=shapeF[n]*force[d]*w*detJ;
      }
    }

  }

}




}} // end namespace itk::fem
