/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkMinimumImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2007/08/30 15:01:09 $
  Version:   $Revision: 1.9 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkMinimumImageFilter_h
#define __itkMinimumImageFilter_h

#include "itkBinaryFunctorImageFilter.h"

namespace itk
{
  
/** \class MinimumImageFilter
 * \brief Implements a pixel-wise operator Min(a,b) between two images.
 *
 * The pixel values of the output image are the minimum between the 
 * corresponding pixels of the two input images.
 *
 * This class is parametrized over the types of the two 
 * input images and the type of the output image. 
 * Numeric conversions (castings) are done by the C++ defaults.
+ * 
 * \ingroup IntensityImageFilters  Multithreaded
 */
namespace Function {  

template< class TInput1, class TInput2=TInput1, class TOutput=TInput1>
class Minimum
{
public:
  Minimum() {}
  ~Minimum() {}
  bool operator!=( const Minimum & ) const
  {
    return false;
  }
  bool operator==( const Minimum & other ) const
  {
    return !(*this != other);
  }
  inline TOutput operator()( const TInput1 & A, const TInput2 & B)
  { return static_cast<TOutput>( (A < B)? A : B ); }
}; 
}

template <class TInputImage1, class TInputImage2=TInputImage1, class TOutputImage=TInputImage1>
class ITK_EXPORT MinimumImageFilter :
    public
BinaryFunctorImageFilter<TInputImage1,TInputImage2,TOutputImage, 
                         Function::Minimum< 
  typename TInputImage1::PixelType, 
  typename TInputImage2::PixelType,
  typename TOutputImage::PixelType>   >
{
public:
  /** Standard class typedefs. */
  typedef MinimumImageFilter  Self;
  typedef BinaryFunctorImageFilter<TInputImage1,TInputImage2,TOutputImage, 
                                   Function::Minimum< 
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
  itkConceptMacro(Input1ConvertibleToInput2Check,
    (Concept::Convertible<typename TInputImage1::PixelType,
                          typename TInputImage2::PixelType>));
  itkConceptMacro(Input2ConvertibleToOutputCheck,
    (Concept::Convertible<typename TInputImage2::PixelType,
                          typename TOutputImage::PixelType>));
  itkConceptMacro(Input1LessThanInput2Check,
    (Concept::LessThanComparable<typename TInputImage1::PixelType,
                                 typename TInputImage2::PixelType>));
  /** End concept checking */
#endif

protected:
  MinimumImageFilter() {}
  virtual ~MinimumImageFilter() {}

private:
  MinimumImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

};

} // end namespace itk


#endif
