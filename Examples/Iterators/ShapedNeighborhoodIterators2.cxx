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
// SoftwareGuide : BeginCommandLineArgs
//  INPUTS: {BinaryImage.png}
//  OUTPUTS: {ShapedNeighborhoodIterators1b.png}
//  4
// SoftwareGuide : EndCommandLineArgs

#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkConstShapedNeighborhoodIterator.h"
#include "itkImageRegionIterator.h"
#include "itkNeighborhoodAlgorithm.h"
#include <math.h>

int main( int argc, char *argv[] )
{
  if ( argc < 4 )
  {
    std::cerr << "Missing parameters. " << std::endl;
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0]
              << " inputImageFile outputImageFile element_radius"
              << std::endl;
    return -1;
  }

  typedef unsigned char PixelType;
  typedef otb::Image< PixelType, 2 >  ImageType;
  typedef otb::ImageFileReader< ImageType > ReaderType;

  typedef itk::ConstShapedNeighborhoodIterator< ImageType > ShapedNeighborhoodIteratorType;
  typedef itk::ImageRegionIterator< ImageType> IteratorType;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );

  unsigned int element_radius = ::atoi( argv[3] );

  try
  {
    reader->Update();
  }
  catch ( itk::ExceptionObject &err)
  {
    std::cout << "ExceptionObject caught !" << std::endl;
    std::cout << err << std::endl;
    return -1;
  }

  ImageType::Pointer output = ImageType::New();
  output->SetRegions(reader->GetOutput()->GetRequestedRegion());
  output->Allocate();

  typedef itk::NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<ImageType> FaceCalculatorType;

  FaceCalculatorType faceCalculator;
  FaceCalculatorType::FaceListType faceList;
  FaceCalculatorType::FaceListType::iterator fit;

  ShapedNeighborhoodIteratorType::RadiusType radius;
  radius.Fill(element_radius);

  faceList = faceCalculator(reader->GetOutput(), output->GetRequestedRegion(), radius);

  IteratorType out;
  const float rad = static_cast<float>(element_radius);

  const PixelType background_value = 0;
  const PixelType foreground_value = 255;

  for ( fit=faceList.begin(); fit != faceList.end(); ++fit)
  {
    ShapedNeighborhoodIteratorType it( radius, reader->GetOutput(), *fit );
    out = IteratorType( output, *fit );

    // Creates a circular structuring element by activating all the pixels less
    // than radius distance from the center of the neighborhood.
    for (float y = -rad; y <= rad; y++)
    {
      for (float x = -rad; x <= rad; x++)
      {
        ShapedNeighborhoodIteratorType::OffsetType off;

        float dis = ::sqrt( x*x + y*y );
        if (dis <= rad)
        {
          off[0] = static_cast<int>(x);
          off[1] = static_cast<int>(y);
          it.ActivateOffset(off);
        }
      }
    }

// Software Guide : BeginLatex
//
// The logic of the inner loop can be rewritten to perform
// dilation.  Dilation of the set $I$ by $E$ is the set of all $x$ such that
// $E$ positioned at $x$ contains at least one element in $I$.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
    // Implements dilation
    for (it.GoToBegin(), out.GoToBegin(); !it.IsAtEnd(); ++it, ++out)
    {
      ShapedNeighborhoodIteratorType::ConstIterator ci;

      bool flag = false;
      for (ci = it.Begin(); ci != it.End(); ci++)
      {
        if (ci.Get() != background_value)
        {
          flag = true;
          break;
        }
      }
      if (flag == true)
      {
        out.Set(foreground_value);
      }
      else
      {
        out.Set(background_value);
      }
    }
  }
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// The output image is written and visualized directly as a binary image of
// \code{unsigned chars}.  Figure~\ref{fig:ShapedNeighborhoodExample2}
// illustrates the results of  dilation on the image
// \code{Examples/Data/BinaryImage.png}.  Applying erosion and dilation
// in sequence effects the morphological operations of opening and closing.
//
// \begin{figure} \centering
// \includegraphics[width=0.18\textwidth]{BinaryImage.eps}
// %\includegraphics[width=0.18\textwidth]{ShapedNeighborhoodIterators1a.eps}
// \includegraphics[width=0.18\textwidth]{ShapedNeighborhoodIterators1b.eps}
// %\includegraphics[width=0.18\textwidth]{ShapedNeighborhoodIterators1c.eps}
// %\includegraphics[width=0.18\textwidth]{ShapedNeighborhoodIterators1d.eps}
// \itkcaption[Binary image morphology]{The effects of morphological operations
// on a binary image using a circular structuring element of size 4.
// Left: original image. Right: dilation.}
// \protect\label{fig:ShapedNeighborhoodExample2}
// \end{figure}
//
// Software Guide : EndLatex

  typedef otb::ImageFileWriter< ImageType > WriterType;

  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName( argv[2] );
  writer->SetInput( output );
  try
  {
    writer->Update();
  }
  catch ( itk::ExceptionObject &err)
  {
    std::cout << "ExceptionObject caught !" << std::endl;
    std::cout << err << std::endl;
    return -1;
  }

  return EXIT_SUCCESS;
}
