/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkSubtractImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2009-04-01 14:36:35 $
  Version:   $Revision: 1.29 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkSubtractImageFilter_h
#define __itkSubtractImageFilter_h

#include "itkBinaryFunctorImageFilter.h"

namespace itk
{
  
/** \class SubtractImageFilter
 * \brief Implements an operator for pixel-wise subtraction of two images.
 *
 * Output = Input1 - Input2.
 * 
 * This class is parametrized over the types of the two 
 * input images and the type of the output image. 
 * Numeric conversions (castings) are done by the C++ defaults.
 *
 * \ingroup IntensityImageFilters Multithreaded
 */
namespace Function {  
  
template< class TInput1, class TInput2=TInput1, class TOutput=TInput1>
class Sub2
{
public:
  Sub2() {}
  ~Sub2() {}
  bool operator!=( const Sub2 & ) const
    {
    return false;
    }
  bool operator==( const Sub2 & other ) const
    {
    return !(*this != other);
    }
  inline TOutput operator()( const TInput1 & A, const TInput2 & B) const
    { return (TOutput)(A - B); }
}; 
}

template <class TInputImage1, class TInputImage2=TInputImage1, class TOutputImage=TInputImage1>
class ITK_EXPORT SubtractImageFilter :
    public
BinaryFunctorImageFilter<TInputImage1,TInputImage2,TOutputImage, 
                         Function::Sub2< 
  typename TInputImage1::PixelType, 
  typename TInputImage2::PixelType,
  typename TOutputImage::PixelType>   >
{
public:
  /** Standard class typedefs. */
  typedef SubtractImageFilter                            Self;
  typedef BinaryFunctorImageFilter<
    TInputImage1,TInputImage2,TOutputImage, 
    Function::Sub2< typename TInputImage1::PixelType, 
                    typename TInputImage2::PixelType,
                    typename TOutputImage::PixelType> >  Superclass;
  typedef SmartPointer<Self>                             Pointer;
  typedef SmartPointer<const Self>                       ConstPointer;


  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(SubtractImageFilter, 
               BinaryFunctorImageFilter);

#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  itkConceptMacro(Input1Input2OutputAdditiveOperatorsCheck,
    (Concept::AdditiveOperators<typename TInputImage1::PixelType,
                                typename TInputImage2::PixelType,
                                typename TOutputImage::PixelType>));
  /** End concept checking */
#endif

protected:
  SubtractImageFilter() {}
  virtual ~SubtractImageFilter() {}

private:
  SubtractImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented


};

} // end namespace itk


#endif
