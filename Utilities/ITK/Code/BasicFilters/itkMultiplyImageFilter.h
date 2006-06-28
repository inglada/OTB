/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkMultiplyImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2006/03/31 14:31:04 $
  Version:   $Revision: 1.23 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkMultiplyImageFilter_h
#define __itkMultiplyImageFilter_h

#include "itkBinaryFunctorImageFilter.h"

namespace itk
{
  
/** \class MultiplyImageFilter
 * \brief Implements an operator for pixel-wise multiplication of two images.
 *
 * This class is parametrized over the types of the two 
 * input images and the type of the output image. 
 * Numeric conversions (castings) are done by the C++ defaults.
 * 
 * \ingroup IntensityImageFilters  Multithreaded
 */
namespace Function {  

template< class TInput1, class TInput2, class TOutput>
class Mult
{
public:
  Mult() {}
  ~Mult() {}
  bool operator!=( const Mult & ) const
  {
    return false;
  }
  bool operator==( const Mult & other ) const
  {
    return !(*this != other);
  }
  inline TOutput operator()( const TInput1 & A, const TInput2 & B)
  { return (TOutput)(A * B); }
}; 
}

template <class TInputImage1, class TInputImage2, class TOutputImage>
class ITK_EXPORT MultiplyImageFilter :
    public
BinaryFunctorImageFilter<TInputImage1,TInputImage2,TOutputImage, 
                         Function::Mult< 
  typename TInputImage1::PixelType, 
  typename TInputImage2::PixelType,
  typename TOutputImage::PixelType>   >
{
public:
  /** Standard class typedefs. */
  typedef MultiplyImageFilter  Self;
  typedef BinaryFunctorImageFilter<TInputImage1,TInputImage2,TOutputImage, 
                                   Function::Mult< 
    typename TInputImage1::PixelType, 
    typename TInputImage2::PixelType,
    typename TOutputImage::PixelType>   
  > Superclass;
  typedef SmartPointer<Self>   Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  itkConceptMacro(Input1Input2OutputMultiplyOperatorCheck,
    (Concept::MultiplyOperator<typename TInputImage1::PixelType,
                               typename TInputImage2::PixelType,
                               typename TOutputImage::PixelType>));
  /** End concept checking */
#endif

protected:
  MultiplyImageFilter() {}
  virtual ~MultiplyImageFilter() {}

private:
  MultiplyImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

};

} // end namespace itk


#endif
