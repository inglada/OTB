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

#include "otbImageToGrayscaleAnaglyphImageFilter.h"
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbStreamingImageFileWriter.h"

int otbImageToGrayscaleAnaglyphImageFilter(int argc, char * argv[])
{
  try
    {
      const unsigned int Dimension = 2;
      typedef double PixelType;

      typedef otb::Image<PixelType,Dimension> ImageType;
      typedef otb::VectorImage<PixelType,Dimension> VectorImageType;
      typedef otb::ImageToGrayscaleAnaglyphImageFilter<ImageType,ImageType,VectorImageType> FilterType;
      typedef otb::ImageFileReader<ImageType> ReaderType;
      typedef otb::StreamingImageFileWriter<VectorImageType> WriterType;

      // Instantiating object
      FilterType::Pointer filter = FilterType::New();
      ReaderType::Pointer reader1 = ReaderType::New();
      ReaderType::Pointer reader2 = ReaderType::New();
      WriterType::Pointer writer = WriterType::New();

      reader1->SetFileName(argv[1]);
      reader2->SetFileName(argv[2]);
      writer->SetFileName(argv[3]);

      filter->SetInput1(reader1->GetOutput());
      filter->SetInput2(reader2->GetOutput());
      writer->SetInput(filter->GetOutput());
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
