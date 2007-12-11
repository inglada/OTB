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
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#ifdef __BORLANDC__
#define ITK_LEAN_AND_MEAN
#endif


// Software Guide : BeginLatex
//
// Here we are illustrating the use of the 
// \doxygen{otb}{SimpleRcsPanSharpeningFusionImageFilter} for PAN-sharpening. 
// This example takes a PAN and the corresponding XS images as input. These
// images are supposed to be registered.
//
// The fusion operation is defined as
//
// \begin{equation}
// \frac{XS}{\mathrm{Filtered}(PAN)} PAN
// /end{equation}
//
// Software Guide : EndLatex 

//  Software Guide : BeginCommandLineArgs
//    INPUTS: {QB_Toulouse_Ortho_PAN.tif QB_Toulouse_Ortho_XS.tif}
//    OUTPUTS: {QB_Toulouse_Ortho_PXS.tif pretty_QB_Toulouse_Ortho_PXS.png 
//       pretty_QB_Toulouse_Ortho_PAN.png pretty_QB_Toulouse_Ortho_XS.png}
//  Software Guide : EndCommandLineArgs

// Software Guide : BeginLatex
//
// We start by including the required header and declaring the main function:
//
// Software Guide : EndLatex 

// Software Guide : BeginCodeSnippet
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbStreamingImageFileWriter.h"
#include "otbSimpleRcsPanSharpeningFusionImageFilter.h"
// Software Guide : EndCodeSnippet

#include "otbPrintableImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"

// Software Guide : BeginCodeSnippet
int main( int argc, char* argv[] )
{
// Software Guide : EndCodeSnippet
  
  if( argc < 4 )
  {
    std::cerr << "Missing Parameters " << std::endl;
    std::cerr << "Usage: " << argv[0];
    std::cerr << " inputPanchromatiqueImage inputMultiSpectralImage outputImage outputImagePrinted panchroPrinted msPrinted" << std::endl;
    return 1;
  }
  
  // Software Guide : BeginLatex
  //
  // We declare the different image type used here as well as the image reader.
  // Note that, the reader for the PAN image is templated by an 
  // \doxygen{otb}{Image} while the XS reader uses an 
  // \doxygen{otb}{VectorImage}.
  //
  // Software Guide : EndLatex
  
  // Software Guide : BeginCodeSnippet
  typedef otb::Image<double, 2>     ImageType;
  typedef otb::VectorImage<double, 2>     VectorImageType;
  typedef otb::ImageFileReader<ImageType>  ReaderType;
  typedef otb::ImageFileReader<VectorImageType>  ReaderVectorType;
  typedef otb::VectorImage<unsigned int, 2>     VectorIntImageType;
 
					
  ReaderVectorType::Pointer     	readerXS=ReaderVectorType::New();
  ReaderType::Pointer     	readerPAN=ReaderType::New();
  // Software Guide : EndCodeSnippet
  
  // Software Guide : BeginLatex
  //
  // We pass the filenames to the readers
  //
  // Software Guide : EndLatex
  
  // Software Guide : BeginCodeSnippet
  readerPAN->SetFileName(argv[1]);
  readerXS->SetFileName(argv[2]);
  // Software Guide : EndCodeSnippet
  
  // Software Guide : BeginLatex
  //
  // We declare the fusion filter an set its inputs using the readers:
  //
  // Software Guide : EndLatex
  
  // Software Guide : BeginCodeSnippet
  typedef otb::SimpleRcsPanSharpeningFusionImageFilter
      <ImageType,VectorImageType,VectorIntImageType> FusionFilterType;
  FusionFilterType::Pointer fusion = FusionFilterType::New();
  fusion->SetPanInput(readerPAN->GetOutput());
  fusion->SetXsInput(readerXS->GetOutput());
  // Software Guide : EndCodeSnippet
      
  // Software Guide : BeginLatex
  //
  // And finally, we declare the writer and call its \code{Update()} method to
  // trigger the full pipeline execution.
  //
  // Software Guide : EndLatex    
  
  // Software Guide : BeginCodeSnippet
  typedef otb::StreamingImageFileWriter<VectorIntImageType>  WriterType;
  WriterType::Pointer	    	writer=WriterType::New();
  writer->SetFileName(argv[3]);
  writer->SetInput(fusion->GetOutput());
  writer->Update();
  // Software Guide : EndCodeSnippet
  
  
  typedef otb::PrintableImageFilter<VectorIntImageType> PrintableImageType;
  PrintableImageType::Pointer printable = PrintableImageType::New();

  
  typedef otb::VectorImage<unsigned char, 2>     VectorCharImageType;
  typedef otb::StreamingImageFileWriter<VectorCharImageType>  PNGWriterType;
  PNGWriterType::Pointer pngwriter = PNGWriterType::New();
  
  printable->SetInput(fusion->GetOutput());
  printable->SetChannel(3);
  printable->SetChannel(2);
  printable->SetChannel(1); 
  pngwriter->SetFileName(argv[4]);
  pngwriter->SetInput(printable->GetOutput());
  pngwriter->Update();
  
  typedef otb::PrintableImageFilter<VectorImageType> PrintableImageType2;
  PrintableImageType2::Pointer printable2 = PrintableImageType2::New();
  printable2->SetInput(readerXS->GetOutput());
  printable2->SetChannel(3);
  printable2->SetChannel(2);
  printable2->SetChannel(1); 
  pngwriter->SetFileName(argv[6]);
  pngwriter->SetInput(printable2->GetOutput());
  pngwriter->Update();
  
  typedef otb::Image<unsigned char, 2>     CharImageType;
  typedef itk::RescaleIntensityImageFilter <ImageType,CharImageType> RescalerType;
  RescalerType::Pointer rescaler = RescalerType::New();
  typedef otb::StreamingImageFileWriter<CharImageType>  PNGWriterType2;
  PNGWriterType2::Pointer pngwriter2 = PNGWriterType2::New();
  rescaler->SetInput(readerPAN->GetOutput());
  pngwriter2->SetFileName(argv[5]);
  pngwriter2->SetInput(rescaler->GetOutput());
  pngwriter2->Update();
  
  
// Software Guide : BeginCodeSnippet  
  return EXIT_SUCCESS;
  
}
// Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // Figure~\ref{fig:PANSHARP_FILTER} shows the result of applying
  // this PAN sharpening filter to a Quickbird image.
  // \begin{figure}
  // \center
  // \includegraphics[width=0.44\textwidth]{pretty_QB_Toulouse_Ortho_PAN.eps}
  // \includegraphics[width=0.44\textwidth]{pretty_QB_Toulouse_Ortho_XS.eps}
  // \includegraphics[width=0.44\textwidth]{pretty_QB_Toulouse_Ortho_PXS.eps}
  // \itkcaption[Pan sharpening]{Result of applying
  // the \doxygen{otb}{SimpleRcsPanSharpeningFusionImageFilter} to 
  // orthorectified Quickbird
  // image. From left to right : original PAN image, original XS image and the 
  // result of the PAN sharpening}  
  // \label{fig:PANSHARP_FILTER} 
  // \end{figure}
  //
  // Software Guide : EndLatex


