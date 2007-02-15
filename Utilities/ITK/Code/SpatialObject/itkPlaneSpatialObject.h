/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkPlaneSpatialObject.h,v $
  Language:  C++
  Date:      $Date: 2006/06/06 15:42:16 $
  Version:   $Revision: 1.15 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkPlaneSpatialObject_h
#define __itkPlaneSpatialObject_h

#include "itkSpatialObject.h"
#include "itkAffineTransform.h"


namespace itk
{

/** \class PlaneSpatialObject
 * A plane spatial object is defined by two points
 * \brief 
 */

template < unsigned int TDimension = 3  >
class PlaneSpatialObject 
  : public SpatialObject< TDimension >
{

public:

  /** Standard typedefs */
  typedef PlaneSpatialObject Self;
  typedef double ScalarType;
  typedef SmartPointer < Self > Pointer;
  typedef SmartPointer < const Self > ConstPointer;
  typedef SpatialObject< TDimension > Superclass;
  typedef SmartPointer<Superclass> SuperclassPointer;
  typedef typename Superclass::PointType              PointType;
  typedef typename Superclass::TransformType          TransformType;
  typedef VectorContainer<unsigned long,PointType>    PointContainerType;
  typedef SmartPointer<PointContainerType>            PointContainerPointer;
  typedef typename Superclass::BoundingBoxType        BoundingBoxType;


  itkStaticConstMacro(NumberOfDimension, unsigned int,
                      TDimension);

  itkNewMacro( Self );
  itkTypeMacro( PlaneSpatialObject, SpatialObject );

  /** Returns a degree of membership to the object. 
   *  That's useful for fuzzy objects. */ 
  virtual bool ValueAt( const PointType & point, double & value,
                        unsigned int depth=0, char * name=NULL) const;
     
  /** return ture if the object provides a method to evaluate the value 
   * at the specified point, else otherwise.*/
  virtual bool IsEvaluableAt( const PointType & point,
                              unsigned int depth=0, char * name=NULL) const;

  /** Test whether a point is inside or outside the object */ 
  virtual bool IsInside( const PointType & point,
                         unsigned int depth, char * name) const;

  /** Test whether a point is inside or outside the object 
   *  For computational speed purposes, it is faster if the method does not
   *  check the name of the class and the current depth */ 
  virtual bool IsInside( const PointType & point) const;

  /** provide a method to get the boundaries of 
  *  a specific object. Basically, this function need to be called
  *  every time one of the object component is changed. */ 
  virtual bool ComputeLocalBoundingBox() const;

  itkSetMacro(LowerPoint,PointType);
  itkSetMacro(UpperPoint,PointType);
  itkGetMacro(LowerPoint,PointType);
  itkGetMacro(UpperPoint,PointType);

protected:
  PlaneSpatialObject(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
   
  PlaneSpatialObject( void );
  ~PlaneSpatialObject( void );

  PointType m_LowerPoint;
  PointType m_UpperPoint;

  /** Print the object informations in a stream. */
  virtual void PrintSelf( std::ostream& os, Indent indent ) const; 

};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkPlaneSpatialObject.txx"
#endif

#endif // __itkPlaneSpatialObject_h
