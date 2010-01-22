/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: wrap_itkVTKImageExport.cxx,v $
  Language:  C++
  Date:      $Date: 2005-01-13 18:37:50 $
  Version:   $Revision: 1.2 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkVTKImageExport.h"
#include "itkImage.h"

#ifdef CABLE_CONFIGURATION
#include "itkCSwigMacros.h"
#include "itkCSwigImages.h"

namespace _cable_
{
  const char* const group = ITK_WRAP_GROUP(itkVTKImageExport);
  namespace wrappers
  {
    ITK_WRAP_OBJECT(VTKImageExportBase);
    ITK_WRAP_OBJECT1(VTKImageExport, image::F2,
                                     itkVTKImageExportF2);
    ITK_WRAP_OBJECT1(VTKImageExport, image::UC2,
                                     itkVTKImageExportUC2);
    ITK_WRAP_OBJECT1(VTKImageExport, image::US2,
                                     itkVTKImageExportUS2);
    ITK_WRAP_OBJECT1(VTKImageExport, image::UL2,
                                     itkVTKImageExportUL2);
  
    ITK_WRAP_OBJECT1(VTKImageExport, image::F3, 
                                     itkVTKImageExportF3);
    ITK_WRAP_OBJECT1(VTKImageExport, image::UC3, 
                                     itkVTKImageExportUC3);
    ITK_WRAP_OBJECT1(VTKImageExport, image::US3, 
                                     itkVTKImageExportUS3);
    ITK_WRAP_OBJECT1(VTKImageExport, image::UL3, 
                                     itkVTKImageExportUL3);
  
  }
}

#endif
