/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: wrap_itkImageSeriesWriter.cxx,v $
  Language:  C++
  Date:      $Date: 2003/10/30 21:20:57 $
  Version:   $Revision: 1.1 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkImageSeriesWriter.h"
#include "otbImage.h"

#ifdef CABLE_CONFIGURATION
#include "itkCSwigMacros.h"
#include "itkCSwigImages.h"

namespace _cable_
{
  const char* const group = ITK_WRAP_GROUP(itkImageSeriesWriter);
  namespace wrappers
  {
    ITK_WRAP_OBJECT2(ImageSeriesWriter, image::F3,  image::F2,  itkImageSeriesWriterF3F2);
    ITK_WRAP_OBJECT2(ImageSeriesWriter, image::US3, image::US2, itkImageSeriesWriterUS3US2);
  }
}

#endif
