/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkBackwardDifferenceOperator.txx,v $
  Language:  C++
  Date:      $Date: 2003/09/10 14:29:01 $
  Version:   $Revision: 1.18 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itkBackwardDifferenceOperator_txx
#define _itkBackwardDifferenceOperator_txx
#include "itkBackwardDifferenceOperator.h"
namespace itk
{

template <class TPixel, unsigned int VDimension, class TAllocator>
typename BackwardDifferenceOperator<TPixel, VDimension, TAllocator>
::CoefficientVector
BackwardDifferenceOperator<TPixel, VDimension, TAllocator>
::GenerateCoefficients()
{
  CoefficientVector coeff(3);
  coeff[0] = -1.0f *  NumericTraits<PixelType>::One;
  coeff[1] =  NumericTraits<PixelType>::One;
  coeff[2] =  NumericTraits<PixelType>::Zero;

  return coeff;
}
  
} // namespace itk

#endif
