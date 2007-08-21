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

#include "otbRAndBAndNIRVegetationIndexImageFilter.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbVegetationIndex.h"

template<class TInputRImage, class TInputBImage, class TInputNIRImage, class TOutputImage, class TFunction>
int generic_RAndBAndNIRVegetationIndexImageFilter(int argc, char * argv[])
{
  try
    {

        typedef otb::ImageFileReader<TInputRImage> RReaderType;
        typedef otb::ImageFileReader<TInputBImage> BReaderType;
        typedef otb::ImageFileReader<TInputNIRImage> NIRReaderType;
        typedef otb::ImageFileWriter<TOutputImage> WriterType;

        typedef otb::RAndBAndNIRVegetationIndexImageFilter<TInputRImage,TInputBImage,TInputNIRImage,TOutputImage,TFunction> 
                                                RAndBAndNIRVegetationIndexImageFilterType;

        // Instantiating object
        typename RAndBAndNIRVegetationIndexImageFilterType::Pointer filter = RAndBAndNIRVegetationIndexImageFilterType::New();
        typename RReaderType::Pointer readerR = RReaderType::New();
        typename BReaderType::Pointer readerB = BReaderType::New();
        typename NIRReaderType::Pointer readerNIR = NIRReaderType::New();
        typename WriterType::Pointer writer = WriterType::New();

        const char * inputFilenameR  = argv[1];
        const char * inputFilenameB  = argv[2];
        const char * inputFilenameNIR  = argv[3];
        const char * outputFilename = argv[4];
        double gamma(::atof(argv[5]));

        readerR->SetFileName( inputFilenameR );
        readerB->SetFileName( inputFilenameB );
        readerNIR->SetFileName( inputFilenameNIR );
        writer->SetFileName( outputFilename  );
        filter->SetInputR( readerR->GetOutput() ); 
        filter->SetInputB( readerB->GetOutput() ); 
        filter->SetInputNIR( readerNIR->GetOutput() ); 
        filter->GetFunctor().SetGamma(gamma);
        writer->SetInput( filter->GetOutput() );
        writer->Update();

    }

  catch( itk::ExceptionObject & err ) 
    { 
    std::cout << "Exception itk::ExceptionObject thrown !" << std::endl; 
    std::cout << err << std::endl; 
    return EXIT_FAILURE;
    } 

  catch( ... ) 
    { 
    std::cout << "Unknown exception thrown !" << std::endl; 
    return EXIT_FAILURE;
    } 
  return EXIT_SUCCESS;


}

int otbRAndBAndNIRVegetationIndexImageFilter(int argc, char * argv[])
{
        const unsigned int Dimension = 2;
        typedef unsigned char PixelType;
        typedef otb::Image<PixelType,Dimension> InputRImageType;
        typedef otb::Image<PixelType,Dimension> InputBImageType;
        typedef otb::Image<PixelType,Dimension> InputNIRImageType;
        typedef otb::Image<float,Dimension> OutputImageType;
        
        std::string strArgv(argv[1]);
        argc--;
        argv++;
	if ( strArgv == "ARVI" ) return( generic_RAndBAndNIRVegetationIndexImageFilter<InputRImageType, InputBImageType, InputNIRImageType, OutputImageType,
                                                otb::Functor::ARVI<     InputRImageType::PixelType,
                                                                        InputBImageType::PixelType,
                                                                        InputNIRImageType::PixelType,
                                                                        OutputImageType::PixelType> >
                                                                        (argc,argv) );
        else
                return EXIT_FAILURE;
        return EXIT_SUCCESS;
}
