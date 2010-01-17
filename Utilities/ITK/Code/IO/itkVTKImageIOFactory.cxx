/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkVTKImageIOFactory.cxx,v $
  Language:  C++
  Date:      $Date: 2007-03-22 14:28:53 $
  Version:   $Revision: 1.8 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

  Portions of this code are covered under the VTK copyright.
  See VTKCopyright.txt or http://www.kitware.com/VTKCopyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkVTKImageIOFactory.h"
#include "itkCreateObjectFunction.h"
#include "itkVTKImageIO.h"
#include "itkVersion.h"

namespace itk
{

VTKImageIOFactory::VTKImageIOFactory()
{
  this->RegisterOverride("itkImageIOBase",
                         "itkVTKImageIO",
                         "VTK Image IO",
                         1,
                         CreateObjectFunction<VTKImageIO>::New());
}
  
VTKImageIOFactory::~VTKImageIOFactory()
{
}

const char* 
VTKImageIOFactory::GetITKSourceVersion(void) const
{
  return ITK_SOURCE_VERSION;
}

const char* 
VTKImageIOFactory::GetDescription(void) const
{
  return "VTK ImageIO Factory, allows the loading of VTK images into ITK";
}

} // end namespace itk
