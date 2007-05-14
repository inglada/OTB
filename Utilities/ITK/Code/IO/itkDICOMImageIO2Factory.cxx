/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkDICOMImageIO2Factory.cxx,v $
  Language:  C++
  Date:      $Date: 2004/01/08 00:39:04 $
  Version:   $Revision: 1.3 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkDICOMImageIO2Factory.h"
#include "itkCreateObjectFunction.h"
#include "itkDICOMImageIO2.h"
#include "itkVersion.h"

  
namespace itk
{
DICOMImageIO2Factory::DICOMImageIO2Factory()
{
  this->RegisterOverride("itkImageIOBase",
                         "itkDICOMImageIO2",
                         "DICOM Image IO",
                         1,
                         CreateObjectFunction<DICOMImageIO2>::New());
}
  
DICOMImageIO2Factory::~DICOMImageIO2Factory()
{
}

const char* 
DICOMImageIO2Factory::GetITKSourceVersion(void) const
{
  return ITK_SOURCE_VERSION;
}

const char* 
DICOMImageIO2Factory::GetDescription() const
{
  return "DICOM ImageIO Factory, allows the loading of DICOM images into Insight";
}

} // end namespace itk

