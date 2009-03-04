/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkCentralDifferenceImageFunction.txx,v $
  Language:  C++
  Date:      $Date: 2008-11-10 16:55:01 $
  Version:   $Revision: 1.33 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkCentralDifferenceImageFunction_txx
#define __itkCentralDifferenceImageFunction_txx

#include "itkCentralDifferenceImageFunction.h"

namespace itk
{


/**
 * Constructor
 */
template <class TInputImage, class TCoordRep>
CentralDifferenceImageFunction<TInputImage,TCoordRep>
::CentralDifferenceImageFunction()
{
#if defined(ITK_IMAGE_BEHAVES_AS_ORIENTED_IMAGE)
  this->m_UseImageDirection = true;
#else
  this->m_UseImageDirection = false;
#endif
}


/**
 *
 */
template <class TInputImage, class TCoordRep>
void
CentralDifferenceImageFunction<TInputImage,TCoordRep>
::PrintSelf(std::ostream& os, Indent indent) const
{
  this->Superclass::PrintSelf(os,indent);
  os << indent << "UseImageDirection = " << this->m_UseImageDirection << std::endl;
}


/**
 *
 */
template <class TInputImage, class TCoordRep>
typename CentralDifferenceImageFunction<TInputImage,TCoordRep>::OutputType
CentralDifferenceImageFunction<TInputImage,TCoordRep>
::EvaluateAtIndex( const IndexType& index ) const
{
  
  OutputType derivative;
  derivative.Fill( 0.0 );
  
  IndexType neighIndex = index;

  const InputImageType * inputImage = this->GetInputImage();

  const typename InputImageType::RegionType region = 
    inputImage->GetBufferedRegion();

  const typename InputImageType::SizeType& size   = region.GetSize();
  const typename InputImageType::IndexType& start = region.GetIndex();

  for ( unsigned int dim = 0; dim < TInputImage::ImageDimension; dim++ )
    {
    // bounds checking
    if( index[dim] < static_cast<long>(start[dim]) + 1 ||
        index[dim] > (start[dim] + static_cast<long>(size[dim]) - 2 ) )
      {
      derivative[dim] = 0.0;
      continue;
      }
    
    // compute derivative
    neighIndex[dim] += 1;
    derivative[dim] = inputImage->GetPixel( neighIndex );

    neighIndex[dim] -= 2;
    derivative[dim] -= inputImage->GetPixel( neighIndex );

    derivative[dim] *= 0.5 / inputImage->GetSpacing()[dim];
    neighIndex[dim] += 1;
    }

#ifdef ITK_USE_ORIENTED_IMAGE_DIRECTION
  if( this->m_UseImageDirection )
    {
    OutputType orientedDerivative;
    inputImage->TransformLocalVectorToPhysicalVector( derivative, orientedDerivative );
    return orientedDerivative;
    }
#endif

  return ( derivative );
}


} // end namespace itk

#endif
