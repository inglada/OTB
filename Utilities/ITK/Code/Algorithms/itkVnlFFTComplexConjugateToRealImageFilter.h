/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkVnlFFTComplexConjugateToRealImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2009-01-27 19:30:16 $
  Version:   $Revision: 1.13 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkVnlFFTComplexConjugateToRealImageFilter_h
#define __itkVnlFFTComplexConjugateToRealImageFilter_h
#include "itkFFTComplexConjugateToRealImageFilter.h"

namespace itk
{

/** \class VnlFFTComplexConjugateToRealImageFilter
 * 
 * \brief TODO
 */
template <class TPixel, unsigned int VDimension = 3>
class VnlFFTComplexConjugateToRealImageFilter :
    public FFTComplexConjugateToRealImageFilter<TPixel,VDimension>
{
public:
  /** Standard class typedefs. */ 
  typedef Image< std::complex<TPixel>,VDimension> TInputImageType;
  typedef Image<TPixel,VDimension>                TOutputImageType;

  typedef VnlFFTComplexConjugateToRealImageFilter                 Self;
  typedef FFTComplexConjugateToRealImageFilter<TPixel,VDimension> Superclass;
  typedef SmartPointer<Self>                                      Pointer;
  typedef SmartPointer<const Self>                                ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(VnlFFTComplexConjugateToRealImageFilter,
               FFTComplexConjugateToRealImageFilter);

  /** Image type typedef support. */
  typedef TInputImageType              ImageType;
  typedef typename ImageType::SizeType ImageSizeType;

  //
  // these should be defined in every FFT filter class
  virtual void GenerateData();  // generates output from input
  virtual bool FullMatrix();

#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  itkConceptMacro(PixelUnsignedIntDivisionOperatorsCheck,
    (Concept::DivisionOperators<TPixel, unsigned int>));
  /** End concept checking */
#endif

protected:
  VnlFFTComplexConjugateToRealImageFilter()  { }
  virtual ~VnlFFTComplexConjugateToRealImageFilter(){ }

private:
  inline std::complex<TPixel> myConj(const std::complex<TPixel>& __z)
    {
    return std::complex<TPixel>(__z.real(), -__z.imag());
    }
  VnlFFTComplexConjugateToRealImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
};

}

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkVnlFFTComplexConjugateToRealImageFilter.txx"
#endif

#endif
