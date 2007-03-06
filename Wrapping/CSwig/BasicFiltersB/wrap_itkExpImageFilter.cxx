/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: wrap_itkExpImageFilter.cxx,v $
  Language:  C++
  Date:      $Date: 2005/01/13 18:37:50 $
  Version:   $Revision: 1.2 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkExpImageFilter.h"
#include "otbImage.h"

#ifdef CABLE_CONFIGURATION
#include "itkCSwigMacros.h"
#include "itkCSwigImages.h"

namespace _cable_
{
  const char* const group = ITK_WRAP_GROUP(itkExpImageFilter);
  namespace wrappers
  {
    ITK_WRAP_OBJECT2_WITH_SUPERCLASS(ExpImageFilter, image::F2, image::F2,
                     itkExpImageFilterF2F2);
    ITK_WRAP_OBJECT2_WITH_SUPERCLASS(ExpImageFilter, image::F3, image::F3,
                     itkExpImageFilterF3F3);
    ITK_WRAP_OBJECT2_WITH_SUPERCLASS(ExpImageFilter, image::US2, image::US2,
                     itkExpImageFilterUS2US2);
    ITK_WRAP_OBJECT2_WITH_SUPERCLASS(ExpImageFilter, image::US3, image::US3,
                     itkExpImageFilterUS3US3);
  }
}

#endif
