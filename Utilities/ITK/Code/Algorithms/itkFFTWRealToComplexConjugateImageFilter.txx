/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkFFTWRealToComplexConjugateImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2010-02-26 23:50:55 $
  Version:   $Revision: 1.13 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkFFTWRealToComplexConjugateImageFilter_txx
#define __itkFFTWRealToComplexConjugateImageFilter_txx

#include "itkFFTWRealToComplexConjugateImageFilter.h"
#include "itkFFTRealToComplexConjugateImageFilter.txx"
#include <iostream>
#include "itkIndent.h"
#include "itkMetaDataObject.h"
#include "itkProgressReporter.h"

namespace itk
{
/** TODO:  There should be compile time type checks so that
           if only USE_FFTWF is defined, then only floats are valid.
           and if USE_FFTWD is defined, then only doubles are valid.
*/

template <typename TPixel, unsigned int VDimension>
void
FFTWRealToComplexConjugateImageFilter<TPixel,VDimension>::
GenerateData()
{
  // get pointers to the input and output
  typename TInputImageType::ConstPointer  inputPtr  = this->GetInput();
  typename TOutputImageType::Pointer      outputPtr = this->GetOutput();

  if ( !inputPtr || !outputPtr )
    {
    return;
    }

  // we don't have a nice progress to report, but at least this simple line
  // reports the begining and the end of the process
  ProgressReporter progress(this, 0, 1);

  // allocate output buffer memory
  outputPtr->SetBufferedRegion( outputPtr->GetRequestedRegion() );
  outputPtr->Allocate();

  const typename TInputImageType::SizeType&   inputSize
    = inputPtr->GetLargestPossibleRegion().GetSize();
  const typename TOutputImageType::SizeType&   outputSize
    = outputPtr->GetLargestPossibleRegion().GetSize();

  // figure out sizes
  // size of input and output aren't the same which is handled in the superclass,
  // sort of.
  // the input size and output size only differ in the fastest moving dimension
  unsigned int total_inputSize = 1;
  unsigned int total_outputSize = 1;

  for(unsigned i = 0; i < VDimension; i++)
    {
    total_inputSize *= inputSize[i];
    total_outputSize *= outputSize[i];
    }

  if(this->m_PlanComputed)            // if we've already computed a plan
    {
    // if the image sizes aren't the same,
    // we have to compute the plan again
    if(this->m_LastImageSize != total_inputSize)
      {
      delete [] this->m_InputBuffer;
      delete [] this->m_OutputBuffer;
      FFTWProxyType::DestroyPlan(this->m_Plan);
      this->m_PlanComputed = false;
      }
    }
  if(!this->m_PlanComputed)
    {
    this->m_InputBuffer = new TPixel[total_inputSize];
    this->m_OutputBuffer = 
      new typename FFTWProxyType::ComplexType[total_outputSize];
    this->m_LastImageSize = total_inputSize;
    switch(VDimension)
      {
      case 1:
        this->m_Plan = FFTWProxyType::Plan_dft_r2c_1d(inputSize[0],
                                             this->m_InputBuffer,
                                             this->m_OutputBuffer,
                                             FFTW_ESTIMATE);
        break;
      case 2:
        this->m_Plan = FFTWProxyType::Plan_dft_r2c_2d(inputSize[1],
                                             inputSize[0],
                                             this->m_InputBuffer,
                                             this->m_OutputBuffer,
                                             FFTW_ESTIMATE);
        break;
      case 3:
        this->m_Plan = FFTWProxyType::Plan_dft_r2c_3d(inputSize[2],
                                             inputSize[1],
                                             inputSize[0],
                                             this->m_InputBuffer,
                                             this->m_OutputBuffer,
                                             FFTW_ESTIMATE);
        break;
      default:
        int *sizes = new int[VDimension];
        for(unsigned int i = 0; i < VDimension; i++)
          {
          sizes[(VDimension - 1) - i] = inputSize[i];
          }

        this->m_Plan = FFTWProxyType::Plan_dft_r2c(VDimension,sizes,
                                          this->m_InputBuffer,
                                          this->m_OutputBuffer,
                                          FFTW_ESTIMATE);
        delete [] sizes;
      }
    this->m_PlanComputed = true;
    }
  memcpy(this->m_InputBuffer,
         inputPtr->GetBufferPointer(),
         total_inputSize * sizeof(TPixel));
  FFTWProxyType::Execute(this->m_Plan);
  memcpy(outputPtr->GetBufferPointer(),
         this->m_OutputBuffer,
         total_outputSize * sizeof(typename FFTWProxyType::ComplexType));
}

template <typename TPixel,unsigned int VDimension>
bool
FFTWRealToComplexConjugateImageFilter<TPixel,VDimension>::
FullMatrix()
{
  return false;
}

} // namespace itk

#endif //_itkFFTWRealToComplexConjugateImageFilter_txx
