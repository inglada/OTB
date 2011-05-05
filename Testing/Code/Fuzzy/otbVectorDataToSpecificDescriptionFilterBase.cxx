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

#include "otbVectorDataToSpecificDescriptionFilterBase.h"

#include "otbVectorData.h"

int otbVectorDataToSpecificDescriptionFilterBaseNew(int argc, char* argv[])
{
  typedef float                           PrecisionType;
  typedef otb::VectorData<PrecisionType>  VectorDataType;
  typedef otb::VectorDataToSpecificDescriptionFilterBase<VectorDataType>                                                 VectorDataToSpecificDescriptionFilterType;

  VectorDataToSpecificDescriptionFilterType::Pointer filter = 
    VectorDataToSpecificDescriptionFilterType::New();
  
  std::cout<<filter<<std::endl;

  return EXIT_SUCCESS;
}
