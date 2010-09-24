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

#include "otbLabelizeConfidenceConnectedImageFilter.h"
#include "otbImage.h"

int otbLabelizeConfidenceConnectedImageFilterNew(int argc, char * argv[])
{
  typedef unsigned char InputPixelType;
  typedef unsigned char OutputPixelType;
  const unsigned int Dimension = 2;

  typedef otb::Image<InputPixelType, Dimension>  InputImageType;
  typedef otb::Image<OutputPixelType, Dimension> OutputImageType;

  typedef otb::LabelizeConfidenceConnectedImageFilter
  <InputImageType, OutputImageType> LabelizeFilterType;
  typedef LabelizeFilterType::Pointer LabelizeFilterPointerType;

  LabelizeFilterPointerType filter = LabelizeFilterType::New();

  std::cout << filter << std::endl;

  return EXIT_SUCCESS;
}
