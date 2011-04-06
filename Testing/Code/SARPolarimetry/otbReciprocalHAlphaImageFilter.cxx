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
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbReciprocalHAlphaImageFilter.h"

int otbReciprocalHAlphaImageFilter(int argc, char * argv[])
{
  const char * inputFilename  = argv[1];
  const char * outputFilename = argv[2];

  typedef double                   PixelType;
  typedef std::complex<PixelType>  InputPixelType;
  const unsigned int Dimension = 2;

  
  typedef otb::VectorImage<InputPixelType, Dimension>  ImageType;
  typedef otb::VectorImage<PixelType, Dimension>       RealImageType;
 
  typedef otb::ReciprocalHAlphaImageFilter<ImageType, RealImageType> FilterType;

  typedef otb::ImageFileReader<ImageType>  ReaderType;
  typedef otb::ImageFileWriter<RealImageType> WriterType;

  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  reader->SetFileName(inputFilename);

  FilterType::Pointer filter = FilterType::New();
  filter->SetInput(reader->GetOutput());

  writer->SetFileName(outputFilename);
  writer->SetInput(filter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
