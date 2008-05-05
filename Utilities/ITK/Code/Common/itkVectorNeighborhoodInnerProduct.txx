/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkVectorNeighborhoodInnerProduct.txx,v $
  Language:  C++
  Date:      $Date: 2003-09-10 14:29:29 $
  Version:   $Revision: 1.9 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itkVectorNeighborhoodInnerProduct_txx
#define _itkVectorNeighborhoodInnerProduct_txx
#include "itkVectorNeighborhoodInnerProduct.h"


namespace itk {

template<class TImage>
typename VectorNeighborhoodInnerProduct<TImage>::PixelType
VectorNeighborhoodInnerProduct<TImage>
::operator()(const std::slice &s,
             const ConstNeighborhoodIterator<TImage> &it,
             const OperatorType &op) const
{
  PixelType sum;
  unsigned int j;
  
  typename OperatorType::ConstIterator o_it;

  for (j = 0; j < VectorDimension; ++j)
    { sum[j] = NumericTraits<ScalarValueType>::Zero; }
  
  o_it = op.Begin();
  const typename OperatorType::ConstIterator op_end = op.End();

  const unsigned int start  = static_cast<unsigned int>( s.start() );
  const unsigned int stride = static_cast<unsigned int>( s.stride() );
  for ( unsigned int i = start; o_it < op_end; i+=stride, ++o_it )
    {
    for (j = 0; j< VectorDimension; ++j)
      {  sum[j] += *o_it * (it.GetPixel(i))[j]; }
    }
  
  return sum;
}


template<class TImage>
typename VectorNeighborhoodInnerProduct<TImage>::PixelType
VectorNeighborhoodInnerProduct<TImage>
::operator()(const std::slice &s,
             const NeighborhoodType &it,
             const OperatorType &op) const
{
  PixelType sum;
  unsigned int j;
  
  typename OperatorType::ConstIterator o_it;

  for (j = 0; j < VectorDimension; ++j)
    { sum[j] = NumericTraits<ScalarValueType>::Zero; }
  
  o_it = op.Begin();
  const typename OperatorType::ConstIterator op_end = op.End();

  const unsigned int start  = static_cast<unsigned int>( s.start() );
  const unsigned int stride = static_cast<unsigned int>( s.stride() );
  for ( unsigned int i = start; o_it < op_end; i+=stride, ++o_it )
    {
    for (j = 0; j< VectorDimension; ++j)
      {  sum[j] += *o_it * it[i][j]; }
    }
  
  return sum;
} 



}// end namespace itk
#endif
