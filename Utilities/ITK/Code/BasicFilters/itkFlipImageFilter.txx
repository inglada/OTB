/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkFlipImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2008-10-07 17:31:02 $
  Version:   $Revision: 1.19 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkFlipImageFilter_txx
#define __itkFlipImageFilter_txx

#include "itkFlipImageFilter.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkExceptionObject.h"
#include "itkProgressReporter.h"

namespace itk
{

/**
 * Constructor
 */
template <class TImage>
FlipImageFilter<TImage>
::FlipImageFilter()
{

  m_FlipAxes.Fill( false );
  m_FlipAboutOrigin = true;

}


/**
 * PrintSelf
 */
template <class TImage>
void 
FlipImageFilter<TImage>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);
  os << indent << "FlipAxes: " << m_FlipAxes << std::endl;
  os << indent << "FlipAboutOrigin: " << m_FlipAboutOrigin << std::endl;

}


/**
 * The output image meta information is obtained by permuting
 * the input image meta information.
 */
template <class TImage>
void
FlipImageFilter<TImage>
::GenerateOutputInformation()
{
  // call the superclass's implementation of this method
  Superclass::GenerateOutputInformation();

  // get pointers to the input and output
  InputImagePointer inputPtr = 
    const_cast< TImage * >( this->GetInput() );
  OutputImagePointer outputPtr = this->GetOutput();

  if( !inputPtr || !outputPtr )
    {
    return;
    }

  const typename TImage::DirectionType& inputDirection = inputPtr->GetDirection();
  const typename TImage::SizeType& inputSize =
    inputPtr->GetLargestPossibleRegion().GetSize();
  const typename TImage::IndexType& inputStartIndex =
    inputPtr->GetLargestPossibleRegion().GetIndex();

  typename TImage::PointType outputOrigin;
  typename TImage::IndexType newIndex = inputStartIndex;

  unsigned int j;

  typename TImage::DirectionType flipMatrix;
  flipMatrix.SetIdentity();

  // Need the coordinate of the pixel that will become the first pixel
  // and need a matrix to model the flip
  for ( j = 0; j < ImageDimension; j++ )
    {
    if ( m_FlipAxes[j] )
      {
      // If flipping the axis, then we need to know the last pixel in
      // that dimension
      newIndex[j] += (inputSize[j] - 1);

      // What we really want is the index padded out past this point
      // by the amount the start index is from [0,0,0] (because the
      // output regions have the same index layout as the input
      // regions)
      newIndex[j] += inputStartIndex[j];

      // Only flip the directions if we are NOT flipping about the
      // origin (when flipping about the origin, the pixels are
      // ordered in the same direction as the input directions. when
      // NOT flipping about the origin, the pixels traverse space in
      // the opposite direction. when flipping about the origin,
      // increasing indices traverse space in the same direction as
      // the original data.).
      if (!m_FlipAboutOrigin)
        {
        flipMatrix[j][j] = -1.0;
        }
      }
    }

  inputPtr->TransformIndexToPhysicalPoint( newIndex, outputOrigin );

  // Finally, flip about the origin if needed
  if (m_FlipAboutOrigin)
    {
    for ( j = 0; j < ImageDimension; j++ )
      {
      if ( m_FlipAxes[j] )
        {
        outputOrigin[j] *= -1;
        }
      }
    }
                                           
  outputPtr->SetDirection( inputDirection * flipMatrix );
  outputPtr->SetOrigin( outputOrigin );
}


/**
 * The required input requested region is obtained by permuting
 * the index and size of the output requested region
 */
template <class TImage>
void
FlipImageFilter<TImage>
::GenerateInputRequestedRegion()
{
  // call the superclass's implementation of this method
  Superclass::GenerateInputRequestedRegion();

  // get pointers to the input and output
  InputImagePointer inputPtr = 
    const_cast< TImage * >( this->GetInput());
  OutputImagePointer outputPtr = this->GetOutput();

  if( !inputPtr || !outputPtr )
    {
    return;
    }

  const typename TImage::SizeType& outputRequestedSize =
    outputPtr->GetRequestedRegion().GetSize();
  const typename TImage::IndexType& outputRequestedIndex =
    outputPtr->GetRequestedRegion().GetIndex();
  
  const typename TImage::SizeType & outputLargestPossibleSize =
    outputPtr->GetLargestPossibleRegion().GetSize();
  const typename TImage::IndexType& outputLargestPossibleIndex =
    outputPtr->GetLargestPossibleRegion().GetIndex();
  
  IndexType inputRequestedIndex;

  unsigned int j;
  for ( j = 0; j < ImageDimension; j++ )
    {
    if ( m_FlipAxes[j] )
      {
      inputRequestedIndex[j] =
        2 * outputLargestPossibleIndex[j] 
        + static_cast<IndexValueType>( outputLargestPossibleSize[j] )
        - static_cast<IndexValueType>( outputRequestedSize[j] ) 
        - outputRequestedIndex[j];
      }
    else
      {
      inputRequestedIndex[j] = outputRequestedIndex[j];
      }
    }

  typename TImage::RegionType inputRequestedRegion;
  inputRequestedRegion.SetSize( outputRequestedSize );
  inputRequestedRegion.SetIndex( inputRequestedIndex );

  inputPtr->SetRequestedRegion( inputRequestedRegion );


}


/**
 *
 */
template <class TImage>
void
FlipImageFilter<TImage>
::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                       int threadId)
{

  unsigned long i;
  unsigned int j;

  // Get the input and output pointers
  InputImageConstPointer inputPtr = this->GetInput();
  OutputImagePointer outputPtr = this->GetOutput();

  // Setup output region iterator
  typedef ImageRegionIteratorWithIndex<TImage> OutputIterator;
  OutputIterator outIt(outputPtr, outputRegionForThread);

  typename TImage::IndexType outputIndex;
  typename TImage::IndexType inputIndex;

  // support progress methods/callbacks
  ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());
    
  const typename TImage::SizeType & outputLargestPossibleSize =
    outputPtr->GetLargestPossibleRegion().GetSize();
  const typename TImage::IndexType& outputLargestPossibleIndex =
    outputPtr->GetLargestPossibleRegion().GetIndex();

  IndexValueType offset[ImageDimension];
  for ( j = 0; j < ImageDimension; j++ )
    {
    if ( m_FlipAxes[j] )
      {
      offset[j] = 2 * outputLargestPossibleIndex[j] 
        + static_cast<IndexValueType>( outputLargestPossibleSize[j] ) - 1;
      } 
    }

  // walk the output region, and sample the input image
  for ( i = 0; !outIt.IsAtEnd(); ++outIt, i++ )
    {
    // determine the index of the output pixel
    outputIndex = outIt.GetIndex();
    
    // determine the input pixel location associated with this output pixel
    for ( j = 0; j < ImageDimension; j++ )
      {
      if ( m_FlipAxes[j] )
        {
        inputIndex[j] = - 1 * outputIndex[j] + offset[j];
        }
      else 
        {
        inputIndex[j] = outputIndex[ j ];
        }
      }
    
    // copy the input pixel to the output
    outIt.Set( inputPtr->GetPixel(inputIndex) );
    progress.CompletedPixel();
    }

}


} // namespace itk

#endif
