/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkSpatialObjectPoint.h,v $
  Language:  C++
  Date:      $Date: 2004-10-05 15:40:49 $
  Version:   $Revision: 1.14 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkSpatialObjectPoint_h
#define __itkSpatialObjectPoint_h

#include "itkPoint.h"
#include "vnl/vnl_vector_fixed.h"
#include "itkRGBAPixel.h"

namespace itk 
{

/** \class SpatialObjectPoint
* \brief Point used for spatial objets
*
* This class contains all the functions necessary to define a point
*
* \sa TubeSpatialObjectPoint SurfaceSpatialObjectPoint
*/ 
template < unsigned int TPointDimension = 3 >
class SpatialObjectPoint 
{

public:

  /** Constructor. This one defines the # of dimensions in the 
   * SpatialObjectPoint */
  SpatialObjectPoint( void );

  /** Default destructor. */
  virtual ~SpatialObjectPoint( void );

  typedef SpatialObjectPoint                Self;
  typedef Point< double, TPointDimension >  PointType;
  typedef vnl_vector< double >              VectorType;
  typedef RGBAPixel< float >                PixelType;
  typedef PixelType                         ColorType;

  /** Get the SpatialObjectPoint Id. */
  int GetID( void ) const;

  /** Set the SpatialObjectPoint Id.*/
  void SetID(const int newID);

  /** Return a pointer to the point object.*/
  const PointType & GetPosition( void ) const;

  /** Set the point object. Couldn't use macros for these methods. */
  void SetPosition(const PointType & newX);
  void SetPosition(const double x0, const double x1);
  void SetPosition(const double x0, const double x1, const double x2);

  /** Copy one SpatialObjectPoint to another */
  Self & operator=(const SpatialObjectPoint & rhs);

  /** Set/Get color of the point */
  const PixelType & GetColor( void ) const;
  void SetColor(const PixelType & color );
  void SetColor(float r, float g, float b, float a=1);

  /** Set/Get red color of the point */
  void SetRed( float r );
  float GetRed( void ) const;

  /** Set/Get Green color of the point */
  void SetGreen( float g );
  float GetGreen( void ) const;

  /** Set/Get blue color of the point */
  void SetBlue( float b );
  float GetBlue( void ) const;

  /** Set/Get alpha value of the point */
  void SetAlpha( float a);
  float GetAlpha( void ) const;

  /** PrintSelf method */
  void Print(std::ostream &os) const;  

protected:

  /** PrintSelf method */
  virtual void PrintSelf(std::ostream &os, Indent indent) const;  

  /** A unique ID assigned to this SpatialObjectPoint */
  int m_ID;

  /** Position of the point */
  PointType m_X;

  /** Color of the point */
  PixelType m_Color;

};

} // end of namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkSpatialObjectPoint.txx"
#endif

#endif // __itkSpatialObjectPoint_h
