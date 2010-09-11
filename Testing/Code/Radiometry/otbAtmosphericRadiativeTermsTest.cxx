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

#include "itkExceptionObject.h"
#include "otbAtmosphericRadiativeTerms.h"
#include <cstdlib>

int otbAtmosphericRadiativeTermsNew(int argc, char * argv[])
{
  typedef otb::AtmosphericRadiativeTerms AtmosphericRadiativeTermsType;

  // Instantiating object
  AtmosphericRadiativeTermsType::Pointer object = AtmosphericRadiativeTermsType::New();

  return EXIT_SUCCESS;
}

int otbAtmosphericRadiativeTermsSingleChannelNew(int argc, char * argv[])
{
  typedef otb::AtmosphericRadiativeTermsSingleChannel AtmosphericRadiativeTermsType;

  // Instantiating object
  AtmosphericRadiativeTermsType::Pointer object = AtmosphericRadiativeTermsType::New();

  return EXIT_SUCCESS;
}

