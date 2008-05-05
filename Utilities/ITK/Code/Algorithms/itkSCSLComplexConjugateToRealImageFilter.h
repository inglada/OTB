/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkSCSLComplexConjugateToRealImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2003-11-11 21:46:57 $
  Version:   $Revision: 1.1 $

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkSCSLComplexConjugateToRealImageFilter_h
#define __itkSCSLComplexConjugateToRealImageFilter_h
#include "itkFFTComplexConjugateToRealImageFilter.h"
#ifdef USE_SCSL
#include <complex.h>
#include <scsl_fft.h>

namespace itk
{

template <typename TPixel, unsigned int Dimension = 3>
class SCSLComplexConjugateToRealImageFilter :
    public FFTComplexConjugateToRealImageFilter<TPixel,Dimension>
{
public:
  /** Standard class typedefs.*/ 
  typedef Image< std::complex<TPixel>,Dimension> TInputImageType;
  typedef Image<TPixel,Dimension> TOutputImageType;

  typedef SCSLComplexConjugateToRealImageFilter Self;
  typedef FFTComplexConjugateToRealImageFilter<TPixel,Dimension> Superclass;
  typedef SmartPointer<Self> Pointer;
  typedef SmartPointer<const Self> constPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(SCSLComplexConjugateToRealImageFilter,
               FFTComplexConjugateToRealImageFilter);

  /** Image type typedef support. */
  typedef TInputImageType ImageType;
  typedef typename ImageType::SizeType ImageSizeType;

  //
  // these should be defined in every FFT filter class
  virtual void GenerateData();  // generates output from input
  virtual bool FullMatrix(); // must be implemented in child
protected:
  SCSLComplexConjugateToRealImageFilter()  
  { 
  }
  virtual ~SCSLComplexConjugateToRealImageFilter()
  {
  }
  void PrintSelf(std::ostream& os, Indent indent) const;

private:
  SCSLComplexConjugateToRealImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
};

}

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkSCSLComplexConjugateToRealImageFilter.txx"
#endif
#endif // USE_SCSL
#endif
