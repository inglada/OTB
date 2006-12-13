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

#define MAIN


#include "itkExceptionObject.h"
#include "itkImage.h"
#include "itkImageFileWriter.h"
#include <iostream>

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbLineCorrelationDetectorImageFilter.h"

#include "itkLinearInterpolateImageFunction.h"

int otbLineCorrelationDetectorLinear( int argc, char* argv[] )
{
  try 
    { 
        const char * inputFilename  = argv[1];
        const char * outputFilename1 = argv[2];
	const char * outputFilename2 = argv[3];

	// Largeur de la ligne � detecter = 2*WidthLine+1
        unsigned int  WidthLine((unsigned int)::atoi(argv[4]));
        // Longueur de la ligne � detecter = 2*LengthLine+1
        unsigned int  LengthLine((unsigned int)::atoi(argv[5]));
        
        typedef unsigned char                                   InputPixelType;
        typedef double		   	                        OutputPixelType;
        const   unsigned int        	                        Dimension = 2;

        typedef itk::Image< InputPixelType,  Dimension >        InputImageType;
        typedef itk::Image< OutputPixelType, Dimension >        OutputImageType;

        typedef otb::ImageFileReader< InputImageType  >         ReaderType;
        typedef otb::ImageFileWriter< OutputImageType >         WriterType;

        typedef itk::LinearInterpolateImageFunction< InputImageType, double >	InterpolatorType;
        typedef otb::LineCorrelationDetectorImageFilter< InputImageType, OutputImageType, InterpolatorType >   FilterType;
	
        FilterType::Pointer FilterLineCorrelation = FilterType::New();
        
	FilterLineCorrelation->SetWidthLine( WidthLine );
	FilterLineCorrelation->SetLengthLine( LengthLine );
	
        ReaderType::Pointer reader = ReaderType::New();
        WriterType::Pointer writer1 = WriterType::New();
	WriterType::Pointer writer2 = WriterType::New();

        reader->SetFileName( inputFilename  );
        writer1->SetFileName( outputFilename1 );
        writer2->SetFileName(outputFilename2);


        FilterLineCorrelation->SetInput( reader->GetOutput() );
        writer1->SetInput( FilterLineCorrelation->GetOutput() );
        writer2->SetInput( FilterLineCorrelation->GetOutputDirection() );


        writer1->Update();
	writer2->Update();
    } 
  catch( itk::ExceptionObject & err ) 
    { 
    std::cout << "Exception itk::ExceptionObject levee !" << std::endl; 
    std::cout << err << std::endl; 
    return EXIT_FAILURE;
    } 
  catch( ... ) 
    { 
    std::cout << "Exception levee inconnue !" << std::endl; 
    return EXIT_FAILURE;
    } 
  // Software Guide : EndCodeSnippet

//#endif
  return EXIT_SUCCESS;
}


