/*=========================================================================

Program:   Insight Segmentation & Registration Toolkit
Module:    $RCSfile: itkAnalyzeImageIOFactory.cxx,v $
Language:  C++
Date:      $Date: 2007/03/22 14:28:47 $
Version:   $Revision: 1.6 $

Copyright (c) Insight Software Consortium. All rights reserved.
See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkAnalyzeImageIOFactory.h"
#include "itkCreateObjectFunction.h"
#include "itkAnalyzeImageIO.h"
#include "itkVersion.h"


namespace itk
{
void AnalyzeImageIOFactory::PrintSelf(std::ostream&, Indent) const
{

}


AnalyzeImageIOFactory::AnalyzeImageIOFactory()
{
  this->RegisterOverride("itkImageIOBase",
                         "itkAnalyzeImageIO",
                         "Analyze Image IO",
                         1,
                         CreateObjectFunction<AnalyzeImageIO>::New());
}

AnalyzeImageIOFactory::~AnalyzeImageIOFactory()
{
}

const char*
AnalyzeImageIOFactory::GetITKSourceVersion(void) const
{
  return ITK_SOURCE_VERSION;
}

const char*
AnalyzeImageIOFactory::GetDescription() const
{
  return "Analyze ImageIO Factory, allows the loading of Analyze images into insight";
}

} // end namespace itk
