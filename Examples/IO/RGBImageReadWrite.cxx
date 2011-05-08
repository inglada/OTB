/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Some parts of this code are derived from ITK. See ITKCopyright.txt
  for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


//  Software Guide : BeginLatex
//
//  RGB images are commonly used for representing data acquired from
//  multispectral sensors. This example
//  illustrates how to read and write RGB color images to and from a file.
//  This requires the following headers as shown.
//
//  \index{itk::RGBPixel!Image}
//  \index{RGB!writing Image}
//  \index{RGB!reading Image}
//
//  Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "itkRGBPixel.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
// Software Guide : EndCodeSnippet

int main(int argc, char * argv[])
{
  // Verify the number of parameters in the command line
  if (argc < 3)
    {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << " inputRGBImageFile  outputRGBImageFile " <<
    std::endl;
    return EXIT_FAILURE;
    }

  // Software Guide : BeginLatex
  //
  // The \doxygen{itk}{RGBPixel} class is templated over the type used to
  // represent each one of the red, green and blue components. A typical
  // instantiation of the RGB image class might be as follows.
  //
  //  \index{itk::RGBPixel!Instantiation}
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef itk::RGBPixel<unsigned char> PixelType;
  typedef otb::Image<PixelType, 2>     ImageType;
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // The image type is used as a template parameter to instantiate
  // the reader and writer.
  //
  // \index{otb::ImageFileReader!RGB Image}
  // \index{otb::ImageFileWriter!RGB Image}
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::ImageFileReader<ImageType> ReaderType;
  typedef otb::ImageFileWriter<ImageType> WriterType;

  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();
  // Software Guide : EndCodeSnippet

  const char * inputFilename  = argv[1];
  const char * outputFilename = argv[2];

  //  Software Guide : BeginLatex
  //
  //  The filenames of the input and output files must be provided to the
  //  reader and writer respectively.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  reader->SetFileName(inputFilename);
  writer->SetFileName(outputFilename);
  // Software Guide : EndCodeSnippet

  ImageType::Pointer image = reader->GetOutput();
  writer->SetInput(image);

  //  Software Guide : BeginLatex
  //
  //  Finally, execution of the pipeline can be triggered by invoking the
  //  \code{Update()} method in the writer.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  writer->Update();
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  You may have noticed that apart from the declaration of the
  //  \code{PixelType} there is nothing in this code that is specific for RGB
  //  images. All the actions required to support color images are implemented
  //  internally in the \doxygen{itk}{ImageIO} objects.
  //
  //  Software Guide : EndLatex

  return EXIT_SUCCESS;
}
