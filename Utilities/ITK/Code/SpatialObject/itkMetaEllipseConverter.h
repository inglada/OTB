/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkMetaEllipseConverter.h,v $
  Language:  C++
  Date:      $Date: 2009-04-07 14:34:04 $
  Version:   $Revision: 1.4 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkMetaEllipseConverter_h
#define __itkMetaEllipseConverter_h

#include "itkEllipseSpatialObject.h"
#include "metaEllipse.h"

namespace itk 
{

template <unsigned int NDimensions = 3>
class ITK_EXPORT MetaEllipseConverter
{

public:

  MetaEllipseConverter();
  ~MetaEllipseConverter() {};

  typedef itk::EllipseSpatialObject<NDimensions>    SpatialObjectType;
  typedef typename SpatialObjectType::TransformType TransformType;

  typedef typename SpatialObjectType::Pointer SpatialObjectPointer;

  SpatialObjectPointer ReadMeta(const char* name);

  bool WriteMeta(SpatialObjectType* spatialObject,const char* name);

  SpatialObjectPointer MetaEllipseToEllipseSpatialObject(MetaEllipse * ellipse);
  MetaEllipse* EllipseSpatialObjectToMetaEllipse(
                                             SpatialObjectType * spatialObject);

};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
  #include "itkMetaEllipseConverter.txx"
#endif


#endif
