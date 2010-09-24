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

#include "otbMRFOptimizerMetropolis.h"
#include <cstdlib>

int otbMRFOptimizerMetropolisNew(int argc, char * argv[])
{
  typedef otb::MRFOptimizerMetropolis MRFOptimizerType;

  MRFOptimizerType::Pointer object = MRFOptimizerType::New();

  std::cout << object << std::endl;

  return EXIT_SUCCESS;
}
