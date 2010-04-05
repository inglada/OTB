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
#include "otbSVMImageClassificationFilter.h"
#include "otbVectorImage.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbStreamingImageFileWriter.h"

int otbSVMImageClassificationFilter(int argc, char * argv[])
{
  const char * infname = argv[1];
  const char * modelfname = argv[2];
  const char * outfname = argv[3];

  const unsigned int Dimension = 2;
  typedef double         PixelType;
  typedef unsigned short LabeledPixelType;

  typedef otb::VectorImage<PixelType, Dimension>                         ImageType;
  typedef otb::Image<LabeledPixelType, Dimension>                        LabeledImageType;
  typedef otb::SVMImageClassificationFilter<ImageType, LabeledImageType> ClassificationFilterType;
  typedef ClassificationFilterType::ModelType                            ModelType;
  typedef otb::ImageFileReader<ImageType>                                ReaderType;
  typedef otb::StreamingImageFileWriter<LabeledImageType>                WriterType;

  // Instantiating object
  ClassificationFilterType::Pointer filter = ClassificationFilterType::New();

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(infname);

  ModelType::Pointer model = ModelType::New();
  model->LoadModel(modelfname);

  filter->SetModel(model);
  filter->SetInput(reader->GetOutput());

  WriterType::Pointer writer = WriterType::New();
  writer->SetInput(filter->GetOutput());
  writer->SetFileName(outfname);
  writer->Update();

  return EXIT_SUCCESS;
}
