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
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "otbTestMain.h"

void RegisterTests()
{
  REGISTER_TEST(otbPanTexTextureImageFilterNew);
  REGISTER_TEST(otbPanTexTextureImageFilter);
  REGISTER_TEST(otbPanTexTextureImageFunctionFilterNew);
  REGISTER_TEST(otbPanTexTextureImageFunctionFilter);
  REGISTER_TEST(otbSFSTexturesImageFilterNew);
  REGISTER_TEST(otbSFSTexturesImageFilterTest);
  REGISTER_TEST(otbUrbanAreaDetectionImageFilterNew);
  REGISTER_TEST(otbUrbanAreaDetectionImageFilter);
  REGISTER_TEST(otbRegionImageToRectangularPathListFilterNew);
  REGISTER_TEST(otbRegionImageToRectangularPathListFilter);
}
