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

#include <iostream>
#include "otbTestMain.h"

void RegisterTests()
{
  REGISTER_TEST(MarkovClassification1ExampleTest);
  REGISTER_TEST(MarkovClassification2ExampleTest);
  REGISTER_TEST(MarkovClassification3ExampleTest);
  REGISTER_TEST(MarkovRestaurationExampleTest);
  REGISTER_TEST(MarkovRegularizationExampleTest);
}

#undef main
#define main MarkovClassification1ExampleTest
#include "MarkovClassification1Example.cxx"

#undef main
#define main MarkovClassification2ExampleTest
#include "MarkovClassification2Example.cxx"

#undef main
#define main MarkovClassification3ExampleTest
#include "MarkovClassification3Example.cxx"

#undef main
#define main MarkovRestaurationExampleTest
#include "MarkovRestaurationExample.cxx"

#undef main
#define main MarkovRegularizationExampleTest
#include "MarkovRegularizationExample.cxx"
