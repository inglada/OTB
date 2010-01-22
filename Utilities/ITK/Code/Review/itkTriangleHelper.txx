/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkTriangleHelper.txx,v $
  Language:  C++
  Date:      $Date: 2009-03-26 15:00:27 $
  Version:   $Rev$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkTriangleHelper_txx
#define __itkTriangleHelper_txx

#include "itkTriangleHelper.h"

namespace itk
{
template< typename TPoint >
bool TriangleHelper< TPoint >::
IsObtuse( const PointType& iA, const PointType& iB, const PointType& iC )
{
  VectorType v01 = iB - iA;
  VectorType v02 = iC - iA;
  VectorType v12 = iC - iB;

  if( v01 * v02 < 0.0 )
    {
    return true;
    }
  else
    {
    if( v02 * v12 < 0.0 )
      {
      return true;
      }
    else
      {
      if( v01 * -v12 < 0.0 )
        {
        return true;
        }
      else
        {
        return false;
        }
      }
    }
}

template< typename TPoint >
typename TriangleHelper< TPoint >::VectorType
TriangleHelper< TPoint >::ComputeNormal( const PointType& iA,
    const PointType& iB,
    const PointType& iC )
{
  CrossVectorType cross;
  VectorType w = cross ( iB - iA, iC - iA );
  CoordRepType l2 = w.GetSquaredNorm();

  if( l2 != 0.0 )
    {
    w /= vcl_sqrt( l2 );
    }

  return w;
}

template< typename TPoint >
typename TriangleHelper< TPoint >::CoordRepType
TriangleHelper< TPoint >::Cotangent ( const PointType& iA,
                                  const PointType& iB,
                                  const PointType& iC )
{
  VectorType v21 = iA - iB;
  CoordRepType v21_l2 = v21.GetSquaredNorm();
  if( v21_l2 != 0.0 )
    {
    v21 /= vcl_sqrt( v21_l2 );
    }
  else
    {
    }

  VectorType v23 = iC - iB;
  CoordRepType v23_l2 = v23.GetSquaredNorm();
  if( v23_l2 != 0.0 )
    {
    v23 /= vcl_sqrt( v23_l2 );
    }
  else
    {
    }

  CoordRepType bound( 0.999999 );

  CoordRepType cos_theta = vnl_math_max( -bound,
    vnl_math_min( bound, v21 * v23 ) );

  return 1.0 / vcl_tan( vcl_acos( cos_theta ) );
}

template< typename TPoint >
typename TriangleHelper< TPoint >::PointType
TriangleHelper< TPoint >::ComputeBarycenter (
      const CoordRepType& iA1, const PointType& iP1,
      const CoordRepType& iA2, const PointType& iP2,
      const CoordRepType& iA3, const PointType& iP3 )
{
  PointType oPt;

  CoordRepType total = iA1 + iA2 + iA3;
  if( total == 0. )
    {
    //in such case there is no barycenter; 
    oPt.Fill( 0. );
    return oPt; 
    }
  
  CoordRepType inv_total = 1. / total;  
  CoordRepType a1 = iA1 * inv_total;
  CoordRepType a2 = iA2 * inv_total;
  CoordRepType a3 = iA3 * inv_total;
  
  for ( unsigned int dim = 0; dim < PointDimension; ++dim )
    {
    oPt[dim] = a1 * iP1[dim] + a2 * iP2[dim] + a3 * iP3[dim];
    }

  return oPt;
}

template< typename TPoint >
typename TriangleHelper< TPoint >::CoordRepType
TriangleHelper< TPoint >::ComputeAngle( const PointType& iP1,
    const PointType& iP2,
    const PointType& iP3 )
{
  VectorType v21 = iP1 - iP2;
  VectorType v23 = iP3 - iP2;

  CoordRepType v21_l2 = v21.GetSquaredNorm();
  CoordRepType v23_l2 = v23.GetSquaredNorm();

  if( v21_l2 != 0.0 )
    v21 /= vcl_sqrt( v21_l2 );
  if( v23_l2 != 0.0 )
    v23 /= vcl_sqrt( v23_l2 );

  CoordRepType bound( 0.999999 );

  CoordRepType cos_theta = vnl_math_max( -bound,
    vnl_math_min( bound, v21 * v23 ) );

  return vcl_acos( cos_theta );
}

template< typename TPoint >
typename TriangleHelper< TPoint >::PointType
TriangleHelper< TPoint >::ComputeGravityCenter (
      const PointType& iP1,
      const PointType& iP2,
      const PointType& iP3 )
{
  return ComputeBarycenter( 1., iP1, 1., iP2, 1., iP3 );
}

template< typename TPoint >
typename TriangleHelper< TPoint >::PointType
TriangleHelper< TPoint >::ComputeCircumCenter (
      const PointType& iP1,
      const PointType& iP2,
      const PointType& iP3 )
{
  PointType oPt;
  oPt.Fill ( 0.0 );

  CoordRepType a = iP2.SquaredEuclideanDistanceTo ( iP3 );
  CoordRepType b = iP1.SquaredEuclideanDistanceTo ( iP3 );
  CoordRepType c = iP2.SquaredEuclideanDistanceTo ( iP1 );

  CoordRepType Weight[3];
  Weight[0] = a * ( b + c - a );
  Weight[1] = b * ( c + a - b );
  Weight[2] = c * ( a + b - c );

  return ComputeBarycenter( Weight[0], iP1, Weight[1], iP2, Weight[2], iP3 );
}

template< typename TPoint >
typename TriangleHelper< TPoint >::PointType
TriangleHelper< TPoint >::ComputeConstrainedCircumCenter ( const PointType& iP1,
      const PointType& iP2, const PointType& iP3 )
{
  PointType oPt;
  CoordRepType a = iP2.SquaredEuclideanDistanceTo ( iP3 );
  CoordRepType b = iP1.SquaredEuclideanDistanceTo ( iP3 );
  CoordRepType c = iP2.SquaredEuclideanDistanceTo ( iP1 );

  CoordRepType Weight[3];
  Weight[0] = a * ( b + c - a );
  Weight[1] = b * ( c + a - b );
  Weight[2] = c * ( a + b - c );

  for ( unsigned int i = 0; i < 3; i++ )
    {
    if ( Weight[i] < 0.0 )
      {
      Weight[i] = 0.;
      }
    }

  return ComputeBarycenter( Weight[0], iP1, Weight[1], iP2, Weight[2], iP3 );
}

template< typename TPoint >
typename TriangleHelper< TPoint >::CoordRepType
TriangleHelper< TPoint >::ComputeArea ( const PointType& iP1,
    const PointType& iP2,
    const PointType& iP3 )
{
  CoordRepType a = iP2.EuclideanDistanceTo ( iP3 );
  CoordRepType b = iP1.EuclideanDistanceTo ( iP3 );
  CoordRepType c = iP2.EuclideanDistanceTo ( iP1 );

  CoordRepType s = 0.5 * ( a + b + c );
  return static_cast< CoordRepType > ( vcl_sqrt ( s * ( s - a ) * ( s - b ) * ( s - c ) ) );
}

}

#endif
