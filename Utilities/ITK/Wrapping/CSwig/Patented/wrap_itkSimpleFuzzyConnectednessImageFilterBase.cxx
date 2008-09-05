/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: wrap_itkSimpleFuzzyConnectednessImageFilterBase.cxx,v $
  Language:  C++
  Date:      $Date: 2004-10-09 01:49:55 $
  Version:   $Revision: 1.1 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkImage.h"
#include "itkSimpleFuzzyConnectednessImageFilterBase.h"

#ifdef CABLE_CONFIGURATION
#include "itkCSwigMacros.h"
#include "itkCSwigImages.h"

namespace _cable_
{
  const char* const group = ITK_WRAP_GROUP(itkSimpleFuzzyConnectednessImageFilterBase);
  namespace wrappers
  {
    ITK_WRAP_OBJECT2(SimpleFuzzyConnectednessImageFilterBase, image::F2, image::F2,
                     itkSimpleFuzzyConnectednessImageFilterBaseF2F2);
    ITK_WRAP_OBJECT2(SimpleFuzzyConnectednessImageFilterBase, image::F3, image::F3,
                     itkSimpleFuzzyConnectednessImageFilterBaseF3F3);
    ITK_WRAP_OBJECT2(SimpleFuzzyConnectednessImageFilterBase, image::US2, image::US2,
                     itkSimpleFuzzyConnectednessImageFilterBaseUS2US2);
    ITK_WRAP_OBJECT2(SimpleFuzzyConnectednessImageFilterBase, image::US3, image::US3,
                     itkSimpleFuzzyConnectednessImageFilterBaseUS3US3);
  }
}

#endif
