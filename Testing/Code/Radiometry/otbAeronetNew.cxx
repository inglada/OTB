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
#include "otbAeronetData.h"
#include "otbAeronetFileReader.h"

int otbAeronetNew(int argc, char * argv[])
{
  otb::AeronetData::Pointer       data = otb::AeronetData::New();
  otb::AeronetFileReader::Pointer reader = otb::AeronetFileReader::New();

  std::cout << data << std::endl;
  std::cout << reader << std::endl;

  return EXIT_SUCCESS;
}
