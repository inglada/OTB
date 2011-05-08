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
  REGISTER_TEST(otbMarkovRandomFieldFilterNew);
  REGISTER_TEST(otbMarkovRandomFieldFilter);
  REGISTER_TEST(otbMRFEnergyEdgeFidelityNew);
  REGISTER_TEST(otbMRFEnergyEdgeFidelity);
  REGISTER_TEST(otbMRFEnergyPottsNew);
  REGISTER_TEST(otbMRFEnergyPotts);
  REGISTER_TEST(otbMRFEnergyGaussianNew);
  REGISTER_TEST(otbMRFEnergyGaussian);
  REGISTER_TEST(otbMRFEnergyGaussianClassificationNew);
  REGISTER_TEST(otbMRFEnergyGaussianClassification);
  REGISTER_TEST(otbMRFOptimizerICMNew);
  REGISTER_TEST(otbMRFOptimizerICM);
}
