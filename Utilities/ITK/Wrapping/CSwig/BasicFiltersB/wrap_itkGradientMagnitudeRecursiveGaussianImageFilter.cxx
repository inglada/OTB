/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: wrap_itkGradientMagnitudeRecursiveGaussianImageFilter.cxx,v $
  Language:  C++
  Date:      $Date: 2004-04-21 20:24:21 $
  Version:   $Revision: 1.1 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkGradientMagnitudeRecursiveGaussianImageFilter.h"
#include "itkImage.h"

#ifdef CABLE_CONFIGURATION
#include "itkCSwigMacros.h"
#include "itkCSwigImages.h"

namespace _cable_
{
  const char* const group = ITK_WRAP_GROUP(itkGradientMagnitudeRecursiveGaussianImageFilter);
  namespace wrappers
  {
    ITK_WRAP_OBJECT2(GradientMagnitudeRecursiveGaussianImageFilter, image::F2, image::F2,
                     itkGradientMagnitudeRecursiveGaussianImageFilterF2F2);
    ITK_WRAP_OBJECT2(GradientMagnitudeRecursiveGaussianImageFilter, image::F3, image::F3,
                     itkGradientMagnitudeRecursiveGaussianImageFilterF3F3);
    ITK_WRAP_OBJECT2(GradientMagnitudeRecursiveGaussianImageFilter, image::US2, image::US2,
                     itkGradientMagnitudeRecursiveGaussianImageFilterUS2US2);
    ITK_WRAP_OBJECT2(GradientMagnitudeRecursiveGaussianImageFilter, image::US3, image::US3,
                     itkGradientMagnitudeRecursiveGaussianImageFilterUS3US3);
  }
}

#endif
