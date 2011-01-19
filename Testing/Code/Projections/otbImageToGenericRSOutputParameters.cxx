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

#include "otbImageToGenericRSOutputParameters.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"

#include "otbGeoInformationConversion.h"
#include "otbMapProjections.h"

#include <iostream>
#include <fstream>

typedef otb::VectorImage<double,2>       ImageType;
typedef otb::ImageToGenericRSOutputParameters<ImageType>  FilterType;

int otbImageToGenericRSOutputParametersNew (int argc, char * argv[])
{
  FilterType::Pointer filter = FilterType::New();

  return EXIT_SUCCESS;
}

int otbImageToGenericRSOutputParameters (int argc, char * argv[])
{
  const char * infname = argv[1];
  const char * outfname = argv[2];
  
  typedef ImageType::SizeType              SizeType;
  typedef ImageType::SpacingType           SpacingType;
  typedef otb::ImageFileReader<ImageType>  ReaderType;
  
  // Reader 
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(infname);
  reader->UpdateOutputInformation();

  // ForceSize
  SizeType  size;
  size[0] = 400;
  size[1] = 399;

  SpacingType  spacing;
  spacing[0] = 0.000006;
  spacing[1] = -0.000006;
  
  // Filter  : Taget SRS : WGS84
  FilterType::Pointer filter = FilterType::New();
  filter->SetInput(reader->GetOutput());
  filter->SetOutputProjectionRef(otb::GeoInformationConversion::ToWKT(4326));  //WGS84
  filter->Compute();
    
  // Otuput file 
  std::ofstream outfile(outfname);

  outfile<<"Output Parameters for SRID : 4326 (WGS84)"<<std::endl;
  outfile<<"Output Origin : "<<filter->GetOutputOrigin()<<std::endl;
  outfile<<"Output Spacing : "<<filter->GetOutputSpacing()<<std::endl;
  outfile<<"Output Size : "<<filter->GetOutputSize()<<std::endl;
  outfile<< std::endl;

  // target SRS : 32631 UTM 31 N
  filter->SetOutputProjectionRef(otb::GeoInformationConversion::ToWKT(32631));  //WGS84
  filter->Compute();
  
  outfile<<"Output Parameters for SRID : 32631 (UTM 31 N)"<<std::endl;
  outfile<<"Output Origin : "<<filter->GetOutputOrigin()<<std::endl;
  outfile<<"Output Spacing : "<<filter->GetOutputSpacing()<<std::endl;
  outfile<<"Output Size : "<<filter->GetOutputSize()<<std::endl;
  outfile<< std::endl;

  // target SRS : lambertII
  typedef otb::Lambert2EtenduForwardProjection Lambert2Type;
  Lambert2Type::Pointer lambert2Projection = Lambert2Type::New();
  std::string  lambertRef = lambert2Projection->GetWkt();

  filter->SetOutputProjectionRef(lambertRef);  
  filter->Compute();
  
  outfile<<"Output Parameters for SRS : Lambert II Etendu"<<std::endl;
  outfile<<"Output Origin : "<<filter->GetOutputOrigin()<<std::endl;
  outfile<<"Output Spacing : "<<filter->GetOutputSpacing()<<std::endl;
  outfile<<"Output Size : "<<filter->GetOutputSize()<<std::endl;
  outfile<< std::endl;
  

  outfile.close();
    
  return EXIT_SUCCESS;
}
