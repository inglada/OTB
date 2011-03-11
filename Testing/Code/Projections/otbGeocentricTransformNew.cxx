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
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "otbMacro.h"
#include "otbImage.h"
#include "otbGeocentricTransform.h"

int otbGeocentricTransformNew(int argc, char* argv[])
{

  typedef otb::GeocentricTransform<otb::TransformDirection::FORWARD> ProjectionType;
  ProjectionType::Pointer projection = ProjectionType::New();

  std::cout << projection << std::endl;

  return EXIT_SUCCESS;
}
