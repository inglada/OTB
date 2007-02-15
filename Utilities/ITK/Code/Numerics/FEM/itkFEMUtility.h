/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkFEMUtility.h,v $
  Language:  C++
  Date:      $Date: 2003/09/10 14:29:44 $
  Version:   $Revision: 1.14 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkFEMUtility_h
#define __itkFEMUtility_h

#include <string>
#include <iostream>

namespace itk {
namespace fem {




/**
 * \file itkFEMUtility.h
 * \brief Includes various helper classes and functions used 
          througout the FEM code.
 */




/**
 * \class GaussIntegrate
 * \brief Use the Gauss-Legendre formula to perform integration
 *
 * Numerical integration (Gauss-Legendre formula).
 * Integrates function f(x) from x=a to x=b in n points.
 */
class GaussIntegrate {
public:
  static const double zero;
  static const double one;
  static const double two;
  static const double z[110];
  static const double w[110];
  double Integrate(double (*f)(double), double a, double b, int n=3);
};




}} /* end namespace itk */

#endif /* #ifndef __itkFEMUtility_h */
