/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: wrap_itkThresholdSegmentationLevelSetImageFilter.cxx,v $
  Language:  C++
  Date:      $Date: 2005/01/26 17:15:22 $
  Version:   $Revision: 1.5 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "otbImage.h"
#include "itkThresholdSegmentationLevelSetImageFilter.h"

#ifdef CABLE_CONFIGURATION
#include "itkCSwigMacros.h"
#include "itkCSwigImages.h"

namespace _cable_
{
  const char* const group = ITK_WRAP_GROUP(itkThresholdSegmentationLevelSetImageFilter);
  namespace wrappers
  {
    ITK_WRAP_OBJECT2(ThresholdSegmentationLevelSetImageFilter, image::F3, image::F3,
                     itkThresholdSegmentationLevelSetImageFilterF3F3);
    ITK_WRAP_OBJECT2(ThresholdSegmentationLevelSetImageFilter, image::F2, image::F2,
                     itkThresholdSegmentationLevelSetImageFilterF2F2);
  }
}

#endif
