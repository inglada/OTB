/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkVarianceImageFunction.txx,v $
  Language:  C++
  Date:      $Date: 2006-02-06 22:01:58 $
  Version:   $Revision: 1.13 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkVarianceImageFunction_txx
#define __itkVarianceImageFunction_txx

#include "itkVarianceImageFunction.h"

#include "itkNumericTraits.h"
#include "itkConstNeighborhoodIterator.h"

namespace itk
{

/**
 * Constructor
 */
template <class TInputImage, class TCoordRep>
VarianceImageFunction<TInputImage,TCoordRep>
::VarianceImageFunction()
{
  m_NeighborhoodRadius = 1;
}


/**
 *
 */
template <class TInputImage, class TCoordRep>
void
VarianceImageFunction<TInputImage,TCoordRep>
::PrintSelf(std::ostream& os, Indent indent) const
{
  this->Superclass::PrintSelf(os,indent);
  os << indent << "NeighborhoodRadius: "  << m_NeighborhoodRadius << std::endl;
}


/**
 *
 */
template <class TInputImage, class TCoordRep>
typename VarianceImageFunction<TInputImage,TCoordRep>
::RealType
VarianceImageFunction<TInputImage,TCoordRep>
::EvaluateAtIndex(const IndexType& index) const
{
  RealType sum;
  RealType sumOfSquares;
  RealType var;

  sum = NumericTraits<RealType>::Zero;
  sumOfSquares = NumericTraits<RealType>::Zero;
  
  if( !this->GetInputImage() )
    {
    return ( NumericTraits<RealType>::max() );
    }
  
  if ( !this->IsInsideBuffer( index ) )
    {
    return ( NumericTraits<RealType>::max() );
    }

  // Create an N-d neighborhood kernel, using a zeroflux boundary condition
  typename InputImageType::SizeType kernelSize;
  kernelSize.Fill( m_NeighborhoodRadius );
  
  ConstNeighborhoodIterator<InputImageType>
    it(kernelSize, this->GetInputImage(), this->GetInputImage()->GetBufferedRegion());

  // Set the iterator at the desired location
  it.SetLocation(index);

  // Walk the neighborhood
  const unsigned int size = it.Size();
  for (unsigned int i = 0; i < size; ++i)
    {
    const RealType value = static_cast<RealType>( it.GetPixel(i) );
    sum           += value;
    sumOfSquares  += value * value;
    }

  const double num = static_cast<double>( size );
  var = ( sumOfSquares - ( sum*sum / num ) ) / ( num - 1.0 );
  
  return ( var );
}


} // end namespace itk

#endif
