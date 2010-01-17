/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkShapedFloodFilledFunctionConditionalConstIterator.txx,v $
  Language:  C++
  Date:      $Date: 2009-02-16 21:17:19 $
  Version:   $Revision: 1.1 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkShapedFloodFilledFunctionConditionalConstIterator_txx
#define __itkShapedFloodFilledFunctionConditionalConstIterator_txx

#include "itkShapedFloodFilledFunctionConditionalConstIterator.h"
#include "itkImageRegionConstIterator.h"

namespace itk
{
template<class TImage, class TFunction>
ShapedFloodFilledFunctionConditionalConstIterator<TImage, TFunction>
::ShapedFloodFilledFunctionConditionalConstIterator(const ImageType *imagePtr,
                                              FunctionType *fnPtr,
                                              IndexType startIndex) :
  m_FullyConnected(false)
{
  this->m_Image = imagePtr;
  m_Function = fnPtr;
  m_StartIndices.push_back ( startIndex );

  // Set up the temporary image
  this->InitializeIterator();
}

template<class TImage, class TFunction>
ShapedFloodFilledFunctionConditionalConstIterator<TImage, TFunction>
::ShapedFloodFilledFunctionConditionalConstIterator(const ImageType *imagePtr,
      FunctionType *fnPtr,
      std::vector<IndexType>& startIndex) :
  m_Function(fnPtr),
  m_FullyConnected(false)
{  
  this->m_Image = imagePtr; // can not be done in the initialization list
  
  unsigned int i;
  for (i = 0; i < startIndex.size(); i++ )
    {
    m_StartIndices.push_back ( startIndex[i] );
    }

  // Set up the temporary image
  this->InitializeIterator();
}

template<class TImage, class TFunction>
ShapedFloodFilledFunctionConditionalConstIterator<TImage, TFunction>
::ShapedFloodFilledFunctionConditionalConstIterator(const ImageType *imagePtr,
                                              FunctionType *fnPtr) :
  m_FullyConnected(false)
{
  this->m_Image = imagePtr;
  m_Function = fnPtr;

  // Set up the temporary image
  this->InitializeIterator();
}

template<class TImage, class TFunction>
void
ShapedFloodFilledFunctionConditionalConstIterator<TImage, TFunction>
::InitializeIterator()
{
  // Get the origin and spacing from the image in simple arrays
  m_ImageOrigin  = this->m_Image->GetOrigin();
  m_ImageSpacing = this->m_Image->GetSpacing();
  m_ImageRegion  = this->m_Image->GetBufferedRegion();
  
  // Build and setup the neighborhood iterator
  typename NeighborhoodIteratorType::RadiusType radius; radius.Fill(1);
  
  NeighborhoodIteratorType tmp_iter(radius, this->m_Image, m_ImageRegion);
  m_NeighborhoodIterator = tmp_iter;
  
  setConnectivity(&m_NeighborhoodIterator, m_FullyConnected);

  // Build a temporary image of chars for use in the flood algorithm
  m_TempPtr = TTempImage::New();
  typename TTempImage::RegionType tempRegion = this->m_Image->GetBufferedRegion();

  m_TempPtr->SetLargestPossibleRegion( tempRegion );
  m_TempPtr->SetBufferedRegion( tempRegion );
  m_TempPtr->SetRequestedRegion( tempRegion );
  m_TempPtr->Allocate();
  m_TempPtr->FillBuffer(NumericTraits<ITK_TYPENAME TTempImage::PixelType>::Zero);

  // Initialize the queue by adding the start index assuming one of
  // the m_StartIndices is "inside" This might not be true, in which
  // case it's up to the programmer to specify a correct starting
  // position later (using FindSeedPixel).  Must make sure that the
  // seed is inside the buffer before touching pixels.
  this->m_IsAtEnd = true;
  for ( unsigned int i = 0; i < m_StartIndices.size(); i++ )
    {
    if ( m_ImageRegion.IsInside ( m_StartIndices[i] ) )
      {
      m_IndexStack.push(m_StartIndices[i]);
      this->m_IsAtEnd = false;
      }
    }
}

template<class TImage, class TFunction>
void
ShapedFloodFilledFunctionConditionalConstIterator<TImage, TFunction>
::FindSeedPixel()
{
  // Create an iterator that will walk the input image
  typedef typename itk::ImageRegionConstIterator<TImage> IRIType;
  IRIType it = IRIType(this->m_Image, this->m_Image->GetBufferedRegion() );
  
  // Now we search the input image for the first pixel which is inside
  // the function of interest
  m_StartIndices.clear();
  for ( it.GoToBegin(); !it.IsAtEnd(); ++it)
    {
    if( this->IsPixelIncluded( it.GetIndex() ) )
      {
      m_StartIndices.push_back ( it.GetIndex() );

      // We need to reset the "beginning" now that we have a real seed
      this->GoToBegin();

      return;
      }
    }
}

template<class TImage, class TFunction>
void
ShapedFloodFilledFunctionConditionalConstIterator<TImage, TFunction>
::FindSeedPixels()
{
  // Create an iterator that will walk the input image
  typedef typename itk::ImageRegionConstIterator<TImage> IRIType;
  IRIType it = IRIType(this->m_Image, this->m_Image->GetBufferedRegion() );
  
  // Now we search the input image for the first pixel which is inside
  // the function of interest
  m_StartIndices.clear();
  bool found = false;
  for ( it.GoToBegin(); !it.IsAtEnd(); ++it)
    {
    if( this->IsPixelIncluded( it.GetIndex() ) )
      {
      m_StartIndices.push_back ( it.GetIndex() );
      found = true;
      }
    }
  if ( found )
    {
    // We need to reset the "beginning" now that we have a real seed
    this->GoToBegin();
    }
}

template<class TImage, class TFunction>
void
ShapedFloodFilledFunctionConditionalConstIterator<TImage, TFunction>
::DoFloodStep()
{
  // The index in the front of the queue should always be
  // valid and be inside since this is what the iterator
  // uses in the Set/Get methods. This is ensured by the
  // GoToBegin() method.

  // Take the index in the front of the queue  
  const IndexType & topIndex = m_IndexStack.front();
  
  // We are explicitly not calling set location since only offsets of
  // the neighborhood iterator are accessed.
  typename NeighborhoodIteratorType::ConstIterator neighborIt =
    m_NeighborhoodIterator.Begin();
  const typename NeighborhoodIteratorType::ConstIterator neighborEnd =
    m_NeighborhoodIterator.End();
  
  for (; neighborIt != neighborEnd; ++neighborIt)
    {
    const OffsetType& offset = neighborIt.GetNeighborhoodOffset();
    const IndexType tempIndex = topIndex + offset;

    // If this is a valid index and have not been tested,
    // then test it.
    if( m_ImageRegion.IsInside( tempIndex ) )
      {
      if( m_TempPtr->GetPixel( tempIndex ) == 0 )
        {
        // if it is inside, push it into the queue  
        if(  this->IsPixelIncluded( tempIndex ) )
          {
          m_IndexStack.push( tempIndex );
          m_TempPtr->SetPixel( tempIndex, 2); 
          }
        else  // If the pixel is outside
          {
          // Mark the pixel as outside and remove it from the queue.
          m_TempPtr->SetPixel( tempIndex, 1);
          }
        }
      }
    } // Finished traversing neighbors
    
  // Now that all the potential neighbors have been 
  // inserted we can get rid of the pixel in the front
  m_IndexStack.pop();
    
  if( m_IndexStack.empty() )
    {
    this->m_IsAtEnd = true;
    }


}

template<class TImage, class TFunction>
void
ShapedFloodFilledFunctionConditionalConstIterator<TImage, TFunction>
::SetFullyConnected(const bool _arg)
{
  if (this->m_FullyConnected != _arg)
    {
    this->m_FullyConnected = _arg;
    setConnectivity(&m_NeighborhoodIterator, m_FullyConnected);
    }
}

template<class TImage, class TFunction>
bool
ShapedFloodFilledFunctionConditionalConstIterator<TImage, TFunction>
::GetFullyConnected() const
{
  return this->m_FullyConnected;
}


} // end namespace itk

#endif
