/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkFFTComplexConjugateToRealImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2003/11/10 18:51:16 $
  Version:   $Revision: 1.3 $

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.


=========================================================================*/
#ifndef __itkFFTComplexConjugateToRealImageFilter_txx
#define __itkFFTComplexConjugateToRealImageFilter_txx
#include "itkFFTComplexConjugateToRealImageFilter.h"
#include "itkMetaDataDictionary.h"
#include "itkMetaDataObject.h"

namespace itk
{
template <class TPixel, unsigned int Dimension>
void
FFTComplexConjugateToRealImageFilter<TPixel,Dimension>::
GenerateOutputInformation()
{
  // call the superclass' implementation of this method
  Superclass::GenerateOutputInformation();
   //
  // If this implementation returns a full result
  // instead of a 'half-complex' matrix, then none of this
  // is necessary
  if(this->FullMatrix())
    return;
 
  // get pointers to the input and output
  typename TInputImageType::ConstPointer  inputPtr  = this->GetInput();
  typename TOutputImageType::Pointer      outputPtr = this->GetOutput();

  if ( !inputPtr || !outputPtr )
    {
    return;
    }
  
  // 
  // This is all based on the same function in itk::ShrinkImageFilter
  // ShrinkImageFilter also modifies the image spacing, but spacing
  // has no meaning in the result of an FFT. For an IFFT, since the
  // spacing is propagated to the complex result, we can use the spacing
  // from the input to propagate back to the output.
  unsigned int i;
  const typename TInputImageType::SizeType&   inputSize
    = inputPtr->GetLargestPossibleRegion().GetSize();
  const typename TInputImageType::IndexType&  inputStartIndex
    = inputPtr->GetLargestPossibleRegion().GetIndex();
  
  typename TOutputImageType::SizeType     outputSize;
  typename TOutputImageType::IndexType    outputStartIndex;
  
  //
  // in 4.3.4 of the FFT documentation, they indicate the size of
  // of a real-to-complex FFT is N * N ... + (N /2+1)
  //                              1   2        d
  // complex numbers.
  // going from complex to real, you know the output is at least
  // twice the size in the last dimension as the input, but it might
  // be 2*size+1.  Consequently, the output of the FFT:R2C operation
  // 
  MetaDataDictionary &InputDic = 
    const_cast<MetaDataDictionary &>(inputPtr->GetMetaDataDictionary());

  typedef typename TInputImageType::SizeType::SizeValueType SizeScalarType;

  SizeScalarType x;
  if(!ExposeMetaData < SizeScalarType > (InputDic,
                                          std::string("FFT_Actual_RealImage_Size"),x))
    {
    outputSize[0] = (static_cast<unsigned int>(inputSize[0]) - 1) * 2;
    }
  else
    outputSize[0] = x;
  outputStartIndex[0] = inputStartIndex[0];

  for (i = 1; i < TOutputImageType::ImageDimension; i++)
    {
    outputSize[i] = inputSize[i];
    outputStartIndex[i] = inputStartIndex[i];
    }
  typename TOutputImageType::RegionType outputLargestPossibleRegion;
  outputLargestPossibleRegion.SetSize( outputSize );
  outputLargestPossibleRegion.SetIndex( outputStartIndex );
  
  outputPtr->SetLargestPossibleRegion( outputLargestPossibleRegion );
}

template <class TPixel, unsigned int Dimension>
void
FFTComplexConjugateToRealImageFilter<TPixel,Dimension>::
GenerateInputRequestedRegion()
{
  Superclass::GenerateInputRequestedRegion();
  // get pointers to the input and output
  typename TInputImageType::Pointer  inputPtr  = 
    const_cast<TInputImageType *>(this->GetInput());
  inputPtr->SetRequestedRegionToLargestPossibleRegion();
}

}
#endif
