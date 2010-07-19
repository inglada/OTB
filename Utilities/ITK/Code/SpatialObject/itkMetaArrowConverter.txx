/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkMetaArrowConverter.txx,v $
  Language:  C++
  Date:      $Date: 2010-06-11 21:14:05 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkMetaArrowConverter_txx
#define __itkMetaArrowConverter_txx

#include "itkMetaArrowConverter.h"

namespace itk  
{

/** Constructor */ 
template <unsigned int NDimensions>
MetaArrowConverter<NDimensions>
::MetaArrowConverter()
{
  
}


/** Convert a metaArrow into an arrow SpatialObject  */
template <unsigned int NDimensions>
typename MetaArrowConverter<NDimensions>::SpatialObjectPointer
MetaArrowConverter<NDimensions>
::MetaArrowToArrowSpatialObject(MetaArrow * arrow)
{ 
  SpatialObjectPointer spatialObject = SpatialObjectType::New();

  double spacing[NDimensions];
  float length=arrow->Length();
  
  for(unsigned int i=0;i<NDimensions;i++)
    {
    spacing[i]=arrow->ElementSpacing()[i];
    }
  
  
  // convert position and direction/orientation
  const double* metaPosition = arrow->Position();
  const double* metaDirection = arrow->Direction();
  typename SpatialObjectType::PointType position;
  typename SpatialObjectType::VectorType direction;
  for (unsigned int i = 0; i < NDimensions; i++)
    {
    position[i] = metaPosition[i];
    direction[i] = metaDirection[i];
    }
  spatialObject->SetPosition(position);
  spatialObject->SetDirection(direction);
  
  // convert the other fields
  spatialObject->GetIndexToObjectTransform()->SetScaleComponent(spacing);
  spatialObject->SetLength(length);
  spatialObject->GetProperty()->SetName(arrow->Name());
  spatialObject->SetId(arrow->ID());
  spatialObject->SetParentId(arrow->ParentID());
  spatialObject->GetProperty()->SetRed(arrow->Color()[0]);
  spatialObject->GetProperty()->SetGreen(arrow->Color()[1]);
  spatialObject->GetProperty()->SetBlue(arrow->Color()[2]);
  spatialObject->GetProperty()->SetAlpha(arrow->Color()[3]);
  
  return spatialObject;
}

/** Convert an arrow SpatialObject into a metaArrow */
template <unsigned int NDimensions>
MetaArrow*
MetaArrowConverter<NDimensions>
::ArrowSpatialObjectToMetaArrow(SpatialObjectType * spatialObject)
{ 
  MetaArrow* arrow = new MetaArrow(NDimensions);
  
  float length = spatialObject->GetLength();

  if(spatialObject->GetParent())
    {
    arrow->ParentID(spatialObject->GetParent()->GetId());
    }
  
  // convert position and direction
  double position[NDimensions];
  double direction[NDimensions];
  typename SpatialObjectType::PointType spPosition = spatialObject->GetPosition();
  typename SpatialObjectType::VectorType spDirection = spatialObject->GetDirection();
  for (unsigned int i = 0; i < NDimensions; i++)
    {
    position[i] = spPosition[i];
    direction[i] = spDirection[i];
    }
  arrow->Position(position);
  arrow->Direction(direction);
  
  // convert the rest of the parameters
  arrow->Length(length);
  arrow->ID(spatialObject->GetId());

  arrow->Color(spatialObject->GetProperty()->GetRed(),
                 spatialObject->GetProperty()->GetGreen(),
                 spatialObject->GetProperty()->GetBlue(),
                 spatialObject->GetProperty()->GetAlpha());
  
  for(unsigned int i=0;i<NDimensions;i++)
    {
    arrow->ElementSpacing(i,spatialObject->GetIndexToObjectTransform()
                                         ->GetScaleComponent()[i]);
    }

  return arrow;
}


/** Read a meta file give the type */
template <unsigned int NDimensions>
typename MetaArrowConverter<NDimensions>::SpatialObjectPointer
MetaArrowConverter<NDimensions>
::ReadMeta(const char* name)
{
  SpatialObjectPointer spatialObject;
  MetaArrow* arrow = new MetaArrow();
  arrow->Read(name);
  spatialObject = MetaArrowToArrowSpatialObject(arrow);

  return spatialObject;
}


/** Write a meta arrow file */
template <unsigned int NDimensions>
bool
MetaArrowConverter<NDimensions>
::WriteMeta(SpatialObjectType* spatialObject,const char* name)
{
  MetaArrow* arrow = ArrowSpatialObjectToMetaArrow(spatialObject);
  arrow->Write(name);
  return true;
}

} // end namespace itk 

#endif
