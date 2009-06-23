/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkCompose3DVectorImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2009-01-14 18:39:05 $
  Version:   $Revision: 1.7 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkCompose3DVectorImageFilter_h
#define __itkCompose3DVectorImageFilter_h

#include "itkTernaryFunctorImageFilter.h"
#include "itkVector.h"

namespace itk
{
  
namespace Function {  
  
template< class TInput >
class Compose3DVector
{
public:
  typedef Vector<TInput,3> OutputType;
  Compose3DVector() {}
  ~Compose3DVector() {}
  bool operator!=( const Compose3DVector & ) const
    {
    return false;
    }
  bool operator==( const Compose3DVector & other ) const
    {
    return !(*this != other);
    }
  inline OutputType operator()(  const TInput & s1, 
                                 const TInput & s2,
                                 const TInput & s3)
    {
    OutputType v;
    v[0] = s1;
    v[1] = s2;
    v[2] = s3;
    return v;
    }
}; 
}

/** \class Compose3DVectorImageFilter
 * \brief Implements pixel-wise composition of an 3D vector pixel from three scalar images.
 *
 * This filter receives three scalar images as input. Each image
 * containing one of the 3D vector components. The filter produces as
 * output a 3D vector image in which the three components have been
 * unified. The Component type is preserved from the PixelType of the
 * input images.
 *
 * \ingroup IntensityImageFilters
 */

template <typename TInputImage, 
          typename TOutputImage= 
          Image< Vector< ITK_TYPENAME TInputImage::PixelType,3 >,
                 ::itk::GetImageDimension<TInputImage>::ImageDimension > >
class ITK_EXPORT Compose3DVectorImageFilter :
    public
TernaryFunctorImageFilter<TInputImage,TInputImage,
                          TInputImage,TOutputImage, 
                          Function::Compose3DVector< ITK_TYPENAME TInputImage::PixelType >   >
{
public:
  /** Standard class typedefs. */
  typedef Compose3DVectorImageFilter  Self;
  typedef TernaryFunctorImageFilter<
    TInputImage,TInputImage,TInputImage,
    TOutputImage, 
    Function::Compose3DVector< 
      ITK_TYPENAME TInputImage::PixelType > >
                                      Superclass;
  typedef SmartPointer<Self>          Pointer;
  typedef SmartPointer<const Self>    ConstPointer;

  typedef typename Superclass::OutputImageType OutputImageType;
  
  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(Compose3DVectorImageFilter, 
               TernaryFunctorImageFilter);

#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  itkConceptMacro(InputHasNumericTraitsCheck,
    (Concept::HasNumericTraits<typename TInputImage::PixelType>));
  /** End concept checking */
#endif

protected:
  Compose3DVectorImageFilter() {}
  virtual ~Compose3DVectorImageFilter() {}

private:
  Compose3DVectorImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented


};

} // end namespace itk


#endif
