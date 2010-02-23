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
#include "otbRCC8Graph.h"
#include "otbPolygon.h"
#include "otbObjectList.h"
#include "otbRCC8VertexBase.h"
#include "otbPolygonListToRCC8GraphFilter.h"
#include "otbImageFileReader.h"
#include "otbImage.h"
#include "itkMinimumMaximumImageCalculator.h"
#include "otbImageToEdgePathFilter.h"
#include "otbRCC8GraphFileWriter.h"
#include "otbSimplifyPathListFilter.h"
#include "itkMinimumMaximumImageCalculator.h"

int otbPolygonListToRCC8GraphFilter(int argc, char* argv[])
{
  const unsigned int Dimension = 2;

  int    nbImages  = atoi(argv[1]);
  char * outfile   = argv[2];

  // typedefs
  typedef otb::Polygon<> PolygonType;
  typedef otb::ObjectList<PolygonType> PolygonListType;
  typedef otb::RCC8VertexBase<PolygonType> VertexType;
  typedef otb::RCC8Graph<VertexType> RCC8GraphType;
  typedef otb::PolygonListToRCC8GraphFilter<PolygonListType,RCC8GraphType> RCC8GraphFilterType;
  typedef otb::RCC8GraphFileWriter<RCC8GraphType> GraphWriterType;

  typedef unsigned short PixelType;
  typedef otb::Image<PixelType,Dimension> ImageType;
  typedef otb::ImageFileReader<ImageType> ReaderType;
  typedef otb::ImageToEdgePathFilter<ImageType,PolygonType> EdgeExtractionFilterType;
  typedef itk::MinimumMaximumImageCalculator<ImageType> MinMaxCalculatorType;
  typedef otb::SimplifyPathListFilter<PolygonType> SimplifyPathFilterType;

  PolygonListType::Pointer regions = PolygonListType::New();

  RCC8GraphFilterType::SegmentationRangesType ranges;


  // Reading input images
  for (int cpt=1;cpt<=nbImages;++cpt)
  {
    ranges.push_back(regions->Size());
    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName(argv[2+cpt]);
    reader->Update();
    MinMaxCalculatorType::Pointer minMax = MinMaxCalculatorType::New();
    minMax->SetImage(reader->GetOutput());
    minMax->Compute();
    for (PixelType p = minMax->GetMinimum();p<=minMax->GetMaximum();++p)
    {
      if (p!=0)
      {
        EdgeExtractionFilterType::Pointer extraction = EdgeExtractionFilterType::New();
        extraction->SetInput(reader->GetOutput());
        extraction->SetForegroundValue(p);
        extraction->Update();
        if (extraction->GetOutput()->GetVertexList()->Size()>2)
        {
          regions->PushBack(extraction->GetOutput());
        }
      }
    }
  }

  // Simplifying regions
  SimplifyPathFilterType::Pointer simplifier = SimplifyPathFilterType::New();
  simplifier->SetInput(regions);
  simplifier->GetFunctor().SetTolerance(0.1);
  simplifier->Update();

  // Filter instantiation
  RCC8GraphFilterType::Pointer filter = RCC8GraphFilterType::New();

  filter->SetInput(simplifier->GetOutput());
  filter->SetSegmentationRanges(ranges);

  GraphWriterType::Pointer writer = GraphWriterType::New();
  writer->SetInput(filter->GetOutput());
  writer->SetFileName(outfile);
  writer->Update();

  return EXIT_SUCCESS;
}
