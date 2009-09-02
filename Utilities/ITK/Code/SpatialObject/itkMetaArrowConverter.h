/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkMetaArrowConverter.h,v $
  Language:  C++
  Date:      $Date: 2009-04-07 14:34:04 $
  Version:   $Revision: 1.3 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkMetaArrowConverter_h
#define __itkMetaArrowConverter_h

#include "itkArrowSpatialObject.h"
#include "metaArrow.h"

namespace itk 
{

template <unsigned int NDimensions = 3>
class ITK_EXPORT MetaArrowConverter
{

public:

  MetaArrowConverter();
  ~MetaArrowConverter() {};

  typedef itk::ArrowSpatialObject<NDimensions>      SpatialObjectType;
  typedef typename SpatialObjectType::TransformType TransformType;

  typedef typename SpatialObjectType::Pointer SpatialObjectPointer;

  SpatialObjectPointer ReadMeta(const char* name);

  bool WriteMeta(SpatialObjectType* spatialObject,const char* name);

  SpatialObjectPointer MetaArrowToArrowSpatialObject(MetaArrow * arrow);
  MetaArrow* ArrowSpatialObjectToMetaArrow(SpatialObjectType * spatialObject);

};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
  #include "itkMetaArrowConverter.txx"
#endif


#endif
