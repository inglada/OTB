/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkDivideImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2006/03/31 14:31:04 $
  Version:   $Revision: 1.11 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkDivideImageFilter_h
#define __itkDivideImageFilter_h

#include "itkBinaryFunctorImageFilter.h"
#include "itkNumericTraits.h"

namespace itk
{
  
/** \class DivideImageFilter
 * \brief Implements an operator for pixel-wise division of two images.
 *
 * This class is parametrized over the types of the two 
 * input images and the type of the output image. When the divisor is zero,
 * the division result is set to the maximum number that can be represneted  by default to 
 * avoid exception. Numeric conversions (castings) are done by the C++ defaults.
 * 
 * \ingroup IntensityImageFilters  Multithreaded
 */

namespace Function {  
  
template< class TInput1, class TInput2, class TOutput>
class Div
{
public:
  Div() {};
  ~Div() {};
  bool operator!=( const Div & ) const
  {
    return false;
  }
  bool operator==( const Div & other ) const
  {
    return !(*this != other);
  }
  inline TOutput operator()( const TInput1 & A, const TInput2 & B)
  {
    if(B != (TInput2) 0)
      return (TOutput)(A / B);
    else
      return NumericTraits<TOutput>::max();
  }
}; 
}

template <class TInputImage1, class TInputImage2, class TOutputImage>
class ITK_EXPORT DivideImageFilter :
    public
BinaryFunctorImageFilter<TInputImage1,TInputImage2,TOutputImage, 
                         Function::Div< 
  typename TInputImage1::PixelType, 
  typename TInputImage2::PixelType,
  typename TOutputImage::PixelType>   >
{
public:
  /**
   * Standard "Self" typedef.
   */
  typedef DivideImageFilter  Self;

  /**
   * Standard "Superclass" typedef.
   */
  typedef BinaryFunctorImageFilter<TInputImage1,TInputImage2,TOutputImage, 
                                   Function::Div< 
    typename TInputImage1::PixelType, 
    typename TInputImage2::PixelType,
    typename TOutputImage::PixelType>   
  > Superclass;

  /** 
   * Smart pointer typedef support 
   */
  typedef SmartPointer<Self>   Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /**
   * Method for creation through the object factory.
   */
  itkNewMacro(Self);

#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  itkConceptMacro(IntConvertibleToInput2Check,
    (Concept::Convertible<int, typename TInputImage2::PixelType>));
  itkConceptMacro(Input1Input2OutputDivisionOperatorsCheck,
    (Concept::DivisionOperators<typename TInputImage1::PixelType,
                                typename TInputImage2::PixelType,
                                typename TOutputImage::PixelType>));
  /** End concept checking */
#endif

protected:
  DivideImageFilter() {}
  virtual ~DivideImageFilter() {}
  DivideImageFilter(const Self&) {}
  void operator=(const Self&) {}

};

} // end namespace itk


#endif
