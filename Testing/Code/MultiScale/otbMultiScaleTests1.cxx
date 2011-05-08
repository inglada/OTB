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

// this file defines the otbMultiScaleTest for the test driver
// and all it expects is that you have a function called RegisterTests


#include "otbTestMain.h"

void RegisterTests()
{
  REGISTER_TEST(otbMorphologicalPyramidResamplerNew);
  REGISTER_TEST(otbMorphologicalPyramidResampler);
  REGISTER_TEST(otbMorphologicalPyramidAnalysisFilterNew);
  REGISTER_TEST(otbMorphologicalPyramidAnalysisFilter);
  REGISTER_TEST(otbMorphologicalPyramidSynthesisFilterNew);
  REGISTER_TEST(otbMorphologicalPyramidSynthesisFilter);
  REGISTER_TEST(otbMorphologicalPyramidMRToMSConverterNew);
  REGISTER_TEST(otbMorphologicalPyramidMRToMSConverter);
  REGISTER_TEST(otbMorphologicalPyramidSegmenterNew);
  REGISTER_TEST(otbMorphologicalPyramidSegmenter);
}
