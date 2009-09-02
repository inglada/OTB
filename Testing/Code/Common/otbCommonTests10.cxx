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
  REGISTER_TEST(otbPostGISConnectionImplementationNew);
  REGISTER_TEST(otbPostGISConnectionImplementationConnectToDB);
  REGISTER_TEST(otbPostGISConnectionImplementationPerformTransaction);  
  REGISTER_TEST(otbGISTableNew); 
  REGISTER_TEST(otbPostGISTableNew); 
  REGISTER_TEST(otbVectorDataToGISTableFilterNew); 
//   REGISTER_TEST(otbVectorDataToGISTableFilter); 
  REGISTER_TEST(otbLabelMapToGISTableFilterNew); 
  REGISTER_TEST(otbGISTableToVectorDataFilterNew); 
}
