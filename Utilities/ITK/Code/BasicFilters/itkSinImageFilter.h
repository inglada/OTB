/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkSinImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2008-10-17 20:49:56 $
  Version:   $Revision: 1.22 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkSinImageFilter_h
#define __itkSinImageFilter_h

#include "itkUnaryFunctorImageFilter.h"
#include "vnl/vnl_math.h"

namespace itk
{
  
/** \class SinImageFilter
 * \brief Computes the vcl_sin(x) pixel-wise
 * 
 * \ingroup IntensityImageFilters  Multithreaded
 */
namespace Function {  
  
template< class TInput, class TOutput>
class Sin
{
public:
  Sin() {}
  ~Sin() {}
  bool operator!=( const Sin & ) const
    {
    return false;
    }
  bool operator==( const Sin & other ) const
    {
    return !(*this != other);
    }
  inline TOutput operator()( const TInput & A )
    { return (TOutput)vcl_sin((double)A); }
}; 
}

template <class TInputImage, class TOutputImage>
class ITK_EXPORT SinImageFilter :
    public
UnaryFunctorImageFilter<TInputImage,TOutputImage, 
                        Function::Sin< 
  typename TInputImage::PixelType, 
  typename TOutputImage::PixelType>   >
{
public:
  /** Standard class typedefs. */
  typedef SinImageFilter                                Self;
  typedef UnaryFunctorImageFilter<
    TInputImage,TOutputImage, 
    Function::Sin< typename TInputImage::PixelType, 
                   typename TOutputImage::PixelType> >  Superclass;
  typedef SmartPointer<Self>                            Pointer;
  typedef SmartPointer<const Self>                      ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(SinImageFilter, 
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
  SinImageFilter() {}
  virtual ~SinImageFilter() {}

private:
  SinImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

};

} // end namespace itk


#endif
