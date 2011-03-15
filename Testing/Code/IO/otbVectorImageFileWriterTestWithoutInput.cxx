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

#include <complex>
#include <iostream>

#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

#include "itkExceptionObject.h"
#include "itkImageRegionIterator.h"

/***********
 * 1.
 * Write VectorImage<scalar> with an VectorImageFileWriter<scalar>
 ***********/
template<class InternalType>
int otbVectorImageFileWriterScalarTestWithoutInputGeneric(int argc, char* argv[])
{
  if (argc != 4)
    {
    std::cout << "Invalid arguments" << std::endl;
    std::cout << "1:Output File Name" << std::endl;
    std::cout << "2:Image Size" << std::endl;
    std::cout << "3:Nb of Channels" << std::endl;
    return 1;
    }

  // TYPEDEFS
  typedef InternalType                        PixelType;
  typedef otb::VectorImage<PixelType, 2>      ImageType;
  typedef typename ImageType::PixelType       ImagePixelType;
  typedef typename ImageType::RegionType      RegionType;
  typedef typename ImageType::SizeType        SizeType;
  typedef typename ImageType::IndexType       IndexType;
  typedef itk::ImageRegionIterator<ImageType> IteratorType;
  typedef otb::ImageFileWriter<ImageType>     WriterType;

  // INSTANCIATIONS
  typename WriterType::Pointer writer = WriterType::New();
  typename ImageType::Pointer image = ImageType::New();

  RegionType region;
  SizeType size;
  size.Fill(atoi(argv[2]));
  IndexType orig;
  orig.Fill(0);

  region.SetSize(size);
  region.SetIndex(orig);

  image->SetRegions(region);
  image->SetNumberOfComponentsPerPixel(atoi(argv[3]));
  image->Allocate();

  ImagePixelType pix0;
  pix0.SetSize(atoi(argv[3]));
  for (unsigned int i = 0; i < static_cast<unsigned int> (atoi(argv[3])); i++)
    {
    pix0[i] = static_cast<PixelType> (0);
    }
  image->FillBuffer(pix0);

  IteratorType it(image, image->GetLargestPossibleRegion());
  it.GoToBegin();

  double val = 0.;
  ImagePixelType pixVal;
  pixVal.SetSize(atoi(argv[3]));

  while (!it.IsAtEnd())
    {
    for (unsigned int i = 0; i < static_cast<unsigned int> (atoi(argv[3])); i++)
      {
      pixVal[i] = static_cast<PixelType> (i * size[0] * size[1] + size[0] * it.GetIndex()[1] + it.GetIndex()[0]);
      }

    it.Set(pixVal);

    ++it;
    }

  writer->SetFileName(argv[1]);
  writer->SetInput(image);
  writer->Update();


  return EXIT_SUCCESS;
}

int otbVectorImageFileWriterScalarTestWithoutInputShort(int argc, char * argv[])
{
  return otbVectorImageFileWriterScalarTestWithoutInputGeneric<short>(argc, argv);
}
int otbVectorImageFileWriterScalarTestWithoutInputInt(int argc, char * argv[])
{
  return otbVectorImageFileWriterScalarTestWithoutInputGeneric<int>(argc, argv);
}
int otbVectorImageFileWriterScalarTestWithoutInputFloat(int argc, char * argv[])
{
  return otbVectorImageFileWriterScalarTestWithoutInputGeneric<float>(argc, argv);
}
int otbVectorImageFileWriterScalarTestWithoutInputDouble(int argc, char * argv[])
{
  return otbVectorImageFileWriterScalarTestWithoutInputGeneric<double>(argc, argv);
}

/***********
 * 2.
 * Write VectorImage<complex> with an VectorImageFileWriter<complex>
 ***********/
template<class InternalType>
int otbVectorImageFileWriterComplexTestWithoutInputGeneric(int argc, char* argv[])
{
  if (argc != 4)
    {
    std::cout << "Invalid arguments" << std::endl;
    std::cout << "1:Output File Name" << std::endl;
    std::cout << "2:Image Size" << std::endl;
    std::cout << "3:Nb of Channels" << std::endl;
    return 1;
    }

  // TYPEDEFS
  typedef std::complex<InternalType>          PixelType;
  typedef otb::VectorImage<PixelType, 2>      ImageType;
  typedef typename ImageType::PixelType       ImagePixelType;
  typedef typename ImageType::RegionType      RegionType;
  typedef typename ImageType::SizeType        SizeType;
  typedef typename ImageType::IndexType       IndexType;
  typedef itk::ImageRegionIterator<ImageType> IteratorType;
  typedef otb::ImageFileWriter<ImageType>     WriterType;

  // INSTANCIATIONS
  typename WriterType::Pointer writer = WriterType::New();
  typename ImageType::Pointer image = ImageType::New();

  RegionType region;
  SizeType size;
  size.Fill(atoi(argv[2]));
  IndexType orig;
  orig.Fill(0);

  region.SetSize(size);
  region.SetIndex(orig);

  image->SetRegions(region);
  image->SetNumberOfComponentsPerPixel(atoi(argv[3]));
  image->Allocate();

  ImagePixelType pix0;
  pix0.SetSize(atoi(argv[3]));
  for (unsigned int i = 0; i < static_cast<unsigned int> (atoi(argv[3])); i++)
    {
    pix0[i] = static_cast<PixelType> (0);
    }
  image->FillBuffer(pix0);

  IteratorType it(image, image->GetLargestPossibleRegion());
  it.GoToBegin();

  double val = 0.;
  ImagePixelType pixVal;
  pixVal.SetSize(atoi(argv[3]));

  typedef typename itk::NumericTraits<PixelType>::ScalarRealType ScalarRealType;
  while (!it.IsAtEnd())
    {
    for (unsigned int i = 0; i < static_cast<unsigned int> (atoi(argv[3])); i++)
      {
      ScalarRealType realPart = 2 * (i *size[0]*size[1] + size[0] * it.GetIndex()[1] + it.GetIndex()[0]);
      ScalarRealType imagPart = 2 * (i *size[0]*size[1] + size[0] * it.GetIndex()[1] + it.GetIndex()[0]) + 1;

      PixelType internalPixelVal  (realPart, imagPart);

      //std::cout << "internalPixelVal = " << internalPixelVal <<std::endl;

      pixVal[i] = internalPixelVal;
      }

    it.Set(pixVal);

    ++it;
    }

  writer->SetFileName(argv[1]);
  writer->SetInput(image);
  writer->Update();

  return EXIT_SUCCESS;
}

int otbVectorImageFileWriterComplexTestWithoutInputShort(int argc, char * argv[])
{
  return otbVectorImageFileWriterComplexTestWithoutInputGeneric<short>(argc, argv);
}
int otbVectorImageFileWriterComplexTestWithoutInputInt(int argc, char * argv[])
{
  return otbVectorImageFileWriterComplexTestWithoutInputGeneric<int>(argc, argv);
}
int otbVectorImageFileWriterComplexTestWithoutInputFloat(int argc, char * argv[])
{
  return otbVectorImageFileWriterComplexTestWithoutInputGeneric<float>(argc, argv);
}
int otbVectorImageFileWriterComplexTestWithoutInputDouble(int argc, char * argv[])
{
  return otbVectorImageFileWriterComplexTestWithoutInputGeneric<double>(argc, argv);
}
