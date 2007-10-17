/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkPNGImageIOFactory.cxx,v $
  Language:  C++
  Date:      $Date: 2007/03/22 14:28:51 $
  Version:   $Revision: 1.12 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkPNGImageIOFactory.h"
#include "itkCreateObjectFunction.h"
#include "itkPNGImageIO.h"
#include "itkVersion.h"

namespace itk
{

PNGImageIOFactory::PNGImageIOFactory()
{
  this->RegisterOverride("itkImageIOBase",
                         "itkPNGImageIO",
                         "PNG Image IO",
                         1,
                         CreateObjectFunction<PNGImageIO>::New());
}
  
PNGImageIOFactory::~PNGImageIOFactory()
{
}

const char* 
PNGImageIOFactory::GetITKSourceVersion(void) const
{
  return ITK_SOURCE_VERSION;
}

const char* 
PNGImageIOFactory::GetDescription(void) const
{
  return "PNG ImageIO Factory, allows the loading of PNG images into insight";
}

} // end namespace itk
