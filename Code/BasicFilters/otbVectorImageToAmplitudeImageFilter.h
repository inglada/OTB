/*=========================================================================

Program:   ORFEO Toolbox
Language:  C++
Date:      $Date$
Version:   $Revision$


Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
See OTBCopyright.txt for details.


This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbVectorImageToAmplitudeImageFilter_h
#define __otbVectorImageToAmplitudeImageFilter_h

#include "itkUnaryFunctorImageFilter.h"
#include "otbMath.h"

namespace otb
{
namespace Functor
{
template < class TInput, class TOutput>
class VectorToAmplitudeFunctor
{
public:
  VectorToAmplitudeFunctor() {};
  virtual ~VectorToAmplitudeFunctor() {};

  inline TOutput operator()(const TInput & A)
  {
    return static_cast<TOutput>(vcl_sqrt(A.GetSquaredNorm()));
  }
}; // end namespace Functor
}


/** \class VectorImageToAmplitudeImageFilter
 *  \brief This filter returns the the amplitude of the pixel vector.
 *
 * \ingroup Streamed
 * \ingroup Threaded
 */
template <class TInputImage, class TOutputImage>
class ITK_EXPORT VectorImageToAmplitudeImageFilter
      : public itk::UnaryFunctorImageFilter<TInputImage,TOutputImage,Functor::VectorToAmplitudeFunctor<
      typename TInputImage::PixelType, typename TOutputImage::PixelType> >
{
public:
  /** Standard typedefs */
  typedef VectorImageToAmplitudeImageFilter            Self;
  typedef itk::UnaryFunctorImageFilter<TInputImage,TOutputImage,Functor::VectorToAmplitudeFunctor<
  typename TInputImage::PixelType, typename TOutputImage::PixelType> > Superclass;
  typedef itk::SmartPointer<Self>           Pointer;
  typedef itk::SmartPointer<const Self>     ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(VectorImageToAmplitudeImageFilter, UnaryFunctorImageFilter);

protected:
  /** Constructor */
  VectorImageToAmplitudeImageFilter() {};
  /** Destructor */
  virtual ~VectorImageToAmplitudeImageFilter() {};
  /**PrintSelf method */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    Superclass::PrintSelf(os,indent);
  };

private:
  VectorImageToAmplitudeImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
};
}// End namespace otb
#endif
