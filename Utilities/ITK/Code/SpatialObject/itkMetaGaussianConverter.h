/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkMetaGaussianConverter.h,v $
  Language:  C++
  Date:      $Date: 2004/02/19 15:58:44 $
  Version:   $Revision: 1.1 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __MetaGaussianConverter__h
#define __MetaGaussianConverter__h

#include "itkGaussianSpatialObject.h"
#include "metaGaussian.h"

namespace itk 
{

template <unsigned int NDimensions = 3>
class MetaGaussianConverter
{

public:

  MetaGaussianConverter();
  ~MetaGaussianConverter() {};

  typedef itk::GaussianSpatialObject<NDimensions> SpatialObjectType;
  typedef typename SpatialObjectType::TransformType TransformType;

  typedef typename SpatialObjectType::Pointer SpatialObjectPointer;

  SpatialObjectPointer ReadMeta(const char* name);

  bool WriteMeta(SpatialObjectType* spatialObject,const char* name);

  SpatialObjectPointer MetaGaussianToGaussianSpatialObject(MetaGaussian * gaussian);
  MetaGaussian* GaussianSpatialObjectToMetaGaussian(SpatialObjectType * spatialObject);

};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
  #include "itkMetaGaussianConverter.txx"
#endif


#endif
