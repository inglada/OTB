/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkSCSLRealToComplexConjugateImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2003/11/11 21:46:57 $
  Version:   $Revision: 1.1 $

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkSCSLRealToComplexConjugateImageFilter_h
#define __itkSCSLRealToComplexConjugateImageFilter_h
#include "itkFFTRealToComplexConjugateImageFilter.h"
#ifdef USE_SCSL

#include <complex.h>

#include <scsl_fft.h>

namespace itk
{
template <class TPixel, unsigned int Dimension = 3>
class SCSLRealToComplexConjugateImageFilter :
    public FFTRealToComplexConjugateImageFilter<TPixel,Dimension>
{
public:
  /** Standard class typedefs.*/ 
  typedef SCSLRealToComplexConjugateImageFilter Self;
  typedef FFTRealToComplexConjugateImageFilter<TPixel,Dimension> Superclass;
  typedef SmartPointer<Self> Pointer;
  typedef SmartPointer<const Self> constPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(SCSLRealToComplexConjugateImageFilter,
               FFTRealToComplexConjugateImageFilter);

  //
  // these should be defined in every FFT filter class
  virtual void GenerateData();  // generates output from input
  void PrintSelf(std::ostream& os,Indent indent);
  virtual bool FullMatrix(); // must be implemented in child


protected:
  SCSLRealToComplexConjugateImageFilter() 
  {
  }
  ~SCSLRealToComplexConjugateImageFilter() 
  { 
  }
  void PrintSelf(std::ostream& os, Indent indent) const;

private:
  SCSLRealToComplexConjugateImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
};

}

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkSCSLRealToComplexConjugateImageFilter.txx"
#endif
#endif // USE_SCSL
#endif
