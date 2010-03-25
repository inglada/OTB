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
#include "otbSiftFastImageFilter.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "itkPointSet.h"
#include "itkVariableLengthVector.h"
#include "itkRGBPixel.h"
#include "itkImageRegionIterator.h"

#include <iostream>
#include <fstream>
#include <algorithm>

bool CMPData(std::vector<float>  a ,std::vector<float>  b )
{
  return lexicographical_compare(a.begin(),a.begin()+2 , b.begin(),b.begin()+2);
}

int otbImageToFastSIFTKeyPointSetFilterOutputDescriptorAscii(int argc, char * argv[])
{
  const char * infname = argv[1];
  const char * outfname = argv[2];

  const unsigned int scales = atoi(argv[3]);

  typedef float RealType;
  const unsigned int Dimension =2;

  typedef otb::Image<RealType,Dimension> ImageType;
  typedef itk::VariableLengthVector<RealType> RealVectorType;
  typedef otb::ImageFileReader<ImageType> ReaderType;
  typedef itk::PointSet<RealVectorType,Dimension> PointSetType;
  typedef otb::SiftFastImageFilter<ImageType,PointSetType> ImageToFastSIFTKeyPointSetFilterType;

  // PointSet iterator types
  typedef PointSetType::PointsContainer PointsContainerType;
  typedef PointsContainerType::Iterator PointsIteratorType;
  typedef PointSetType::PointDataContainer PointDataContainerType;
  typedef PointDataContainerType::Iterator PointDataIteratorType;

  typedef std::vector<float> siftDataVector;
  typedef std::vector<siftDataVector> ImageDataType;   //Kind of PointSet with vectors

  // Instantiating object
  ReaderType::Pointer reader = ReaderType::New();
  ImageToFastSIFTKeyPointSetFilterType::Pointer filter = ImageToFastSIFTKeyPointSetFilterType::New();

  //Instantiation of std::vector for lexicographiacal sorting
  ImageDataType  imageData;

  reader->SetFileName(infname);
  filter->SetInput(reader->GetOutput());
  filter->SetScalesNumber(scales);
  filter->Update();

  PointsIteratorType pIt = filter->GetOutput()->GetPoints()->Begin();
  PointDataIteratorType pDataIt = filter->GetOutput()->GetPointData()->Begin();

  std::ofstream outfile(outfname);

  outfile << "Number of scales: "<<scales << std::endl;
  outfile << "Number of SIFT key points: " << filter->GetOutput()->GetNumberOfPoints() << std::endl;

 if(filter->GetOutput()->GetPointData()->Size() != filter->GetOutput()->GetPoints()->Size() )
       return EXIT_FAILURE;
 if(filter->GetOutput()->GetPointData()->Size() == 0 )
       return EXIT_FAILURE;

  // Copy the PointSet to std::vector< std::vector >
  while ( pIt!=filter->GetOutput()->GetPoints()->End() &&  pDataIt!=filter->GetOutput()->GetPointData()->End() )
  {
    siftDataVector siftData;

    siftData.push_back(pIt.Value()[0]);
    siftData.push_back(pIt.Value()[1]);

    unsigned int lIterDesc=0;
    while (lIterDesc < pDataIt.Value().Size())
    {
      siftData.push_back(pDataIt.Value()[lIterDesc]);
      lIterDesc++;
    }

    imageData.push_back(siftData);
    ++pIt;
    ++pDataIt;
  }

  //Sorting the vectors
  ImageDataType::iterator itData;
  sort(imageData.begin() , imageData.end(),CMPData);

  itData = imageData.begin();
  unsigned int stopVal = static_cast<unsigned int>(filter->GetOutput()->GetPointData()->Begin().Value().Size());

  while ( itData != imageData.end() )
  {
       unsigned int itDescriptor = 0;
       outfile << "[ ";
    while (itDescriptor < stopVal )
    //while (itDescriptor < static_cast<int>((*itData).size()-2) )
     {
         outfile << std::fixed << std::setprecision(4) << (*itData)[itDescriptor+2] << " ";
      itDescriptor++;
    }
    outfile << "]" << std::endl;
    ++itData;
  }

  outfile.close();

  return EXIT_SUCCESS;
}
