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
#include "otbVectorDataToVectorDataFilter.h"
#include "otbVectorData.h"

int otbVectorDataToVectorDataFilterNew(int argc, char * argv[])
{


  typedef otb::VectorData<double > InputVectorDataType;
  typedef otb::VectorData<unsigned char,2 > OutputVectorDataType;

  typedef otb::VectorDataToVectorDataFilter<InputVectorDataType,OutputVectorDataType> VectorDataFilterType;

  VectorDataFilterType::Pointer vectorData = VectorDataFilterType::New();


  return EXIT_SUCCESS;
}


