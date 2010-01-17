/*=========================================================================

    Program:   Insight Segmentation & Registration Toolkit
    Module:    $RCSfile: itkVectorMeanImageFunction.txx,v $
    Language:  C++
    Date:      $Date: 2009-03-03 15:11:44 $
    Version:   $Revision: 1.6 $

    Copyright (c) Insight Software Consortium. All rights reserved.
    See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

       This software is distributed WITHOUT ANY WARRANTY; without even 
       the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
       PURPOSE.  See the above copyright notices for more information.

  =========================================================================*/
#ifndef __itkVectorMeanImageFunction_txx
#define __itkVectorMeanImageFunction_txx

#include "itkVectorMeanImageFunction.h"
#include "itkNumericTraits.h"
#include "itkConstNeighborhoodIterator.h"

namespace itk
{

/**
 * Constructor
 */
template <class TInputImage, class TCoordRep>
VectorMeanImageFunction<TInputImage,TCoordRep>
::VectorMeanImageFunction()
{
  m_NeighborhoodRadius = 1;
}


/**
 *
 */
template <class TInputImage, class TCoordRep>
void
VectorMeanImageFunction<TInputImage,TCoordRep>
::PrintSelf(std::ostream& os, Indent indent) const
{
  this->Superclass::PrintSelf(os,indent);
  os << indent << "NeighborhoodRadius: "  << m_NeighborhoodRadius << std::endl;
}


/**
 *
 */
template <class TInputImage, class TCoordRep>
typename VectorMeanImageFunction<TInputImage,TCoordRep>
::RealType
VectorMeanImageFunction<TInputImage,TCoordRep>
::EvaluateAtIndex(const IndexType& index) const
{
  RealType sum;
  typedef  typename TInputImage::PixelType   PixelType;
  typedef  typename PixelType::ValueType     PixelComponentType;
  typedef  typename NumericTraits< PixelComponentType >::RealType PixelComponentRealType;
  
  const unsigned int VectorDimension = 
      ::itk::GetVectorDimension< PixelType >::VectorDimension;

  sum.Fill( NumericTraits< PixelComponentRealType >::Zero );
  
  if( !this->GetInputImage() )
    {
    sum.Fill( NumericTraits< PixelComponentRealType >::max() );
    return sum;
    }
  
  if ( !this->IsInsideBuffer( index ) )
    {
    sum.Fill( NumericTraits< PixelComponentRealType >::max() );
    return sum;
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
    for(unsigned int dim=0; dim<VectorDimension; dim++)
      {
      sum[dim] += static_cast<PixelComponentRealType>( it.GetPixel(i)[dim] );
      }
    }
  for(unsigned int dim=0; dim<VectorDimension; dim++)
    {
    sum[dim] /= double(it.Size());
    }
             
  return ( sum );
}


} // end namespace itk

#endif
