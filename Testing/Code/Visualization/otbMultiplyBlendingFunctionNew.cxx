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
#include "otbMultiplyBlendingFunction.h"
#include "itkRGBPixel.h"

int otbMultiplyBlendingFunctionNew( int argc, char * argv[] )
{
  typedef itk::RGBPixel<unsigned char> RGBPixelType;
  typedef otb::Function::MultiplyBlendingFunction
    <RGBPixelType> BlendingFunctionType;
  BlendingFunctionType::Pointer function = BlendingFunctionType::New();

  return EXIT_SUCCESS;
}
