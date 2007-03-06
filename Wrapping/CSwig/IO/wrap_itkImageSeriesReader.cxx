/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: wrap_itkImageSeriesReader.cxx,v $
  Language:  C++
  Date:      $Date: 2003/10/30 00:15:10 $
  Version:   $Revision: 1.1 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkImageSeriesReader.h"
#include "otbImage.h"

#ifdef CABLE_CONFIGURATION
#include "itkCSwigMacros.h"
#include "itkCSwigImages.h"

namespace _cable_
{
  const char* const group = ITK_WRAP_GROUP(itkImageSeriesReader);
  namespace wrappers
  {
    ITK_WRAP_OBJECT1(ImageSeriesReader, image::F2, itkImageSeriesReaderF2);
    ITK_WRAP_OBJECT1(ImageSeriesReader, image::F3, itkImageSeriesReaderF3);
    ITK_WRAP_OBJECT1(ImageSeriesReader, image::US2, itkImageSeriesReaderUS2);
    ITK_WRAP_OBJECT1(ImageSeriesReader, image::US3, itkImageSeriesReaderUS3);
    ITK_WRAP_OBJECT1(ImageSeriesReader, image::UC2, itkImageSeriesReaderUC2);
    ITK_WRAP_OBJECT1(ImageSeriesReader, image::UC3, itkImageSeriesReaderUC3);
  }
}

#endif
