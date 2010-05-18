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
#include "otbImage.h"
#include "itkVector.h"
#include "otbStreamingWarpImageFilter.h"

int otbStreamingWarpImageFilterNew(int argc, char* argv[])
{
  // Images definition
  const unsigned int Dimension = 2;
  typedef double                              PixelType;
  typedef itk::Vector<PixelType, 2>           DeformationValueType;
  typedef otb::Image<PixelType, Dimension>    ImageType;
  typedef otb::Image<DeformationValueType, 2> DeformationFieldType;

  // Warper
  typedef otb::StreamingWarpImageFilter<ImageType, ImageType, DeformationFieldType> ImageWarperType;

  // Objects creation
  ImageWarperType::Pointer warper = ImageWarperType::New();

  return EXIT_SUCCESS;
}
