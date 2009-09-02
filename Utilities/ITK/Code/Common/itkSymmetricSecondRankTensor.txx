/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkSymmetricSecondRankTensor.txx,v $
  Language:  C++
  Date:      $Date: 2009-03-03 15:09:43 $
  Version:   $Revision: 1.15 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkSymmetricSecondRankTensor_txx
#define __itkSymmetricSecondRankTensor_txx

#include "itkSymmetricSecondRankTensor.h"
#include "itkNumericTraitsTensorPixel.h"

namespace itk
{

/**
 * Assignment Operator
 */
template<class T,unsigned int NDimension>
SymmetricSecondRankTensor<T,NDimension>&
SymmetricSecondRankTensor<T,NDimension>
::operator= (const Self& r)
{
  BaseArray::operator=(r);
  return *this;
}


/**
 * Assignment Operator from a scalar constant
 */
template<class T,unsigned int NDimension>
SymmetricSecondRankTensor<T,NDimension>&
SymmetricSecondRankTensor<T,NDimension>
::operator= (const ComponentType & r)
{
  BaseArray::operator=(&r);
  return *this;
}


/**
 * Assigment from a plain array
 */
template<class T,unsigned int NDimension>
SymmetricSecondRankTensor<T,NDimension>&
SymmetricSecondRankTensor<T,NDimension>
::operator= (const ComponentArrayType r )
{
  BaseArray::operator=(r);
  return *this;
}

/**
 * Returns a temporary copy of a vector
 */
template<class T,unsigned int NDimension>
SymmetricSecondRankTensor<T,NDimension> 
SymmetricSecondRankTensor<T,NDimension>
::operator+(const Self & r) const
{
  Self result;
  for( unsigned int i=0; i<InternalDimension; i++) 
    {
    result[i] = (*this)[i] + r[i];
    }
  return result;
}

/**
 * Returns a temporary copy of a vector
 */
template<class T,unsigned int NDimension>
SymmetricSecondRankTensor<T,NDimension> 
SymmetricSecondRankTensor<T,NDimension>
::operator-(const Self & r) const
{
  Self result;
  for( unsigned int i=0; i<InternalDimension; i++) 
    {
    result[i] = (*this)[i] - r[i];
    }
  return result;
}
 
/**
 * Performs addition in place
 */
template<class T,unsigned int NDimension>
const SymmetricSecondRankTensor<T,NDimension> & 
SymmetricSecondRankTensor<T,NDimension>
::operator+=(const Self & r) 
{
  for( unsigned int i=0; i<InternalDimension; i++) 
    {
    (*this)[i] += r[i];
    }
  return *this;
}

/**
 * Performs subtraction in place 
 */
template<class T,unsigned int NDimension>
const SymmetricSecondRankTensor<T,NDimension> & 
SymmetricSecondRankTensor<T,NDimension>
::operator-=(const Self & r)
{
  for( unsigned int i=0; i<InternalDimension; i++) 
    {
    (*this)[i] -= r[i];
    }
  return *this;
}

/**
 * Performs multiplication by a scalar, in place
 */
template<class T,unsigned int NDimension>
const SymmetricSecondRankTensor<T,NDimension> & 
SymmetricSecondRankTensor<T,NDimension>
::operator*=(const RealValueType & r) 
{
  for( unsigned int i=0; i<InternalDimension; i++) 
    {
    (*this)[i] *= r;
    }
  return *this;
}

/**
 * Performs division by a scalar, in place
 */
template<class T,unsigned int NDimension>
const SymmetricSecondRankTensor<T,NDimension> & 
SymmetricSecondRankTensor<T,NDimension>
::operator/=(const RealValueType & r) 
{
  for( unsigned int i=0; i<InternalDimension; i++) 
    {
    (*this)[i] /= r;
    }
  return *this;
}

/**
 * Performs multiplication with a scalar
 */
template<class T,unsigned int NDimension>
SymmetricSecondRankTensor<T,NDimension> 
SymmetricSecondRankTensor<T,NDimension>
::operator*(const RealValueType & r) const
{
  Self result;
  for( unsigned int i=0; i<InternalDimension; i++) 
    {
    result[i] = (*this)[i] * r;
    }
  return result;
}

/**
 * Performs division by a scalar
 */
template<class T,unsigned int NDimension>
SymmetricSecondRankTensor<T,NDimension> 
SymmetricSecondRankTensor<T,NDimension>
::operator/(const RealValueType & r) const
{
  Self result;
  for( unsigned int i=0; i<InternalDimension; i++) 
    {
    result[i] = (*this)[i] / r;
    }
  return result;
}

/**
 * Matrix notation access to elements
 */
template<class T,unsigned int NDimension>
const typename SymmetricSecondRankTensor<T,NDimension>::ValueType &
SymmetricSecondRankTensor<T,NDimension>
::operator()(unsigned int row, unsigned int col) const
{
  unsigned int k; 

  if( row < col ) 
    {
    k = row * Dimension + col - row * ( row + 1 ) / 2; 
    }
  else
    {
    k = col * Dimension + row - col * ( col + 1 ) / 2; 
    }
  

  if( k >= InternalDimension )
    {
    k = 0;
    }

  return (*this)[k];
}


/**
 * Matrix notation access to elements
 */
template<class T,unsigned int NDimension>
typename SymmetricSecondRankTensor<T,NDimension>::ValueType &
SymmetricSecondRankTensor<T,NDimension>
::operator()(unsigned int row, unsigned int col)
{
  unsigned int k; 

  if( row < col ) 
    {
    k = row * Dimension + col - row * ( row + 1 ) / 2; 
    }
  else
    {
    k = col * Dimension + row - col * ( col + 1 ) / 2; 
    }
  

  if( k >= InternalDimension )
    {
    k = 0;
    }

  return (*this)[k];
}

/**
 * Set the Tensor to an Identity.
 * Set ones in the diagonal and zeroes every where else.
 */
template<class T,unsigned int NDimension>
void 
SymmetricSecondRankTensor<T,NDimension>
::SetIdentity() 
{
  this->Fill(NumericTraits< T >::Zero);
  for( unsigned int i=0; i < Dimension; i++)
    {
    (*this)(i,i) = NumericTraits< T >::One;
    }
}


/**
 * Get the Trace
 */
template<class T,unsigned int NDimension>
typename SymmetricSecondRankTensor<T,NDimension>::AccumulateValueType
SymmetricSecondRankTensor<T,NDimension>
::GetTrace() const
{
  AccumulateValueType trace = NumericTraits< AccumulateValueType >::Zero;
  unsigned int k = 0;
  for(unsigned int i=0; i<Dimension; i++)
    {
    trace += (*this)[k];
    k += (Dimension-i);
    }
  return trace;
}


/**
 * Compute Eigen Values 
 */
template<class T,unsigned int NDimension>
void
SymmetricSecondRankTensor<T,NDimension>
::ComputeEigenValues( EigenValuesArrayType & eigenValues ) const 
{
  
  SymmetricEigenAnalysisType symmetricEigenSystem = SymmetricEigenAnalysisType( Dimension );

  MatrixType tensorMatrix;
  for( unsigned int row=0; row < Dimension; row++ )
    {
    for( unsigned int col=0; col < Dimension; col++ )
      {
      tensorMatrix[row][col] = (*this)(row,col);
      }
    }
  
  symmetricEigenSystem.ComputeEigenValues( tensorMatrix, eigenValues );
  
}

/**
 * Compute Eigen analysis, it returns an array with eigen values
 * and a Matrix with eigen vectors
 */
template<class T,unsigned int NDimension>
void
SymmetricSecondRankTensor<T,NDimension>
::ComputeEigenAnalysis( EigenValuesArrayType & eigenValues,
                        EigenVectorsMatrixType & eigenVectors ) const
{
  SymmetricEigenAnalysisType symmetricEigenSystem = SymmetricEigenAnalysisType( Dimension );

  MatrixType tensorMatrix;
  for( unsigned int row=0; row < Dimension; row++ )
    {
    for( unsigned int col=0; col < Dimension; col++ )
      {
      tensorMatrix[row][col] = (*this)(row,col);
      }
    }
  
  symmetricEigenSystem.ComputeEigenValuesAndVectors( 
                      tensorMatrix, eigenValues, eigenVectors );

}

/**
 * Pre-multiply the Tensor by a Matrix
 */
template<class T,unsigned int NDimension>
SymmetricSecondRankTensor<T,NDimension>
SymmetricSecondRankTensor<T,NDimension>
::PreMultiply( const MatrixType & m ) const
{
  Self result;
  typedef typename NumericTraits<T>::AccumulateType  AccumulateType;
  for(unsigned int r=0; r<NDimension; r++)
    {
    for(unsigned int c=0; c<NDimension; c++)
      {
      AccumulateType sum = NumericTraits<AccumulateType>::ZeroValue();
      for(unsigned int t=0; t<NDimension; t++)
        {
        sum += m(r,t) * (*this)(t,c);
        }
      result(r,c) = static_cast<T>( sum );
      }
    }
  return result;
}

/**
 * Post-multiply the Tensor by a Matrix
 */
template<class T,unsigned int NDimension>
SymmetricSecondRankTensor<T,NDimension>
SymmetricSecondRankTensor<T,NDimension>
::PostMultiply( const MatrixType & m ) const
{
  Self result;
  typedef typename NumericTraits<T>::AccumulateType  AccumulateType;
  for(unsigned int r=0; r<NDimension; r++)
    {
    for(unsigned int c=0; c<NDimension; c++)
      {
      AccumulateType sum = NumericTraits<AccumulateType>::ZeroValue();
      for(unsigned int t=0; t<NDimension; t++)
        {
        sum += (*this)(r,t) * m(t,c);
        }
      result(r,c) = static_cast<T>( sum );
      }
    }
  return result;
}

/**
 * Print content to an ostream
 */
template<class T,unsigned int NDimension>
std::ostream &
operator<<(std::ostream& os,const SymmetricSecondRankTensor<T,NDimension> & c ) 
{
  for(unsigned int i=0; i<c.GetNumberOfComponents(); i++)
    {
    os <<  static_cast<typename NumericTraits<T>::PrintType>(c[i]) << "  ";
    }
  return os;
}


/**
 * Read content from an istream
 */
template<class T,unsigned int NDimension>
std::istream &
operator>>(std::istream& is, SymmetricSecondRankTensor<T,NDimension> & dt ) 
{
  for(unsigned int i=0; i < dt.GetNumberOfComponents(); i++)
    {
    is >> dt[i];
    }
  return is;
}

} // end namespace itk

#endif
