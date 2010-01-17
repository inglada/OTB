/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: wrap_itkLevelSetFunction.cxx,v $
  Language:  C++
  Date:      $Date: 2005-01-25 22:50:35 $
  Version:   $Revision: 1.1 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkImage.h"
#include "itkLevelSetFunction.h"

#ifdef CABLE_CONFIGURATION
#include "itkCSwigMacros.h"
#include "itkCSwigImages.h"

namespace _cable_
{
  const char* const group = ITK_WRAP_GROUP(itkLevelSetFunction);
  namespace wrappers
  {
    
    ITK_WRAP_OBJECT1(LevelSetFunction,image::F2,itkLevelSetFunctionF2);
    ITK_WRAP_OBJECT1(LevelSetFunction,image::F3,itkLevelSetFunctionF3);
  }
}

#endif
