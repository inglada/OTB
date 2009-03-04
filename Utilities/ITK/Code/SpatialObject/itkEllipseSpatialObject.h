/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkEllipseSpatialObject.h,v $
  Language:  C++
  Date:      $Date: 2009-01-28 20:10:27 $
  Version:   $Revision: 1.28 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkEllipseSpatialObject_h
#define __itkEllipseSpatialObject_h

#include "itkSpatialObject.h"
#include "itkAffineTransform.h"
#include "itkFixedArray.h"

namespace itk
{

/** \class EllipseSpatialObject
 * 
 * \brief TODO
 */

template < unsigned int TDimension = 3 >
class EllipseSpatialObject 
  : public SpatialObject< TDimension >
{

public:

  typedef EllipseSpatialObject                     Self;
  typedef double                                   ScalarType;
  typedef SmartPointer < Self >                    Pointer;
  typedef SmartPointer < const Self >              ConstPointer;
  typedef SpatialObject< TDimension >              Superclass;
  typedef SmartPointer<Superclass>                 SuperclassPointer;
  typedef typename Superclass::PointType           PointType;
  typedef typename Superclass::TransformType       TransformType;
  typedef typename Superclass::BoundingBoxType     BoundingBoxType;
  typedef VectorContainer<unsigned long,PointType> PointContainerType;
  typedef SmartPointer<PointContainerType>         PointContainerPointer;

  typedef FixedArray<double,TDimension> ArrayType;
  itkStaticConstMacro(NumberOfDimension, unsigned int,
                      TDimension);

  itkNewMacro( Self );
  itkTypeMacro( EllipseSpatialObject, SpatialObject );

  /** Set all radii to the same radius value.  Each radius is
   *  half the length of one axis of the ellipse.  */
  void SetRadius(double radius);

  /** Set radii via an array of radius values */
  itkSetMacro(Radius,ArrayType);

  /** Get radii via an array of radius values */
  itkGetConstReferenceMacro(Radius,ArrayType);

  /** Returns a degree of membership to the object. 
   *  That's useful for fuzzy objects. */ 
  virtual bool ValueAt( const PointType & point, double & value, 
                        unsigned int depth=0,
                        char * name=NULL) const;
     
  /** Return true if the object provides a method to evaluate the value 
   * at the specified point, false otherwise. */
  virtual bool IsEvaluableAt( const PointType & point, 
                              unsigned int depth=0,
                              char * name=NULL) const;

  /** Test whether a point is inside or outside the object */ 
  virtual bool IsInside( const PointType & point,
                         unsigned int depth,
                         char *) const;

  /** Test whether a point is inside or outside the object 
   *  For computational speed purposes, it is faster if the method does not
   *  check the name of the class and the current depth */ 
  virtual bool IsInside( const PointType & point) const;

  /** Get the boundaries of a specific object.  This function needs to
   *  be called every time one of the object's components is
   *  changed. */ 
  virtual bool ComputeLocalBoundingBox() const;

  /** Copy the information from another SpatialObject */
  void CopyInformation(const DataObject *data);

protected:
  EllipseSpatialObject(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
   
  EllipseSpatialObject( void );
  ~EllipseSpatialObject( void );

  ArrayType m_Radius;

  /** Print the object informations in a stream. */
  virtual void PrintSelf( std::ostream& os, Indent indent ) const; 

};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkEllipseSpatialObject.txx"
#endif

#endif // __itkEllipseSpatialObject_h
