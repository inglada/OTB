/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkMetaGaussianConverter.h,v $
  Language:  C++
  Date:      $Date: 2009-04-07 14:34:05 $
  Version:   $Revision: 1.3 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkMetaGaussianConverter_h
#define __itkMetaGaussianConverter_h

#include "itkGaussianSpatialObject.h"
#include "metaGaussian.h"

namespace itk 
{

template <unsigned int NDimensions = 3>
class ITK_EXPORT MetaGaussianConverter
{

public:

  MetaGaussianConverter();
  ~MetaGaussianConverter() {};

  typedef itk::GaussianSpatialObject<NDimensions>   SpatialObjectType;
  typedef typename SpatialObjectType::TransformType TransformType;

  typedef typename SpatialObjectType::Pointer SpatialObjectPointer;

  SpatialObjectPointer ReadMeta(const char* name);

  bool WriteMeta(SpatialObjectType* spatialObject,const char* name);

  SpatialObjectPointer MetaGaussianToGaussianSpatialObject(
                                                      MetaGaussian * gaussian);
  MetaGaussian* GaussianSpatialObjectToMetaGaussian(
                                            SpatialObjectType * spatialObject);

};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
  #include "itkMetaGaussianConverter.txx"
#endif


#endif
