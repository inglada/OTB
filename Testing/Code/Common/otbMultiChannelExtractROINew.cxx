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
#include "otbMultiChannelExtractROI.h"


int otbMultiChannelExtractROINew ( int argc, char * argv[] )
{
  //        typedef unsigned char                                    InputPixelType;
  //        typedef unsigned char                                    OutputPixelType;

  typedef std::complex<float>                              InputPixelType;
  typedef std::complex<double>                      OutputPixelType;

  typedef otb::MultiChannelExtractROI< InputPixelType,
                                       OutputPixelType >  ExtractROIFilterType;


  ExtractROIFilterType::Pointer extractROIFilter = ExtractROIFilterType::New();



  return EXIT_SUCCESS;
}

