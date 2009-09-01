/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkShrinkImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2009-05-05 21:45:49 $
  Version:   $Revision: 1.61 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

  Portions of this code are covered under the VTK copyright.
  See VTKCopyright.txt or http://www.kitware.com/VTKCopyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkShrinkImageFilter_txx
#define __itkShrinkImageFilter_txx

#include "itkShrinkImageFilter.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkContinuousIndex.h"
#include "itkObjectFactory.h"
#include "itkProgressReporter.h"

namespace itk
{

/**
 *
 */
template <class TInputImage, class TOutputImage>
ShrinkImageFilter<TInputImage,TOutputImage>
::ShrinkImageFilter()
{
  for( unsigned int j = 0; j < ImageDimension; j++ )
    {
    m_ShrinkFactors[j] = 1;
    }
}
  

/**
 *
 */
template <class TInputImage, class TOutputImage>
void 
ShrinkImageFilter<TInputImage,TOutputImage>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);
  
  os << indent << "Shrink Factor: ";
  for( unsigned int j = 0; j < ImageDimension; j++ )
    {
    os << m_ShrinkFactors[j] << " ";
    } 
  os << std::endl;
}


/**
 *
 */
template <class TInputImage, class TOutputImage>
void 
ShrinkImageFilter<TInputImage,TOutputImage>
::SetShrinkFactors(unsigned int factors[])
{
  unsigned int j;
  for( j = 0; j < ImageDimension; j++ )
    {
    if( factors[j] != m_ShrinkFactors[j] ) break;
    }
  if( j < ImageDimension )
    {
    this->Modified();
    for( j = 0; j < ImageDimension; j++ )
      {
      m_ShrinkFactors[j] = factors[j];
      if( m_ShrinkFactors[j] < 1 ) 
        {
        m_ShrinkFactors[j] = 1;
        }
      }
    }
}


/**
 *
 */
template <class TInputImage, class TOutputImage>
void 
ShrinkImageFilter<TInputImage,TOutputImage>
::SetShrinkFactors(unsigned int factor)
{
  unsigned int j;
  for( j = 0; j < ImageDimension; j++ )
    {
    if( factor != m_ShrinkFactors[j] ) break;
    }
  if( j < ImageDimension )
    {
    this->Modified();
    for( j = 0; j < ImageDimension; j++ )
      {
      m_ShrinkFactors[j] = factor;
      if( m_ShrinkFactors[j] < 1 ) 
        {
        m_ShrinkFactors[j] = 1;
        }
      }
    }
}


/**
 *
 */
template <class TInputImage, class TOutputImage>
void 
ShrinkImageFilter<TInputImage,TOutputImage>
::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                       int threadId)
{
  // Get the input and output pointers
  InputImageConstPointer  inputPtr = this->GetInput();
  OutputImagePointer      outputPtr = this->GetOutput();
  
    
  // convert the factor for convenient multiplication
  unsigned int i;
  typename TOutputImage::SizeType  factorSize;
  for ( i=0; i < TInputImage::ImageDimension; i++ )
    {
    factorSize[i] = m_ShrinkFactors[i];
    }

  // Define a few indices that will be used to transform from an input pixel
  // to an output pixel
  OutputIndexType           outputIndex;
  InputIndexType            inputIndex;
  OutputOffsetType          offsetIndex;
  
  typename TOutputImage::PointType tempPoint;

  // use this index to compute the offset everywhere in this class
  outputIndex = outputPtr->GetLargestPossibleRegion().GetIndex();
  inputIndex = inputPtr->GetLargestPossibleRegion().GetIndex();

  // We wish to perform the following mapping of outputIndex to
  // inputIndex on all points in our region
  outputPtr->TransformIndexToPhysicalPoint( outputIndex, tempPoint );
  inputPtr->TransformPhysicalPointToIndex( tempPoint, inputIndex );


  // Given that the size is scaled by a constant factor eq:
  // inputIndex = outputIndex * factorSize 
  // is equivalent up to a fixed offset which we now compute
  typename OutputOffsetType::OffsetValueType zeroOffset = 0;
  for ( i=0; i < TInputImage::ImageDimension; i++ )
    {
    offsetIndex[i] = inputIndex[i] - outputIndex[i]*m_ShrinkFactors[i];
    // it is plausible that due to small amounts of loss of numerical
    // precision that the offset it negaive, this would cause sampling
    // out of out region, this is insurance against that possibility
    offsetIndex[i] = vnl_math_max( zeroOffset, offsetIndex[i] );
    }

    
  // support progress methods/callbacks
  ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());
  
  // Define/declare an iterator that will walk the output region for this
  // thread.
  typedef ImageRegionIteratorWithIndex<TOutputImage> OutputIterator;
  OutputIterator outIt( outputPtr, outputRegionForThread );
  
  while ( !outIt.IsAtEnd() ) 
    {
    // determine the index and physical location of the output pixel
    outputIndex = outIt.GetIndex();

    // an optimized version of 
    // outputPtr->TransformIndexToPhysicalPoint(outputIndex, tempPoint);
    // inputPtr->TransformPhysicalPointToIndex(tempPoint, inputIndex);
    // but without the rounding and precision issues
    inputIndex = outputIndex * factorSize + offsetIndex;

    // copy the input pixel to the output
    outIt.Set( inputPtr->GetPixel(inputIndex) );
    ++outIt;

    progress.CompletedPixel();
    }
}

/** 
 *
 */
template <class TInputImage, class TOutputImage>
void 
ShrinkImageFilter<TInputImage,TOutputImage>
::GenerateInputRequestedRegion()
{
  // call the superclass' implementation of this method
  Superclass::GenerateInputRequestedRegion();
  
  // get pointers to the input and output
  InputImagePointer  inputPtr = const_cast<TInputImage *> (this->GetInput());
  OutputImagePointer outputPtr = this->GetOutput();
  
  if ( !inputPtr || !outputPtr )
    {
    return;
    }
  
  // Compute the input requested region (size and start index)
  // Use the image transformations to insure an input requested region
  // that will provide the proper range
  unsigned int i;
  const typename TOutputImage::SizeType& outputRequestedRegionSize
    = outputPtr->GetRequestedRegion().GetSize();
  const typename TOutputImage::IndexType& outputRequestedRegionStartIndex
    = outputPtr->GetRequestedRegion().GetIndex();

  
  // convert the factor for convenient multiplication
  typename TOutputImage::SizeType  factorSize;
  for ( i=0; i < TInputImage::ImageDimension; i++ )
    {
    factorSize[i] = m_ShrinkFactors[i];
    }

  OutputIndexType           outputIndex;
  InputIndexType            inputIndex, inputRequestedRegionIndex;
  OutputOffsetType          offsetIndex;

  typename TInputImage::SizeType   inputRequestedRegionSize;
  typename TOutputImage::PointType tempPoint;


 // use this index to compute the offset everywhere in this class
  outputIndex = outputPtr->GetLargestPossibleRegion().GetIndex();
  inputIndex = inputPtr->GetLargestPossibleRegion().GetIndex();

  // We wish to perform the following mapping of outputIndex to
  // inputIndex on all points in our region
  outputPtr->TransformIndexToPhysicalPoint( outputIndex, tempPoint );
  inputPtr->TransformPhysicalPointToIndex( tempPoint, inputIndex );


  // Given that the size is scaled by a constant factor eq:
  // inputIndex = outputIndex * factorSize 
  // is equivalent up to a fixed offset which we now compute
  typename OutputOffsetType::OffsetValueType zeroOffset = 0;
  for ( i=0; i < TInputImage::ImageDimension; i++ )
    {
    offsetIndex[i] = inputIndex[i] - outputIndex[i]*m_ShrinkFactors[i];
    // it is plausible that due to small amounts of loss of numerical
    // precision that the offset it negaive, this would cause sampling
    // out of out region, this is insurance against that possibility
    offsetIndex[i] = vnl_math_max( zeroOffset, offsetIndex[i] );
    }

  inputRequestedRegionIndex = outputRequestedRegionStartIndex*factorSize + offsetIndex;

  // originally this was
  // inputSize = outputRequestedRegionSize * factorSize
  // but since we don't sample edge to edge, we can reduce the size
  for ( i=0; i < TInputImage::ImageDimension; ++i )
    {
    inputRequestedRegionSize[i] = (outputRequestedRegionSize[i] - 1 ) * factorSize[i] + 1;
    }


  typename TInputImage::RegionType inputRequestedRegion;
  inputRequestedRegion.SetIndex( inputRequestedRegionIndex );
  inputRequestedRegion.SetSize( inputRequestedRegionSize );
  inputRequestedRegion.Crop( inputPtr->GetLargestPossibleRegion() );

  inputPtr->SetRequestedRegion( inputRequestedRegion );
}

/** 
 *
 */
template <class TInputImage, class TOutputImage>
void 
ShrinkImageFilter<TInputImage,TOutputImage>
::GenerateOutputInformation()
{
  // call the superclass' implementation of this method
  Superclass::GenerateOutputInformation();
  
  // get pointers to the input and output
  InputImageConstPointer  inputPtr  = this->GetInput();
  OutputImagePointer      outputPtr = this->GetOutput();

  if ( !inputPtr || !outputPtr )
    {
    return;
    }
  
  // we need to compute the output spacing, the output image size, and the
  // output image start index
  unsigned int i;
  const typename TInputImage::SpacingType&
    inputSpacing = inputPtr->GetSpacing();
  const typename TInputImage::SizeType&   inputSize
    = inputPtr->GetLargestPossibleRegion().GetSize();
  const typename TInputImage::IndexType&  inputStartIndex
    = inputPtr->GetLargestPossibleRegion().GetIndex();

  typename TOutputImage::SpacingType  outputSpacing;
  typename TOutputImage::SizeType     outputSize;
  typename TOutputImage::IndexType    outputStartIndex;
  
  for (i = 0; i < TOutputImage::ImageDimension; i++)
    {
    outputSpacing[i] = inputSpacing[i] * (double) m_ShrinkFactors[i];

    // we round down so that all output pixels fit input input region
    outputSize[i] = (unsigned long)
      vcl_floor((double) inputSize[i] / (double) m_ShrinkFactors[i]);

    if( outputSize[i] < 1 )
      {
      outputSize[i] = 1;
      }

    // because of the later origin shift this starting index is not
    // critical
    outputStartIndex[i] = (long)
      vcl_ceil((double) inputStartIndex[i] / (double) m_ShrinkFactors[i] );
    }
  
  outputPtr->SetSpacing( outputSpacing );

  // compute origin offset
  // The physical center's of the input and output should be the same
  ContinuousIndex<double, TOutputImage::ImageDimension> inputCenterIndex;
  ContinuousIndex<double, TOutputImage::ImageDimension> outputCenterIndex;
  for (i = 0; i < TOutputImage::ImageDimension; i++)
    {
    inputCenterIndex[i] = inputStartIndex[i] + (inputSize[i] - 1) / 2.0;
    outputCenterIndex[i] = outputStartIndex[i] + (outputSize[i] - 1) / 2.0;
    }

  typename TOutputImage::PointType inputCenterPoint;
  typename TOutputImage::PointType outputCenterPoint;
  inputPtr->TransformContinuousIndexToPhysicalPoint(inputCenterIndex, inputCenterPoint);
  outputPtr->TransformContinuousIndexToPhysicalPoint(outputCenterIndex, outputCenterPoint);
  
  typename TOutputImage::PointType outputOrigin = outputPtr->GetOrigin();
  outputOrigin = outputOrigin + (inputCenterPoint - outputCenterPoint);
  outputPtr->SetOrigin(outputOrigin);
  
  // set region
  typename TOutputImage::RegionType outputLargestPossibleRegion;
  outputLargestPossibleRegion.SetSize( outputSize );
  outputLargestPossibleRegion.SetIndex( outputStartIndex );

  outputPtr->SetLargestPossibleRegion( outputLargestPossibleRegion );
}

} // end namespace itk

#endif
