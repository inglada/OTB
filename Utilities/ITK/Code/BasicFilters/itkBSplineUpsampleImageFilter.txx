/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkBSplineUpsampleImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2008-10-09 20:43:43 $
  Version:   $Revision: 1.9 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

  Portions of this code are covered under the VTK copyright.
  See VTKCopyright.txt or http://www.kitware.com/VTKCopyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkBSplineUpsampleImageFilter_txx
#define __itkBSplineUpsampleImageFilter_txx
#include "itkBSplineUpsampleImageFilter.h"

namespace itk
{

/**
 * Constructor
 */
template <class TInputImage, class TOutputImage, class ResamplerType>
BSplineUpsampleImageFilter<TInputImage,TOutputImage, ResamplerType>
::BSplineUpsampleImageFilter()
{

}

/**
 * Standard "PrintSelf" method
 */
template <class TInputImage, class TOutputImage, class ResamplerType>
void
BSplineUpsampleImageFilter<TInputImage,TOutputImage, ResamplerType>
::PrintSelf(
  std::ostream& os, 
  Indent indent) const
{
  Superclass::PrintSelf( os, indent );
}

template <class TInputImage, class TOutputImage, class ResamplerType>
void
BSplineUpsampleImageFilter<TInputImage,TOutputImage, ResamplerType>
::GenerateData()
{
  itkDebugMacro(<<"Actually executing");

  // Get the input and output pointers
  InputImagePointer  inputPtr = const_cast< TInputImage * > ( this->GetInput() );
  OutputImagePointer outputPtr = this->GetOutput();

  // Since we are providing a GenerateData() method, we need to allocate the
  // output buffer memory (if we provided a ThreadedGenerateData(), then
  // the memory would have already been allocated for us).
  outputPtr->SetBufferedRegion( outputPtr->GetRequestedRegion() );
  outputPtr->Allocate();

  // Iterator for walking the output region is defined in the SuperClass
  OutputImageIterator outIt = OutputImageIterator(outputPtr,
                                                  outputPtr->GetRequestedRegion());

  // Calculate actual output
  // TODO:  Need to verify outIt is correctly sized.
  Superclass::ExpandNDImage(outIt);

  
}


/** 
 *
 */
template <class TInputImage, class TOutputImage, class ResamplerType>
void 
BSplineUpsampleImageFilter<TInputImage,TOutputImage, ResamplerType>
::GenerateInputRequestedRegion()
{
  // call the superclass' implementation of this method
  Superclass::GenerateInputRequestedRegion();

  // get pointers to the input and output
  InputImagePointer  inputPtr = const_cast< TInputImage * > ( this->GetInput() );
  OutputImagePointer outputPtr = this->GetOutput();
  
  if( !outputPtr || !inputPtr )
    {
    return;
    }
  
  inputPtr->SetRequestedRegionToLargestPossibleRegion();

  // Compute the input requested region (size and start index)
  unsigned int i;
  const typename TOutputImage::SizeType& outputRequestedRegionSize
    = outputPtr->GetRequestedRegion().GetSize();
  const typename TOutputImage::IndexType& outputRequestedRegionStartIndex
    = outputPtr->GetRequestedRegion().GetIndex();
  
  typename TInputImage::SizeType  inputRequestedRegionSize;
  typename TInputImage::IndexType inputRequestedRegionStartIndex;
  
  for (i = 0; i < TInputImage::ImageDimension; i++)
    {
    inputRequestedRegionSize[i]
      = outputRequestedRegionSize[i] / 2;
    inputRequestedRegionStartIndex[i]
      = outputRequestedRegionStartIndex[i] / (int)2;
    }

  typename TInputImage::RegionType inputRequestedRegion;
  inputRequestedRegion.SetSize( inputRequestedRegionSize );
  inputRequestedRegion.SetIndex( inputRequestedRegionStartIndex );

  inputPtr->SetRequestedRegion( inputRequestedRegion );
}


/** 
 *
 */
template <class TInputImage, class TOutputImage, class ResamplerType>
void 
BSplineUpsampleImageFilter<TInputImage,TOutputImage, ResamplerType>
::GenerateOutputInformation()
{
  // call the superclass' implementation of this method
  Superclass::GenerateOutputInformation();

  // get pointers to the input and output
  InputImagePointer inputPtr = const_cast< TInputImage * > ( this->GetInput() );
  OutputImagePointer outputPtr = this->GetOutput();
  
  if( !outputPtr || !inputPtr )
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

  typename TOutputImage::SpacingType      outputSpacing;
  typename TOutputImage::SizeType         outputSize;
  typename TOutputImage::IndexType        outputStartIndex;
  
  for (i = 0; i < TOutputImage::ImageDimension; i++)
    {
    //TODO:  Verify this is being rounded correctly.
    outputSpacing[i] = inputSpacing[i] / (double) 2;
    //TODO:  Verify this is being rounded correctly.
    outputSize[i] = (unsigned int) vcl_floor((double)(inputSize[i] * 2.0) );
    outputStartIndex[i] = (int)vcl_ceil((double) inputStartIndex[i] * 2.0 );
    }

  outputPtr->SetSpacing( outputSpacing );

  typename TOutputImage::RegionType outputLargestPossibleRegion;
  outputLargestPossibleRegion.SetSize( outputSize );
  outputLargestPossibleRegion.SetIndex( outputStartIndex );

  outputPtr->SetLargestPossibleRegion( outputLargestPossibleRegion );
}

/*
 * EnlargeOutputRequestedRegion method.
 */
template <class TInputImage, class TOutputImage, class ResamplerType>
void
BSplineUpsampleImageFilter<TInputImage,TOutputImage, ResamplerType>
::EnlargeOutputRequestedRegion(
  DataObject *output )
{

  // this filter requires the all of the output image to be in
  // the buffer
  TOutputImage *imgData;
  imgData = dynamic_cast<TOutputImage*>( output );
  if ( imgData )
    {
    imgData->SetRequestedRegionToLargestPossibleRegion();
    }
  else
    {
    // pointer could not be cast to TLevelSet *
    itkWarningMacro(<< "itk::BSplineUpsampleImageFilter" <<
                    "::EnlargeOutputRequestedRegion cannot cast "
                    << typeid(output).name() << " to "
                    << typeid(TOutputImage*).name() );

    }

}


} // namespace itk

#endif
