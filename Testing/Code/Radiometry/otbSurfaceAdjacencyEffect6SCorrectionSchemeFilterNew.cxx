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

#include "otbSurfaceAdjacencyEffect6SCorrectionSchemeFilter.h"
#include "otbVectorImage.h"
#include "otbImage.h"

int otbSurfaceAdjacencyEffect6SCorrectionSchemeFilterNew(int argc, char * argv[])
{
  const unsigned int Dimension = 2;
  typedef double                                 PixelType;
  typedef otb::VectorImage<PixelType, Dimension> InputImageType;
  typedef otb::SurfaceAdjacencyEffect6SCorrectionSchemeFilter<InputImageType,
      InputImageType>
  SurfaceAdjacencyEffect6SCorrectionSchemeFilterType;

  // Instantiating object
  SurfaceAdjacencyEffect6SCorrectionSchemeFilterType::Pointer filter =
    SurfaceAdjacencyEffect6SCorrectionSchemeFilterType::New();

  return EXIT_SUCCESS;
}
