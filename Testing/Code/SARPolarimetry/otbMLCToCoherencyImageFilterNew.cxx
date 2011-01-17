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

#include "itkExceptionObject.h"
#include <iostream>

#include "otbVectorImage.h"
#include "otbMLCToCoherencyImageFilter.h"

int otbMLCToCoherencyImageFilterNew(int argc, char * argv[])
{
  const unsigned int Dimension = 2;

  typedef std::complex<double>   PixelType;
  typedef otb::VectorImage<PixelType, Dimension> ImageType;


  typedef otb::MLCToCoherencyImageFilter<ImageType, ImageType> FilterType;

  FilterType::Pointer filter = FilterType::New();

  std::cout << filter << std::endl;

  return EXIT_SUCCESS;
}
