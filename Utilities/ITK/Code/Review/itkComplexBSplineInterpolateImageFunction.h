/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkComplexBSplineInterpolateImageFunction.h,v $
  Language:  C++
  Date:      $Date: 2009-05-15 20:10:02 $
  Version:   $Revision: 1.2 $

  Copyright ( c ) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkComplexBSplineInterpolateImageFunction_h
#define __itkComplexBSplineInterpolateImageFunction_h

#include "itkInterpolateImageFunction.h"
#include "itkBSplineInterpolateImageFunction.h"
#include "itkComplexToRealImageFilter.h"
#include "itkComplexToImaginaryImageFilter.h"

namespace itk {
/** 
 * \class ComplexBSplineInterpolateImageFunction.
 * \brief Complex wrapper around BSplineInterpolateImageFunction.
 *
 * A complex wrapper class that splits complex input image in two real-type
 * subimages containing real and imaginary parts, that are interpolated using
 * the standard itkBSplineInterpolateImageFunction. The same requirements apply
 * for this class: Set spline order before setting the input image!
 * Derivative support is currently not implemented
 *
 * This implementation was taken from the Insight Journal paper:
 * http://hdl.handle.net/1926/585
 *
 * \ingroup ImageInterpolators
 */
 template< class TImageType, class TCoordRep = double, class TCoefficientType = double >
 class ITK_EXPORT ComplexBSplineInterpolateImageFunction : 
  public InterpolateImageFunction< TImageType, TCoordRep > 
{
public:
  /** Standard class typedef. */
  typedef ComplexBSplineInterpolateImageFunction       Self;
  /** Standard class typedef. */
  typedef InterpolateImageFunction< TImageType, TCoordRep >  Superclass;
  /** Standard class typedef. */
  typedef SmartPointer< Self >                    Pointer;
  /** Standard class typedef. */
  typedef SmartPointer< const Self >              ConstPointer;
  
  /** Run-time type information (and related methods). */
    itkTypeMacro( ComplexBSplineInterpolateImageFunction, InterpolateImageFunction );

 
  /** New macro for creation of through a Smart Pointer */
  itkNewMacro( Self );
  
  /** Dimension underlying input image. */
  itkStaticConstMacro( ImageDimension, unsigned int,Superclass::ImageDimension );
  
  /** OutputType typedef support. */
  typedef typename Superclass::OutputType OutputType;

  /** InputImageType typedef support. */
  typedef typename Superclass::InputImageType InputImageType;
  
  /** Index typedef support. */
  typedef typename Superclass::IndexType IndexType;

  /** ContinuousIndex typedef support. */
  typedef typename Superclass::ContinuousIndexType ContinuousIndexType;
  
  /** PointType typedef support */
  typedef typename Superclass::PointType PointType;
  
  /** Internal Real and imaginary image type */
  typedef Image< double, itkGetStaticConstMacro( ImageDimension ) > InternalImageType;

  /** Complex to Real filter type */
  typedef ComplexToRealImageFilter< InputImageType, InternalImageType > RealFilterType;
  typedef ComplexToImaginaryImageFilter< InputImageType, InternalImageType > ImaginaryFilterType;
  
  /** Underlying real BSpline interpolator */
  typedef BSplineInterpolateImageFunction< InternalImageType, TCoordRep, TCoefficientType > InterpolatorType;
  

  
  /** Evaluate the function at a ContinuousIndex position.
  *
  * Returns the B-Spline interpolated image intensity at a 
  * specified point position. No bounds checking is done.
  * The point is assumed to lie within the image buffer.
  *
  * ImageFunction::IsInsideBuffer() can be used to check bounds before
  * calling the method. */
  virtual OutputType EvaluateAtContinuousIndex( const ContinuousIndexType & index ) const; 

  /** Derivative typedef support */
/*  typedef CovariantVector< OutputType, itkGetStaticConstMacro( ImageDimension ) > CovariantVectorType;

  CovariantVectorType EvaluateDerivative( const PointType & point ) const
  {
   ContinuousIndexType index;
   this->GetInputImage()->TransformPhysicalPointToContinuousIndex( point, index );
   return ( this->EvaluateDerivativeAtContinuousIndex( index ) );
  } 

  CovariantVectorType EvaluateDerivativeAtContinuousIndex( const ContinuousIndexType & x ) const;
*/

  /** Get/Sets the Spline Order, supports 0th - 5th order splines. The default
  *  is a 3rd order spline. */
  void SetSplineOrder( unsigned int SplineOrder );
  itkGetConstMacro( SplineOrder, int );
  
  /** Set the input image.  This must be set by the user, after setting the spline order! */
  virtual void SetInputImage(const TImageType * inputData);
  
  
protected:
  ComplexBSplineInterpolateImageFunction();
  virtual ~ComplexBSplineInterpolateImageFunction() {};
  void operator=( const Self& ); //purposely not implemented
  void PrintSelf(std::ostream& os, Indent indent) const;
  
  
private:
  ComplexBSplineInterpolateImageFunction( const Self& ); //purposely not implemented
  
  
  unsigned int                          m_SplineOrder;
  typename InterpolatorType::Pointer    m_RealInterpolator;
  typename InterpolatorType::Pointer    m_ImaginaryInterpolator;
  typename RealFilterType::Pointer      m_RealFilter;
  typename ImaginaryFilterType::Pointer m_ImaginaryFilter;
  
}; // class


} // namespace


#ifndef ITK_MANUAL_INSTANTIATION
#include "itkComplexBSplineInterpolateImageFunction.txx"
#endif

#endif
