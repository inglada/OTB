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
#include "otbStreamingManager.h"

#include "otbVectorImage.h"
#include "otbNumberOfLinesStrippedStreamingManager.h"
#include "otbRAMDrivenStrippedStreamingManager.h"
#include "otbTileDimensionTiledStreamingManager.h"
#include "otbRAMDrivenTiledStreamingManager.h"

#include <fstream>

const int Dimension = 2;
typedef otb::VectorImage<unsigned short, Dimension>           ImageType;
typedef otb::NumberOfLinesStrippedStreamingManager<ImageType> NbLinesStrippedStreamingManagerType;
typedef otb::RAMDrivenStrippedStreamingManager<ImageType>     RAMDrivenStrippedStreamingManagerType;
typedef otb::TileDimensionTiledStreamingManager<ImageType>    TileDimensionTiledStreamingManagerType;
typedef otb::RAMDrivenTiledStreamingManager<ImageType>        RAMDrivenTiledStreamingManagerType;

ImageType::Pointer makeImage(ImageType::RegionType region)
{
  ImageType::Pointer image = ImageType::New();

  image->SetRegions(region);
  image->SetNumberOfComponentsPerPixel(10);

  return image;
}

int otbStreamingManagerNew(int argc, char * argv[])
{
  NbLinesStrippedStreamingManagerType::Pointer streamingManager1 = NbLinesStrippedStreamingManagerType::New();
  std::cout << streamingManager1 << std::endl;

  RAMDrivenStrippedStreamingManagerType::Pointer streamingManager2 = RAMDrivenStrippedStreamingManagerType::New();
  std::cout << streamingManager2 << std::endl;

  TileDimensionTiledStreamingManagerType::Pointer streamingManager3 = TileDimensionTiledStreamingManagerType::New();
  std::cout << streamingManager3 << std::endl;

  RAMDrivenTiledStreamingManagerType::Pointer streamingManager4 = RAMDrivenTiledStreamingManagerType::New();
  std::cout << streamingManager4 << std::endl;

  return EXIT_SUCCESS;
}


int otbNumberOfLinesStrippedStreamingManager(int argc, char * argv[])
{
  std::ofstream outfile(argv[1]);

  NbLinesStrippedStreamingManagerType::Pointer streamingManager = NbLinesStrippedStreamingManagerType::New();

  ImageType::RegionType region;
  region.SetIndex(0, 0);
  region.SetIndex(1, 0);
  region.SetSize(0, 10013);
  region.SetSize(1, 5727);

  streamingManager->SetNumberOfLinesPerStrip(10);
  streamingManager->PrepareStreaming( makeImage(region), region );

  unsigned int nbSplits = streamingManager->GetNumberOfSplits();

  ImageType::RegionType split;

  split = streamingManager->GetSplit(0);
  outfile << split << std::endl;

  split = streamingManager->GetSplit(1);
  outfile << split << std::endl;

  split = streamingManager->GetSplit(nbSplits - 1);
  outfile << split << std::endl;

  return EXIT_SUCCESS;
}


int otbRAMDrivenStrippedStreamingManager(int argc, char * argv[])
{
  std::ofstream outfile(argv[1]);

  RAMDrivenStrippedStreamingManagerType::Pointer streamingManager = RAMDrivenStrippedStreamingManagerType::New();

  ImageType::RegionType region;
  region.SetIndex(0, 0);
  region.SetIndex(1, 0);
  region.SetSize(0, 10013);
  region.SetSize(1, 5727);

  streamingManager->SetAvailableRAMInMB(1);
  streamingManager->PrepareStreaming( makeImage(region), region );

  unsigned int nbSplits = streamingManager->GetNumberOfSplits();

  ImageType::RegionType split;

  split = streamingManager->GetSplit(0);
  outfile << split << std::endl;

  split = streamingManager->GetSplit(1);
  outfile << split << std::endl;

  split = streamingManager->GetSplit(nbSplits - 1);
  outfile << split << std::endl;

  return EXIT_SUCCESS;

}

int otbTileDimensionTiledStreamingManager(int argc, char * argv[])
{
  std::ofstream outfile(argv[1]);

  TileDimensionTiledStreamingManagerType::Pointer streamingManager = TileDimensionTiledStreamingManagerType::New();

  ImageType::RegionType region;
  region.SetIndex(0, 0);
  region.SetIndex(1, 0);
  region.SetSize(0, 10013);
  region.SetSize(1, 5727);

  streamingManager->SetTileDimension(100);
  streamingManager->PrepareStreaming( makeImage(region), region );

  unsigned int nbSplits = streamingManager->GetNumberOfSplits();

  ImageType::RegionType split;

  split = streamingManager->GetSplit(0);
  outfile << split << std::endl;

  split = streamingManager->GetSplit(1);
  outfile << split << std::endl;

  split = streamingManager->GetSplit(nbSplits - 1);
  outfile << split << std::endl;

  return EXIT_SUCCESS;
}

int otbRAMDrivenTiledStreamingManager(int argc, char * argv[])
{
  std::ofstream outfile(argv[1]);

  RAMDrivenTiledStreamingManagerType::Pointer streamingManager = RAMDrivenTiledStreamingManagerType::New();

  ImageType::RegionType region;
  region.SetIndex(0, 0);
  region.SetIndex(1, 0);
  region.SetSize(0, 10013);
  region.SetSize(1, 5727);

  streamingManager->SetAvailableRAMInMB(1);
  streamingManager->PrepareStreaming( makeImage(region), region );

  unsigned int nbSplits = streamingManager->GetNumberOfSplits();

  ImageType::RegionType split;

  split = streamingManager->GetSplit(0);
  outfile << split << std::endl;

  split = streamingManager->GetSplit(1);
  outfile << split << std::endl;

  split = streamingManager->GetSplit(nbSplits - 1);
  outfile << split << std::endl;

  return EXIT_SUCCESS;
}
