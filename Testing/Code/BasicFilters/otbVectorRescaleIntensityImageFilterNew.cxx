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

#include "otbVectorRescaleIntensityImageFilter.h"
#include "otbVectorImage.h"

int otbVectorRescaleIntensityImageFilterNew(int argc, char * argv[])
{
  const unsigned int Dimension = 2;
  typedef double InputPixelType;
  typedef unsigned char OutputPixelType;
  typedef otb::VectorImage<InputPixelType,Dimension> InputImageType;
  typedef otb::VectorImage<OutputPixelType,Dimension> OutputImageType;
  typedef otb::VectorRescaleIntensityImageFilter<InputImageType,OutputImageType> VectorRescaleIntensityImageFilterType;
  
  // Instantiating object
  VectorRescaleIntensityImageFilterType::Pointer object = VectorRescaleIntensityImageFilterType::New();
  

  return EXIT_SUCCESS;
}
