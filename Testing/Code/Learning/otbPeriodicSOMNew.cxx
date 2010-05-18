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
#include "otbSOMMap.h"
#include "otbPeriodicSOM.h"
#include "itkRGBPixel.h"
#include "itkEuclideanDistance.h"
#include "itkListSample.h"

int otbPeriodicSOMNew(int argc, char* argv[])
{
  const unsigned int Dimension = 2;
  typedef float                                           ComponentType;
  typedef itk::VariableLengthVector<ComponentType>        PixelType;
  typedef itk::Statistics::EuclideanDistance<PixelType>   DistanceType;
  typedef otb::SOMMap<PixelType, DistanceType, Dimension> SOMMapType;
  //     typedef itk::Statistics::ImageToListAdaptor<SOMMapType> AdaptorType;
  typedef itk::Statistics::ListSample<PixelType>       ListSampleType;
  typedef otb::PeriodicSOM<ListSampleType, SOMMapType> SOMType;

  // Instantiation
  SOMType::Pointer som = SOMType::New();

  return EXIT_SUCCESS;
}
