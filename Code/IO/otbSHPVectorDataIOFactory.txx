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
#ifndef __otbSHPVectorDataIOFactory_txx
#define __otbSHPVectorDataIOFactory_txx

#include "otbSHPVectorDataIOFactory.h"

#include "itkCreateObjectFunction.h"
#include "itkVersion.h"
#include "otbSHPVectorDataIO.h"

namespace otb
{
template<class TData>
SHPVectorDataIOFactory<TData>::SHPVectorDataIOFactory()
{
  this->RegisterOverride("otbVectorDataIOBase",
                         "otbSHPVectorDataIO",
                         "SHP Vectordata IO",
                         1,
                         itk::CreateObjectFunction<SHPVectorDataIO<TData> >::New());
}
template<class TData>
SHPVectorDataIOFactory<TData>::~SHPVectorDataIOFactory()
{
}
template<class TData>
const char*
SHPVectorDataIOFactory<TData>::GetITKSourceVersion(void) const
{
  return ITK_SOURCE_VERSION;
}
template<class TData>
const char*
SHPVectorDataIOFactory<TData>::GetDescription() const
{
  return "SHP VectorDataIO Factory, allows the loading of ESRI shapefile vector data into OTB";
}

} // end namespace otb

#endif
