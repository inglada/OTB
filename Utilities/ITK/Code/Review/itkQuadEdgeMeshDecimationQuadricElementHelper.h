/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkQuadEdgeMeshDecimationQuadricElementHelper.h,v $
  Language:  C++
  Date:      $Date: 2008-10-01 18:36:29 $
  Version:   $Revision: 1.5 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkQuadEdgeMeshDecimationQuadricElementHelper_h
#define __itkQuadEdgeMeshDecimationQuadricElementHelper_h

#include <itkPoint.h>
#include <vnl/vnl_vector_fixed.h>
#include <vnl/vnl_matrix.h>
#include <vnl/algo/vnl_matrix_inverse.h>

#include "itkTriangleHelper.h"
#include "itkNumericTraits.h"

namespace itk
{
///TODO explicit specification for VDimension=3!!!
template< class TPoint >
class QuadEdgeMeshDecimationQuadricElementHelper
{
public:

  typedef QuadEdgeMeshDecimationQuadricElementHelper    Self;
  typedef TPoint                                        PointType;
  typedef typename PointType::CoordRepType              CoordType;

  itkStaticConstMacro(PointDimension, unsigned int, PointType::PointDimension);
  itkStaticConstMacro(NumberOfCoefficients, unsigned int,
    PointDimension * ( PointDimension + 1 ) / 2 + PointDimension + 1);

//    static const unsigned int PointDimension = PointType::PointDimension;
//    static const unsigned int NumberOfCoefficients = 
//      PointDimension * ( PointDimension + 1 ) / 2 + PointDimension + 1;
  
  typedef typename PointType::VectorType                VectorType;
  typedef vnl_matrix< CoordType >                       VNLMatrixType;
  typedef vnl_vector_fixed< CoordType,
    itkGetStaticConstMacro(PointDimension) >            VNLVectorType;
  typedef vnl_vector_fixed< CoordType,
    itkGetStaticConstMacro(NumberOfCoefficients) >      CoefficientVectorType;
  typedef TriangleHelper< PointType >                   TriangleType;
  
  // *****************************************************************
  QuadEdgeMeshDecimationQuadricElementHelper():
    m_Coefficients( itk::NumericTraits< CoordType >::Zero ),
    m_A( PointDimension, PointDimension, itk::NumericTraits< CoordType >::Zero ),
    m_B( itk::NumericTraits< CoordType >::Zero )
    {
    this->m_Rank = PointDimension;
    }
    
  QuadEdgeMeshDecimationQuadricElementHelper( const CoefficientVectorType& iCoefficients ):
    m_Coefficients( iCoefficients ),
    m_A( PointDimension, PointDimension, itk::NumericTraits< CoordType >::Zero ),
    m_B( itk::NumericTraits< CoordType >::Zero )
    {
    this->m_Rank = PointDimension;
    this->ComputeAMatrixAndBVector();
    }

  ~QuadEdgeMeshDecimationQuadricElementHelper()
    {}
  
  CoefficientVectorType GetCoefficients( ) const
    {
    return this->m_Coefficients;
    }
  
  VNLMatrixType GetAMatrix()
    {
    this->ComputeAMatrixAndBVector();
    return m_A;
    }
  
  VNLVectorType GetBVector()
    {
    ComputeAMatrixAndBVector();
    return m_B;
    }
  
  unsigned int GetRank() const
    {
    return m_Rank;
    }
  
  ///TODO this method should be really optimized!!!
  inline CoordType ComputeError( const PointType& iP )
    {
    CoordType oError( 0. );
    
    std::vector< CoordType > pt( PointDimension + 1, 1. );

    unsigned int dim1 = 0;
    unsigned int k =0;

    while( dim1 < PointDimension )
      {
      pt[dim1] = iP[dim1];
      dim1++;
      }
    
    for( dim1 = 0; dim1 < PointDimension + 1; dim1++ )
      {
      oError += this->m_Coefficients[k++] * pt[dim1] * pt[dim1];
      
      for( unsigned int dim2 = dim1 + 1; dim2 < PointDimension + 1; dim2++ )
        {
        oError += 2. * this->m_Coefficients[k++] * pt[dim1] * pt[dim2];
        }
      }
    return oError;
    }
  
  ///TODO this method should be really optimized!!!
  inline CoordType ComputeErrorAtOptimalLocation()
  {
    return ComputeError( ComputeOptimalLocation() );
  }
  
  PointType ComputeOptimalLocation()
  {
    ComputeAMatrixAndBVector();
    vnl_svd< CoordType > svd( m_A );
    svd.zero_out_relative( 1e-10 );
    m_Rank = svd.rank();
    
    VNLVectorType location = svd.solve( m_B.as_vector() );
    PointType oP;
    
    for( unsigned int dim = 0; dim < PointDimension; dim++ )
      oP[dim] = location[dim];
    
    return oP;
  }
  
  ///TODO to be implemented!!!
  PointType ComputeOptimalLocation( 
    const unsigned int& iNumberOfEigenValues )
  {
  }

  PointType ComputeOptimalLocation( 
    const CoordType& iValue )
  {
    ComputeAMatrixAndBVector();
    vnl_svd< CoordType > svd( m_A );
    svd.zero.zero_out_relative( iValue );
    m_Rank = svd.rank();
    
    VNLVectorType location = svd.solve( m_B );
    PointType oP;
    
    for( unsigned int dim = 0; dim < PointDimension; dim++ )
      oP[dim] = location[dim];
    
    return oP;
  }

  void AddTriangle( const PointType& iP1, 
                    const PointType& iP2, 
                    const PointType& iP3,
                    const CoordType& iWeight = static_cast< CoordType >( 1. ) ) 
    {
    AddPoint( iP1, TriangleType::ComputeNormal( iP1, iP2, iP3 ), iWeight );
    }
  
  void AddPoint( const PointType& iP, 
                 const VectorType& iN, 
                 const CoordType& iWeight = static_cast< CoordType >( 1. ) )
    {
    unsigned int k = 0;
    
    CoordType d = -iN * iP.GetVectorFromOrigin();
    
    for( unsigned int dim1 = 0; dim1 < PointDimension; dim1++ )
      {
      for( unsigned int dim2 = dim1; dim2 < PointDimension; dim2++ )
        {
        this->m_Coefficients[k++] += iWeight * iN[dim1] * iN[dim2];
        }
      this->m_Coefficients[k++] += iWeight * iN[dim1] * d;
      }
    
    this->m_Coefficients[k++] += iWeight * d * d;
    }
  
  
  
  
  // ***********************************************************************
  // operators
  Self& operator=( const Self& iRight )
    {
    this->m_Coefficients = iRight.m_Coefficients;
    return *this;
    }
  
  Self operator+( const Self & iRight ) const
    {
    return Self( this->m_Coefficients + iRight.m_Coefficients );
    }

  Self& operator+=( const Self& iRight )
    {
    this->m_Coefficients += iRight.m_Coefficients;
    return *this;
    }

  Self operator-( const Self& iRight ) const
    {
    return Self( this->m_Coefficients - iRight.m_Coefficients );
    }

  Self& operator-=( const Self& iRight )
    {
    this->m_Coefficients -= iRight.m_Coefficients;
    return *this;
    }

  Self operator*( const CoordType& iV ) const
    {
    Self oElement = Self( this->m_Coefficients * iV );
    return oElement;
    }

  Self& operator*=( const CoordType& iV )
    {
    this->m_Coefficients *= iV;
    return *this;
    }
  
protected:

  CoefficientVectorType       m_Coefficients;
  VNLMatrixType               m_A;
  VNLVectorType               m_B;
  unsigned int                m_Rank;
  
  void ComputeAMatrixAndBVector()
    {
    unsigned int k = 0;
    
    for( unsigned int dim1 = 0; dim1 < PointDimension; dim1++ )
      {
      for( unsigned int dim2 = dim1; dim2 < PointDimension; dim2++ )
        {
        m_A[dim1][dim2] = m_A[dim2][dim1] = m_Coefficients[k++];
        }
      m_B[dim1] = - m_Coefficients[k++];
      }
    }
  
};

}
#endif
