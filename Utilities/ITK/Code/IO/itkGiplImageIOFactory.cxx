/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkGiplImageIOFactory.cxx,v $
  Language:  C++
  Date:      $Date: 2007-03-22 14:28:50 $
  Version:   $Revision: 1.5 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkGiplImageIOFactory.h"
#include "itkCreateObjectFunction.h"
#include "itkGiplImageIO.h"
#include "itkVersion.h"

  
namespace itk
{

GiplImageIOFactory::GiplImageIOFactory()
{
  this->RegisterOverride("itkImageIOBase",
                         "itkGiplImageIO",
                         "Gipl Image IO",
                         1,
                         CreateObjectFunction<GiplImageIO>::New());
}
  
GiplImageIOFactory::~GiplImageIOFactory()
{
}

const char* 
GiplImageIOFactory::GetITKSourceVersion(void) const
{
  return ITK_SOURCE_VERSION;
}

const char* 
GiplImageIOFactory::GetDescription() const
{
  return "Gipl ImageIO Factory, allows the loading of Gipl images into Insight";
}

} // end namespace itk
