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
#include "itkImageFileWriter.h"
#include <iostream>

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbTouziEdgeDetectorImageFilter.h"


int otbTouziEdgeDetectorDirection( int argc, char* argv[] )
{
  const char * inputFilename  = argv[1];
  const char * outputFilename = argv[2];

  unsigned int  RadiusX((unsigned int)::atoi(argv[3]));

  typedef unsigned char                                   InputPixelType;
  typedef double                                 OutputPixelType;
  const   unsigned int                                  Dimension = 2;

  typedef itk::Image< InputPixelType,  Dimension >        InputImageType;
  typedef itk::Image< OutputPixelType, Dimension >        OutputImageType;

  typedef otb::ImageFileReader< InputImageType  >         ReaderType;
  typedef otb::ImageFileWriter< OutputImageType >         WriterType;

  typedef otb::TouziEdgeDetectorImageFilter< InputImageType,OutputImageType >   FilterType;

  FilterType::SizeType Radius;
  Radius[0] = RadiusX;
  Radius[1] = RadiusX;

  FilterType::Pointer filterTouzi = FilterType::New();

  filterTouzi->SetRadius( Radius );

  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  reader->SetFileName( inputFilename  );
  writer->SetFileName( outputFilename );

  filterTouzi->SetInput( reader->GetOutput() );
  writer->SetInput( filterTouzi->GetOutputDirection() );

  writer->Update();


  return EXIT_SUCCESS;
}


