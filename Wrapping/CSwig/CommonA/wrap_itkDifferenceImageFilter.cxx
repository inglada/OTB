/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: wrap_itkDifferenceImageFilter.cxx,v $
  Language:  C++
  Date:      $Date: 2005/03/25 13:17:57 $
  Version:   $Revision: 1.1 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "otbImage.h"
#include "itkDifferenceImageFilter.h"

#ifdef CABLE_CONFIGURATION
#include "itkCSwigImages.h"
#include "itkCSwigMacros.h"

namespace _cable_
{
  const char* const group = ITK_WRAP_GROUP(itkDifferenceImageFilter);
  namespace wrappers
  {
    ITK_WRAP_OBJECT2(DifferenceImageFilter, image::F2, image::F2,
                       itkDifferenceImageFilterF2);
    ITK_WRAP_OBJECT2(DifferenceImageFilter, image::F3, image::F3,
                       itkDifferenceImageFilterF3);
    ITK_WRAP_OBJECT2(DifferenceImageFilter, image::US2, image::US2,
                       itkDifferenceImageFilterUS2);
    ITK_WRAP_OBJECT2(DifferenceImageFilter, image::US3, image::US3,
                       itkDifferenceImageFilterUS3);
  }
}
#endif
