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
#include "otbVectorImage.h"
#include "otbImage.h"
#include "otbMeanShiftVectorImageFilter.h"

int otbMeanShiftVectorImageFilterNew(int argc, char * argv[])
{
  const unsigned int Dimension = 2;
  typedef short                                                 PixelType;
  typedef otb::VectorImage<PixelType, Dimension>                ImageType;
  typedef otb::MeanShiftVectorImageFilter<ImageType, ImageType> FilterType;

  // Instantiating object
  FilterType::Pointer filter = FilterType::New();

  std::cout << filter << std::endl;

  return EXIT_SUCCESS;
}
