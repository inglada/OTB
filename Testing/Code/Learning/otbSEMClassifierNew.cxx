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
#include "itkImage.h"
#include "itkListSample.h"
#include "itkSampleClassifier.h"
#include "otbVectorImage.h"
#include "otbModelComponentBase.h"
#include "otbSEMClassifier.h"

#include <iostream>

int otbSEMClassifierNew(int argc, char* argv[])
{
  typedef double                                            PixelType;
  typedef otb::VectorImage<PixelType, 2>                    ImageType;
  typedef itk::Image<unsigned char, 2>                      OutputImageType;
  typedef otb::SEMClassifier<ImageType, OutputImageType>    ClassifType;
  typedef itk::Statistics::ListSample<ImageType::PixelType> SampleType;
  typedef itk::Statistics::Subsample<SampleType>            ClassSampleType;

  typedef otb::Statistics::ModelComponentBase<ClassSampleType> ComponentType;

  ClassifType::Pointer   classifier = ClassifType::New();
  ComponentType::Pointer component = ComponentType::New();

  std::cout << classifier << std::endl;

  return EXIT_SUCCESS;
}
