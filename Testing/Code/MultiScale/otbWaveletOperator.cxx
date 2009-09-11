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

#include "otbWaveletOperator.h"

int otbWaveletOperator(int argc, char * argv[])
{
  const otb::InverseOrForwardTransformationEnum directionID = otb::FORWARD;
  const otb::MotherWaveletOperatorEnum wvltID = otb::HAAR;

  const int Dimension = 2;
  typedef double PixelType;


  typedef otb::WaveletOperator< wvltID, directionID, PixelType, Dimension > 
    WaveletOperator;
  typedef WaveletOperator::LowPassOperator LowPassOperatorType;
  typedef WaveletOperator::HighPassOperator HighPassOperatorType;

  LowPassOperatorType h_filter;
  h_filter.SetDirection(0);
  h_filter.SetUpSampleFactor(2);
  h_filter.CreateDirectional();

  HighPassOperatorType g_filter;
  g_filter.SetDirection(0);
  g_filter.SetUpSampleFactor(2);
  g_filter.CreateDirectional();

  return EXIT_SUCCESS;
}




