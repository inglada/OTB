/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: wrap_itkBinaryDilateImageFilter.cxx,v $
  Language:  C++
  Date:      $Date: 2004/04/19 18:50:54 $
  Version:   $Revision: 1.1 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkBinaryDilateImageFilter.h"
#include "itkBinaryBallStructuringElement.h"
#include "otbImage.h"

#ifdef CABLE_CONFIGURATION
#include "itkCSwigMacros.h"
#include "itkCSwigImages.h"
#include "itkCSwigBinaryBallStructuringElement.h"

namespace _cable_
{
  const char* const group = ITK_WRAP_GROUP(itkBinaryDilateImageFilter);
  namespace wrappers
  {
    // NOTE: since both the BinaryDilateImageFilter and BinaryErodeImageFilter derive from the same superclass, only one of
    //       them should do the wrapping WITH_SUPERCLASS.
    //
    //===========2D Wrapped Filters==============
    ITK_WRAP_OBJECT3(BinaryDilateImageFilter, image::F2 , image::F2 , structuringElement::F2,   itkBinaryDilateImageFilterF2F2  );
    ITK_WRAP_OBJECT3(BinaryDilateImageFilter, image::UC2, image::UC2, structuringElement::UC2,  itkBinaryDilateImageFilterUC2UC2);
    ITK_WRAP_OBJECT3(BinaryDilateImageFilter, image::US2, image::US2, structuringElement::US2,  itkBinaryDilateImageFilterUS2US2);

    //===========3D Wrapped Filters==============
    ITK_WRAP_OBJECT3(BinaryDilateImageFilter, image::F3 , image::F3 , structuringElement::F3,   itkBinaryDilateImageFilterF3F3  );
    ITK_WRAP_OBJECT3(BinaryDilateImageFilter, image::UC3, image::UC3, structuringElement::UC3,  itkBinaryDilateImageFilterUC3UC3);
    ITK_WRAP_OBJECT3(BinaryDilateImageFilter, image::US3, image::US3, structuringElement::US3,  itkBinaryDilateImageFilterUS3US3);
  }
}


#endif
