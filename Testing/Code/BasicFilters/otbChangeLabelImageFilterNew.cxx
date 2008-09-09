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

#include "otbVectorImage.h"
#include "otbChangeLabelImageFilter.h"
#include "itkChangeLabelImageFilter.h"
#include "otbImage.h"

int otbChangeLabelImageFilterNew(int argc, char * argv[])
{
  const unsigned int ImageDimension = 2;

  typedef otb::Image<unsigned int, ImageDimension> InputImageType;
  typedef otb::VectorImage<unsigned int, ImageDimension> OutputImageType;

  typedef otb::ChangeLabelImageFilter<InputImageType, OutputImageType>   FilterType;

  FilterType::Pointer filter = FilterType::New();


  return EXIT_SUCCESS;

}
