/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Copyright (c) Institut Telecom / Telecom Bretagne. All rights reserved.
  See ITCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "otbImage.h"
#include "otbWaveletOperator.h"
#include "otbWaveletFilterBank.h"
#include "otbWaveletTransform.h"

int otbWaveletInverseTransformNew(int argc, char * argv[])
{
  const int Dimension = 2;
  typedef double                           PixelType;
  typedef otb::Image<PixelType, Dimension> ImageType;

  /* Wavelet choice */
  const otb::Wavelet::Wavelet wvltID = otb::Wavelet::SYMLET8;

  /* Inverse Transformation */
  typedef otb::WaveletOperator<wvltID, otb::Wavelet::INVERSE, PixelType, Dimension>
  InverseWaveletOperator;
  typedef otb::WaveletFilterBank<ImageType, ImageType, InverseWaveletOperator, otb::Wavelet::INVERSE>
  InverseFilterBank;
  typedef otb::WaveletTransform<ImageType, ImageType, InverseFilterBank, otb::Wavelet::INVERSE>
  InvFilterType;

  InvFilterType::Pointer invFilter = InvFilterType::New();

  return EXIT_SUCCESS;
}
