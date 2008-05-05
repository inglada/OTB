/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkNonUniformBSpline.h,v $
  Language:  C++
  Date:      $Date: 2005-01-21 20:16:27 $
  Version:   $Revision: 1.4 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#ifndef __itkNonUniformBSpline_h
#define __itkNonUniformBSpline_h

#include <vector>

#include "itkPoint.h"
#include "itkObject.h"
#include "itkArray.h"

namespace itk {

/** 
* \class NonUniformBSpline
* \brief BSpline with nonuniform knot spacing.
*
* This class is a bspline with nonuniform knot spacing. The 
* use can specify a set of points and a set of knots and the 
* spline will attempt to find the control points which will 
* cause the spline to interpolate the points. 
*
* CAUTION: THIS CLASS IS STILL UNDER DEVELOPMENT.
*
*/

template < unsigned int TDimension = 3 >
class NonUniformBSpline 
  : public Object
{
 public:
  /**
  Typedefs
  */
  typedef NonUniformBSpline                            Self;
  typedef Object                                       Superclass;
  typedef SmartPointer < Self >                        Pointer;
  typedef SmartPointer < const Self >                  ConstPointer;
  typedef double                                       ScalarType;
  typedef itk::Point< ScalarType, TDimension >         PointType;
  typedef std::vector < PointType >                    PointListType;
  typedef PointListType *                              PointListPointer;
  typedef std::vector < double >                       KnotListType;
  typedef std::vector<double>                          CoordinateListType;
  typedef itk::Point<double, TDimension >              ControlPointType;
  typedef std::vector< ControlPointType >              ControlPointListType;
  typedef ControlPointListType *                       ControlPointListPointer;
  typedef std::vector<double>                          ChordLengthListType;

  /** Method for creation through the object factory. */
  itkNewMacro( Self );

  /** Method for creation through the object factory. */
  itkTypeMacro( NonUniformBSpline, Object );

  /**
  Set points which the spline will attempt to interpolate.
  */
  void SetPoints( PointListType & newPoints );

  /**
  Get the points the spline is trying to interpolate.
  */
  PointListType& GetPoints()
    {
    return m_Points;
    }

  /**
  Set the knot vector. Knots may be nonuniformly spaced.
  Knots will be rescaled to be between 0 and 1.
  */
  void SetKnots( KnotListType & newKnots);

  /**
  Get the knot vector.
  */
  KnotListType& GetKnots();

  /**
  Computes the chord lengths based on the points.
  */
  void ComputeChordLengths();

  /**
  Methods for evaluating the spline.
  The parameterization is always between 0 and 1.
  */
  PointType  EvaluateSpline(const Array<double> & p) const;
  PointType  EvaluateSpline( double t ) const;

  /**
  Compute the control points.
  */
  void ComputeControlPoints();

  /**
  Set the control points for the spline.
  */
  void SetControlPoints( ControlPointListType& ctrlpts );

  /**
  Get the control points for the spline
  */
  ControlPointListType& GetControlPoints()
    { 
    return m_ControlPoints;
    }

  /**
  Evaluate the basis functions directly. 
  order - order of the basis function, i.e. 3 = cubic.
  i - basis function number, zero based.
  t - parameter of the spline.
  */
  double NonUniformBSplineFunctionRecursive(unsigned int order, unsigned int i, double t) const;

  /**
  Set the order of the spline.
  */
  void SetSplineOrder(unsigned int order)
    {
    m_SplineOrder = order;
    this->Modified();
    }

  /**
  Get the order of the spline.
  */
  unsigned int GetSplineOrder()
    {
    return m_SplineOrder;
    }

 protected:

  /**
  Constructor
  */
  NonUniformBSpline();

  /**
  Virtual destructor
  */
  virtual ~NonUniformBSpline();

  /** 
    Method to print the object.
  */
  virtual void PrintSelf( std::ostream& os, Indent indent ) const;

  /**
  Points that the spline attempts to intepolate.
  */
  PointListType         m_Points;  

  /**
  The knots of spline.
  */
  KnotListType          m_Knots;                 

  /**
  The control points of the spline.
  */
  ControlPointListType  m_ControlPoints;

  /**
  The chord length computed from m_Points.
  */
  ChordLengthListType   m_ChordLength;  

  /**
  The cumulative chord length computed from m_Points
  */
  ChordLengthListType   m_CumulativeChordLength;

  /**
  The order of the spline.
  */
  unsigned int          m_SplineOrder;

  /**
  The spatial dimension. Saved from the template parameter.
  */
  unsigned int          m_SpatialDimension;

};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION 
#include "itkNonUniformBSpline.txx" 
#endif 


#endif // __itkNonUniformBSpline_h
