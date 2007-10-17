/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkMetaContourConverter.h,v $
  Language:  C++
  Date:      $Date: 2007/01/28 19:24:38 $
  Version:   $Revision: 1.2 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkMetaContourConverter_h
#define __itkMetaContourConverter_h


#include "metaContour.h"
#include "itkContourSpatialObject.h"
#include "itkSpatialObject.h"

namespace itk 
{

template <unsigned int NDimensions = 3>
class MetaContourConverter
{

public:

  MetaContourConverter();
  ~MetaContourConverter() {};

  typedef itk::ContourSpatialObject<NDimensions>    SpatialObjectType;
  typedef typename SpatialObjectType::TransformType TransformType;
  typedef typename SpatialObjectType::Pointer       SpatialObjectPointer;


  SpatialObjectPointer ReadMeta(const char* name);

  bool WriteMeta(SpatialObjectType* spatialObject,const char* name);

  SpatialObjectPointer MetaContourToContourSpatialObject(MetaContour * Contour);
  MetaContour* ContourSpatialObjectToMetaContour(
                                             SpatialObjectType * spatialObject);

};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkMetaContourConverter.txx"
#endif

#endif
