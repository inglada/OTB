/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkTernaryMagnitudeSquaredImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2007-09-27 11:36:41 $
  Version:   $Revision: 1.20 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkTernaryMagnitudeSquaredImageFilter_h
#define __itkTernaryMagnitudeSquaredImageFilter_h

#include "itkTernaryFunctorImageFilter.h"

namespace itk
{
  
/** \class TernaryMagnitudeSquaredImageFilter
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
class ModulusSquare3
{
public:
  ModulusSquare3() {}
  ~ModulusSquare3() {}
  bool operator!=( const ModulusSquare3 & ) const
  {
    return false;
  }
  bool operator==( const ModulusSquare3 & other ) const
  {
    return !(*this != other);
  }
  inline TOutput operator()( const TInput1 & A, 
                             const TInput2 & B,
                             const TInput3 & C)
  { return (TOutput)(A*A + B*B + C*C); }
}; 
}

template <class TInputImage1, class TInputImage2, 
          class TInputImage3, class TOutputImage>
class ITK_EXPORT TernaryMagnitudeSquaredImageFilter :
    public
TernaryFunctorImageFilter<TInputImage1,TInputImage2,
                          TInputImage3,TOutputImage, 
                          Function::ModulusSquare3< 
  typename TInputImage1::PixelType, 
  typename TInputImage2::PixelType,
  typename TInputImage3::PixelType,
  typename TOutputImage::PixelType >   >
{
public:
  /** Standard class typedefs. */
  typedef TernaryMagnitudeSquaredImageFilter  Self;
  typedef TernaryFunctorImageFilter<TInputImage1,TInputImage2,
                                    TInputImage3,TOutputImage, 
                                    Function::ModulusSquare3< 
    typename TInputImage1::PixelType, 
    typename TInputImage2::PixelType,
    typename TInputImage3::PixelType,
    typename TOutputImage::PixelType >   >  Superclass;
  typedef SmartPointer<Self>   Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  
  /** Runtime information support. */
  itkTypeMacro(TernaryMagnitudeSquaredImageFilter, 
               TernaryFunctorImageFilter);

protected:
  TernaryMagnitudeSquaredImageFilter() {}
  virtual ~TernaryMagnitudeSquaredImageFilter() {}

private:
  TernaryMagnitudeSquaredImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented


};

} // end namespace itk


#endif
