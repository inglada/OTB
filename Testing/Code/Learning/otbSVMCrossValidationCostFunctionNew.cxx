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
#include "otbImage.h"
#include <iostream>

#include "otbSVMModel.h"
#include "otbSVMCrossValidationCostFunction.h"

int otbSVMCrossValidationCostFunctionNew( int argc, char* argv[] )
{
  typedef unsigned char                                   InputPixelType;
  typedef unsigned char                                   LabelPixelType;
  const   unsigned int                                    Dimension = 2;

  typedef otb::Image< InputPixelType,  Dimension >         InputImageType;
  typedef otb::SVMModel< InputPixelType, LabelPixelType >  ModelType;
  typedef otb::SVMCrossValidationCostFunction<ModelType>   FunctionType;

  FunctionType::Pointer function = FunctionType::New();

  return EXIT_SUCCESS;
}


