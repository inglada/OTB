/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkTernaryAddImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2009-04-01 14:36:36 $
  Version:   $Revision: 1.24 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkTernaryAddImageFilter_h
#define __itkTernaryAddImageFilter_h

#include "itkTernaryFunctorImageFilter.h"

namespace itk
{
  
/** \class TernaryAddImageFilter
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
class Add3
{
public:
  Add3() {}
  ~Add3() {}
  bool operator!=( const Add3 & ) const
    {
    return false;
    }
  bool operator==( const Add3 & other ) const
    {
    return !(*this != other);
    }
  inline TOutput operator()( const TInput1 & A, 
                             const TInput2 & B,
                             const TInput3 & C) const
    { return (TOutput)(A + B + C); }
}; 
}

template <class TInputImage1, class TInputImage2, 
          class TInputImage3, class TOutputImage>
class ITK_EXPORT TernaryAddImageFilter :
    public
TernaryFunctorImageFilter<TInputImage1,TInputImage2,
                          TInputImage3,TOutputImage, 
                          Function::Add3< typename TInputImage1::PixelType, 
                                          typename TInputImage2::PixelType,
                                          typename TInputImage3::PixelType,
                                          typename TOutputImage::PixelType>   >
{
public:
  /** Standard class typedefs. */
  typedef TernaryAddImageFilter                            Self;
  typedef TernaryFunctorImageFilter<
    TInputImage1,TInputImage2,
    TInputImage3,TOutputImage, 
    Function::Add3< typename TInputImage1::PixelType,
                    typename TInputImage2::PixelType,
                    typename TInputImage3::PixelType,
                    typename TOutputImage::PixelType>   >  Superclass;
  typedef SmartPointer<Self>                               Pointer;
  typedef SmartPointer<const Self>                         ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  
  /** Runtime information support. */
  itkTypeMacro(TernaryAddImageFilter, 
               TernaryFunctorImageFilter);

protected:
  TernaryAddImageFilter() {}
  virtual ~TernaryAddImageFilter() {}

private:
  TernaryAddImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

};

} // end namespace itk


#endif
