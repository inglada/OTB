/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkGradientToMagnitudeImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2006/03/28 19:59:04 $
  Version:   $Revision: 1.20 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkGradientToMagnitudeImageFilter_h
#define __itkGradientToMagnitudeImageFilter_h

#include "itkUnaryFunctorImageFilter.h"

namespace itk
{
  
/** \class GradientToMagnitudeImageFilter
 *
 * \brief Take an image of vectors as input and produce an image with the
 *  magnitude of those vectors.
 *
 * The filter expects the input image pixel type to be a vector and 
 * the output image pixel type to be a scalar.
 *
 * This filter assumes that the PixelType of the input image is a VectorType
 * that provides a GetNorm() method.
 *
 * \ingroup IntensityImageFilters  Multithreaded
 */

namespace Functor {  
  
template< class TInput, class TOutput>
class GradientMagnitude
{
public:
  GradientMagnitude() {}
  ~GradientMagnitude() {}

  bool operator!=( const GradientMagnitude & ) const
  {
    return false;
  }
  bool operator==( const GradientMagnitude & other ) const
  {
    return !(*this != other);
  }
  inline TOutput operator()( const TInput & A )
  {
    return static_cast<TOutput>( A.GetNorm() );
  }
}; 
}
 
template <class TInputImage, class TOutputImage>
class ITK_EXPORT GradientToMagnitudeImageFilter :
    public
UnaryFunctorImageFilter<TInputImage,TOutputImage, 
                        Functor::GradientMagnitude< typename TInputImage::PixelType, 
                                                    typename TOutputImage::PixelType>   >
{
public:
  /** Standard class typedefs. */
  typedef GradientToMagnitudeImageFilter Self;
  typedef UnaryFunctorImageFilter<TInputImage,TOutputImage, 
                                  Functor::GradientMagnitude< typename TInputImage::PixelType, 
                                                              typename TOutputImage::PixelType> > Superclass;
  typedef SmartPointer<Self> Pointer;
  typedef SmartPointer<const Self> ConstPointer;
    
  /** Method for creation through the object factory. */
  itkNewMacro(Self);

#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  itkConceptMacro(InputHasNumericTraitsCheck,
    (Concept::HasNumericTraits<typename TInputImage::PixelType::ValueType>));
  /** End concept checking */
#endif

protected:
  GradientToMagnitudeImageFilter() {}
  virtual ~GradientToMagnitudeImageFilter() {}
    
private:
  GradientToMagnitudeImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
};
 
} // end namespace itk


#endif
