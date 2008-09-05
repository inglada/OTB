/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: wrap_itkDanielssonDistanceMapImageFilter.cxx,v $
  Language:  C++
  Date:      $Date: 2004-04-19 18:50:54 $
  Version:   $Revision: 1.1 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkDanielssonDistanceMapImageFilter.h"
#include "itkImage.h"

#ifdef CABLE_CONFIGURATION
#include "itkCSwigMacros.h"
#include "itkCSwigImages.h"

namespace _cable_
{
  const char* const group = ITK_WRAP_GROUP(itkDanielssonDistanceMapImageFilter);
  namespace wrappers
  {
    ITK_WRAP_OBJECT2(DanielssonDistanceMapImageFilter, image::F2, image::F2,
                     itkDanielssonDistanceMapImageFilterF2F2);
    ITK_WRAP_OBJECT2(DanielssonDistanceMapImageFilter, image::F2, image::US2,
                     itkDanielssonDistanceMapImageFilterF2US2);
    ITK_WRAP_OBJECT2(DanielssonDistanceMapImageFilter, image::US2, image::F2,
                     itkDanielssonDistanceMapImageFilterUS2F2);
    ITK_WRAP_OBJECT2(DanielssonDistanceMapImageFilter, image::F3, image::F3,
                     itkDanielssonDistanceMapImageFilterF3F3);
    ITK_WRAP_OBJECT2(DanielssonDistanceMapImageFilter, image::F3, image::US3,
                     itkDanielssonDistanceMapImageFilterF3US3);
    ITK_WRAP_OBJECT2(DanielssonDistanceMapImageFilter, image::US2, image::US2,
                     itkDanielssonDistanceMapImageFilterUS2US2);
    ITK_WRAP_OBJECT2(DanielssonDistanceMapImageFilter, image::US3, image::US3,
                     itkDanielssonDistanceMapImageFilterUS3US3);
    ITK_WRAP_OBJECT2(DanielssonDistanceMapImageFilter, image::US3, image::F3,
                     itkDanielssonDistanceMapImageFilterUS3F3);
  }
}
#endif
