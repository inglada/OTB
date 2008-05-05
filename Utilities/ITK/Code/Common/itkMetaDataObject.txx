/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkMetaDataObject.txx,v $
  Language:  C++
  Date:      $Date: 2005-04-05 19:17:24 $
  Version:   $Revision: 1.10 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

  Portions of this code are covered under the VTK copyright.
  See VTKCopyright.txt or http://www.kitware.com/VTKCopyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itkMetaDataObject_txx
#define _itkMetaDataObject_txx

#include "itkMetaDataObject.h"

template<class MetaDataObjectType>
itk::MetaDataObject<MetaDataObjectType>
::MetaDataObject(void)
{
  //Nothing to do, m_MetaDataObjectValue takes this types default value.
}

template<class MetaDataObjectType>
itk::MetaDataObject<MetaDataObjectType>
::~MetaDataObject(void)
{
  //std::cout << "                            MetaDataObject Deleteing: " << this << std::endl;
  //Nothing to do here.
}


template<class MetaDataObjectType>
itk::MetaDataObject<MetaDataObjectType>
::MetaDataObject(const MetaDataObjectType InitializerValue)
  :m_MetaDataObjectValue(InitializerValue)
{
  //Nothing to be done here
}

template<class MetaDataObjectType>
itk::MetaDataObject<MetaDataObjectType>
::MetaDataObject(const MetaDataObject<MetaDataObjectType> &TemplateObject)
  :m_MetaDataObjectValue(TemplateObject.m_MetaDataObjectValue)
{
  //Nothing to be done here
}

template<class MetaDataObjectType>
const char *
itk::MetaDataObject<MetaDataObjectType>
::GetMetaDataObjectTypeName(void) const
{
  return typeid(MetaDataObjectType).name();
}

template<class MetaDataObjectType>
const std::type_info &
itk::MetaDataObject<MetaDataObjectType>
::GetMetaDataObjectTypeInfo(void) const
{
  return typeid(MetaDataObjectType);
}

template<class MetaDataObjectType>
const MetaDataObjectType &
itk::MetaDataObject<MetaDataObjectType>
::GetMetaDataObjectValue(void) const
{
  return m_MetaDataObjectValue;
}

template<class MetaDataObjectType>
void
itk::MetaDataObject<MetaDataObjectType>
::SetMetaDataObjectValue(const MetaDataObjectType & NewValue )
{
  m_MetaDataObjectValue=NewValue;
}

template<class MetaDataObjectType>
void
itk::MetaDataObject<MetaDataObjectType>
::Print(std::ostream& os) const
{
        //  os << "[UNKNOWN PRINT CHARACTERISTICS]" << std::endl;
        Superclass::Print(os);
}
#endif
