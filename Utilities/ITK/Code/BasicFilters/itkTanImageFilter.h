/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkTanImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2009-04-01 14:36:36 $
  Version:   $Revision: 1.23 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkTanImageFilter_h
#define __itkTanImageFilter_h

#include "itkUnaryFunctorImageFilter.h"
#include "vnl/vnl_math.h"

namespace itk
{
  
/** \class TanImageFilter
 * \brief Computes the vcl_tan(x) pixel-wise
 *
 * \ingroup IntensityImageFilters  Multithreaded
 */
namespace Function {  
  
template< class TInput, class TOutput>
class Tan
{
public:
  Tan() {}
  ~Tan() {}
  bool operator!=( const Tan & ) const
    {
    return false;
    }
  bool operator==( const Tan & other ) const
    {
    return !(*this != other);
    }
  inline TOutput operator()( const TInput & A ) const
    { return (TOutput)vcl_tan((double)A); }
}; 
}
template <class TInputImage, class TOutputImage>
class ITK_EXPORT TanImageFilter :
    public
UnaryFunctorImageFilter<TInputImage,TOutputImage, 
                        Function::Tan< typename TInputImage::PixelType, 
                                       typename TOutputImage::PixelType>   >
{
public:
  /** Standard class typedefs. */
  typedef TanImageFilter                                Self;
  typedef UnaryFunctorImageFilter<
    TInputImage,TOutputImage, 
    Function::Tan< typename TInputImage::PixelType, 
                   typename TOutputImage::PixelType> >  Superclass;
  typedef SmartPointer<Self>                            Pointer;
  typedef SmartPointer<const Self>                      ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(TanImageFilter, 
               UnaryFunctorImageFilter);

#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  itkConceptMacro(InputConvertibleToDoubleCheck,
    (Concept::Convertible<typename TInputImage::PixelType, double>));
  itkConceptMacro(DoubleConvertibleToOutputCheck,
    (Concept::Convertible<double, typename TOutputImage::PixelType>));
  /** End concept checking */
#endif

protected:
  TanImageFilter() {}
  virtual ~TanImageFilter() {}

private:
  TanImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented


};

} // end namespace itk


#endif
