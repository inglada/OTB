/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkComplexToModulusImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2009-04-01 14:36:10 $
  Version:   $Revision: 1.10 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkComplexToModulusImageFilter_h
#define __itkComplexToModulusImageFilter_h

#include "itkUnaryFunctorImageFilter.h"
#include "vnl/vnl_math.h"

namespace itk
{
  
/** \class ComplexToModulusImageFilter
 * \brief Computes pixel-wise the Modulus of a complex image.
 * 
 * \ingroup IntensityImageFilters  Multithreaded
 */
namespace Function {  
  
template< class TInput, class TOutput>
class ComplexToModulus
{
public:
  ComplexToModulus() {}
  ~ComplexToModulus() {}
  bool operator!=( const ComplexToModulus & ) const
    {
    return false;
    }
  bool operator==( const ComplexToModulus & other ) const
    {
    return !(*this != other);
    }
  inline TOutput operator()( const TInput & A ) const
    {
    return (TOutput)( vcl_sqrt(A.real() * A.real() + 
                               A.imag() * A.imag() ) );
    }
};
}

template <class TInputImage, class TOutputImage>
class ITK_EXPORT ComplexToModulusImageFilter :
    public
UnaryFunctorImageFilter<TInputImage,TOutputImage, 
                        Function::ComplexToModulus< 
  typename TInputImage::PixelType, 
  typename TOutputImage::PixelType>   >
{
public:
  /** Standard class typedefs. */
  typedef ComplexToModulusImageFilter  Self;
  typedef UnaryFunctorImageFilter<
    TInputImage,TOutputImage, 
    Function::ComplexToModulus< typename TInputImage::PixelType, 
                                typename TOutputImage::PixelType> >
                                       Superclass;
  typedef SmartPointer<Self>           Pointer;
  typedef SmartPointer<const Self>     ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(ComplexToModulusImageFilter, 
               UnaryFunctorImageFilter);

  typedef typename TInputImage::PixelType                     InputPixelType;
  typedef typename TOutputImage::PixelType                    OutputPixelType;
  typedef typename NumericTraits< InputPixelType >::ValueType InputPixelValueType;


#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  itkConceptMacro(InputMultiplyOperatorCheck,
    (Concept::MultiplyOperator<InputPixelValueType>));
  /** End concept checking */
#endif

protected:
  ComplexToModulusImageFilter() {}
  virtual ~ComplexToModulusImageFilter() {}

private:
  ComplexToModulusImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

};

} // end namespace itk


#endif
