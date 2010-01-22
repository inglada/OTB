/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkOptNoiseImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2008-08-05 12:13:44 $
  Version:   $Revision: 1.1 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkOptNoiseImageFilter_h
#define __itkOptNoiseImageFilter_h

#include "itkBoxImageFilter.h"
#include "itkImage.h"
#include "itkNumericTraits.h"

namespace itk
{
/** \class NoiseImageFilter
 * \brief Calculate the local noise in an image.
 *
 * Computes an image where a given pixel is the standard deviation of
 * the pixels in a neighborhood about the corresponding input pixel.
 * This serves as an estimate of the local noise (or texture) in an
 * image. Currently, this noise estimate assume a piecewise constant
 * image.  This filter should be extended to fitting a (hyper) plane
 * to the neighborhood and calculating the standard deviation of the
 * residuals to this (hyper) plane.
 *
 * \sa Image
 * \sa Neighborhood
 * \sa NeighborhoodOperator
 * \sa NeighborhoodIterator
 * 
 * \ingroup IntensityImageFilters
 */
template <class TInputImage, class TOutputImage>
class ITK_EXPORT NoiseImageFilter :
    public BoxImageFilter< TInputImage, TOutputImage >
{
public:
  /** Extract dimension from input and output image. */
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int,
                      TOutputImage::ImageDimension);

  /** Convenient typedefs for simplifying declarations. */
  typedef TInputImage                                      InputImageType;
  typedef TOutputImage                                     OutputImageType;

  /** Standard class typedefs. */
  typedef NoiseImageFilter                                 Self;
  typedef BoxImageFilter< InputImageType, OutputImageType> Superclass;
  typedef SmartPointer<Self>                               Pointer;
  typedef SmartPointer<const Self>                         ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(NoiseImageFilter, BoxImageFilter);
  
  /** Image typedef support. */
  typedef typename InputImageType::PixelType               InputPixelType;
  typedef typename OutputImageType::PixelType              OutputPixelType;
  typedef typename NumericTraits<InputPixelType>::RealType InputRealType;
  
  typedef typename InputImageType::RegionType              InputImageRegionType;
  typedef typename OutputImageType::RegionType             OutputImageRegionType;

  typedef typename InputImageType::SizeType                InputSizeType;

#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  itkConceptMacro(InputHasNumericTraitsCheck,
                  (Concept::HasNumericTraits<InputPixelType>));
  /** End concept checking */
#endif

protected:
  NoiseImageFilter();
  virtual ~NoiseImageFilter() {}

  /** NoiseImageFilter can be implemented as a multithreaded filter.
   * Therefore, this implementation provides a ThreadedGenerateData()
   * routine which is called for each processing thread. The output
   * image data is allocated automatically by the superclass prior to
   * calling ThreadedGenerateData().  ThreadedGenerateData can only
   * write to the portion of the output image specified by the
   * parameter "outputRegionForThread"
   *
   * \sa BoxImageFilter::ThreadedGenerateData(),
   *     BoxImageFilter::GenerateData() */
  void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                            int threadId );

private:
  NoiseImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
};
  
} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkNoiseImageFilter.txx"
#endif

#endif
