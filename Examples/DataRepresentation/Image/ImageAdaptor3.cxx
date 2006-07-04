/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: ImageAdaptor3.cxx,v $
  Language:  C++
  Date:      $Date: 2005/11/20 13:27:52 $
  Version:   $Revision: 1.13 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif


// Software Guide : BeginLatex
//
// This example illustrates the use of \doxygen{itk}{ImageAdaptor}
// to obtain access to the components of a vector image. 
// Specifically, it shows how to manage pixel accessors containing
// internal parameters. In this example we create an image of vectors by using
// a gradient filter. Then, we use an image adaptor to extract one of the
// components of the vector image. The vector type used by the gradient filter
// is the \doxygen{itk}{CovariantVector} class. 
//
// We start by including the relevant headers.
// 
// \index{itk::ImageAdaptor!Instantiation}
// \index{itk::ImageAdaptor!Header}
// \index{itk::PixelAccessor!with parameters}
//
// Software Guide : EndLatex 

#include "otbImage.h"
#include "itkImageAdaptor.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkRescaleIntensityImageFilter.h"


// Software Guide : BeginCodeSnippet
#include "itkCovariantVector.h"
#include "itkGradientRecursiveGaussianImageFilter.h"
// Software Guide : EndCodeSnippet


//  Software Guide : BeginLatex
//
//  A pixel accessors class may have internal parameters that affect the
//  operations performed on input pixel data. Image adaptors support
//  parameters in their internal pixel accessor by using
//  the assignment operator. Any pixel accessor which has internal
//  parameters must therefore implement the assignment operator. 
//  The following defines a pixel accessor for extracting 
//  components from a vector pixel. The 
//  \code{m\_Index} member variable is used to select the vector component 
//  to be returned.
//
//  Software Guide : EndLatex 


// Software Guide : BeginCodeSnippet
class VectorPixelAccessor  
{
public:
  typedef itk::CovariantVector<float,2>   InternalType;
  typedef                      float      ExternalType;

  void operator=( const VectorPixelAccessor & vpa )
    {
      m_Index = vpa.m_Index;
    }
  ExternalType Get( const InternalType & input ) const 
    {
      return static_cast<ExternalType>( input[ m_Index ] );
    }
  void SetIndex( unsigned int index )
    {
      m_Index = index;
    }
private:
  unsigned int m_Index;
};
// Software Guide : EndCodeSnippet


//  Software Guide : BeginLatex
//
//  The \code{Get()} method simply returns the \emph{i}-th component of
//  the vector as indicated by the index. The assignment operator transfers the
//  value of the index member variable from one instance of the pixel accessor
//  to another.
//
//  Software Guide : EndLatex 


//-------------------------
//
//   Main code
//
//-------------------------

int main( int argc, char *argv[] ) 
{
  if( argc < 4 )
    {
    std::cerr << "Usage: " << std::endl;
    std::cerr << "ImageAdaptor3   inputFileName outputComponentFileName ";
    std::cerr << " indexOfComponentToExtract" << std::endl;
    return -1;
    }


//  Software Guide : BeginLatex
//
//  In order to test the pixel accessor, we generate an image of vectors using
//  the \doxygen{itk}{GradientRecursiveGaussianImageFilter}. This
//  filter produces an output image of \doxygen{itk}{CovariantVector} pixel type.
//  Covariant vectors are the natural representation for gradients since they
//  are the equivalent of normals to iso-values manifolds.
//
//  Software Guide : EndLatex 


// Software Guide : BeginCodeSnippet
  typedef unsigned char  InputPixelType;
  const   unsigned int   Dimension = 2;
  typedef otb::Image< InputPixelType,  Dimension  >   InputImageType;
  typedef itk::CovariantVector< float, Dimension  >   VectorPixelType; 
  typedef otb::Image< VectorPixelType, Dimension  >   VectorImageType;
  typedef itk::GradientRecursiveGaussianImageFilter< InputImageType,
                                        VectorImageType> GradientFilterType;

  GradientFilterType::Pointer gradient = GradientFilterType::New();
// Software Guide : EndCodeSnippet


//  Software Guide : BeginLatex
//
//  We instantiate the ImageAdaptor using the vector image type as
//  the first template parameter and the pixel accessor as the second
//  template parameter.
//
//  Software Guide : EndLatex 



// Software Guide : BeginCodeSnippet
  typedef itk::ImageAdaptor<  VectorImageType, 
                              VectorPixelAccessor > ImageAdaptorType;

  ImageAdaptorType::Pointer adaptor = ImageAdaptorType::New();
// Software Guide : EndCodeSnippet


//  Software Guide : BeginLatex
//
//  The index of the component to be extracted is specified
//  from the command line. In the following, we create the accessor,
//  set the index and connect the accessor to the image adaptor using
//  the \code{SetPixelAccessor()} method.
//
//  Software Guide : EndLatex 


// Software Guide : BeginCodeSnippet
  VectorPixelAccessor  accessor;
  accessor.SetIndex( atoi( argv[3] ) );
  adaptor->SetPixelAccessor( accessor );
// Software Guide : EndCodeSnippet


//  Software Guide : BeginLatex
//
//  We create a reader to load the image specified from the 
//  command line and pass its output as the input to the gradient filter.
//
//  Software Guide : EndLatex 


// Software Guide : BeginCodeSnippet
  typedef otb::ImageFileReader< InputImageType >   ReaderType;
  ReaderType::Pointer reader = ReaderType::New();  
  gradient->SetInput( reader->GetOutput() );

  reader->SetFileName( argv[1] );
  gradient->Update();
//  Software Guide : EndCodeSnippet 


//  Software Guide : BeginLatex
//
//  We now connect the output of the gradient filter as input to the 
//  image adaptor.  The adaptor emulates a  scalar image whose pixel values 
//  are taken from the selected component of the vector image.
//
//  Software Guide : EndLatex 


// Software Guide : BeginCodeSnippet
  adaptor->SetImage( gradient->GetOutput() );
// Software Guide : EndCodeSnippet
 

  typedef otb::Image< unsigned char, Dimension >   OutputImageType;
  typedef itk::RescaleIntensityImageFilter< ImageAdaptorType, OutputImageType> 
    RescalerType;
  RescalerType::Pointer rescaler = RescalerType::New();
  typedef otb::ImageFileWriter< OutputImageType >   WriterType;
  WriterType::Pointer writer = WriterType::New();

  writer->SetFileName( argv[2] );

  rescaler->SetOutputMinimum(  0  );
  rescaler->SetOutputMaximum( 255 );

  rescaler->SetInput( adaptor );
  writer->SetInput( rescaler->GetOutput() );
  writer->Update();



  return 0;
}



