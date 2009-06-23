/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkDiscreteGaussianDerivativeImageFunction.txx,v $
  Language:  C++
  Date:      $Date: 2008-12-06 13:28:10 $
  Version:   $Revision: 1.4 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkDiscreteGaussianDerivativeImageFunction_txx
#define __itkDiscreteGaussianDerivativeImageFunction_txx

#include "itkDiscreteGaussianDerivativeImageFunction.h"
#include "itkNeighborhoodOperatorImageFilter.h"

namespace itk
{

/** Set the Input Image */
template <class TInputImage, class TOutput>
DiscreteGaussianDerivativeImageFunction<TInputImage,TOutput>
::DiscreteGaussianDerivativeImageFunction() :
  m_MaximumError( 0.005 ),
  m_MaximumKernelWidth( 30 ),
  m_NormalizeAcrossScale( true ),
  m_UseImageSpacing( true ),
  m_InterpolationMode( NearestNeighbourInterpolation )
{
  m_Variance.Fill(1.0);
  m_Order.Fill(0);
  m_Order[0] = 1; // by default calculate derivative in x
  m_OperatorImageFunction = OperatorImageFunctionType::New();
}


/** Print self method */
template <class TInputImage, class TOutput>
void
DiscreteGaussianDerivativeImageFunction<TInputImage,TOutput>
::PrintSelf(std::ostream& os, Indent indent) const
{
  this->Superclass::PrintSelf(os,indent);
  os << indent << "UseImageSpacing: " << m_UseImageSpacing << std::endl;
  os << indent << "NormalizeAcrossScale: " << m_NormalizeAcrossScale << std::endl;
  os << indent << "Variance: " << m_Variance << std::endl;
  os << indent << "Order: " << m_Order << std::endl;
  os << indent << "MaximumError: " << m_MaximumError << std::endl;
  os << indent << "MaximumKernelWidth: " << m_MaximumKernelWidth << std::endl;
  os << indent << "InterpolationMode: " << m_InterpolationMode << std::endl;
  os << indent << "OperatorArray: " << m_OperatorArray << std::endl;
  os << indent << "DerivativeKernel: " << m_DerivativeKernel << std::endl;
  os << indent << "OperatorImageFunction: " << m_OperatorImageFunction << std::endl;
}


/** Set the input image */
template <class TInputImage, class TOutput>
void
DiscreteGaussianDerivativeImageFunction<TInputImage,TOutput>
::SetInputImage( const InputImageType * ptr )
{
  Superclass::SetInputImage(ptr);
  m_OperatorImageFunction->SetInputImage(ptr);
}


/** Recompute the gaussian kernel used to evaluate indexes
 *  This should use a fastest Derivative Gaussian operator */
template <class TInputImage, class TOutput>
void
DiscreteGaussianDerivativeImageFunction<TInputImage,TOutput>
::RecomputeGaussianKernel()
{
  // Create N operators (N=ImageDimension) with the order specified in m_Order
  unsigned int idx;

  for(unsigned int direction = 0; 
   direction < itkGetStaticConstMacro(ImageDimension2); 
   direction++ )
    {
    m_OperatorArray[direction].SetDirection( direction );
    m_OperatorArray[direction].SetMaximumKernelWidth( m_MaximumKernelWidth );
    m_OperatorArray[direction].SetMaximumError( m_MaximumError );

    if( ( m_UseImageSpacing == true ) && ( this->GetInputImage() ) )
      {
      if ( this->GetInputImage()->GetSpacing()[direction] == 0.0)
        {
        itkExceptionMacro(<< "Pixel spacing cannot be zero");
        }
      else
        {
        m_OperatorArray[direction].SetSpacing(this->GetInputImage()->GetSpacing()[direction]);
        }
      }

    // GaussianDerivativeOperator modifies the variance when setting
    // image spacing
    m_OperatorArray[direction].SetVariance( m_Variance[direction] );
    m_OperatorArray[direction].SetOrder( m_Order[direction] );
    m_OperatorArray[direction].SetNormalizeAcrossScale( m_NormalizeAcrossScale );
    m_OperatorArray[direction].CreateDirectional();
    }

  // Now precompute the N-dimensional kernel. This fastest as we don't
  // have to perform N convolutions for each point we calculate but
  // only one.

  typedef itk::Image<TOutput,itkGetStaticConstMacro(ImageDimension2)>  KernelImageType;
  typename KernelImageType::Pointer kernelImage = KernelImageType::New();

  typedef typename KernelImageType::RegionType RegionType;
  RegionType region;

  typename RegionType::SizeType size;
  size.Fill( 4 * m_OperatorArray[0].GetRadius()[0] + 1 );
  region.SetSize( size );

  kernelImage->SetRegions( region );
  kernelImage->Allocate();
  kernelImage->FillBuffer( itk::NumericTraits<TOutput>::Zero );

  // Initially the kernel image will be an impulse at the center
  typename KernelImageType::IndexType centerIndex;
  centerIndex.Fill( 2 * m_OperatorArray[0].GetRadius()[0] ); // include also boundaries
  kernelImage->SetPixel( centerIndex, itk::NumericTraits<TOutput>::One );

  // Create an image region to be used later that does not include boundaries
  RegionType kernelRegion;
  size.Fill( 2 * m_OperatorArray[0].GetRadius()[0] + 1 );
  typename RegionType::IndexType origin;
  origin.Fill( m_OperatorArray[0].GetRadius()[0] );
  kernelRegion.SetSize( size );
  kernelRegion.SetIndex( origin );

  // Now create an image filter to perform sucessive convolutions
  typedef itk::NeighborhoodOperatorImageFilter<KernelImageType,KernelImageType>
    NeighborhoodFilterType;
  typename NeighborhoodFilterType::Pointer convolutionFilter = NeighborhoodFilterType::New();

  for( unsigned int direction = 0; direction<itkGetStaticConstMacro(ImageDimension2); ++direction )
    {
    convolutionFilter->SetInput( kernelImage );
    convolutionFilter->SetOperator( m_OperatorArray[direction] );
    convolutionFilter->Update();
    kernelImage = convolutionFilter->GetOutput();
    kernelImage->DisconnectPipeline();
    }

  // Set the size of the kernel
  m_DerivativeKernel.SetRadius( m_OperatorArray[0].GetRadius()[0] );

  // Copy kernel image to neighborhood. Do not copy boundaries.
  ImageRegionConstIterator<KernelImageType> it( kernelImage, kernelRegion );
  it.GoToBegin();
  idx = 0;

  while( !it.IsAtEnd() )
    {
    m_DerivativeKernel[idx] = it.Get();
    ++idx;
    ++it;
    }
}


/** Evaluate the function at the specifed index */
template <class TInputImage, class TOutput>
typename DiscreteGaussianDerivativeImageFunction<TInputImage,TOutput>::OutputType
DiscreteGaussianDerivativeImageFunction<TInputImage,TOutput>
::EvaluateAtIndex(const IndexType& index) const
{
  OutputType derivative;
  m_OperatorImageFunction->SetOperator( m_DerivativeKernel );
  derivative = m_OperatorImageFunction->EvaluateAtIndex( index );
  return derivative;
}


/** Evaluate the function at the specifed point */
template <class TInputImage, class TOutput>
typename DiscreteGaussianDerivativeImageFunction<TInputImage,TOutput>::OutputType
DiscreteGaussianDerivativeImageFunction<TInputImage,TOutput>
::Evaluate(const PointType& point) const
{
  if( m_InterpolationMode == NearestNeighbourInterpolation )
    {
    IndexType index;
    this->ConvertPointToNearestIndex( point , index );
    return this->EvaluateAtIndex ( index );
    }
  else
    {
    ContinuousIndexType cindex;
#if ( ITK_VERSION_MAJOR < 3 ) || ( ITK_VERSION_MAJOR == 3 && ITK_VERSION_MINOR < 6 )
    this->ConvertPointToContinousIndex( point, cindex );
#else
    this->ConvertPointToContinuousIndex( point, cindex );
#endif
    return this->EvaluateAtContinuousIndex( cindex );
    }
}


/** Evaluate the function at specified ContinousIndex position.*/
template <class TInputImage, class TOutput>
typename DiscreteGaussianDerivativeImageFunction<TInputImage,TOutput>::OutputType
DiscreteGaussianDerivativeImageFunction<TInputImage,TOutput>
::EvaluateAtContinuousIndex(const ContinuousIndexType & cindex ) const
{
  if( m_InterpolationMode == NearestNeighbourInterpolation )
    {
    IndexType index;
    this->ConvertContinuousIndexToNearestIndex( cindex, index  );
    return this->EvaluateAtIndex( index );
    }
  else
    {
    unsigned int dim;  // index over dimension
    unsigned long neighbors = 1 << ImageDimension2;

    // Compute base index = closet index below point
    // Compute distance from point to base index
    signed long baseIndex[ImageDimension2];
    double distance[ImageDimension2];
    long tIndex;

    for( dim = 0; dim < ImageDimension2; dim++ )
      {
      // The following "if" block is equivalent to the following line without
      // having to call floor.
      //    baseIndex[dim] = (long) vcl_floor(index[dim] );
      if (cindex[dim] >= 0.0)
        {
        baseIndex[dim] = (long) cindex[dim];
        }
      else
        {
        tIndex = (long) cindex[dim];
        if (double(tIndex) != cindex[dim])
          {
          tIndex--;
          }
        baseIndex[dim] = tIndex;
        }
      distance[dim] = cindex[dim] - double( baseIndex[dim] );
      }

    // Interpolated value is the weighted sum of each of the surrounding
    // neighbors. The weight for each neighbor is the fraction overlap
    // of the neighbor pixel with respect to a pixel centered on point.
    TOutput value = NumericTraits<TOutput>::Zero;
    TOutput totalOverlap = NumericTraits<TOutput>::Zero;

    for( unsigned int counter = 0; counter < neighbors; counter++ )
      {
      double overlap = 1.0;          // fraction overlap
      unsigned int upper = counter;  // each bit indicates upper/lower neighbour
      IndexType neighIndex;

      // get neighbor index and overlap fraction
      for( dim = 0; dim < ImageDimension2; dim++ )
        {
        if ( upper & 1 )
          {
          neighIndex[dim] = baseIndex[dim] + 1;
          overlap *= distance[dim];
          }
        else
          {
          neighIndex[dim] = baseIndex[dim];
          overlap *= 1.0 - distance[dim];
          }
        upper >>= 1;
        }

      // get neighbor value only if overlap is not zero
      if( overlap )
        {
        value += overlap * static_cast<TOutput>( this->EvaluateAtIndex( neighIndex ) );
        totalOverlap += overlap;
        }

      if( totalOverlap == 1.0 )
        {
        // finished
        break;
        }
      }
    return ( static_cast<OutputType>( value ) );
    }
}

} // end namespace itk

#endif
