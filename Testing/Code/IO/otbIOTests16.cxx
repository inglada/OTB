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
  REGISTER_TEST(otbRADImageIOTestCanRead);
  REGISTER_TEST(otbImageFileReaderRADChar);
  REGISTER_TEST(otbImageFileReaderRADInt);
  REGISTER_TEST(otbImageFileReaderRADFloat);
  REGISTER_TEST(otbImageFileReaderRADComplexDouble);
  REGISTER_TEST(otbImageFileReaderRADComplexFloat);
  REGISTER_TEST(otbImageFileReaderRADComplexFloatExtract);
  REGISTER_TEST(otbImageFileReaderRADComplexInt);
  REGISTER_TEST(otbImageSeriesFileReader);
}
