/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkBSplineScatteredDataPointSetToImageFilter.h,v $
  Language:  C++
  Date:      $Date: $
  Version:   $Revision: $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __ijBSplineScatteredDataPointSetToImageFilter_h
#define __ijBSplineScatteredDataPointSetToImageFilter_h

#include "ijPointSetToImageFilter.h"
#include "ijBSplineKernelFunction.h"
#include "itkFixedArray.h"
#include "itkVector.h"
#include "itkVectorContainer.h"

#include "vnl/vnl_matrix.h"

namespace ij
{

/** \class BSplineScatteredDataPointSetToImageFilter.h
 * \brief Image filter which provides a B-spline output approximation.
 * 
 * Given an n-D image with scattered data, this filter finds 
 * a fast approximation to that irregulary spaced data using uniform 
 * B-splines.  The traditional method of inverting the observation 
 * matrix to find a least-squares fit is made obsolete.  Therefore, 
 * memory issues are not a concern and inverting large matrices are 
 * unnecessary.  The reference below gives the algorithm for 2-D images
 * and cubic splines.  This class generalizes that work to encompass n-D
 * images and any *feasible* B-spline order.
 *
 * In addition to specifying the input point set, one must specify the number
 * of control points.  If one wishes to use the multilevel component of 
 * this algorithm, one must also specify the number of levels in the
 * hieararchy.  If this is desired, the number of control points becomes
 * the number of control points for the coarsest level.  The algorithm
 * then increases the number of control points at each level so that
 * the B-spline n-D grid is refined to twice the previous level.  The 
 * scattered data is specified by the pixel values.  Pixels which 
 * are not to be included in the calculation of the B-spline grid must 
 * have a value equal to that specified by the variable m_BackgroundValue.
 * 
 * Note that the specified number of control points must be > m_SplineOrder.
 *
 * \par REFERENCE
 * S. Lee, G. Wolberg, and S. Y. Shin, "Scattered Data Interpolation
 * with Multilevel B-Splines", IEEE Transactions on Visualization and 
 * Computer Graphics, 3(3):228-244, 1997.
 * 
 * N.J. Tustison and J.C. Gee, "Generalized n-D C^k Scattered Data Approximation
 * with COnfidence Values", Proceedings of the MIAR conference, August 2006.
 */

template <class TInputPointSet, class TOutputImage>
class BSplineScatteredDataPointSetToImageFilter 
: public ij::PointSetToImageFilter<TInputPointSet, TOutputImage>
{
public:
  typedef BSplineScatteredDataPointSetToImageFilter           Self;
  typedef ij::PointSetToImageFilter<TInputPointSet, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>                                  Pointer;
  typedef itk::SmartPointer<const Self>                            ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Extract dimension from input and output image. */
  itkStaticConstMacro( ImageDimension, unsigned int,
                       TOutputImage::ImageDimension );
		      
  typedef TOutputImage                                        ImageType;
  typedef TInputPointSet                                      PointSetType;

  /** Image typedef support. */
  typedef typename ImageType::PixelType                       PixelType;
  typedef typename ImageType::RegionType                      RegionType;
  typedef typename ImageType::SizeType                        SizeType;
  typedef typename ImageType::IndexType                       IndexType;
  typedef typename ImageType::PointType                       ContinuousIndexType;

  /** PointSet typedef support. */
  typedef typename PointSetType::PointType                    PointType;
  typedef typename PointSetType::PixelType                    PointDataType;
  typedef typename PointSetType::PointDataContainer           PointDataContainerType;

  /** Other typedef */
  typedef double                                              RealType;
  typedef itk::VectorContainer<unsigned, RealType>                 WeightsContainerType;
  typedef itk::FixedArray<unsigned, 
    itkGetStaticConstMacro( ImageDimension )>                 ArrayType;
  typedef vnl_matrix<RealType>                                GradientType;
  typedef itk::Image<RealType, 
    itkGetStaticConstMacro( ImageDimension )>                 RealImageType;
  typedef itk::Image<PointDataType, 
    itkGetStaticConstMacro( ImageDimension )>                 PointDataImageType;
  
  void SetNumberOfLevels( unsigned int );
  void SetNumberOfLevels( ArrayType );
  itkGetConstReferenceMacro( NumberOfLevels, ArrayType );

  void SetSplineOrder( unsigned int );
  void SetSplineOrder( ArrayType );
  itkGetConstReferenceMacro( SplineOrder, ArrayType );

  itkSetMacro( NumberOfControlPoints, ArrayType );
  itkGetConstReferenceMacro( NumberOfControlPoints, ArrayType );
  itkGetConstReferenceMacro( CurrentNumberOfControlPoints, ArrayType );

  itkSetMacro( CloseDimension, ArrayType );
  itkGetConstReferenceMacro( CloseDimension, ArrayType );

  itkSetMacro( GenerateOutputImage, bool );
  itkGetConstReferenceMacro( GenerateOutputImage, bool );
  itkBooleanMacro( GenerateOutputImage );

  void SetPointWeights( typename WeightsContainerType::Pointer weights );

  /** 
   * Get the control point lattice.
   */
  itkGetConstMacro( PhiLattice, typename PointDataImageType::Pointer );  

  /** 
   * Evaluate the resulting B-spline object at a specified
   * point or index within the image domain.  
   */
  void EvaluateAtPoint( PointType, PointDataType & );
  void EvaluateAtIndex( IndexType, PointDataType & );
  void EvaluateAtContinuousIndex( ContinuousIndexType, PointDataType & );

  /** 
   * Evaluate the resulting B-spline object at a specified
   * parameteric point.  Note that the parameterization over
   * each dimension of the B-spline object is [0, 1).
   */
  void Evaluate( PointType, PointDataType & );

  /** 
   * Evaluate the gradient of the resulting B-spline object at a specified
   * point or index within the image domain.  
   */
  void EvaluateGradientAtPoint( PointType, GradientType & );
  void EvaluateGradientAtIndex( IndexType, GradientType & );
  void EvaluateGradientAtContinuousIndex( ContinuousIndexType, GradientType & );

  /** 
   * Evaluate the gradient of the resulting B-spline object at a specified
   * parameteric point.  Note that the parameterization over
   * each dimension of the B-spline object is [0, 1).
   */
  void EvaluateGradient( PointType, GradientType & );

protected:
  BSplineScatteredDataPointSetToImageFilter();
  virtual ~BSplineScatteredDataPointSetToImageFilter();
  void PrintSelf( std::ostream& os, itk::Indent indent ) const;

  void GenerateData();

private:
  BSplineScatteredDataPointSetToImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
  
  void GenerateControlLattice();
  void RefineControlLattice();
  void UpdatePointSet();
  void GenerateOutputImage();

  /** Interpolation kernel type (default spline order = 3) */
  typedef BSplineKernelFunction<3> KernelType;  

  bool                                                        m_DoMultilevel;
  bool                                                        m_GenerateOutputImage;
  bool                                                        m_UsePointWeights;
  unsigned int                                                m_MaximumNumberOfLevels;
  unsigned int                                                m_CurrentLevel;
  ArrayType                                                   m_NumberOfControlPoints;
  ArrayType                                                   m_CurrentNumberOfControlPoints;
  ArrayType                                                   m_CloseDimension;
  ArrayType                                                   m_SplineOrder;
  ArrayType                                                   m_NumberOfLevels;
  typename WeightsContainerType::Pointer                      m_PointWeights;
  typename KernelType::Pointer                                m_Kernel[ImageDimension];
  typename KernelType::MatrixType                             m_RefinedLatticeCoefficients[ImageDimension];
  typename PointDataImageType::Pointer                        m_PhiLattice;
  typename PointDataImageType::Pointer                        m_PsiLattice;
  typename PointDataContainerType::Pointer                    m_InputPointData;
  typename PointDataContainerType::Pointer                    m_OutputPointData; 
  
  inline typename RealImageType::IndexType 
  IndexToSubscript(unsigned int index, typename RealImageType::SizeType size)
    {
    typename RealImageType::IndexType k;     
    k[0] = 1;    
    
    for ( unsigned int i = 1; i < ImageDimension; i++ )
      {
      k[i] = size[ImageDimension-i-1]*k[i-1];
      }  
    typename RealImageType::IndexType sub;
    for ( unsigned int i = 0; i < ImageDimension; i++ )
      {
      sub[ImageDimension-i-1] = static_cast<unsigned int>( index/k[ImageDimension-i-1] );
      index %= k[ImageDimension-i-1];
      }
    return sub;
    }
};

} // end namespace ij

#ifndef OTB_MANUAL_INSTANTIATION
#include "ijBSplineScatteredDataPointSetToImageFilter.txx"
#endif

#endif

