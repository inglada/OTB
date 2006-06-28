/*=========================================================================

Program:   Insight Segmentation & Registration Toolkit
Module:    $RCSfile: itkPolygonSpatialObject.txx,v $
Language:  C++
Date:      $Date: 2006/03/19 04:37:20 $
Version:   $Revision: 1.16 $

Copyright (c) Insight Software Consortium. All rights reserved.
See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

This software is distributed WITHOUT ANY WARRANTY; without even 
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkPolygonSpatialObject_txx
#define __itkPolygonSpatialObject_txx

#include "itkPolygonSpatialObject.h"
#include "itkExceptionObject.h"

namespace itk
{

template <unsigned int TDimension >
PolygonGroupOrientation
PolygonSpatialObject<TDimension>
::Plane()
{
  PolygonGroupOrientation plane;
  // local typedef to shut up the compiler...
  
  PointListType &points = this->GetPoints();
  typename PointListType::iterator it = points.begin();
  typename PointListType::iterator itend = points.end();
  double min[3],max[3];       // x, y, z
  int i;
  for(i = 0; i < 3; i++)
    {
    max[i] = NumericTraits<double>::NonpositiveMin();
    min[i] = NumericTraits<double>::max();
    }
  while (it != itend)
    {
    PointType curpoint = (*it).GetPosition();
    for(i = 0; i < 3; i++) 
      {
      if(min[i] > curpoint[i]) min[i] = curpoint[i];
      if(max[i] < curpoint[i]) max[i] = curpoint[i];
      }
    it++;
    }
  if(min[0] == max[0] && min[1] != max[1] && min[2] != max[2])
    {
    plane = Sagittal;
    }
  else if(min[0] != max[0] && min[1] == max[1] && min[2] != max[2])
    {
    plane = Axial;
    }
  else if(min[0] != max[0] && min[1] != max[1] && min[2] == max[2])
    {
    plane = Coronal;
    }
  return plane;
}

template <unsigned int TDimension >
bool 
PolygonSpatialObject<TDimension>
::IsClosed()
{
  PointListType &points = this->GetPoints();
  typename PointListType::iterator it = points.begin();
  typename PointListType::iterator itend = points.end();
  itend--;
  return (*it).GetPosition() == (*itend).GetPosition();
}

template <unsigned int TDimension >
unsigned int
PolygonSpatialObject<TDimension>
::NumberOfPoints() const
{
  return (this->GetPoints()).size();
}

template <unsigned int TDimension >
typename PolygonSpatialObject<TDimension>::PointType
PolygonSpatialObject<TDimension>
::ClosestPoint(PointType &curPoint)
{
  PointListType &points = this->GetPoints();
  typename PointListType::iterator it = points.begin();
  typename PointListType::iterator itend = points.end();
  double distance = NumericTraits<double>::max();
  
  if(it == itend)
    { 
    ExceptionObject exception(__FILE__, __LINE__);
    exception.SetDescription(
      "PolygonSpatialObject: ClosestPoint called using an empty point list");
    throw exception;
    }
    
  PointType closestPoint;
  while (it != itend)
    {
    typename SpatialObjectPoint<TDimension>::PointType curpos 
      = (*it).GetPosition();
    double curdistance = curpos.EuclideanDistanceTo(curPoint);
    if(curdistance < distance)
      {
      closestPoint = (*it).GetPosition();
      }
    it++;
    }
  return closestPoint;
}

template <unsigned int TDimension >
double
PolygonSpatialObject<TDimension>
::MeasureArea()
{
  //To find the area of a planar polygon not in the x-y plane, use:
  //2 A(P) = vcl_abs(N . (sum_{i=0}^{n-1} (v_i x v_{i+1})))
  //where N is a unit vector normal to the plane. The `.' represents the
  //dot product operator, the `x' represents the cross product operator,
  //        and vcl_abs() is the absolute value function.
  double area = 0.0;
  int numpoints = this->NumberOfPoints();
  int X, Y;
  if(numpoints < 3)
    {
    return 0;
    }
  switch(this->Plane())
    {
    case Sagittal:
      X = 1; Y = 2;
      break;
    case Axial:
      X = 0; Y = 2;
      break;
    case Coronal:
      X = 0; Y = 1;
      break;
    default:
      ExceptionObject exception(__FILE__, __LINE__);
      exception.SetDescription("File cannot be read");
      throw exception;
    }
  PointListType &points = this->GetPoints();
  typename PointListType::iterator it = points.begin();
  PointType start = (*it).GetPosition();
  for(int i = 0; i < numpoints; i++)
    {
    PointType a = (*it).GetPosition();
    PointType b;
    it++;
    if(i == numpoints - 1)
      {
      b = start;
      }
    else
      {
      b = (*it).GetPosition();
      }
    //
    // closed PolygonGroup has first and last points the same
    if(a == b)
      {
      continue;
      }
    area += a[X] * b[Y] - a[Y] * b[X];
    }
  area *= 0.5;
  return area < 0.0 ? -area : area;
}

template <unsigned int TDimension >
double 
PolygonSpatialObject<TDimension>
::MeasureVolume()
{
  return m_Thickness * this->MeasureArea();
}

template <unsigned int TDimension >
double 
PolygonSpatialObject<TDimension>
::MeasurePerimeter()
{
  double perimeter = 0.0;
  int numpoints = this->NumberOfPoints();
  if(numpoints < 3)
    {
    return 0;
    }
  PointListType &points = this->GetPoints();
  typename PointListType::iterator it = points.begin();
  PointType start = (*it).GetPosition();
  for(int i = 0; i < numpoints; i++)
    {
    PointType a = (*it).GetPosition();
    PointType b;
    it++;
    if(i == numpoints - 1)
      {
      b = start;
      }
    else
      {
      b = (*it).GetPosition();
      }
    //
    // closed PolygonGroup has first and last points the same
    if(a == b)
      {
      continue;
      }
    double curdistance = a.EuclideanDistanceTo(b);
    perimeter += curdistance;
    }
  return perimeter;
}

template <unsigned int TDimension >
bool 
PolygonSpatialObject<TDimension>
::DeletePoint(PointType &pointToDelete)
{
    
  PointListType &points = this->GetPoints();
  typename PointListType::iterator it = points.begin();
  typename PointListType::iterator itend = points.end();
  if(it == itend)
    { 
    return false;
    }
    
  while (it != itend)
    {
    BlobPointType &curPoint = (*it);
    typename SpatialObjectPoint<TDimension>::PointType curpos 
      = curPoint.GetPosition();
    if(curpos == pointToDelete)
      {
      points.erase(it);
      return true;
      }
    it++;
    }
  return false;
}

template <unsigned int TDimension >
bool 
PolygonSpatialObject<TDimension>
::AddPoint(PointType &pointToAdd)
{
  BlobPointType newPoint;
  newPoint.SetPosition(pointToAdd);
  this->GetPoints().push_back(newPoint);
  return true;
}

template <unsigned int TDimension >
bool 
PolygonSpatialObject<TDimension>
::InsertPoint(PointType &point1, PointType &pointToAdd)
{
  
  PointListType &points = this->GetPoints();
  typename PointListType::iterator it = points.begin();
  typename PointListType::iterator itend = points.end();
  if(it == itend)
    { 
    this->AddPoint(pointToAdd);
    return true;
    }
    
  while (it != itend)
    {
    BlobPointType &curPoint = (*it);
    typename SpatialObjectPoint<TDimension>::PointType curpos 
      = curPoint.GetPosition();
    if(curpos == point1)
      {
      typename PointListType::iterator after = it;
      after++;
      BlobPointType newPoint;
      newPoint.SetPosition(pointToAdd);
      points.insert(after,1,newPoint);
      return true;
      }
    it++;
    }
  return false;
}

template <unsigned int TDimension >
bool 
PolygonSpatialObject<TDimension>
::ReplacePoint(PointType &oldpoint, PointType &newPoint)
{
  PointListType &points = this->GetPoints();
  typename PointListType::iterator it = points.begin();
  typename PointListType::iterator itend = points.end();
  if(it == itend)
    { 
    this->AddPoint(newPoint);
    return true;
    }
    
  while(it != itend)
    {
    BlobPointType &curPoint = (*it);
    typename SpatialObjectPoint<TDimension>::PointType curpos 
      = curPoint.GetPosition();
    if(curpos == oldpoint)
      {
      typename PointListType::iterator after = it;
      after++;
      BlobPointType newBlobPoint;
      newBlobPoint.SetPosition(newPoint);
      points.insert(after,1,newBlobPoint);
      points.erase(it);
      return true;
      }
    it++;
    }
  return false;
}

template <unsigned int TDimension >
bool 
PolygonSpatialObject<TDimension>
::RemoveSegment(PointType &startpoint, PointType &endPoint)
{
  PointListType &points = this->GetPoints();
  typename PointListType::iterator it = points.begin();
  typename PointListType::iterator itend = points.end();
  typename PointListType::iterator first;
  typename PointListType::iterator last;
  if(it == itend)
    { 
    return false;
    }
  int foundcount = 0;
  while(it != itend)
    {
    BlobPointType &curPoint = (*it);
    typename SpatialObjectPoint<TDimension>::PointType curpos 
      = curPoint.GetPosition();
    if(curpos == startpoint) 
      {
      first = it;
      foundcount++;
      } 
    //
    // make sure you find the start before you find the end
    else if(foundcount > 0 && curpos == endPoint)
      {
      last = it;
      foundcount++;
      }
    if(foundcount == 2)
      {
      break;
      }
    it++;
    }
  if(foundcount != 2)
    {
    return false;
    }

  points.erase(last);         // stl_vector doesn't erase end of range
  points.erase(first,last);
  return true;
}

template <unsigned int TDimension >
bool 
PolygonSpatialObject<TDimension>
::IsInside( const PointType & point,unsigned int ,char * ) const
{
  int numpoints = this->NumberOfPoints();
  int X, Y;
  if(numpoints < 3)
    {
    return false;
    }
  switch(const_cast<Self *>(this)->Plane())
    {
    case Sagittal:
      X = 1; Y = 2;
      break;
    case Axial:
      X = 0; Y = 2;
      break;
    case Coronal:
      X = 0; Y = 1;
      break;
    default:
      ExceptionObject exception(__FILE__, __LINE__);
      exception.SetDescription("non-planar polygon");
      throw exception;
    }

  typename TransformType::Pointer inverse = TransformType::New();
  if(!this->GetIndexToWorldTransform()->GetInverse(inverse))
    {
    return false;
    }

  PointType transformedPoint = inverse->TransformPoint(point);

  PointListType &points = const_cast<Self *>(this)->GetPoints();
  typename PointListType::iterator it = points.begin();
  typename PointListType::iterator itend = points.end();
  PointType start = (*it).GetPosition();
  PointType last = (*itend).GetPosition();
  //
  // if last point same as first, don't bother with it.
  if(start == last)
    {
    numpoints--;
    }
  bool oddNodes = false;
  for(int i = 0; i < numpoints; i++)
    {
    PointType start = (*it).GetPosition();
    it++;
    PointType end;
    if(i == numpoints - 1)
      {
      end = start;
      }
    else
      {
      end = (*it).GetPosition();
      }
    double x = transformedPoint[X]; double y = transformedPoint[Y];

    if((start[Y] < y && end[Y] >= y) ||
       (end[Y] < y && start[Y] >= y))
      {
      if( start[X] + (y - start[Y])/
          (end[Y] - start[Y]) * (end[X] - start[X]) < x )
        {
        oddNodes = !oddNodes;
        }
      }
    }
  return oddNodes;
}
}
#endif
