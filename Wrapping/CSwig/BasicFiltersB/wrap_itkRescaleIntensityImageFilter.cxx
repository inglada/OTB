/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: wrap_itkRescaleIntensityImageFilter.cxx,v $
  Language:  C++
  Date:      $Date: 2004/04/21 20:24:21 $
  Version:   $Revision: 1.1 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkRescaleIntensityImageFilter.h"
#include "otbImage.h"

#ifdef CABLE_CONFIGURATION
#include "itkCSwigMacros.h"
#include "itkCSwigImages.h"

namespace _cable_
{
  const char* const group = ITK_WRAP_GROUP(itkRescaleIntensityImageFilter);
  namespace wrappers
  {
    ITK_WRAP_OBJECT2_WITH_SUPERCLASS(RescaleIntensityImageFilter, image::F2, image::US2, 
                                     itkRescaleIntensityImageFilterF2US2);
    ITK_WRAP_OBJECT2_WITH_SUPERCLASS(RescaleIntensityImageFilter, image::US2, image::F2, 
                                     itkRescaleIntensityImageFilterUS2F2);
    ITK_WRAP_OBJECT2_WITH_SUPERCLASS(RescaleIntensityImageFilter, image::F2, image::F2, 
                                     itkRescaleIntensityImageFilterF2F2);
    ITK_WRAP_OBJECT2_WITH_SUPERCLASS(RescaleIntensityImageFilter, image::F2, image::US2, 
                                     itkRescaleIntensityImageFilterF2US2);
    ITK_WRAP_OBJECT2_WITH_SUPERCLASS(RescaleIntensityImageFilter, image::F2, image::UC2, 
                                     itkRescaleIntensityImageFilterF2UC2);
    ITK_WRAP_OBJECT2_WITH_SUPERCLASS(RescaleIntensityImageFilter, image::US2, image::UC2, 
                                     itkRescaleIntensityImageFilterUS2UC2);
    ITK_WRAP_OBJECT2_WITH_SUPERCLASS(RescaleIntensityImageFilter, image::US2, image::US2, 
                                     itkRescaleIntensityImageFilterUS2US2);
    ITK_WRAP_OBJECT2_WITH_SUPERCLASS(RescaleIntensityImageFilter, image::F3, image::US3, 
                                     itkRescaleIntensityImageFilterF3US3);
    ITK_WRAP_OBJECT2_WITH_SUPERCLASS(RescaleIntensityImageFilter, image::F3, image::F3, 
                                     itkRescaleIntensityImageFilterF3F3);
    ITK_WRAP_OBJECT2_WITH_SUPERCLASS(RescaleIntensityImageFilter, image::US3, image::US3, 
                                     itkRescaleIntensityImageFilterUS3US3);
    ITK_WRAP_OBJECT2_WITH_SUPERCLASS(RescaleIntensityImageFilter, image::US3, image::F3, 
                                     itkRescaleIntensityImageFilterUS3F3);
    ITK_WRAP_OBJECT2_WITH_SUPERCLASS(RescaleIntensityImageFilter, image::F3, image::UC3, 
                                     itkRescaleIntensityImageFilterF3UC3);
    ITK_WRAP_OBJECT2_WITH_SUPERCLASS(RescaleIntensityImageFilter, image::US3, image::UC3, 
                                     itkRescaleIntensityImageFilterUS3UC3);
  }
}


#endif
