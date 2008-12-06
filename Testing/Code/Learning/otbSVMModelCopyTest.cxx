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


int otbSVMModelCopyTest( int argc, char* argv[] )
{
  typedef unsigned char                                   InputPixelType;
  typedef unsigned char                                   LabelPixelType;
  const   unsigned int        	                          Dimension = 2;

  typedef otb::Image< InputPixelType,  Dimension >        InputImageType;
  typedef otb::SVMModel< InputPixelType, LabelPixelType > ModelType;

  ModelType::Pointer svmModel = ModelType::New();

  svmModel->LoadModel(argv[1]);

  ModelType::Pointer svmModelCopy = svmModel->GetCopy();
  svmModelCopy->SaveModel(argv[2]);


  return EXIT_SUCCESS;
}


