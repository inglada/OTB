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

#include "otbImportGeoInformationImageFilter.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

int otbImportGeoInformationImageFilter(int argc, char * argv[])
{
  try
    {
      const char * infname = argv[1];
      const char * outfname = argv[2];

      const unsigned int Dimension = 2;
      typedef unsigned int PixelType;
      typedef otb::Image<PixelType,Dimension> ImageType;
      typedef otb::ImageFileReader<ImageType> ReaderType;
      typedef otb::ImageFileWriter<ImageType> WriterType;
      typedef otb::ImportGeoInformationImageFilter<ImageType,ImageType> ImportGeoInformationImageFilterType;

      // Instantiating objects
      ReaderType::Pointer reader = ReaderType::New();
      WriterType::Pointer writer = WriterType::New();
      ImportGeoInformationImageFilterType::Pointer import = ImportGeoInformationImageFilterType::New();
      
      
      reader->SetFileName(infname);
      reader->GenerateOutputInformation();
      ImageType::IndexType index;
      index.Fill(0);
      ImageType::RegionType region;
      region.SetIndex(index);
      region.SetSize(reader->GetOutput()->GetLargestPossibleRegion().GetSize());
      ImageType::Pointer black = ImageType::New();
      black->SetRegions(region);
      black->Allocate();
      black->FillBuffer(128);
      std::cout<<"black: " <<black->GetLargestPossibleRegion()<<std::endl;
      
      import->SetInput(black);
      import->SetSource(reader->GetOutput());
     //  import->Update();
//       std::cout<<"import output: "<<import->GetOutput()<<std::endl;
      

      writer->SetFileName(outfname);
      writer->SetInput(import->GetOutput());
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
