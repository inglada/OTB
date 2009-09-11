/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkMetaLandmarkConverter.h,v $
  Language:  C++
  Date:      $Date: 2009-04-07 14:34:06 $
  Version:   $Revision: 1.4 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkMetaLandmarkConverter_h
#define __itkMetaLandmarkConverter_h


#include "metaLandmark.h"
#include "itkLandmarkSpatialObject.h"
#include "itkSpatialObject.h"

namespace itk 
{

template <unsigned int NDimensions = 3>
class ITK_EXPORT MetaLandmarkConverter
{

public:

  MetaLandmarkConverter();
  ~MetaLandmarkConverter() {};

  typedef itk::LandmarkSpatialObject<NDimensions> SpatialObjectType;


  typedef typename SpatialObjectType::TransformType TransformType;

  typedef typename SpatialObjectType::Pointer SpatialObjectPointer;

  //typedef typename itk::NDimensionalSpatialObject NDimSpatialObject;

  SpatialObjectPointer ReadMeta(const char* name);

  bool WriteMeta(SpatialObjectType* spatialObject,const char* name);

  SpatialObjectPointer MetaLandmarkToLandmarkSpatialObject(
                                                   MetaLandmark * landmark);
  MetaLandmark* LandmarkSpatialObjectToMetaLandmark(
                                         SpatialObjectType * spatialObject);

};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
  #include "itkMetaLandmarkConverter.txx"
#endif


#endif
