/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkGaussianRadialBasisFunction.txx,v $
  Language:  C++
  Date:      $Date: 2006/04/18 11:23:29 $
  Version:   $Revision: 1.4 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkGaussianRadialBasisFunction_txx
#define __itkGaussianRadialBasisFunction_txx

#include "itkGaussianRadialBasisFunction.h"

#include <math.h>

namespace itk
{
namespace Statistics
{

/** Constructor */
template<class ScalarType>
GaussianRadialBasisFunction< ScalarType>
::GaussianRadialBasisFunction()
{
}

/** Destructor */
template<class ScalarType>
GaussianRadialBasisFunction< ScalarType>
::~GaussianRadialBasisFunction()
{
}

/** Evaluate function */
template<class ScalarType>
ScalarType
GaussianRadialBasisFunction< ScalarType>
::Evaluate(const ScalarType& input) const
{ 
  ScalarType val;
  ScalarType radius = Superclass::GetRadius(); 
  val = vcl_exp(-0.5*vcl_pow(input,2)/vcl_pow(radius,2)); 
  return val;
}

/** Evaluate derivative function */
template<class ScalarType>
ScalarType
GaussianRadialBasisFunction< ScalarType>
::EvaluateDerivative(const ScalarType& dist,const ArrayType& input,
                          char mode,int element_id) const
{
  ScalarType val = 0;;
  ScalarType radius=Superclass::GetRadius(); 
  ArrayType center = Superclass::GetCenter();
  if(mode=='u') //w.r.t centers
    {
    ScalarType temp1= vcl_pow(radius,2);
    val=Evaluate(dist)
                      *(input.GetElement(element_id)-center.GetElement(element_id))/temp1;
    }
  else if(mode=='s') // w.r.t radius
    {
    val=Evaluate(dist)*vcl_pow(dist,2)/vcl_pow(radius,3);
    }
  return val;
}

/** Print the object */
template<class ScalarType>
void  
GaussianRadialBasisFunction<ScalarType>
::PrintSelf( std::ostream& os, Indent indent ) const 
{ 
  os << indent << "GaussianRadialBasisFunction(" << this << ")" << std::endl; 
  Superclass::PrintSelf( os, indent ); 
} 

} // end namespace Statistics
} // end namespace itk



#endif
