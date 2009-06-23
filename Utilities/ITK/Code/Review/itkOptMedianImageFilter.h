/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkOptMedianImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2008-08-05 12:30:34 $
  Version:   $Revision: 1.1 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkOptMedianImageFilter_h
#define __itkOptMedianImageFilter_h

#include "itkBoxImageFilter.h"
#include "itkImage.h"

namespace itk
{
/** \class MedianImageFilter
 * \brief Applies a median filter to an image
 *
 * Computes an image where a given pixel is the median value of the
 * the pixels in a neighborhood about the corresponding input pixel.
 *
 * A median filter is one of the family of nonlinear filters.  It is
 * used to smooth an image without being biased by outliers or shot noise.
 *
 * This filter requires that the input pixel type provides an operator<()
 * (LessThan Comparable).
 *
 * \sa Image
 * \sa Neighborhood
 * \sa NeighborhoodOperator
 * \sa NeighborhoodIterator
 * 
 * \ingroup IntensityImageFilters
 */
template <class TInputImage, class TOutputImage>
class ITK_EXPORT MedianImageFilter :
    public BoxImageFilter< TInputImage, TOutputImage >
{
public:
  /** Extract dimension from input and output image. */
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int,
                      TOutputImage::ImageDimension);

  /** Convenient typedefs for simplifying declarations. */
  typedef TInputImage                                          InputImageType;
  typedef TOutputImage                                         OutputImageType;

  /** Standard class typedefs. */
  typedef MedianImageFilter                                    Self;
  typedef ImageToImageFilter< InputImageType, OutputImageType> Superclass;
  typedef SmartPointer<Self>                                   Pointer;
  typedef SmartPointer<const Self>                             ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(MedianImageFilter, BoxImageFilter);
  
  /** Image typedef support. */
  typedef typename InputImageType::PixelType                   InputPixelType;
  typedef typename OutputImageType::PixelType                  OutputPixelType;

  typedef typename InputImageType::RegionType                  InputImageRegionType;
  typedef typename OutputImageType::RegionType                 OutputImageRegionType;

  typedef typename InputImageType::SizeType                    InputSizeType;

#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  itkConceptMacro(SameDimensionCheck,
                  (Concept::SameDimension<InputImageDimension, OutputImageDimension>));
  itkConceptMacro(InputConvertibleToOutputCheck,
                  (Concept::Convertible<InputPixelType, OutputPixelType>));
  itkConceptMacro(InputLessThanComparableCheck,
                  (Concept::LessThanComparable<InputPixelType>));
  /** End concept checking */
#endif

protected:
  MedianImageFilter();
  virtual ~MedianImageFilter() {}

  /** MedianImageFilter can be implemented as a multithreaded filter.
   * Therefore, this implementation provides a ThreadedGenerateData()
   * routine which is called for each processing thread. The output
   * image data is allocated automatically by the superclass prior to
   * calling ThreadedGenerateData().  ThreadedGenerateData can only
   * write to the portion of the output image specified by the
   * parameter "outputRegionForThread"
   *
   * \sa ImageToImageFilter::ThreadedGenerateData(),
   *     ImageToImageFilter::GenerateData() */
  void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                            int threadId );

private:
  MedianImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
};
  
} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkOptMedianImageFilter.txx"
#endif

#endif
