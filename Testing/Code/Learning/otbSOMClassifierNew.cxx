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

#include <fstream>
#include "otbSOMMap.h"
#include "otbSOMClassifier.h"
#include "itkListSample.h"
#include "otbVectorImage.h"

int otbSOMClassifierNew(int argc, char* argv[] )
{
  typedef double                         InputPixelType;
  typedef int                            LabelPixelType;
  const   unsigned int               Dimension = 2;

  typedef itk::VariableLengthVector<InputPixelType> PixelType;
  typedef itk::Statistics::EuclideanDistance<PixelType> DistanceType;
  typedef otb::SOMMap<PixelType,DistanceType,Dimension> SOMMapType;
  typedef otb::VectorImage<InputPixelType,Dimension> InputImageType;
  typedef itk::Statistics::ListSample< PixelType > SampleType;
  typedef otb::SOMClassifier<SampleType,SOMMapType,LabelPixelType> ClassifierType;

  ClassifierType::Pointer classifier = ClassifierType::New() ;


  return EXIT_SUCCESS;
}







