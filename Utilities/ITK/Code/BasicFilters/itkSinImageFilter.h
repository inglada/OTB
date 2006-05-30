/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkSinImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2006/01/23 17:55:48 $
  Version:   $Revision: 1.18 $

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
 * \brief Computes the sin(x) pixel-wise
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
  { return (TOutput)sin((double)A); }
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
  typedef SinImageFilter  Self;
  typedef UnaryFunctorImageFilter<TInputImage,TOutputImage, 
                                  Function::Sin< typename TInputImage::PixelType, 
                                                 typename TOutputImage::PixelType> >  Superclass;
  typedef SmartPointer<Self>   Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  
protected:
  SinImageFilter() {}
  virtual ~SinImageFilter() {}

private:
  SinImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

};

} // end namespace itk


#endif
