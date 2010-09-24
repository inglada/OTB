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
#include "itkPointSet.h"
#include "itkVariableLengthVector.h"
#include "otbKeyPointSetsMatchingFilter.h"

int otbKeyPointSetsMatchingFilterNew(int argc, char * argv[])
{
  typedef float RealType;
  const unsigned int Dimension = 2;

  typedef itk::VariableLengthVector<RealType>           RealVectorType;
  typedef itk::PointSet<RealVectorType, Dimension>      PointSetType;
  typedef otb::KeyPointSetsMatchingFilter<PointSetType> EuclideanDistanceMatchingFilterType;

  // Instantiating object
  EuclideanDistanceMatchingFilterType::Pointer filter = EuclideanDistanceMatchingFilterType::New();

  std::cout << filter << std::endl;

  return EXIT_SUCCESS;
}
