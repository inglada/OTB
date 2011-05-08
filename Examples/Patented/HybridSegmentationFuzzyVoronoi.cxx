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


//  Software Guide : BeginCommandLineArgs
//    INPUTS: {QB_Suburb.png}
//    OUTPUTS: {HybridSegmentationFuzzyVoronoiOutput.png}
//    111 38 75 20 0.5 3.0
//  Software Guide : EndCommandLineArgs

// Software Guide : BeginLatex
//
// This example illustrates the use of the
// \doxygen{itk}{SimpleFuzzyConnectednessScalarImageFilter} and
// \doxygen{itk}{VoronoiSegmentationImageFilter} to build a hybrid segmentation that
// integrates fuzzy connectedness with the Voronoi diagram classification.
//
// Please note that the Fuzzy Connectedness algorithm is covered by a Patent
// \cite{Udupa1998}. For this reason the current example is located in the
// \texttt{Examples/Patented} subdirectory.
//
// First, we include the header files of the two filters.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "itkSimpleFuzzyConnectednessScalarImageFilter.h"
#include "itkVoronoiSegmentationImageFilter.h"
// Software Guide : EndCodeSnippet

#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

#include "itkRescaleIntensityImageFilter.h"

int main(int argc, char *argv[])
{
  if (argc < 9)
    {
    std::cerr << "Missing Parameters " << std::endl;
    std::cerr << "Usage: " << argv[0];
    std::cerr << " inputImage  outputImage seedX seedY " << std::endl;
    std::cerr <<
    " estimateMean estimateVariance (used by FuzzySegmentation) " << std::endl;
    std::cerr <<
    " meanTolerance standardDeviationTolerance (used by VoronoiSegmentation) "
              << std::endl;
    return 1;
    }

  //  Software Guide : BeginLatex
  //
  //  Next, we declare the pixel type and image dimension and
  //  specify the image type to be used as input.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef  float InputPixelType;
  const unsigned int Dimension = 2;
  typedef otb::Image<InputPixelType, Dimension> InputImageType;
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  Fuzzy connectedness segmentation is performed first to generate
  //  a rough segmentation that yields a sample from the
  //  region to be segmented.  A binary result, representing the
  //  sample, is used as a prior for the next step.  Here, we use the
  //  \doxygen{itk}{SimpleFuzzyConnectednessScalarImageFilter}, but we may
  //  also utilize any other image segmentation filter instead.  The
  //  result produced by the fuzzy segmentation filter is stored in a
  //  binary image.  Below, we declare the type of the image using a
  //  pixel type and a spatial dimension.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef unsigned char                          BinaryPixelType;
  typedef otb::Image<BinaryPixelType, Dimension> BinaryImageType;
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  The fuzzy segmentation filter type is instantiated here using the input
  //  and binary image types as template parameters.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef   itk::SimpleFuzzyConnectednessScalarImageFilter<
      InputImageType,
      BinaryImageType
      >  FuzzySegmentationFilterType;
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  The fuzzy connectedness segmentation filter is created by invoking the
  //  \code{New()} method and assigning the result to a
  //  \doxygen{itk}{SmartPointer}.
  //
  //  \index{itk::SimpleFuzzy\-Connectedness\-Scalar\-Image\-Filter!New()}
  //  \index{itk::SimpleFuzzy\-Connectedness\-Scalar\-Image\-Filter!Pointer}
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  FuzzySegmentationFilterType::Pointer fuzzysegmenter =
    FuzzySegmentationFilterType::New();
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  In the next step of the hybrid segmentation method, the prior generated
  //  from the fuzzy segmentation is used to build a homogeneity measurement
  //  for the object.  A VoronoiSegmentationImageFilter uses the
  //  homogeneity measurement to drive iterative subdivision of Voronoi regions
  //  and to generate the final segmentation result (for details, please see
  //  \cite{Imielinska2000b}).  In this example, the result of the
  //  VoronoiSegmentationImageFilter is sent to a writer. Its output
  //  type is conveniently declared as one that is compatible with the writer.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef unsigned char                          OutputPixelType;
  typedef otb::Image<OutputPixelType, Dimension> OutputImageType;
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  The following lines instantiate  the Voronoi segmentation filter.
  //
  //  \index{itk::Voronoi\-Segmentation\-Image\-Filter!New()}
  //  \index{itk::Voronoi\-Segmentation\-Image\-Filter!Pointer}
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef  itk::VoronoiSegmentationImageFilter<
      InputImageType,
      OutputImageType,
      BinaryImageType>
  VoronoiSegmentationFilterType;

  VoronoiSegmentationFilterType::Pointer voronoisegmenter =
    VoronoiSegmentationFilterType::New();
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  // \begin{figure} \center
  // \includegraphics[width=0.44\textwidth]{QB_Suburb.eps}
  // \includegraphics[width=0.44\textwidth]{HybridSegmentationFuzzyVoronoiOutput.eps}
  // \itkcaption[Segmentation results for the hybrid segmentation
  // approach]{Segmentation results for the hybrid segmentation approach.}
  // \label{fig:HybridSegmentationFuzzyVoronoiOutput}
  // \end{figure}
  //
  //  Software Guide : EndLatex

  // We instantiate reader and writer types
  //
  typedef  otb::ImageFileReader<InputImageType>  ReaderType;
  typedef  otb::ImageFileWriter<OutputImageType> WriterType;

  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  reader->SetFileName(argv[1]);
  writer->SetFileName(argv[2]);

  //  Software Guide : BeginLatex
  //
  //  The input that is passed to the fuzzy segmentation filter is taken from
  //  the reader.
  //
  //  \index{itk::Simple\-Fuzzy\-Connectedness\-Scalar\-Image\-Filter!SetInput()}
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  fuzzysegmenter->SetInput(reader->GetOutput());
  // Software Guide : EndCodeSnippet

  InputImageType::IndexType index;

  index[0] = atoi(argv[3]);
  index[1] = atoi(argv[4]);

  const float mean              = atof(argv[5]);
  const float variance          = atof(argv[6]);

  const float meanTolerance     = atof(argv[7]);
  const float stdTolerance      = atof(argv[8]);

  //  Software Guide : BeginLatex
  //
  //  The parameters of the fuzzy segmentation filter are defined here. A seed
  //  point is provided with the method \code{SetObjectSeed()} in order to
  //  initialize the region to be grown.  Estimated values for the mean and
  //  variance of the object intensities are also provided with the methods
  //  \code{SetMean()} and \code{SetVariance()}, respectively. A threshold
  //  value for generating the binary object is preset with the method
  //  \code{SetThreshold()}.  For details describing the role of the mean and
  //  variance on the computation of the segmentation, please see
  //  \cite{Udupa1996}.
  //
  //  \index{itk::Simple\-Fuzzy\-Connectedness\-Scalar\-Image\-Filter!SetObjectSeed()}
  //  \index{itk::Simple\-Fuzzy\-Connectedness\-Scalar\-Image\-Filter!SetMean()}
  //  \index{itk::Simple\-Fuzzy\-Connectedness\-Scalar\-Image\-Filter!SetVariance()}
  //  \index{itk::Simple\-Fuzzy\-Connectedness\-Scalar\-Image\-Filter!SetThreshold()}
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  fuzzysegmenter->SetObjectSeed(index);
  fuzzysegmenter->SetMean(mean);
  fuzzysegmenter->SetVariance(variance);
  fuzzysegmenter->SetThreshold(0.5);
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  The execution of the fuzzy segmentation filter is triggered by the
  //  \code{Update()} method.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  fuzzysegmenter->Update();
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  The input to the Voronoi diagram classification filter is obtained from
  //  the reader and the prior is obtained from the fuzzy segmentation filter.
  //
  //  \index{itk::VoronoiSegmentationImageFilter!SetInput()}
  //  \index{itk::VoronoiSegmentationImageFilter!TakeAPrior()}
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  voronoisegmenter->SetInput(reader->GetOutput());
  voronoisegmenter->TakeAPrior(fuzzysegmenter->GetOutput());
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  The tolerance levels for testing the mean and standard deviation are set
  //  with the methods \code{SetMeanPercentError()} and
  //  \code{SetSTDPercentError()}. Note that the
  //  fuzzy segmentation filter uses \emph{variance} as parameter while
  //  the Voronoi segmentation filter uses the tolerance of the
  //  \emph{standard deviation} as a parameter. For more details on how these
  //  parameters should be selected, please see \cite{Imielinska2000b}.
  //
  //  \index{itk::VoronoiSegmentationImageFilter!SetMeanPercentError()}
  //  \index{itk::VoronoiSegmentationImageFilter!SetSTDPercentError()}
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  voronoisegmenter->SetMeanPercentError(meanTolerance);
  voronoisegmenter->SetSTDPercentError(stdTolerance);
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  The \emph{resolution} of the Voronoi diagram classification can be
  //  chosen with the method \code{SetMinRegion()}.
  //
  //  \index{itk::VoronoiSegmentationImageFilter!SetMinRegion()}
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  voronoisegmenter->SetMinRegion(5);
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  The execution of the Voronoi segmentation filter is triggered with the
  //  \code{Update()} method.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  voronoisegmenter->Update();
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  The output of the Voronoi diagram classification is an image mask with
  //  zeros everywhere and ones inside the segmented object. This image will
  //  appear black on many image viewers since they do not usually stretch
  //  the gray levels. Here, we add a \doxygen{itk}{RescaleIntensityImageFilter}
  //  in order to expand the dynamic range to more typical values.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef itk::RescaleIntensityImageFilter<OutputImageType, OutputImageType>
  ScalerFilterType;
  ScalerFilterType::Pointer scaler = ScalerFilterType::New();

  scaler->SetOutputMinimum(0);
  scaler->SetOutputMaximum(255);

  scaler->SetInput(voronoisegmenter->GetOutput());
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  // The output of the rescaler is passed to the writer. The invocation
  // of the \code{Update()} method on the writer triggers the execution of
  // the pipeline.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  writer->SetInput(scaler->GetOutput());
  writer->Update();
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  We execute this program on the image \code{QB\_Suburb.png} available
  //  in the directory \code{Examples/Data}. The following parameters are
  //  passed to the command line:
  //
  //  \small
  //  \begin{verbatim}
  //HybridSegmentationFuzzyVoronoi QB_Suburb.png Output.png
  //                                           111 38 75 20 0.5 2.0
  //  \end{verbatim}
  //  \normalsize
  //
  //  $(111, 38)$ specifies the index position of a seed point in the image,
  //  while $75$ and $20$ are the estimated mean and standard deviation,
  //  respectively, of the object to be segmented.  Finally, $0.5$ and $2.0$
  //  are the tolerance for the mean and standard deviation, respectively.
  //  Figure~\ref{fig:HybridSegmentationFuzzyVoronoiOutput} shows the input
  //  image and the binary mask resulting from the segmentation.
  //
  //  Note that in order to successfully segment other images, these
  //  parameters have to be adjusted to reflect the data.
  //
  //  Software Guide : EndLatex

  return EXIT_SUCCESS;
}
