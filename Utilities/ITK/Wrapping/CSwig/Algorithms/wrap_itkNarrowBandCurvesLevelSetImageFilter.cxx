/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: wrap_itkNarrowBandCurvesLevelSetImageFilter.cxx,v $
  Language:  C++
  Date:      $Date: 2003-09-10 14:30:12 $
  Version:   $Revision: 1.3 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkImage.h"
#include "itkNarrowBandCurvesLevelSetImageFilter.h"

#ifdef CABLE_CONFIGURATION
#include "itkCSwigMacros.h"
#include "itkCSwigImages.h"

namespace _cable_
{
  const char* const group = ITK_WRAP_GROUP(itkNarrowBandCurvesLevelSetImageFilter);
  namespace wrappers
  {
    ITK_WRAP_OBJECT2(NarrowBandCurvesLevelSetImageFilter, image::F2, image::F2,
                     itkNarrowBandCurvesLevelSetImageFilterF2F2);
    ITK_WRAP_OBJECT2(NarrowBandCurvesLevelSetImageFilter, image::F3, image::F3,
                     itkNarrowBandCurvesLevelSetImageFilterF3F3);
  }
}

#endif
