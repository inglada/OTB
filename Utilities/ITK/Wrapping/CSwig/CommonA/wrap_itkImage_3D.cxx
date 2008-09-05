/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: wrap_itkImage_3D.cxx,v $
  Language:  C++
  Date:      $Date: 2005-04-01 16:30:14 $
  Version:   $Revision: 1.2 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkImage.h"
#include "itkVector.h"
#include "itkCovariantVector.h"

#ifdef CABLE_CONFIGURATION
#include "itkCSwigImages.h"
#include "itkCSwigMacros.h"

namespace _cable_
{
  const char* const group = ITK_WRAP_GROUP(itkImage_3D);
  namespace wrappers
  {
    ITK_WRAP_OBJECT1(ImageBase, 3, itkImageBase3);
    ITK_WRAP_OBJECT2(Image, float, 3, itkImageF3);
    ITK_WRAP_OBJECT2(Image, double, 3, itkImageD3);
    ITK_WRAP_OBJECT2(Image, unsigned char, 3, itkImageUC3);
    ITK_WRAP_OBJECT2(Image, unsigned short, 3, itkImageUS3);
    ITK_WRAP_OBJECT2(Image, unsigned int, 3, itkImageUI3);
    ITK_WRAP_OBJECT2(Image, unsigned long, 3, itkImageUL3);
    ITK_WRAP_OBJECT2(Image, signed char, 3, itkImageSC3);
    ITK_WRAP_OBJECT2(Image, signed short, 3, itkImageSS3);
    ITK_WRAP_OBJECT2(Image, signed int, 3, itkImageSI3);

    ITK_WRAP_OBJECT2(Image, itkvector::F3, 3, itkImageVF3);
    ITK_WRAP_OBJECT2(Image, itkvector::D3, 3, itkImageVD3);
    ITK_WRAP_OBJECT2(Image, covariantvector::F3, 3, itkImageCVF3);
    ITK_WRAP_OBJECT2(Image, covariantvector::D3, 3, itkImageCVD3);


//    typedef image::F2::PixelContainer::Self itkImageF_PixelContainer;
//    typedef image::D2::PixelContainer::Self itkImageD_PixelContainer;
//    typedef image::UC2::PixelContainer::Self itkImageUC_PixelContainer;
//    typedef image::US2::PixelContainer::Self itkImageUS_PixelContainer;
//    typedef image::UI2::PixelContainer::Self itkImageUI_PixelContainer;
//    typedef image::SC2::PixelContainer::Self itkImageSC_PixelContainer;
//    typedef image::SS2::PixelContainer::Self itkImageSS_PixelContainer;
//    typedef image::SI2::PixelContainer::Self itkImageSI_PixelContainer;

//    typedef itkImageF_PixelContainer::Pointer::SmartPointer itkImageF_PixelContainer_Pointer;
//    typedef itkImageD_PixelContainer::Pointer::SmartPointer itkImageD_PixelContainer_Pointer;
//    typedef itkImageUC_PixelContainer::Pointer::SmartPointer itkImageUC_PixelContainer_Pointer;
//    typedef itkImageUS_PixelContainer::Pointer::SmartPointer itkImageUS_PixelContainer_Pointer;
//    typedef itkImageUI_PixelContainer::Pointer::SmartPointer itkImageUI_PixelContainer_Pointer;
//    typedef itkImageSC_PixelContainer::Pointer::SmartPointer itkImageSC_PixelContainer_Pointer;
//    typedef itkImageSS_PixelContainer::Pointer::SmartPointer itkImageSS_PixelContainer_Pointer;
//    typedef itkImageSI_PixelContainer::Pointer::SmartPointer itkImageSI_PixelContainer_Pointer;
  }
}

#endif
