/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkBSplineInterpolateImageFunction.h,v $
  Language:  C++
  Date:      $Date: 2009-04-25 12:27:05 $
  Version:   $Revision: 1.24 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

  Portions of this code are covered under the VTK copyright.
  See VTKCopyright.txt or http://www.kitware.com/VTKCopyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkBSplineInterpolateImageFunction_h
#define __itkBSplineInterpolateImageFunction_h

// First make sure that the configuration is available.
// This line can be removed once the optimized versions
// gets integrated into the main directories.
#include "itkConfigure.h"

#ifdef ITK_USE_OPTIMIZED_REGISTRATION_METHODS
#include "itkOptBSplineInterpolateImageFunction.h"
#else

#include <vector>

#include "itkImageLinearIteratorWithIndex.h"
#include "itkInterpolateImageFunction.h"
#include "vnl/vnl_matrix.h"

#include "itkBSplineDecompositionImageFilter.h"
#include "itkConceptChecking.h"
#include "itkCovariantVector.h"

namespace itk
{
/** \class BSplineInterpolateImageFunction
 * \brief Evaluates the B-Spline interpolation of an image.  Spline order may be from 0 to 5.
 *
 * This class defines N-Dimension B-Spline transformation.
 * It is based on: 
 *    [1] M. Unser,
 *       "Splines: A Perfect Fit for Signal and Image Processing,"
 *        IEEE Signal Processing Magazine, vol. 16, no. 6, pp. 22-38,
 *        November 1999.
 *    [2] M. Unser, A. Aldroubi and M. Eden,
 *        "B-Spline Signal Processing: Part I--Theory,"
 *        IEEE Transactions on Signal Processing, vol. 41, no. 2, pp. 821-832,
 *        February 1993.
 *    [3] M. Unser, A. Aldroubi and M. Eden,
 *        "B-Spline Signal Processing: Part II--Efficient Design and Applications,"
 *        IEEE Transactions on Signal Processing, vol. 41, no. 2, pp. 834-848,
 *        February 1993.
 * And code obtained from bigwww.epfl.ch by Philippe Thevenaz
 *
 * The B spline coefficients are calculated through the 
 * BSplineDecompositionImageFilter
 * 
 * Limitations:  Spline order must be between 0 and 5.
 *               Spline order must be set before setting the image.
 *               Uses mirror boundary conditions.
 *               Requires the same order of Spline for each dimension.
 *               Spline is determined in all dimensions, cannot selectively
 *                  pick dimension for calculating spline.
 *
 * \sa BSplineDecompositionImageFilter
 *
 * \ingroup ImageFunctions
 */
template <
  class TImageType, 
  class TCoordRep = double,
  class TCoefficientType = double >
class ITK_EXPORT BSplineInterpolateImageFunction : 
    public InterpolateImageFunction<TImageType,TCoordRep> 
{
public:
  /** Standard class typedefs. */
  typedef BSplineInterpolateImageFunction                 Self;
  typedef InterpolateImageFunction<TImageType,TCoordRep>  Superclass;
  typedef SmartPointer<Self>                              Pointer;
  typedef SmartPointer<const Self>                        ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(BSplineInterpolateImageFunction, InterpolateImageFunction);


  /** New macro for creation of through a Smart Pointer */
  itkNewMacro( Self );

  /** OutputType typedef support. */
  typedef typename Superclass::OutputType OutputType;

  /** InputImageType typedef support. */
  typedef typename Superclass::InputImageType InputImageType;

  /** Dimension underlying input image. */
  itkStaticConstMacro(ImageDimension, unsigned int,Superclass::ImageDimension);

  /** Index typedef support. */
  typedef typename Superclass::IndexType IndexType;

  /** ContinuousIndex typedef support. */
  typedef typename Superclass::ContinuousIndexType ContinuousIndexType;

  /** PointType typedef support */
  typedef typename Superclass::PointType PointType;

  /** Iterator typedef support */
  typedef ImageLinearIteratorWithIndex<TImageType> Iterator;

  /** Internal Coefficient typedef support */
  typedef TCoefficientType CoefficientDataType;
  typedef Image<CoefficientDataType, 
                     itkGetStaticConstMacro(ImageDimension)
    >                      CoefficientImageType;

  /** Define filter for calculating the BSpline coefficients */
  typedef BSplineDecompositionImageFilter<TImageType, CoefficientImageType> 
  CoefficientFilter;

  typedef typename CoefficientFilter::Pointer CoefficientFilterPointer;

  /** Evaluate the function at a ContinuousIndex position.
   *
   * Returns the B-Spline interpolated image intensity at a 
   * specified point position. No bounds checking is done.
   * The point is assume to lie within the image buffer.
   *
   * ImageFunction::IsInsideBuffer() can be used to check bounds before
   * calling the method. */
  virtual OutputType EvaluateAtContinuousIndex( 
    const ContinuousIndexType & index ) const; 

  /** Derivative typedef support */
  typedef CovariantVector<OutputType,
                          itkGetStaticConstMacro(ImageDimension)
    > CovariantVectorType;

  CovariantVectorType EvaluateDerivative( const PointType & point ) const
    {
    ContinuousIndexType index;
    this->GetInputImage()->TransformPhysicalPointToContinuousIndex( point, index );
    return ( this->EvaluateDerivativeAtContinuousIndex( index ) );
    }

  CovariantVectorType EvaluateDerivativeAtContinuousIndex( 
    const ContinuousIndexType & x ) const;


  /** Get/Sets the Spline Order, supports 0th - 5th order splines. The default
   *  is a 3rd order spline. */
  void SetSplineOrder(unsigned int SplineOrder);
  itkGetConstMacro(SplineOrder, int);


  /** Set the input image.  This must be set by the user. */
  virtual void SetInputImage(const TImageType * inputData);


  /** The UseImageDirection flag determines whether image derivatives are
   * computed with respect to the image grid or with respect to the physical
   * space. When this flag is ON the derivatives are computed with respect to
   * the coordinate system of physical space. The difference is whether we take
   * into account the image Direction or not. The flag ON will take into
   * account the image direction and will result in an extra matrix
   * multiplication compared to the amount of computation performed when the
   * flag is OFF.
   * The default value of this flag is the same as the CMAKE option
   * ITK_IMAGE_BEHAVES_AS_ORIENTED_IMAGE (i.e ON by default when ITK_IMAGE_BEHAVES_AS_ORIENTED_IMAGE is ON,
   * and  OFF by default when ITK_IMAGE_BEHAVES_AS_ORIENTED_IMAGE is
   * OFF). */
  itkSetMacro( UseImageDirection, bool );
  itkGetConstMacro( UseImageDirection, bool );
  itkBooleanMacro( UseImageDirection );


protected:
  BSplineInterpolateImageFunction();
  virtual ~BSplineInterpolateImageFunction() {};
  void PrintSelf(std::ostream& os, Indent indent) const;

  // These are needed by the smoothing spline routine.
  std::vector<CoefficientDataType>    m_Scratch;        // temp storage for processing of Coefficients
  typename TImageType::SizeType       m_DataLength;  // Image size
  unsigned int                        m_SplineOrder; // User specified spline order (3rd or cubic is the default)

  typename CoefficientImageType::ConstPointer       m_Coefficients; // Spline coefficients  

private:
  BSplineInterpolateImageFunction( const Self& ); //purposely not implemented
  void operator=( const Self& ); //purposely not implemented

  /** Determines the weights for interpolation of the value x */
  void SetInterpolationWeights( const ContinuousIndexType & x, 
                                const vnl_matrix<long> & EvaluateIndex, 
                                vnl_matrix<double> & weights, 
                                unsigned int splineOrder ) const;

  /** Determines the weights for the derivative portion of the value x */
  void SetDerivativeWeights( const ContinuousIndexType & x, 
                             const vnl_matrix<long> & EvaluateIndex, 
                             vnl_matrix<double> & weights, 
                             unsigned int splineOrder ) const;

  /** Precomputation for converting the 1D index of the interpolation neighborhood 
    * to an N-dimensional index. */
  void GeneratePointsToIndex(  );

  /** Determines the indicies to use give the splines region of support */
  void DetermineRegionOfSupport( vnl_matrix<long> & evaluateIndex, 
                                 const ContinuousIndexType & x, 
                                 unsigned int splineOrder ) const;

  /** Set the indicies in evaluateIndex at the boundaries based on mirror 
    * boundary conditions. */
  void ApplyMirrorBoundaryConditions(vnl_matrix<long> & evaluateIndex, 
                                     unsigned int splineOrder) const;


  Iterator                  m_CIterator;    // Iterator for traversing spline coefficients.
  unsigned long             m_MaxNumberInterpolationPoints; // number of neighborhood points used for interpolation
  std::vector<IndexType>    m_PointsToIndex;  // Preallocation of interpolation neighborhood indicies

  CoefficientFilterPointer     m_CoefficientFilter;
  
  // flag to take or not the image direction into account when computing the
  // derivatives.
  bool m_UseImageDirection;

};

} // namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkBSplineInterpolateImageFunction.txx"
#endif

#endif

#endif
