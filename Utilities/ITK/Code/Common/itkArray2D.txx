/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkArray2D.txx,v $
  Language:  C++
  Date:      $Date: 2003/12/15 14:13:19 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itkArray2D_txx
#define _itkArray2D_txx

#include "itkArray2D.h"

namespace itk
{


/**
 * Default constructor 
 */
template < typename TValueType >
Array2D<TValueType >
::Array2D():vnl_matrix<TValueType>()
{
}


/**
 * Constructor with number of rows and columns as arguments 
 */
template < typename TValueType >
Array2D<TValueType >
::Array2D(unsigned int rows,unsigned int cols):vnl_matrix<TValueType>(rows,cols)
{
}



/**
 * Constructor from a vnl_matrix
 */
template < typename TValueType >
Array2D<TValueType >
::Array2D( const VnlMatrixType & matrix ):
  vnl_matrix<TValueType>( matrix )
{
}



/**
 * Copy Constructor 
 */
template < typename TValueType >
Array2D<TValueType >
::Array2D( const Self & array )
  :vnl_matrix<TValueType>( array )
{
}



/**
 * Assignment Operator from Array
 */
template < typename TValueType >
const Array2D<TValueType > &
Array2D<TValueType >
::operator=( const Self & array )
{
  this->VnlMatrixType::operator=( array );
  return *this;
}



/**
 * Assignment Operator from vnl_matrix
 */
template < typename TValueType >
const Array2D<TValueType > &
Array2D<TValueType >
::operator=( const VnlMatrixType & matrix )
{
  this->VnlMatrixType::operator=( matrix );
  return *this;
}

template < typename TValueType >
void Array2D<TValueType >
::SetSize(unsigned int m, unsigned int n)
{
  this->set_size(m, n);
}


  


} // namespace itk

#endif
