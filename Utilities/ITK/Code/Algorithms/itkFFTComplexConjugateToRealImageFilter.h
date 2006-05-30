/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkFFTComplexConjugateToRealImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2004/11/01 16:29:56 $
  Version:   $Revision: 1.3 $

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkFFTComplexConjugateToRealImageFilter_h
#define __itkFFTComplexConjugateToRealImageFilter_h


#include <itkImageToImageFilter.h>
#include <itkImage.h>
#include <complex>

namespace itk
{
/** /class FFTComplexConjugateToRealImageFilter
 * /brief 
 *
 * \ingroup 
 */

template < class TPixel,unsigned int Dimension = 3 >
class FFTComplexConjugateToRealImageFilter :
    public ImageToImageFilter< Image< std::complex< TPixel > , Dimension >,
                               Image< TPixel,Dimension > >

{
public:
  /** Standard class typedefs.*/ 
  typedef Image< std::complex< TPixel > ,Dimension> TInputImageType;
  typedef Image<TPixel,Dimension> TOutputImageType;

  typedef FFTComplexConjugateToRealImageFilter Self;
  typedef ImageToImageFilter< TInputImageType, TOutputImageType > Superclass;
  typedef SmartPointer<Self> Pointer;
  typedef SmartPointer<const Self> constPointer;

  itkStaticConstMacro(ImageDimension, unsigned int,
                      TInputImageType::ImageDimension );

  /** Run-time type information (and related methods). */
  itkTypeMacro(FFTComplexConjugateToRealImageFilter, ImageToImageFilter);

  /** Image type typedef support. */
  typedef TInputImageType ImageType;
  typedef typename ImageType::SizeType ImageSizeType;
  virtual void GenerateOutputInformation(); // figure out allocation for output image
  virtual void GenerateInputRequestedRegion();
  virtual bool FullMatrix() = 0; // must be implemented in child  

protected:
  FFTComplexConjugateToRealImageFilter() {}
  virtual ~FFTComplexConjugateToRealImageFilter(){}

private:
  FFTComplexConjugateToRealImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
};
} // end namespace itk
#ifndef ITK_MANUAL_INSTANTIATION
#include "itkFFTComplexConjugateToRealImageFilter.txx"
#endif


#endif
