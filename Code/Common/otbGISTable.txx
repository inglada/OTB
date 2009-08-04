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
#ifndef __otbGISTable_txx
#define __otbGISTable_txx

#include "otbGISTable.h"
#include "itkPreOrderTreeIterator.h"
#include "otbMetaDataKey.h"

namespace otb
{

  template <class TConnectionImplementation, class TPrecision, unsigned int SpatialDimension>
      GISTable<TConnectionImplementation, TPrecision, SpatialDimension>
::GISTable()
{
  m_TableName = "";
  m_Connection = ConnectionType::New();
}


template <class TConnectionImplementation, class TPrecision, unsigned int SpatialDimension>
void
    GISTable<TConnectionImplementation, TPrecision, SpatialDimension>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os,indent);
  os<<std::endl;


  os<<"Table name: "<< m_TableName<<std::endl;

}


} // end namespace otb

#endif
