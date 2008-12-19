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

//  Software Guide : BeginCommandLineArgs
//    INPUTS: {buildingExtractionIndexed.tif}
//    OUTPUTS: {buildingExtractionRGB.png}, {buildingExtractionIndexed_scaled.png}
//  Software Guide : EndCommandLineArgs

//  Software Guide : BeginLatex
//
//  Some algorithm produce as an output an indexed image. In such images,
// each pixel is given a value according to the region number it belongs to.
// Often, such images are produced by segmentation or classification algorithm.
//
// If such region are easy to manipulate, as it is easier to compare two integer
// than a RGB value, it is different when it comes to displaying the results.
//
// Here we present a convient way to convert such index image to a color image. In
// such conversion, it is important to ensure that neighborhood region have
// easily dicernable colors. This is done randomly using a hash function by
// the \doxygen{itk}{ScalarToRGBPixelFunctor}.
//
//  Software Guide : EndLatex

#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbStreamingImageFileWriter.h"
#include "itkUnaryFunctorImageFilter.h"
#include "itkScalarToRGBPixelFunctor.h"

#include "itkRescaleIntensityImageFilter.h"

int main(int argc, char * argv[])
{
  if( argc != 4 )
  {
    std::cerr << "Usage: " << argv[0] << " <inputImageFile> ";
    std::cerr << " <outputRGBImageFile> <outputScaledImageFile>" << std::endl;
    return EXIT_FAILURE;
  }
  const char * inputFilename  = argv[1];
  const char * outputRGBFilename = argv[2];
  const char * outputScaledFilename = argv[3];

  typedef otb::Image<unsigned long, 2> ImageType;
  typedef otb::Image<itk::RGBPixel<unsigned char>,2> RGBImageType;

  typedef otb::ImageFileReader<ImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();

  reader->SetFileName(inputFilename);

  //  Software Guide : BeginLatex
  //
  //  The \doxygen{itk}{UnaryFunctorImageFilter} will do the work of applying the
  // \doxygen{itk}{ScalarToRGBPixelFunctor} to each pixel.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef itk::Functor::ScalarToRGBPixelFunctor<unsigned long>  ColorMapFunctorType;
  typedef itk::UnaryFunctorImageFilter<ImageType, RGBImageType, ColorMapFunctorType> ColorMapFilterType;
  ColorMapFilterType::Pointer colormapper = ColorMapFilterType::New();

  colormapper->SetInput(reader->GetOutput());
  // Software Guide : EndCodeSnippet

  typedef otb::StreamingImageFileWriter<RGBImageType> WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputRGBFilename);
  writer->SetInput(colormapper->GetOutput());

  writer->Update();


  //The following is just to produce the input image for the software guide
  typedef otb::Image<unsigned char, 2> OutputImageType;
  typedef itk::RescaleIntensityImageFilter<ImageType, OutputImageType> RescalerType;
  RescalerType::Pointer rescaler = RescalerType::New();
  rescaler->SetInput(reader->GetOutput());

  typedef otb::StreamingImageFileWriter<OutputImageType> UCharWriterType;
  UCharWriterType::Pointer writer2 = UCharWriterType::New();
  writer2->SetFileName(outputScaledFilename);
  writer2->SetInput(rescaler->GetOutput());
  writer2->Update();

  //  Software Guide : BeginLatex
  // Figure~\ref{fig:INDEXTORGB_FILTER} shows the result of the conversion
  // from an indexed image to a color image.
  // \begin{figure}
  // \center
  // \includegraphics[width=0.44\textwidth]{buildingExtractionIndexed_scaled.eps}
  // \includegraphics[width=0.44\textwidth]{buildingExtractionRGB.eps}
  // \itkcaption[Scaling images]{The original indexed image (left) and the
  // conversion to color image.}
  // \label{fig:INDEXTORGB_FILTER}
  // \end{figure}
  //
  //  Software Guide : EndLatex

  return EXIT_SUCCESS;
}

