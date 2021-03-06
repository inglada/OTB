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

// this file defines the otbCommonTest for the test driver
// and all it expects is that you have a function called RegisterTests


#include "otbTestMain.h"

void RegisterTests()
{
  REGISTER_TEST(otbExtractSegmentsNew);
  REGISTER_TEST(otbExtractSegments);
  REGISTER_TEST(otbFourierMellinImageFilterNew);
//REGISTER_TEST(otbFourierMellinImageFilterTestFFT);
  REGISTER_TEST(otbFourierMellinImageFilter);
  REGISTER_TEST(otbImageToEdgePathFilterNew);
  REGISTER_TEST(otbImageToEdgePathFilter);
  REGISTER_TEST(otbNeighborhoodScalarProductFilterNew);
  REGISTER_TEST(otbNeighborhoodScalarProductFilter);
}
