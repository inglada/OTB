/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkCovarianceImageFunction.txx,v $
  Language:  C++
  Date:      $Date: 2008-10-07 12:04:18 $
  Version:   $Revision: 1.8 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkCovarianceImageFunction_txx
#define __itkCovarianceImageFunction_txx

#include "itkCovarianceImageFunction.h"
#include "itkMatrix.h"
#include "itkNumericTraits.h"
#include "itkConstNeighborhoodIterator.h"

namespace itk
{

/**
 * Constructor
 */
template <class TInputImage, class TCoordRep>
CovarianceImageFunction<TInputImage,TCoordRep>
::CovarianceImageFunction()
{
  m_NeighborhoodRadius = 1;
}


/**
 *
 */
template <class TInputImage, class TCoordRep>
void
CovarianceImageFunction<TInputImage,TCoordRep>
::PrintSelf(std::ostream& os, Indent indent) const
{
  this->Superclass::PrintSelf(os,indent);
  os << indent << "NeighborhoodRadius: "  << m_NeighborhoodRadius << std::endl;
}


/**
 *
 */
template <class TInputImage, class TCoordRep>
typename CovarianceImageFunction<TInputImage,TCoordRep>
::RealType
CovarianceImageFunction<TInputImage,TCoordRep>
::EvaluateAtIndex(const IndexType& index) const
{
  typedef  typename TInputImage::PixelType  PixelType;
  typedef  typename PixelType::ValueType    PixelComponentType;

  typedef  typename NumericTraits< PixelComponentType >::RealType PixelComponentRealType;

  const unsigned int VectorDimension = 
      ::itk::GetVectorDimension< PixelType >::VectorDimension;

  RealType covariance = RealType( VectorDimension, VectorDimension );
  


  if( !this->GetInputImage() )
    {
    itkExceptionMacro( << "No image connected to CovarianceImageFunction");
    covariance.fill( NumericTraits< PixelComponentRealType >::max() );
    return covariance;
    }
  
  if ( !this->IsInsideBuffer( index ) )
    {
    covariance.fill( NumericTraits< PixelComponentRealType >::max() );
    return covariance;
    }

  covariance.fill( NumericTraits< PixelComponentRealType >::Zero );


  typedef vnl_vector< PixelComponentRealType >    MeanVectorType;
  MeanVectorType mean = MeanVectorType( VectorDimension ); 
  mean.fill( NumericTraits< PixelComponentRealType >::Zero );

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
    const PixelType pixel = it.GetPixel(i);

    for(unsigned int dimx=0; dimx<VectorDimension; dimx++)
      {
      mean[ dimx ] += pixel[ dimx ];
      for(unsigned int dimy=0; dimy<VectorDimension; dimy++)
        {
        covariance[dimx][dimy] += 
            static_cast<PixelComponentRealType>( pixel[dimx] ) *
            static_cast<PixelComponentRealType>( pixel[dimy] );
        }
      }
    }

  const PixelComponentRealType rsize = 
          static_cast< PixelComponentRealType >( size );

  mean /= rsize;

  for(unsigned int dimx=0; dimx<VectorDimension; dimx++)
    {
    for(unsigned int dimy=0; dimy<VectorDimension; dimy++)
      {
      covariance[dimx][dimy] /= rsize;
      covariance[dimx][dimy] -= mean[dimx] * mean[dimy];
      }
    }
             
  return ( covariance );
}


} // end namespace itk

#endif
