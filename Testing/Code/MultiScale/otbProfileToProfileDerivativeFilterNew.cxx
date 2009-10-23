/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE,  See the above copyright notices for more information.

=========================================================================*/
#include "otbProfileToProfileDerivativeFilter.h"
#include "otbImage.h"

#include "itkMacro.h"

int otbProfileToProfileDerivativeFilterNew(int argc, char * argv[])
{
  const unsigned int Dimension = 2;
  typedef double InputPixelType;
  typedef double OutputPixelType;

  typedef otb::Image<InputPixelType,Dimension> InputImageType;
  typedef otb::Image<OutputPixelType,Dimension> OutputImageType;
  typedef otb::ProfileToProfileDerivativeFilter<InputImageType,OutputImageType> DerivativeFilterType;

  DerivativeFilterType::Pointer derivativeFilter = DerivativeFilterType::New();

  return EXIT_SUCCESS;
}
