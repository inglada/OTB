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
#include <iostream>

#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbPolarimetricSynthesisFilter.h"

int otbHHnHVnVVPolarimetricSynthesisFilter(int argc, char * argv[])
{
  const char * inputFilename1  = argv[1];
  const char * inputFilename2  = argv[2];
  const char * inputFilename3  = argv[3];

  const char * outputFilename = argv[4];

  double PsiI = strtod(argv[5], NULL);
  double KhiI = strtod(argv[6], NULL);
  double PsiR = strtod(argv[7], NULL);
  double KhiR = strtod(argv[8], NULL);

  typedef std::complex <double> InputPixelType;
  typedef double                OutputPixelType;
  const unsigned int Dimension = 2;

  typedef otb::Image<InputPixelType,  Dimension> InputImageType;
  typedef otb::Image<OutputPixelType, Dimension> OutputImageType;

  typedef otb::ImageFileReader<InputImageType>  ReaderType;
  typedef otb::ImageFileWriter<OutputImageType> WriterType;

  typedef otb::PolarimetricSynthesisFilter<InputImageType, InputImageType, InputImageType, InputImageType,
      OutputImageType> FilterType;

  FilterType::Pointer polarimetricSynthesis = FilterType::New();

  polarimetricSynthesis->SetPsiI(PsiI);
  polarimetricSynthesis->SetKhiI(KhiI);
  polarimetricSynthesis->SetPsiR(PsiR);
  polarimetricSynthesis->SetKhiR(KhiR);

  ReaderType::Pointer reader1 = ReaderType::New();
  ReaderType::Pointer reader2 = ReaderType::New();
  ReaderType::Pointer reader3 = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  reader1->SetFileName(inputFilename1);
  reader2->SetFileName(inputFilename2);
  reader3->SetFileName(inputFilename3);
  writer->SetFileName(outputFilename);

  polarimetricSynthesis->SetInputHH(reader1->GetOutput());
  polarimetricSynthesis->SetInputHV(reader2->GetOutput());
  polarimetricSynthesis->SetInputVV(reader3->GetOutput());
  writer->SetInput(polarimetricSynthesis->GetOutput());

  writer->Update();

  return EXIT_SUCCESS;
}
