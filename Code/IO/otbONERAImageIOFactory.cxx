/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "otbONERAImageIOFactory.h"

#include "itkCreateObjectFunction.h"
#include "otbONERAImageIO.h"
#include "itkVersion.h"

namespace otb
{

ONERAImageIOFactory::ONERAImageIOFactory()
{
  this->RegisterOverride("itkImageIOBase",
                         "otbONERAImageIO",
                         "ONERA Image IO",
                         1,
                         itk::CreateObjectFunction<ONERAImageIO>::New());
}

ONERAImageIOFactory::~ONERAImageIOFactory()
{
}

const char*
ONERAImageIOFactory::GetITKSourceVersion(void) const
{
  return ITK_SOURCE_VERSION;
}

const char*
ONERAImageIOFactory::GetDescription() const
{
  return "ONERA ImageIO Factory, permettant le chargement d'image au format ONERA dans l'OTB";
}

} // end namespace otb
