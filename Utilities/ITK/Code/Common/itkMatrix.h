/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkMatrix.h,v $
  Language:  C++
  Date:      $Date: 2008-02-14 04:55:49 $
  Version:   $Revision: 1.27 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkMatrix_h
#define __itkMatrix_h


#include "itkPoint.h"
#include "itkVector.h"
#include "itkCovariantVector.h"
#include "vnl/vnl_matrix_fixed.h"
#include "vnl/algo/vnl_matrix_inverse.h"
#include "vnl/vnl_transpose.h"
#include "vnl/vnl_matrix.h"
#include <vnl/algo/vnl_determinant.h>

namespace itk
{

/** \class Matrix
 * \brief A templated class holding a M x N size Matrix
 * This class contains a vnl_matrix_fixed in order 
 * to make all the vnl mathematical methods available.
 *
 * \ingroup DataRepresentation
 */

template<class T, unsigned int NRows=3, unsigned int NColumns=3>
class Matrix {
public:
  /** Standard class typedefs. */
  typedef Matrix  Self;

  /** Component value type */
  typedef T ValueType;
  typedef T ComponentType;

  /** Number Of Columns and Rows. */
  itkStaticConstMacro(RowDimensions,    unsigned int, NRows);
  itkStaticConstMacro(ColumnDimensions, unsigned int, NColumns);

  /** Internal matrix type */
  typedef vnl_matrix_fixed<T,NRows,NColumns>  InternalMatrixType;

  /** Matrix by Vector multiplication.  */
  Vector<T,NRows> operator*(const Vector<T,NColumns> & vector) const;
 
  /** Matrix by Point multiplication.  */
  Point<T,NRows> operator*(const Point<T,NColumns> & vector) const;
 
  /** Matrix by CovariantVector multiplication.  */
  CovariantVector<T,NRows> 
  operator*(const CovariantVector<T,NColumns> & vector) const;
  
  /** Matrix by Matrix multiplication.  */
  Self operator*(const Self & matrix) const;
 
  /** Matrix addition.  */
  Self operator+(const Self & matrix) const;
  const Self & operator+=(const Self & matrix );
 
  /** Matrix addition.  */
  Self operator-(const Self & matrix) const;
  const Self & operator-=(const Self & matrix );
 
  /** Matrix by vnl_matrix multiplication.  */
  vnl_matrix<T> operator*(const vnl_matrix<T> & matrix) const;

  /** Matrix by Matrix multiplication.  */
  void operator*=(const Self & matrix);
 
  /** Matrix by vnl_matrix multiplication.  */
  void operator*=(const vnl_matrix<T> & matrix);

  /** Matrix by vnl_vector multiplication.  */
  vnl_vector<T> operator*(const vnl_vector<T> & matrix) const;

  /** Matrix by scalar multiplication.  */
  void operator*=(const T & value)
    { m_Matrix *= value; }

  /** Matrix by scalar multiplication.  */
  Self operator*(const T & value)
    { Self result( *this );
      result *= value;
      return result; }
      
  /** Matrix by scalar division.  */
  void operator/=(const T & value)
    { m_Matrix /= value; }
  
  /** Matrix by scalar division.  */
  Self operator/(const T & value)
    { Self result( *this );
      result /= value;
      return result; }


  /** Return an element of the matrix. */
  inline T & operator()( unsigned int row, unsigned int col )
    { return m_Matrix(row,col); }

  /** Return an element of the matrix. */
  inline const T & operator()( unsigned int row, unsigned int col ) const
    { return m_Matrix(row,col); }

  /** Return a row of the matrix. */
  inline T * operator[]( unsigned int i )
    { return m_Matrix[i]; }

  /** Return a row of the matrix.*/
  inline const T * operator[]( unsigned int i ) const
    { return m_Matrix[i]; }

  /** Return the matrix. */
  inline InternalMatrixType & GetVnlMatrix( void )
    { return m_Matrix; }

  /** Return the matrix. */
  inline const InternalMatrixType & GetVnlMatrix( void ) const
    { return m_Matrix; }

  /** Set the matrix to identity. */
  inline void SetIdentity( void ) 
    { m_Matrix.set_identity(); }

  /** Fill the matrix with a value. */
  inline void Fill( const T & value ) 
    { m_Matrix.fill( value ); }

  /** Assignment operator. */
  inline const Self & operator=( const vnl_matrix<T> & matrix)
  {
    m_Matrix = matrix;
    return *this;
  }

  /**For every operator=, there should be an equivalent copy constructor. */
  inline Matrix(const vnl_matrix<T> & matrix) {
    this->operator=(matrix);
  }

  /** Comparison operators. */
  inline bool operator==( const Self & matrix) const
  {
    bool equal = true;
    for( unsigned int r=0; r<NRows; r++) 
      {
      for( unsigned int c=0; c<NColumns; c++ ) 
        {
        if (m_Matrix(r,c) != matrix.m_Matrix(r,c))
          {
          equal = false;
          break;
          }
        }
      }
    return equal;
  }
  inline bool operator!=( const Self & matrix) const
  {
    return !this->operator==(matrix);
  }

  inline const Self & operator=( const InternalMatrixType & matrix )
  {
    this->m_Matrix = matrix;
    return *this;
 }
 
  /**For every operator=, there should be an equivalent copy constructor. */
  inline explicit Matrix(const InternalMatrixType & matrix)
  {
    this->operator=(matrix);
  }
  /** Assignment operator. */
  inline const Self & operator=( const Self & matrix)
  {
    m_Matrix = matrix.m_Matrix;
    return *this;
  }

  /** Return the inverse matrix. */
  inline vnl_matrix_fixed<T,NColumns,NRows> GetInverse( void ) const
  {
    if (vnl_determinant(m_Matrix) == 0.0)
      {
      itkGenericExceptionMacro(<< "Singular matrix. Determinant is 0.");
      }
    vnl_matrix<T> temp = vnl_matrix_inverse<T>( m_Matrix );
    return temp;
  }

  /** Return the transposed matrix. */
  inline vnl_matrix_fixed<T,NColumns,NRows> GetTranspose( void ) const
  {
    return m_Matrix.transpose();
  }

  /** Default constructor. */
  Matrix() : m_Matrix(NumericTraits<T>::Zero) {};

  /** Copy constructor. */
  Matrix(const Self & matrix) : m_Matrix( matrix.m_Matrix ) {};
 
private:
  InternalMatrixType     m_Matrix;

};

template< class T, unsigned int NRows, unsigned int NColumns >  
ITK_EXPORT std::ostream& operator<<(std::ostream& os, 
                                    const Matrix<T,NRows,NColumns> & v) 
                            { os << v.GetVnlMatrix(); return os; }


  
} // end namespace itk

// Define instantiation macro for this template.
#define ITK_TEMPLATE_Matrix(_, EXPORT, x, y) namespace itk { \
  _(3(class EXPORT Matrix< ITK_TEMPLATE_3 x >)) \
  namespace Templates { typedef Matrix< ITK_TEMPLATE_3 x > Matrix##y; } \
  }

#if ITK_TEMPLATE_EXPLICIT
# include "Templates/itkMatrix+-.h"
#endif

#if ITK_TEMPLATE_TXX
# include "itkMatrix.txx"
#endif

#endif 
