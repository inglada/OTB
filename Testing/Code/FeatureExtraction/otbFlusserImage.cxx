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

#include <iostream>
#include <iomanip>
#include <fstream>
#include "itkExceptionObject.h"
#include "itkImage.h"

#include "otbImageFileReader.h"
#include "otbFlusserImageFunction.h"

int otbFlusserImage( int argc, char * argv[] )
{
  const char * inputFilename  = argv[1];
  const char * outputFilename  = argv[2];
  unsigned int  Number = 1;

  typedef unsigned char                                   InputPixelType;
  const   unsigned int                                  Dimension = 2;

  typedef itk::Image< InputPixelType,  Dimension >                InputImageType;
  typedef otb::ImageFileReader< InputImageType  >                       ReaderType;
  typedef std::complex<float>                                           ComplexType;
  typedef float                                                        RealType;
  typedef otb::FlusserImageFunction<InputImageType,float,float>                 FunctionType;

  InputImageType::RegionType   region;
  InputImageType::SizeType     size;
  InputImageType::IndexType    start;

  start.Fill( 0 );
  size[0] = 50;
  size[1] = 50;

  ReaderType::Pointer reader         = ReaderType::New();
  FunctionType::Pointer function =FunctionType::New();

  reader->SetFileName( inputFilename  );

  InputImageType::Pointer image = reader->GetOutput();

  region.SetIndex( start );
  region.SetSize( size );

  image->SetRegions(region);
  image->Update();
  function->SetInputImage( image );

  InputImageType::IndexType index;
  index[0]=10;
  index[1]=10;

  RealType Result;

  std::ofstream outputStream(outputFilename);
  outputStream <<std::setprecision(10)<<"Flusser Image moments: [12]"<<std::endl;

  for (Number = 1;Number<12;Number++)
  {
    //OTB-FA-00024-CS
    function->SetMomentNumber(Number);
    //OTB-FA-00025-CS
    function->SetNeighborhoodRadius(3);
    Result = function->EvaluateAtIndex( index );
    outputStream << "Flusser("<<Number<<") = "<< Result <<std::endl;
  }

  outputStream.close();


  return EXIT_SUCCESS;
}

