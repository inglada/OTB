/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: wrap_itkTernaryMagnitudeImageFilter.cxx,v $
  Language:  C++
  Date:      $Date: 2004-04-19 18:50:54 $
  Version:   $Revision: 1.1 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkTernaryMagnitudeImageFilter.h"
#include "itkImage.h"

#ifdef CABLE_CONFIGURATION
#include "itkCSwigMacros.h"
#include "itkCSwigImages.h"

namespace _cable_
{
  const char* const group = ITK_WRAP_GROUP(itkTernaryMagnitudeImageFilter);
  namespace wrappers
  {
    ITK_WRAP_OBJECT4_WITH_SUPERCLASS(TernaryMagnitudeImageFilter,
                                     image::F2, 
                                     image::F2, 
                                     image::F2, 
                                     image::F2, 
                                     itkTernaryMagnitudeImageFilterF2F2);
    ITK_WRAP_OBJECT4_WITH_SUPERCLASS(TernaryMagnitudeImageFilter,
                                     image::F3,
                                     image::F3,
                                     image::F3,
                                     image::F3, 
                                     itkTernaryMagnitudeImageFilterF3F3);
    ITK_WRAP_OBJECT4_WITH_SUPERCLASS(TernaryMagnitudeImageFilter,
                                     image::US2, 
                                     image::US2, 
                                     image::US2, 
                                     image::US2, 
                                     itkTernaryMagnitudeImageFilterUS2US2);
    ITK_WRAP_OBJECT4_WITH_SUPERCLASS(TernaryMagnitudeImageFilter,
                                     image::US3,
                                     image::US3,
                                     image::US3,
                                     image::US3, 
                                     itkTernaryMagnitudeImageFilterUS3US3);
    
  }
}
#endif
