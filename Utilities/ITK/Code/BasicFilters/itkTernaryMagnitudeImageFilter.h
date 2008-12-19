/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkTernaryMagnitudeImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2008-10-18 16:11:14 $
  Version:   $Revision: 1.27 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkTernaryMagnitudeImageFilter_h
#define __itkTernaryMagnitudeImageFilter_h

#include "itkTernaryFunctorImageFilter.h"

namespace itk
{
  
/** \class TernaryMagnitudeImageFilter
 * \brief Implements pixel-wise addition of three images.
 *
 * This class is parametrized over the types of the three 
 * input images and the type of the output image. 
 * Numeric conversions (castings) are done by the C++ defaults.
 *
 * \ingroup IntensityImageFilters
 */
namespace Function {  
  
template< class TInput1, class TInput2, class TInput3, class TOutput>
class Modulus3
{
public:
  Modulus3() {}
  ~Modulus3() {}
  bool operator!=( const Modulus3 & ) const
    {
    return false;
    }
  bool operator==( const Modulus3 & other ) const
    {
    return !(*this != other);
    }
  inline TOutput operator()( const TInput1 & A, 
                             const TInput2 & B,
                             const TInput3 & C)
    { return (TOutput) vcl_sqrt((double)(A*A + B*B + C*C) ); }
}; 
}

template <class TInputImage1, class TInputImage2, 
          class TInputImage3, class TOutputImage>
class ITK_EXPORT TernaryMagnitudeImageFilter :
    public
TernaryFunctorImageFilter<TInputImage1,TInputImage2,
                          TInputImage3,TOutputImage, 
                          Function::Modulus3< 
  typename TInputImage1::PixelType, 
  typename TInputImage2::PixelType,
  typename TInputImage3::PixelType,
  typename TOutputImage::PixelType>   >
{
public:
  /** Standard class typedefs. */
  typedef TernaryMagnitudeImageFilter   Self;
  typedef TernaryFunctorImageFilter<
    TInputImage1,TInputImage2,
    TInputImage3,TOutputImage, 
    Function::Modulus3< 
      typename TInputImage1::PixelType, 
      typename TInputImage2::PixelType,
      typename TInputImage3::PixelType,
      typename TOutputImage::PixelType>   
    >                                   Superclass;
  typedef SmartPointer<Self>            Pointer;
  typedef SmartPointer<const Self>      ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  
  /** Runtime information support. */
  itkTypeMacro(TernaryMagnitudeImageFilter, 
               TernaryFunctorImageFilter);

protected:
  TernaryMagnitudeImageFilter() {}
  virtual ~TernaryMagnitudeImageFilter() {}

private:
  TernaryMagnitudeImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented


};

} // end namespace itk


#endif
