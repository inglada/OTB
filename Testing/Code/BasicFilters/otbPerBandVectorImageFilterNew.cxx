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

#include "otbPerBandVectorImageFilter.h"
#include "otbVectorImage.h"
#include "otbImage.h"
#include "itkMeanImageFilter.h"

int otbPerBandVectorImageFilterNew(int argc, char * argv[])
{
  const unsigned int Dimension = 2;
  typedef unsigned char                              PixelType;
  typedef otb::Image<PixelType, Dimension>           ImageType;
  typedef otb::VectorImage<PixelType, Dimension>     VectorImageType;
  typedef itk::MeanImageFilter<ImageType, ImageType> MeanFilterType;

  typedef otb::PerBandVectorImageFilter<VectorImageType, VectorImageType, MeanFilterType>
  PerBandVectorImageFilterType;

  // Instantiating object
  PerBandVectorImageFilterType::Pointer object = PerBandVectorImageFilterType::New();

  return EXIT_SUCCESS;
}
