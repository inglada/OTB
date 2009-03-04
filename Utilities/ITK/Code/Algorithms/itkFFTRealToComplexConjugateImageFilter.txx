/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkFFTRealToComplexConjugateImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2008-12-21 19:13:11 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkFFTRealToComplexConjugateImageFilter_txx
#define __itkFFTRealToComplexConjugateImageFilter_txx
#include "itkFFTRealToComplexConjugateImageFilter.h"
#include "itkMetaDataObject.h"

#include "itkVnlFFTRealToComplexConjugateImageFilter.h"

#if defined(USE_FFTWD) || defined(USE_FFTWF)
#include "itkFFTWRealToComplexConjugateImageFilter.h"
#endif

namespace itk
{

template < class TPixel , unsigned int VDimension >
typename FFTRealToComplexConjugateImageFilter < TPixel , VDimension >::Pointer
FFTRealToComplexConjugateImageFilter < TPixel , VDimension >
::New(void)
{
  Pointer smartPtr = ::itk::ObjectFactory<Self>::Create();

#ifdef USE_FFTWD
  if(smartPtr.IsNull())
    {
    if (typeid(TPixel) == typeid(double))
      {
      smartPtr = dynamic_cast<Self *>(
        FFTWRealToComplexConjugateImageFilter< double, VDimension >
          ::New().GetPointer() );
      }
    }
#endif
#ifdef USE_FFTWF
  if(smartPtr.IsNull())
    {
    if (typeid(TPixel) == typeid(float))
      {
      smartPtr = dynamic_cast<Self *>(
        FFTWRealToComplexConjugateImageFilter< float, VDimension >
          ::New().GetPointer());
      }
    }
#endif

  if(smartPtr.IsNull())
    {
    smartPtr = VnlFFTRealToComplexConjugateImageFilter< TPixel, VDimension >
                  ::New().GetPointer();
    }

  return smartPtr;
}

template < class TPixel , unsigned int VDimension >
void
FFTRealToComplexConjugateImageFilter < TPixel , VDimension >
::GenerateOutputInformation()
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
  // has no meaning in the result of an FFT.
  unsigned int i;
  const typename TInputImageType::SizeType&   inputSize
    = inputPtr->GetLargestPossibleRegion().GetSize();
  const typename TInputImageType::IndexType&  inputStartIndex
    = inputPtr->GetLargestPossibleRegion().GetIndex();
  
  typename TOutputImageType::SizeType     outputSize;
  typename TOutputImageType::IndexType    outputStartIndex;
  
  //
  // in 4.3.4 of the FFTW documentation, they indicate the size of
  // of a real-to-complex FFT is N * N ... + (N /2+1)
  //                              1   2        d
  // complex numbers.
  // static_cast prob. not necessary but want to make sure integer
  // division is used.
  outputSize[0] = static_cast<unsigned int>(inputSize[0])/2 + 1;
  outputStartIndex[0] = inputStartIndex[0];

  for (i = 1; i < TOutputImageType::ImageDimension; i++)
    {
    outputSize[i] = inputSize[i];
    outputStartIndex[i] = inputStartIndex[i];
    }
  //
  // the halving of the input size hides the actual size of the input.
  // to get the same size image out of the IFFT, need to send it as 
  // Metadata.
  typedef typename TOutputImageType::SizeType::SizeValueType SizeScalarType;
  itk::MetaDataDictionary &OutputDic=outputPtr->GetMetaDataDictionary();
  itk::EncapsulateMetaData<SizeScalarType>(OutputDic,
                                       std::string("FFT_Actual_RealImage_Size"),
                                                     inputSize[0]);
  typename TOutputImageType::RegionType outputLargestPossibleRegion;
  outputLargestPossibleRegion.SetSize( outputSize );
  outputLargestPossibleRegion.SetIndex( outputStartIndex );
  
  outputPtr->SetLargestPossibleRegion( outputLargestPossibleRegion );
}

template < class TPixel , unsigned int VDimension >
void
FFTRealToComplexConjugateImageFilter < TPixel , VDimension >
::GenerateInputRequestedRegion()
{
  // call the superclass' implementation of this method
  Superclass::GenerateInputRequestedRegion();
  
  // get pointers to the inputs
  typename TInputImageType::Pointer input  = 
    const_cast<TInputImageType *> (this->GetInput());
  
  if ( !input )
    {
    return;
    }
  
  input->SetRequestedRegionToLargestPossibleRegion();
}

 
template < class TPixel , unsigned int VDimension >
void
FFTRealToComplexConjugateImageFilter < TPixel , VDimension >
::EnlargeOutputRequestedRegion(DataObject *output)
{
  Superclass::EnlargeOutputRequestedRegion(output);
  output->SetRequestedRegionToLargestPossibleRegion();
}

}
#endif
