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

#include "otbRadarFunctors.h"
#include "itkUnaryFunctorImageFilter.h"
#include "otbImage.h"

int otbTerraSarRadarBrightnessComplexImageFilterNew(int argc, char * argv[])
{
  typedef std::complex<double>                                                                               ComplexPixelType;
  typedef otb::Image< ComplexPixelType >                                                                     ComplexImageType;

  typedef otb::Functor::TerraSarRadarBrightnessComplexImageFunctor< ComplexPixelType, ComplexPixelType >     FunctorType;

  typedef itk::UnaryFunctorImageFilter<ComplexImageType, ComplexImageType, FunctorType >                     FilterType;

  FilterType::Pointer filter = FilterType::New();

  return EXIT_SUCCESS;
}
