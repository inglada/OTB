/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkSquareImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2006/01/23 17:55:48 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkSquareImageFilter_h
#define __itkSquareImageFilter_h

#include "itkUnaryFunctorImageFilter.h"

namespace itk
{
  
/** \class SquareImageFilter
 * \brief Computes the square of the intensity values pixel-wise
 *
 * \ingroup IntensityImageFilters  Multithreaded
 */

namespace Function {  
  
template< class TInput, class TOutput>
class Square
{
public:
  typedef typename NumericTraits<TInput>::RealType RealType;
  Square() {}
  ~Square() {}
  bool operator!=( const Square & ) const
  {
    return false;
  }
  bool operator==( const Square & other ) const
  {
    return !(*this != other);
  }
  inline TOutput operator()( const TInput & A )
  {
    const RealType ra = static_cast<RealType>( A );
    return static_cast<TOutput>( ra * ra );
  }
}; 
}
template <class TInputImage, class TOutputImage>
class ITK_EXPORT SquareImageFilter :
    public
UnaryFunctorImageFilter<TInputImage,TOutputImage, 
                        Function::Square< typename TInputImage::PixelType, 
                                          typename TOutputImage::PixelType>   >
{
public:
  /** Standard class typedefs. */
  typedef SquareImageFilter  Self;
  typedef UnaryFunctorImageFilter<TInputImage,TOutputImage, 
                                  Function::Square< typename TInputImage::PixelType, 
                                                    typename TOutputImage::PixelType> >  Superclass;
  typedef SmartPointer<Self>   Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  
protected:
  SquareImageFilter() {}
  virtual ~SquareImageFilter() {}

private:
  SquareImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

};

} // end namespace itk


#endif
