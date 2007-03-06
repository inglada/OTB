/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: wrap_itkImageFileWriter_3D.cxx,v $
  Language:  C++
  Date:      $Date: 2005/06/03 08:39:17 $
  Version:   $Revision: 1.2 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkImageFileWriter.h"
#include "otbImage.h"

#ifdef CABLE_CONFIGURATION
#include "itkCSwigMacros.h"
#include "itkCSwigImages.h"

namespace _cable_
{
  const char* const group = ITK_WRAP_GROUP(itkImageFileWriter_3D);
  namespace wrappers
  {
    ITK_WRAP_OBJECT1(ImageFileWriter, image::F3, itkImageFileWriterF3);
    ITK_WRAP_OBJECT1(ImageFileWriter, image::VF3, itkImageFileWriterVF3);
    ITK_WRAP_OBJECT1(ImageFileWriter, image::D3, itkImageFileWriterD3);
    ITK_WRAP_OBJECT1(ImageFileWriter, image::UC3, itkImageFileWriterUC3);
    ITK_WRAP_OBJECT1(ImageFileWriter, image::US3, itkImageFileWriterUS3);
    ITK_WRAP_OBJECT1(ImageFileWriter, image::UL3, itkImageFileWriterUL3);
    ITK_WRAP_OBJECT1(ImageFileWriter, image::UI3, itkImageFileWriterUI3);
    ITK_WRAP_OBJECT1(ImageFileWriter, image::SS3, itkImageFileWriterSS3);
    ITK_WRAP_OBJECT1(ImageFileWriter, image::SI3, itkImageFileWriterSI3);
  }
}

#endif
