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

#ifdef __BORLANDC__
#define ITK_LEAN_AND_MEAN
#endif



#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbStreamingImageFileWriter.h"

#include "itkBinaryFunctorImageFilter.h"
#include "otbAmplitudePhaseToRGBFunctor.h"
#include "itkComplexToModulusImageFilter.h"
#include "itkComplexToPhaseImageFilter.h"

int otbAmplitudePhaseToRGBFunctor(int argc, char * argv[])
{
  typedef float PixelType;
  typedef otb::Image<PixelType, 2> ImageType;

  typedef std::complex<PixelType> ComplexPixelType;
  typedef otb::Image<ComplexPixelType, 2> ComplexImageType;

  typedef itk::RGBPixel<unsigned char> RGBPixelType;
  typedef otb::Image<RGBPixelType, 2> RGBImageType;

  typedef otb::ImageFileReader<ComplexImageType> ReaderType;
  typedef otb::StreamingImageFileWriter<RGBImageType> WriterType;

  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();
  reader->SetFileName(argv[1]);
  writer->SetFileName(argv[2]);


  typedef itk::ComplexToModulusImageFilter<ComplexImageType,ImageType> ModulusFilterType;
  ModulusFilterType::Pointer modulusFilter = ModulusFilterType::New();
  modulusFilter->SetInput(reader->GetOutput());


  typedef itk::ComplexToPhaseImageFilter<ComplexImageType,ImageType> PhaseFilterType;
  PhaseFilterType::Pointer phaseFilter = PhaseFilterType::New();
  phaseFilter->SetInput(reader->GetOutput());


  typedef otb::Functor::AmplitudePhaseToRGBFunctor
      <PixelType,PixelType,RGBPixelType> ColorMapFunctorType;
  typedef itk::BinaryFunctorImageFilter
      <ImageType, ImageType, RGBImageType, ColorMapFunctorType> ColorMapFilterType;
  ColorMapFilterType::Pointer colormapper = ColorMapFilterType::New();
  colormapper->GetFunctor().SetMaximum(4000);
  colormapper->GetFunctor().SetMinimum(0);


  colormapper->SetInput1(modulusFilter->GetOutput());
  colormapper->SetInput2(phaseFilter->GetOutput());
  colormapper->SetNumberOfThreads(1);

  writer->SetInput(colormapper->GetOutput());

  writer->Update();


  return EXIT_SUCCESS;
}
