/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkImageTransformHelper.h,v $
  Language:  C++
  Date:      $Date: 2009-10-27 20:25:10 $
  Version:   $Revision: 1.15 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkImageTransformHelper_h
#define __itkImageTransformHelper_h

#include "itkConceptChecking.h"
#include "itkPoint.h"
#include "itkMatrix.h"
#include "vnl/vnl_math.h"

namespace itk
{

/** \class ImageTransformHelper
 *  \brief Fast index/physical index computation
 */
template <unsigned int NImageDimension, unsigned int R, unsigned int C>
class ImageTransformHelper
{
public:
  typedef ImageBase<NImageDimension>                          ImageType;
  typedef typename ImageType::IndexType                       IndexType;
  typedef typename ImageBase<NImageDimension>::IndexValueType IndexValueType;
  typedef typename ImageType::SpacingType                     SpacingType;
  typedef Matrix<double, NImageDimension, NImageDimension>    MatrixType;
  typedef typename ImageType::PointType                       OriginType;
  typedef Point<double, NImageDimension>                      DoublePoint;
  typedef Point<float, NImageDimension>                       FloatPoint;
  typedef Concept::Detail::UniqueType_bool<false>             UniqueTypeBoolFalse;
  typedef Concept::Detail::UniqueType_bool<true>              UniqueTypeBoolTrue;

  //
  // Methods with DoublePoint
  //

  // IndexToPhysicalPoint with full matrix
  //
  //
  inline static void TransformIndexToPhysicalPoint(
    const MatrixType & matrix, const OriginType  & origin,
    const IndexType & index, DoublePoint & point)
    {
    ImageTransformHelper<NImageDimension, R, C>::
      TransformIndexToPhysicalPointRow(
        matrix, origin,
        index, point,
        Concept::Detail::UniqueType_bool<(R+1==0)>());
    }

  inline static void TransformIndexToPhysicalPointRow(
    const MatrixType & matrix, const OriginType  & origin,
    const IndexType & index, DoublePoint & point,
    const UniqueTypeBoolFalse& )
    {
    point[R] = origin[R];

    // Start column
    ImageTransformHelper<NImageDimension,R,C>
      ::TransformIndexToPhysicalPointCol(
        matrix,
        index,point,
        Concept::Detail::UniqueType_bool<(C+1==0)>());
    // Do Next Row
    ImageTransformHelper<NImageDimension,R-1,C>
      ::TransformIndexToPhysicalPointRow(
        matrix,origin,
        index,point,
        Concept::Detail::UniqueType_bool<(R==0)>());
    }

  inline static void TransformIndexToPhysicalPointRow(
    const MatrixType &, const OriginType  &,
    const IndexType &, DoublePoint &,
    const UniqueTypeBoolTrue& )
    {
    // Do last row
    }

  inline static void TransformIndexToPhysicalPointCol(
    const MatrixType & matrix,
    const IndexType & index, DoublePoint & point,
    const UniqueTypeBoolFalse& )
    {
    point[R] = point[R] + matrix[R][C]*index[C];

    // Do next dimension
    ImageTransformHelper<NImageDimension,R,C-1>
      ::TransformIndexToPhysicalPointCol(
        matrix,
        index,point,
        Concept::Detail::UniqueType_bool<(C==0)>());
    }

  inline static void TransformIndexToPhysicalPointCol(
    const MatrixType &,
    const IndexType &, DoublePoint &,
    const UniqueTypeBoolTrue& )
    {
    }

  // PhysicalPointToIndex with full matrix
  //
  //
  inline static void TransformPhysicalPointToIndex(
    const MatrixType & matrix, const OriginType  & origin,
    const DoublePoint & point, IndexType  & index)
    {
    DoublePoint rindex;
    ImageTransformHelper<NImageDimension, R, C>::
      TransformPhysicalPointToIndexRow(
        matrix, origin,
        point, rindex, index,
        Concept::Detail::UniqueType_bool<(R+1==0)>());
    }

  inline static void TransformPhysicalPointToIndexRow(
    const MatrixType & matrix, const OriginType  & origin,
    const DoublePoint & point, DoublePoint & rindex, IndexType & index,
    const UniqueTypeBoolFalse& )
    {
    rindex[R] = 0.0;
    // Start column
    ImageTransformHelper<NImageDimension,R,C>
      ::TransformPhysicalPointToIndexCol(
        matrix,origin,
        point,rindex,index,
        Concept::Detail::UniqueType_bool<(C+1==0)>());
    // Do next row
    ImageTransformHelper<NImageDimension,R-1,C>
      ::TransformPhysicalPointToIndexRow(
        matrix,origin,
        point,rindex,index,
        Concept::Detail::UniqueType_bool<(R==0)>());
    }

  inline static void TransformPhysicalPointToIndexRow(
    const MatrixType &, const OriginType &,
    const DoublePoint &, DoublePoint &, IndexType &,
    const UniqueTypeBoolTrue& )
    {
    // Do last row
    }

  inline static void TransformPhysicalPointToIndexCol(
    const MatrixType & matrix, const OriginType  & origin,
    const DoublePoint & point, DoublePoint & rindex, IndexType & index,
    const UniqueTypeBoolFalse& )
    {
    rindex[R] = rindex[R] + matrix[R][C]*(point[C] - origin[C]);

    // Do next dimension
    ImageTransformHelper<NImageDimension,R,C-1>
      ::TransformPhysicalPointToIndexCol(
        matrix,origin,
        point,rindex,index,
        Concept::Detail::UniqueType_bool<(C==0)>());
    }

  inline static void TransformPhysicalPointToIndexCol(
    const MatrixType &, const OriginType  &,
    const DoublePoint &, DoublePoint &rindex, IndexType &index,
    const UniqueTypeBoolTrue& )
    {
#ifdef ITK_USE_CENTERED_PIXEL_COORDINATES_CONSISTENTLY
      index[R] = Math::RoundHalfIntegerUp<IndexValueType>( rindex[R] );
#else
      index[R] = static_cast<IndexValueType>(rindex[R]);
#endif
    }

  //
  // Methods with FloatPoint
  //

  // IndexToPhysicalPoint with full matrix
  //
  //
  inline static void TransformIndexToPhysicalPoint(
    const MatrixType & matrix, const OriginType  & origin,
    const IndexType & index, FloatPoint & point)
    {
    ImageTransformHelper<NImageDimension, R, C>::
      TransformIndexToPhysicalPointRow(
        matrix, origin,
        index, point,
        Concept::Detail::UniqueType_bool<(R+1==0)>());
    }

  inline static void TransformIndexToPhysicalPointRow(
    const MatrixType & matrix, const OriginType  & origin,
    const IndexType & index, FloatPoint & point,
    const UniqueTypeBoolFalse& )
    {
    point[R] = origin[R];

    // Start column
    ImageTransformHelper<NImageDimension,R,C>
      ::TransformIndexToPhysicalPointCol(
        matrix,
        index,point,
        Concept::Detail::UniqueType_bool<(C+1==0)>());
    // Do Next Row
    ImageTransformHelper<NImageDimension,R-1,C>
      ::TransformIndexToPhysicalPointRow(
        matrix,origin,
        index,point,
        Concept::Detail::UniqueType_bool<(R==0)>());
    }

  inline static void TransformIndexToPhysicalPointRow(
    const MatrixType &, const OriginType  &,
    const IndexType &, FloatPoint &,
    const UniqueTypeBoolTrue& )
    {
    // Do last row
    }

  inline static void TransformIndexToPhysicalPointCol(
    const MatrixType & matrix,
    const IndexType & index, FloatPoint & point,
    const UniqueTypeBoolFalse& )
    {
    point[R] = point[R] + matrix[R][C]*index[C];
    
    // Do next dimension
    ImageTransformHelper<NImageDimension,R,C-1>
      ::TransformIndexToPhysicalPointCol(
        matrix,
        index,point,
        Concept::Detail::UniqueType_bool<(C==0)>());
    }

  inline static void TransformIndexToPhysicalPointCol(
    const MatrixType &,
    const IndexType &, FloatPoint &,
    const UniqueTypeBoolTrue& )
    {
    }

  // PhysicalPointToIndex with full matrix
  //
  //
  inline static void TransformPhysicalPointToIndex(
    const MatrixType & matrix, const OriginType  & origin,
    const FloatPoint & point, IndexType  & index)
    {
    FloatPoint rindex;
    ImageTransformHelper<NImageDimension, R, C>::
      TransformPhysicalPointToIndexRow(
        matrix, origin,
        point, rindex, index,
        Concept::Detail::UniqueType_bool<(R+1==0)>());
    }
  
  inline static void TransformPhysicalPointToIndexRow(
    const MatrixType & matrix, const OriginType  & origin,
    const FloatPoint & point, FloatPoint & rindex, IndexType & index,
    const UniqueTypeBoolFalse& )
    {
    rindex[R] = 0.0;
    // Start column
    ImageTransformHelper<NImageDimension,R,C>
      ::TransformPhysicalPointToIndexCol(
        matrix,origin,
        point,rindex,index,
        Concept::Detail::UniqueType_bool<(C+1==0)>());
    // Do next row
    ImageTransformHelper<NImageDimension,R-1,C>
      ::TransformPhysicalPointToIndexRow(
        matrix,origin,
        point,rindex,index,
        Concept::Detail::UniqueType_bool<(R==0)>());
    }

  inline static void TransformPhysicalPointToIndexRow(
    const MatrixType &, const OriginType &,
    const FloatPoint &, FloatPoint &, IndexType &,
    const UniqueTypeBoolTrue& )
    {
    // Do last row
    }

  inline static void TransformPhysicalPointToIndexCol(
    const MatrixType & matrix, const OriginType  & origin,
    const FloatPoint & point, FloatPoint & rindex, IndexType & index,
    const UniqueTypeBoolFalse& )
    {
    rindex[R] = rindex[R] + matrix[R][C]*(point[C] - origin[C]);

    // Do next dimension
    ImageTransformHelper<NImageDimension,R,C-1>
      ::TransformPhysicalPointToIndexCol(
        matrix,origin,
        point,rindex,index,
        Concept::Detail::UniqueType_bool<(C==0)>());
    }

  inline static void TransformPhysicalPointToIndexCol(
    const MatrixType &, const OriginType  &,
    const FloatPoint &, FloatPoint &rindex, IndexType &index,
    const UniqueTypeBoolTrue& )
    {
#ifdef ITK_USE_CENTERED_PIXEL_COORDINATES_CONSISTENTLY
    index[R] = Math::RoundHalfIntegerUp<IndexValueType>(rindex[R]);
#else
    index[R] = static_cast<IndexValueType>(rindex[R]);
#endif
    }

};
} // end namespace itk

#endif
