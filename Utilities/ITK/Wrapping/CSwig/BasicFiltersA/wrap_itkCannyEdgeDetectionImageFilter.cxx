/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: wrap_itkCannyEdgeDetectionImageFilter.cxx,v $
  Language:  C++
  Date:      $Date: 2004-04-19 18:50:54 $
  Version:   $Revision: 1.1 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkCannyEdgeDetectionImageFilter.h"
#include "itkImage.h"

#ifdef CABLE_CONFIGURATION
#include "itkCSwigMacros.h"
#include "itkCSwigImages.h"

namespace _cable_
{
  const char* const group = ITK_WRAP_GROUP(itkCannyEdgeDetectionImageFilter);
  namespace wrappers
  {
    ITK_WRAP_OBJECT2(CannyEdgeDetectionImageFilter, image::F2, image::F2,
                     itkCannyEdgeDetectionImageFilterF2F2);
    ITK_WRAP_OBJECT2(CannyEdgeDetectionImageFilter, image::F3, image::F3,
                     itkCannyEdgeDetectionImageFilterF3F3);
  }
}

#endif
