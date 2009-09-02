/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkMetaGroupConverter.h,v $
  Language:  C++
  Date:      $Date: 2009-04-07 14:34:05 $
  Version:   $Revision: 1.4 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkMetaGroupConverter_h
#define __itkMetaGroupConverter_h

#include "itkGroupSpatialObject.h"
#include "metaGroup.h"

namespace itk 
{

template <unsigned int NDimensions = 3>
class ITK_EXPORT MetaGroupConverter
{

public:

  MetaGroupConverter();
  ~MetaGroupConverter() {};

  typedef itk::GroupSpatialObject<NDimensions>      SpatialObjectType;
  typedef typename SpatialObjectType::TransformType TransformType;

  typedef typename SpatialObjectType::Pointer SpatialObjectPointer;

  SpatialObjectPointer ReadMeta(const char* name);

  bool WriteMeta(SpatialObjectType* spatialObject,const char* name);

  SpatialObjectPointer MetaGroupToGroupSpatialObject(MetaGroup * group);
  MetaGroup* GroupSpatialObjectToMetaGroup(SpatialObjectType * spatialObject);

};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
  #include "itkMetaGroupConverter.txx"
#endif


#endif
