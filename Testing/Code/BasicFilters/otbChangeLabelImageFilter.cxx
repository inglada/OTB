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

#include "otbVectorImage.h"
#include "otbImage.h"
#include "itkRandomImageSource.h"
#include "otbChangeLabelImageFilter.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "otbImageFileWriter.h"
#include "otbImageFileReader.h"

int otbChangeLabelImageFilter(int argc, char * argv[])
{
  const unsigned int ImageDimension = 2;
  const char * inputFilename(argv[1]);
  const char * outFilename(argv[2]);
  typedef unsigned short PixelType;
  typedef otb::Image<PixelType, ImageDimension>                   InputImageType;
  typedef otb::VectorImage<PixelType, ImageDimension>             OutputImageType;
  typedef InputImageType::PixelType                                    InputPixelType;
  typedef OutputImageType::PixelType                                   OutputPixelType;
  typedef itk::ImageRegionIteratorWithIndex<InputImageType>            InputIteratorType;
  typedef itk::ImageRegionIteratorWithIndex<OutputImageType>           OutputIteratorType;
  typedef itk::RandomImageSource<InputImageType>                       SourceType;
  typedef otb::ChangeLabelImageFilter<InputImageType, OutputImageType> FilterType;
  typedef otb::ImageFileWriter<OutputImageType> WriterType;
  typedef otb::ImageFileReader<InputImageType> ReaderType;

  SourceType::Pointer source = SourceType::New();
  FilterType::Pointer filter = FilterType::New();
  InputImageType::Pointer  vectImage  = InputImageType::New();

  WriterType::Pointer writer = WriterType::New();
  ReaderType::Pointer reader = ReaderType::New();


  reader->SetFileName( inputFilename  );
  writer->SetFileName( outFilename );
  PixelType maskValue =  static_cast<PixelType>(atoi(argv[3]));
  InputPixelType lower = static_cast<PixelType>(atoi(argv[4]));
  InputPixelType upper = static_cast<PixelType>(atoi(argv[5]));

  filter->SetNumberOfComponentsPerPixel(3);
  OutputPixelType background;
  background.SetSize( filter->GetNumberOfComponentsPerPixel() );
  background.Fill(maskValue);

  filter->SetChange( 0,0 );
  for (InputPixelType i = lower; i <= upper; i++)
  {
    filter->SetChange( i, background );
  }

  filter->SetInput( reader->GetOutput() );
  writer->SetInput(filter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
