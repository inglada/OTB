/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkDiscreteHessianGaussianImageFunction.txx,v $
  Language:  C++
  Date:      $Date: 2008-10-25 06:00:24 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkDiscreteHessianGaussianImageFunction_txx
#define __itkDiscreteHessianGaussianImageFunction_txx

#include "itkDiscreteHessianGaussianImageFunction.h"
#include "itkNeighborhoodOperatorImageFilter.h"
#include "itkNeighborhoodOperatorImageFunction.h"

namespace itk
{

/** Set the Input Image */
template <class TInputImage, class TOutput>
DiscreteHessianGaussianImageFunction<TInputImage,TOutput>
::DiscreteHessianGaussianImageFunction() :
  m_MaximumError( 0.005 ),
  m_MaximumKernelWidth( 30 ),
  m_NormalizeAcrossScale( true ),
  m_UseImageSpacing( true ),
  m_InterpolationMode( NearestNeighbourInterpolation )
{
  m_Variance.Fill(1.0);
  m_OperatorImageFunction = OperatorImageFunctionType::New();
}


/** Print self method */
template <class TInputImage, class TOutput>
void
DiscreteHessianGaussianImageFunction<TInputImage,TOutput>
::PrintSelf(std::ostream& os, Indent indent) const
{
  this->Superclass::PrintSelf(os,indent);
  os << indent << "UseImageSpacing: " << m_UseImageSpacing << std::endl;
  os << indent << "NormalizeAcrossScale: " << m_NormalizeAcrossScale << std::endl;
  os << indent << "Variance: " << m_Variance << std::endl;
  os << indent << "MaximumError: " << m_MaximumError << std::endl;
  os << indent << "MaximumKernelWidth: " << m_MaximumKernelWidth << std::endl;
  os << indent << "OperatorArray: " << m_OperatorArray << std::endl;
  os << indent << "KernelArray: " << m_KernelArray << std::endl;
  os << indent << "OperatorImageFunction: " << m_OperatorImageFunction << std::endl;
  os << indent << "InterpolationMode: " << m_InterpolationMode << std::endl;
}


/** Set the input image */
template <class TInputImage, class TOutput>
void
DiscreteHessianGaussianImageFunction<TInputImage,TOutput>
::SetInputImage( const InputImageType * ptr )
{
  Superclass::SetInputImage(ptr);
  m_OperatorImageFunction->SetInputImage(ptr);
}


/** Recompute the gaussian kernel used to evaluate indexes
 *  This should use a fastest Derivative Gaussian operator*/
template <class TInputImage, class TOutput>
void
DiscreteHessianGaussianImageFunction<TInputImage,TOutput>
::RecomputeGaussianKernel()
{
  /* Create 3*N operators (N=ImageDimension) where the
   * first N are zero-order, the second N are first-order
   * and the third N are second order */
  unsigned int idx;
  unsigned int maxRadius = 0;

  for(unsigned int direction=0; direction <
    itkGetStaticConstMacro(ImageDimension2); direction++ )
    {
    for( unsigned int order=0; order <= 2; ++order )
      {
      idx = itkGetStaticConstMacro(ImageDimension2)*order + direction;
      m_OperatorArray[idx].SetDirection( direction );
      m_OperatorArray[idx].SetMaximumKernelWidth( m_MaximumKernelWidth );
      m_OperatorArray[idx].SetMaximumError( m_MaximumError );
      
      if( ( m_UseImageSpacing == true ) && ( this->GetInputImage() ) )
        {
        if ( this->GetInputImage()->GetSpacing()[direction] == 0.0)
          {
          itkExceptionMacro(<< "Pixel spacing cannot be zero");
          }
        else
          {
        m_OperatorArray[idx].SetSpacing(this->GetInputImage()->GetSpacing()[direction]);
          }
        }

      // NOTE: GaussianDerivativeOperator modifies the variance when
      // setting image spacing
      m_OperatorArray[idx].SetVariance( m_Variance[direction] );
      m_OperatorArray[idx].SetOrder( order );
      m_OperatorArray[idx].SetNormalizeAcrossScale( m_NormalizeAcrossScale );
      m_OperatorArray[idx].CreateDirectional();

      // Check for maximum radius
      for( unsigned int i=0; i<itkGetStaticConstMacro(ImageDimension2); ++i )
        {
        if( m_OperatorArray[idx].GetRadius()[i] > maxRadius )
          maxRadius = m_OperatorArray[idx].GetRadius()[i];
        }
      }
      
    }

  // Now precompute the N-dimensional kernel. This fastest as we don't
  // have to perform N convolutions for each point we calculate but
  // only one.

  typedef itk::Image<TOutput,itkGetStaticConstMacro(ImageDimension2)>  KernelImageType;
  typename KernelImageType::Pointer kernelImage = KernelImageType::New();

  typedef typename KernelImageType::RegionType RegionType;
  RegionType region;

  typename RegionType::SizeType size;
  size.Fill( 4 * maxRadius + 1 );
  region.SetSize( size );

  kernelImage->SetRegions( region );
  kernelImage->Allocate();
  kernelImage->FillBuffer( itk::NumericTraits<TOutput>::Zero );

  // Initially the kernel image will be an impulse at the center
  typename KernelImageType::IndexType centerIndex;
  centerIndex.Fill( 2 * maxRadius ); // include also boundaries

  // Create an image region to be used later that does not include boundaries
  RegionType kernelRegion;
  size.Fill( 2 * maxRadius + 1 );
  typename RegionType::IndexType origin;
  origin.Fill( maxRadius );
  kernelRegion.SetSize( size );
  kernelRegion.SetIndex( origin );

  // Now create an image filter to perform sucessive convolutions
  typedef itk::NeighborhoodOperatorImageFilter<KernelImageType,KernelImageType>
    NeighborhoodFilterType;
  typename NeighborhoodFilterType::Pointer convolutionFilter = NeighborhoodFilterType::New();

  // Array that stores the current order for each direction
  typedef FixedArray<unsigned int,itkGetStaticConstMacro(ImageDimension2)>  OrderArrayType;
  OrderArrayType orderArray;

  // Precalculate compound derivative kernels (n-dimensional)
  // The order of calculation in the 3D case is: dxx, dxy, dxz, dyy,
  // dyz, dzz

  unsigned int opidx; // current operator index in m_OperatorArray
  unsigned int kernelidx = 0;

  for( unsigned int i=0; i<itkGetStaticConstMacro(ImageDimension2); ++i )
    {
    for( unsigned int j=i; j<itkGetStaticConstMacro(ImageDimension2); ++j )
      {
      orderArray.Fill(0);
      ++orderArray[i];
      ++orderArray[j];

      // Reset kernel image
      kernelImage->FillBuffer( itk::NumericTraits<TOutput>::Zero );
      kernelImage->SetPixel( centerIndex, itk::NumericTraits<TOutput>::One );

      for( unsigned int direction = 0; direction<itkGetStaticConstMacro(ImageDimension2); ++direction )
        {
        opidx = itkGetStaticConstMacro(ImageDimension2)*orderArray[direction] + direction;
        convolutionFilter->SetInput( kernelImage );
        convolutionFilter->SetOperator( m_OperatorArray[opidx] );
        convolutionFilter->Update();
        kernelImage = convolutionFilter->GetOutput();
        kernelImage->DisconnectPipeline();
        }

      // Set the size of the current kernel
      m_KernelArray[kernelidx].SetRadius( maxRadius );

      // Copy kernel image to neighborhood. Do not copy boundaries.
      ImageRegionConstIterator<KernelImageType> it( kernelImage, kernelRegion );
      it.GoToBegin();
      idx = 0;

      while( !it.IsAtEnd() )
        {
        m_KernelArray[kernelidx][idx] = it.Get();
        ++idx;
        ++it;
        }
      kernelidx++;
      }
    }
}


/** Evaluate the function at the specifed index */
template <class TInputImage, class TOutput>
typename DiscreteHessianGaussianImageFunction<TInputImage,TOutput>::OutputType
DiscreteHessianGaussianImageFunction<TInputImage,TOutput>
::EvaluateAtIndex(const IndexType& index) const
{
  OutputType hessian;

  for( unsigned int i=0; i<m_KernelArray.Size(); ++i )
    {
    m_OperatorImageFunction->SetOperator( m_KernelArray[i] );
    hessian[i] = m_OperatorImageFunction->EvaluateAtIndex( index );
    }
  return hessian;
}


/** Evaluate the function at the specifed point */
template <class TInputImage, class TOutput>
typename DiscreteHessianGaussianImageFunction<TInputImage,TOutput>::OutputType
DiscreteHessianGaussianImageFunction<TInputImage,TOutput>
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
    this->ConvertPointToContinuousIndex( point, cindex );
    return this->EvaluateAtContinuousIndex( cindex );
    }
}


/** Evaluate the function at specified ContinousIndex position.*/
template <class TInputImage, class TOutput>
typename DiscreteHessianGaussianImageFunction<TInputImage,TOutput>::OutputType
DiscreteHessianGaussianImageFunction<TInputImage,TOutput>
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
    OutputType hessian, currentHessian;
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
        currentHessian = this->EvaluateAtIndex( neighIndex );
        for( unsigned int i=0; i<hessian.Size(); ++i )
          hessian[i] += overlap * currentHessian[i];
        totalOverlap += overlap;
        }
  
      if( totalOverlap == 1.0 )
        {
        // finished
        break;
        }
      }

    return hessian;
    }
}

} // end namespace itk

#endif
