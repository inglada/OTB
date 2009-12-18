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
#ifndef __otbCorrelationChangeDetector_h
#define __otbCorrelationChangeDetector_h

#include "otbBinaryFunctorNeighborhoodImageFilter.h"
#include "otbCrossCorrelation.h"

namespace otb
{

/** \class CorrelationChangeDetector
 * \brief Implements neighborhood-wise the computation of crosscorrelation.
 *
 * This filter is parametrized over the types of the two
 * input images and the type of the output image.
 *
 * Numeric conversions (castings) are done by the C++ defaults.
 *
 * The filter will walk over all the pixels in the two input images, and for
 * each one of them it will do the following:
 *
 * - cast the input 1 pixel value to \c double
 * - cast the input 2 pixel value to \c double
 * - compute the difference of the two pixel values
 * - compute the value of the cross-correlation
 * - cast the \c double value resulting to the pixel type of the output image
 * - store the casted value into the output image.
 *
 * The filter expect all images to have the same dimension
 * (e.g. all 2D, or all 3D, or all ND)
 *
 * \ingroup IntensityImageFilters Multithreaded
 */

template <class TInputImage1, class TInputImage2, class TOutputImage>
class ITK_EXPORT CorrelationChangeDetector :
      public BinaryFunctorNeighborhoodImageFilter<
      TInputImage1,TInputImage2,TOutputImage,
      Functor::CrossCorrelation<
      ITK_TYPENAME itk::ConstNeighborhoodIterator<TInputImage1>,
      ITK_TYPENAME itk::ConstNeighborhoodIterator<TInputImage2>,
      ITK_TYPENAME TOutputImage::PixelType>   >
{
public:
  /** Standard class typedefs. */
  typedef CorrelationChangeDetector  Self;
  typedef BinaryFunctorNeighborhoodImageFilter<
  TInputImage1,TInputImage2,TOutputImage,
  Functor::CrossCorrelation<
  ITK_TYPENAME itk::ConstNeighborhoodIterator<TInputImage1>,
  ITK_TYPENAME itk::ConstNeighborhoodIterator<TInputImage2>,
  ITK_TYPENAME TOutputImage::PixelType>
  >  Superclass;
  typedef itk::SmartPointer<Self>   Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

protected:
  CorrelationChangeDetector() {}
  virtual ~CorrelationChangeDetector() {}

private:
  CorrelationChangeDetector(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

};

} // end namespace otb


#endif
