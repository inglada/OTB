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

#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbConcatenateVectorImageFilter.h"

int otbConcatenateVectorImageFilter(int argc, char * argv[])
{
  try
    {
      const char * filename1 = argv[1];
      const char * filename2 = argv[2];
      const char * outputfilename = argv[3];
      const unsigned int Dimension = 2;
      typedef unsigned char InputPixelType;
      typedef otb::VectorImage<InputPixelType,Dimension> VectorImageType;
      typedef otb::ImageFileReader<VectorImageType> ReaderType;
      typedef otb::ImageFileWriter<VectorImageType> WriterType;
      typedef otb::ConcatenateVectorImageFilter<VectorImageType,VectorImageType,VectorImageType> FilterType;

      ReaderType::Pointer reader1 = ReaderType::New();
      ReaderType::Pointer reader2 = ReaderType::New();
      reader1->SetFileName(filename1);
      reader2->SetFileName(filename2);

      // Instantiating filter object
      FilterType::Pointer filter = FilterType::New();
      filter->SetInput1(reader1->GetOutput());
      filter->SetInput2(reader2->GetOutput());

      WriterType::Pointer writer = WriterType::New();
      writer->SetFileName(outputfilename);
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
