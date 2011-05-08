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
  REGISTER_TEST(otbMultiToMonoChannelExtractROI);
  REGISTER_TEST(otbMultiToMonoChannelExtractROINew);
  REGISTER_TEST(otbPointSetSourceTest);
  REGISTER_TEST(otbDrawLineSpatialObjectNew);
  REGISTER_TEST(otbDrawLineSpatialObject);
  REGISTER_TEST(otbDrawLineSpatialObjectListNew);
  REGISTER_TEST(otbDrawLineSpatialObjectList);
  REGISTER_TEST(otbImageToLineSpatialObjectListNew);
  REGISTER_TEST(otbPathListToHistogramGeneratorNew);
  REGISTER_TEST(otbPathListToHistogramGenerator);
  REGISTER_TEST(otbLineSpatialObjectListToPointSetFilterNew);
}
