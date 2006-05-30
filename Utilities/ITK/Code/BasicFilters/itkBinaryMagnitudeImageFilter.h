/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkBinaryMagnitudeImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2006/01/23 17:55:47 $
  Version:   $Revision: 1.11 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkBinaryMagnitudeImageFilter_h
#define __itkBinaryMagnitudeImageFilter_h

#include "itkBinaryFunctorImageFilter.h"

namespace itk
{
  
/** \class BinaryMagnitudeImageFilter
 * \brief Implements pixel-wise the computation of square root of the sum of squares.
 *
 * This filter is parametrized over the types of the two 
 * input images and the type of the output image. 
 *
 * Numeric conversions (castings) are done by the C++ defaults.
 *
 * The filter will walk over all the pixels in the two input images, and for
 * each one of them it will do the following: 
 *
 * - cast the input 1 pixel value to \c double 
 * - cast the input 2 pixel value to \c double 
 * - compute the sum of squares of the two pixel values
 * - compute the square root of the sum
 * - cast the \c double value resulting from \c sqrt() to the pixel type of the output image 
 * - store the casted value into the output image.
 * 
 * The filter expect all images to have the same dimension 
 * (e.g. all 2D, or all 3D, or all ND)
 * 
 * \ingroup IntensityImageFilters Multithreaded
 */
namespace Functor {  
  
template< class TInput1, class TInput2, class TOutput>
class Modulus2
{
public:
  Modulus2() {};
  ~Modulus2() {};
  bool operator!=( const Modulus2 & ) const
  {
    return false;
  }
  bool operator==( const Modulus2 & other ) const
  {
    return !(*this != other);
  }
  inline TOutput operator()( const TInput1 & A, 
                             const TInput2 & B)
  {
    const double dA = static_cast<double>( A );
    const double dB = static_cast<double>( B );
    return static_cast<TOutput>( sqrt( dA*dA + dB*dB) );
  }
}; 
}

template <class TInputImage1, class TInputImage2, class TOutputImage>
class ITK_EXPORT BinaryMagnitudeImageFilter :
    public
BinaryFunctorImageFilter<TInputImage1,TInputImage2,TOutputImage, 
                         Functor::Modulus2< 
  typename TInputImage1::PixelType, 
  typename TInputImage2::PixelType,
  typename TOutputImage::PixelType>   >
{
public:
  /** Standard class typedefs. */
  typedef BinaryMagnitudeImageFilter  Self;
  typedef BinaryFunctorImageFilter<TInputImage1,TInputImage2,TOutputImage, 
                                   Functor::Modulus2< 
    typename TInputImage1::PixelType, 
    typename TInputImage2::PixelType,
    typename TOutputImage::PixelType>   
  >  Superclass;
  typedef SmartPointer<Self>   Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  
protected:
  BinaryMagnitudeImageFilter() {}
  virtual ~BinaryMagnitudeImageFilter() {}

private:
  BinaryMagnitudeImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

};

} // end namespace itk


#endif
