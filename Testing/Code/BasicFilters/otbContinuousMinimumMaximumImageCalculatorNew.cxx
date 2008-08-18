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
#include "itkExceptionObject.h"


#include "otbImage.h"
#include "otbContinuousMinimumMaximumImageCalculator.h"

int otbContinuousMinimumMaximumImageCalculatorNew(int argc, char * argv[])
{
  const unsigned int ImageDimension = 2;

  typedef otb::Image<unsigned int, ImageDimension> InputImageType;

  typedef otb::ContinuousMinimumMaximumImageCalculator<InputImageType>   FilterType;

  FilterType::Pointer filter = FilterType::New();


  return EXIT_SUCCESS;

}
