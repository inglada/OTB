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

#include "otbSHPVectorDataIO.h"
#include "otbVectorData.h"

int otbSHPVectorDataIONew(int argc, char * argv[])
{
  typedef otb::VectorData<> VectorDataType;
  typedef otb::SHPVectorDataIO<VectorDataType> SHPVectorDataIOType;
  SHPVectorDataIOType::Pointer object = SHPVectorDataIOType::New();
  return 0;
}
