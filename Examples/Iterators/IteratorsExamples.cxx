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

// this file defines the Iterator Examples for the test driver
// and all it expects is that you have a function called RegisterTests

#include <iostream>
#include "itkTestMain.h"

void RegisterTests()
{
  REGISTER_TEST(ImageLinearIteratorWithIndexTest);
  REGISTER_TEST(ImageRandomConstIteratorWithIndexTest);
  REGISTER_TEST(ImageRegionIteratorTest);
  REGISTER_TEST(ImageRegionIteratorWithIndexTest);
  REGISTER_TEST(ImageSliceIteratorWithIndexTest);
}

#undef main
#define main ImageLinearIteratorWithIndexTest
#include "ImageLinearIteratorWithIndex.cxx"

#undef main
#define main ImageRandomConstIteratorWithIndexTest
#include "ImageRandomConstIteratorWithIndex.cxx"

#undef main
#define main ImageRegionIteratorTest
#include "ImageRegionIterator.cxx"

#undef main
#define main ImageRegionIteratorWithIndexTest
#include "ImageRegionIteratorWithIndex.cxx"

#undef main
#define main ImageSliceIteratorWithIndexTest
#include "ImageSliceIteratorWithIndex.cxx"
