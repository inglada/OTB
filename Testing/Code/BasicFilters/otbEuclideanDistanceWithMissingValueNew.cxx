/*=========================================================================

Program:   ORFEO Toolbox
Language:  C++
Date:      $Date$
Version:   $Revision$


Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
See OTBCopyright.txt for details.

Copyright (c) Institut Telecom ; Telecom Bretagne. All rights reserved. 
See ITCopyright.txt for details.

This software is distributed WITHOUT ANY WARRANTY; without even 
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


#include <iostream>
#include <cstdlib>
#include "itkVariableLengthVector.h"
#include "otbEuclideanDistanceWithMissingValue.h"

int otbEuclideanDistanceWithMissingValueNew( int argc, char * argv[] )
{

  typedef itk::VariableLengthVector< double > VectorType;
  typedef otb::Statistics::EuclideanDistanceWithMissingValue< VectorType > DistanceType;

  DistanceType::Pointer dist = DistanceType::New();

  return EXIT_SUCCESS;
}



