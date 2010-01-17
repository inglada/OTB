/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkMetaGroupConverter.txx,v $
  Language:  C++
  Date:      $Date: 2007-01-28 19:24:56 $
  Version:   $Revision: 1.4 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkMetaGroupConverter_txx
#define __itkMetaGroupConverter_txx

#include "itkMetaGroupConverter.h"

namespace itk  
{

/** Constructor */ 
template <unsigned int NDimensions>
MetaGroupConverter<NDimensions>
::MetaGroupConverter()
{
  
}


/** Convert a metaGroup into an group SpatialObject  */
template <unsigned int NDimensions>
typename MetaGroupConverter<NDimensions>::SpatialObjectPointer
MetaGroupConverter<NDimensions>
::MetaGroupToGroupSpatialObject(MetaGroup * group)
{ 
  SpatialObjectPointer spatialObject = SpatialObjectType::New();
  double spacing[NDimensions];
  unsigned int i;
  for(i=0; i<NDimensions; i++)
    {
    spacing[i] = group->ElementSpacing()[i];
    }
  spatialObject->GetIndexToObjectTransform()->SetScaleComponent(spacing);
  spatialObject->GetProperty()->SetName(group->Name());
  spatialObject->GetProperty()->SetRed(group->Color()[0]);
  spatialObject->GetProperty()->SetGreen(group->Color()[1]);
  spatialObject->GetProperty()->SetBlue(group->Color()[2]);
  spatialObject->GetProperty()->SetAlpha(group->Color()[3]);
  spatialObject->SetId(group->ID());
  spatialObject->SetParentId(group->ParentID());
  return spatialObject;
}

/** Convert an group SpatialObject into a metaGroup */
template <unsigned int NDimensions>
MetaGroup*
MetaGroupConverter<NDimensions>
::GroupSpatialObjectToMetaGroup(SpatialObjectType * spatialObject)
{ 
  MetaGroup* group = new MetaGroup(NDimensions);

  float color[4];
  for(unsigned int i=0;i<4;i++)
    {
    color[i]=spatialObject->GetProperty()->GetColor()[i];
    }
  group->Color(color);

  for(unsigned int i=0;i<NDimensions;i++)
    {
    group->ElementSpacing(i,spatialObject->GetIndexToObjectTransform()
                                         ->GetScaleComponent()[i]);
    }

  if(spatialObject->GetParent())
    {
    group->ParentID(spatialObject->GetParent()->GetId());
    }
  group->ID(spatialObject->GetId());

  return group;
}


/** Read a meta file give the type */
template <unsigned int NDimensions>
typename MetaGroupConverter<NDimensions>::SpatialObjectPointer
MetaGroupConverter<NDimensions>
::ReadMeta(const char* name)
{
  SpatialObjectPointer spatialObject;
  MetaGroup* group = new MetaGroup();
  group->Read(name);
  spatialObject = this->MetaGroupToGroupSpatialObject(group);

  return spatialObject;
}


/** Write a meta group file */
template <unsigned int NDimensions>
bool
MetaGroupConverter<NDimensions>
::WriteMeta(SpatialObjectType* spatialObject,const char* name)
{
  MetaGroup* group = this->GroupSpatialObjectToMetaGroup(spatialObject);
  group->Write(name);
  return true;
}

} // end namespace itk 

#endif
