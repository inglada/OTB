/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkMatrix.txx,v $
  Language:  C++
  Date:      $Date: 2009-02-05 22:04:01 $
  Version:   $Revision: 1.22 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkMatrix_txx
#define __itkMatrix_txx

#include "itkMatrix.h" 
#include "itkNumericTraits.h"

namespace itk
{
 
/**
 *  Product by a Vector
 */
template<class T, unsigned int NRows, unsigned int NColumns >
Vector<T, NRows> 
Matrix<T, NRows, NColumns>
::operator*( const Vector<T, NColumns> & vect ) const
{
  Vector<T,NRows> result;
  for( unsigned int r=0; r<NRows; r++) 
    {
    T sum = NumericTraits<T>::Zero;   
    for( unsigned int c=0; c<NColumns; c++ ) 
      {
      sum += m_Matrix(r,c) * vect[c];
      }
    result[r] = sum;
    }
  return result;
}
 
/**
 *  Product by a Point
 */
template<class T, unsigned int NRows, unsigned int NColumns >
Point<T, NRows> 
Matrix<T, NRows, NColumns>
::operator*( const Point<T, NColumns> & pnt ) const
{
  Point<T,NRows> result;
  for( unsigned int r=0; r<NRows; r++) 
    {
    T sum = NumericTraits<T>::Zero;   
    for( unsigned int c=0; c<NColumns; c++ ) 
      {
      sum += m_Matrix(r,c) * pnt[c];
      }
    result[r] = sum;
    }
  return result;
}

 
/**
 *  Product by a CovariantVector
 */
template<class T, unsigned int NRows, unsigned int NColumns >
CovariantVector<T, NRows> 
Matrix<T, NRows, NColumns>
::operator*( const CovariantVector<T, NColumns> & covect ) const
{
  CovariantVector<T,NRows> result;
  for( unsigned int r=0; r<NRows; r++) 
    {
    T sum = NumericTraits<T>::Zero;   
    for( unsigned int c=0; c<NColumns; c++ ) 
      {
      sum += m_Matrix(r,c) * covect[c];
      }
    result[r] = sum;
    }
  return result;
}

 
 
/**
 *  Product by a matrix
 */
template<class T, unsigned int NRows, unsigned int NColumns >
Matrix<T, NRows, NColumns>
Matrix<T, NRows, NColumns>
::operator*( const Self & matrix ) const
{
  Self result;
  result = m_Matrix * matrix.m_Matrix;
  return result;
}

  
/**
 *  Matrix Addition 
 */
template<class T, unsigned int NRows, unsigned int NColumns >
Matrix<T, NRows, NColumns>
Matrix<T, NRows, NColumns>
::operator+( const Self & matrix ) const
{
  Self result;
  for( unsigned int r=0; r<NRows; r++) 
    {
    for( unsigned int c=0; c<NColumns; c++ ) 
      {
      result.m_Matrix(r,c) = m_Matrix(r,c) + matrix.m_Matrix(r,c);
      }
    }
  return result;
}

  

/**
 *  Matrix Addition in-place
 */
template<class T, unsigned int NRows, unsigned int NColumns >
const Matrix<T, NRows, NColumns> &
Matrix<T, NRows, NColumns>
::operator+=( const Self & matrix ) 
{
  for( unsigned int r=0; r<NRows; r++) 
    {
    for( unsigned int c=0; c<NColumns; c++ ) 
      {
      m_Matrix(r,c) += matrix.m_Matrix(r,c);
      }
    }
  return *this;
}
  
/**
 *  Matrix Subtraction
 */
template<class T, unsigned int NRows, unsigned int NColumns >
Matrix<T, NRows, NColumns>
Matrix<T, NRows, NColumns>
::operator-( const Self & matrix ) const
{
  Self result;
  for( unsigned int r=0; r<NRows; r++) 
    {
    for( unsigned int c=0; c<NColumns; c++ ) 
      {
      result.m_Matrix(r,c) = m_Matrix(r,c) - matrix.m_Matrix(r,c);
      }
    }
  return result;
}

/**
 *  Matrix subtraction in-place 
 */
template<class T, unsigned int NRows, unsigned int NColumns >
const Matrix<T, NRows, NColumns> &
Matrix<T, NRows, NColumns>
::operator-=( const Self & matrix ) 
{
  for( unsigned int r=0; r<NRows; r++) 
    {
    for( unsigned int c=0; c<NColumns; c++ ) 
      {
      m_Matrix(r,c) -= matrix.m_Matrix(r,c);
      }
    }
  return *this;
}

/**
 *  Product by a vnl_matrix
 */
template<class T, unsigned int NRows, unsigned int NColumns >
vnl_matrix<T> 
Matrix<T, NRows, NColumns>
::operator*( const vnl_matrix<T> & matrix ) const
{
  return m_Matrix * matrix;
}


 
/**
 *  Product by a matrix
 */
template<class T, unsigned int NRows, unsigned int NColumns >
void
Matrix<T, NRows, NColumns>
::operator*=( const Self & matrix ) 
{
  m_Matrix *= matrix.m_Matrix;
}

 
/**
 *  Product by a vnl_matrix
 */
template<class T, unsigned int NRows, unsigned int NColumns >
void
Matrix<T, NRows, NColumns>
::operator*=( const vnl_matrix<T> & matrix ) 
{
  m_Matrix *= matrix;
}

/**
 *  Product by a vnl_vector
 */
template<class T, unsigned int NRows, unsigned int NColumns >
vnl_vector<T>
Matrix<T, NRows, NColumns>
::operator*( const vnl_vector<T> & vc ) const
{
  return m_Matrix * vc;
}


} // end namespace itk


#endif
