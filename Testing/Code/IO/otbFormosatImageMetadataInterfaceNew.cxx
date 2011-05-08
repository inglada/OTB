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



#include "itkExceptionObject.h"

#include <iostream>

#include "otbFormosatImageMetadataInterface.h"

int otbFormosatImageMetadataInterfaceNew(int argc, char* argv[])
{

  otb::FormosatImageMetadataInterface::Pointer object = otb::FormosatImageMetadataInterface::New();

  std::cout << object << std::endl;

  return EXIT_SUCCESS;
}
