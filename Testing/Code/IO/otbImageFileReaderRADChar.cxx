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
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbExtractROI.h"

int otbImageFileReaderRADChar(int argc, char* argv[])
{
  // Verify the number of parameters in the command line
  const char * inputFilename  = argv[1];
  const char * outputFilename = argv[2];

  typedef char                          InputPixelType;
  typedef char                     OutputPixelType;


  const   unsigned int                                  Dimension = 2;

  typedef otb::Image< InputPixelType,  Dimension >        InputImageType;
  typedef otb::Image< OutputPixelType, Dimension >        OutputImageType;

  typedef otb::ImageFileReader< InputImageType  >         ReaderType;
  typedef otb::ImageFileWriter< OutputImageType >         WriterType;
/*
  typedef otb::VectorImage< InputPixelType,  Dimension >     InputImageType;
  typedef otb::VectorImage< OutputPixelType, Dimension >     OutputImageType;
*/
  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  reader->SetFileName( inputFilename  );
  writer->SetFileName( outputFilename );

  writer->SetInput( reader->GetOutput() );
  writer->Update();

  return EXIT_SUCCESS;
}

