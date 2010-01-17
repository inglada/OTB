/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkMetaTubeConverter.h,v $
  Language:  C++
  Date:      $Date: 2009-04-07 14:34:11 $
  Version:   $Revision: 1.4 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkMetaTubeConverter_h
#define __itkMetaTubeConverter_h


#include "metaTube.h"
#include "itkSpatialObject.h"
#include "itkTubeSpatialObject.h"

namespace itk 
{

template <unsigned int NDimensions = 3>
class ITK_EXPORT MetaTubeConverter
{

public:

  MetaTubeConverter();
  ~MetaTubeConverter() {};

  typedef itk::TubeSpatialObject<NDimensions> SpatialObjectType;


  typedef typename SpatialObjectType::TransformType TransformType;

  typedef typename SpatialObjectType::Pointer SpatialObjectPointer;

  //typedef typename itk::NDimensionalSpatialObject NDimSpatialObject;

  SpatialObjectPointer ReadMeta(const char* name);

  bool WriteMeta(SpatialObjectType* spatialObject,const char* name);

  SpatialObjectPointer MetaTubeToTubeSpatialObject(MetaTube * Tube);
  MetaTube* TubeSpatialObjectToMetaTube(SpatialObjectType * spatialObject);

};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
  #include "itkMetaTubeConverter.txx"
#endif


#endif
