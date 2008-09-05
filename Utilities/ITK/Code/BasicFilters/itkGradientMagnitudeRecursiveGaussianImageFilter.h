/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkGradientMagnitudeRecursiveGaussianImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2007-09-27 11:36:40 $
  Version:   $Revision: 1.16 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkGradientMagnitudeRecursiveGaussianImageFilter_h
#define __itkGradientMagnitudeRecursiveGaussianImageFilter_h

#include "itkNthElementImageAdaptor.h"
#include "itkImage.h"
#include "itkPixelTraits.h"
#include "itkRecursiveGaussianImageFilter.h"
#include "itkInPlaceImageFilter.h"

namespace itk
{

/** \class GradientMagnitudeRecursiveGaussianImageFilter
 * \brief Computes the Magnitude of the Gradient of an image by convolution
 *        with the first derivative of a Gaussian.
 * 
 * This filter is implemented using the recursive gaussian
 * filters
 *
 * 
 * \ingroup GradientFilters   
 * \ingroup Singlethreaded
 */
// NOTE that the ITK_TYPENAME macro has to be used here in lieu 
// of "typename" because VC++ doesn't like the typename keyword 
// on the defaults of template parameters
template <typename TInputImage, 
          typename TOutputImage= TInputImage >
class ITK_EXPORT GradientMagnitudeRecursiveGaussianImageFilter:
    public InPlaceImageFilter<TInputImage,TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef GradientMagnitudeRecursiveGaussianImageFilter  Self;
  typedef InPlaceImageFilter<TInputImage,TOutputImage> Superclass;
  typedef SmartPointer<Self>                   Pointer;
  typedef SmartPointer<const Self>        ConstPointer;
  
  
  /** Pixel Type of the input image */
  typedef TInputImage                                    InputImageType;
  typedef typename InputImageType::PixelType             PixelType;


  /** Image dimension. */
  itkStaticConstMacro(ImageDimension, unsigned int,
                      TInputImage::ImageDimension);

  typedef typename NumericTraits<PixelType>::RealType      RealType;

  /** Define the image type for internal computations 
      RealType is usually 'double' in NumericTraits. 
      Here we prefer float in order to save memory.  */

  typedef float                                            InternalRealType;
  typedef Image<InternalRealType, 
                itkGetStaticConstMacro(ImageDimension) >   RealImageType;

  /**  Smoothing filter type */
  typedef RecursiveGaussianImageFilter<
    RealImageType,
    RealImageType
    >    GaussianFilterType;

  /**  Derivative filter type, it will be the first in the pipeline  */
  typedef RecursiveGaussianImageFilter<
    InputImageType,
    RealImageType
    >    DerivativeFilterType;

  /**  Pointer to a gaussian filter.  */
  typedef typename GaussianFilterType::Pointer     GaussianFilterPointer;

  /**  Pointer to a derivative filter.  */
  typedef typename DerivativeFilterType::Pointer   DerivativeFilterPointer;

  /**  Pointer to the Output Image */
  typedef typename TOutputImage::Pointer           OutputImagePointer;

  /** Type of the output Image */
  typedef TOutputImage      OutputImageType;
  typedef typename          OutputImageType::PixelType      OutputPixelType;

  /**  Auxiliary image for holding the values of the squared gradient components */
  typedef Image< InternalRealType, 
                 itkGetStaticConstMacro(ImageDimension) >      CumulativeImageType;
  typedef typename CumulativeImageType::Pointer                CumulativeImagePointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(GradientMagnitudeRecursiveGaussianImageFilter, 
               InPlaceImageFilter);

  /** Set Sigma value. Sigma is measured in the units of image spacing.  */
  void SetSigma( RealType sigma );

  /** Define which normalization factor will be used for the Gaussian */
  void SetNormalizeAcrossScale( bool normalizeInScaleSpace );
  itkGetMacro( NormalizeAcrossScale, bool );

#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  itkConceptMacro(InputHasNumericTraitsCheck,
                  (Concept::HasNumericTraits<PixelType>));
  /** End concept checking */
#endif

protected:
  GradientMagnitudeRecursiveGaussianImageFilter();
  virtual ~GradientMagnitudeRecursiveGaussianImageFilter() {};
  void PrintSelf(std::ostream& os, Indent indent) const;
  
  /** Generate Data */
  void GenerateData( void );

  /** GradientMagnitudeRecursiveGaussianImageFilter needs all of the
   * input to produce an output. Therefore,
   * GradientMagnitudeRecursiveGaussianImageFilter needs to provide an
   * implementation for GenerateInputRequestedRegion in order to
   * inform the pipeline execution model.  \sa
   * ImageToImageFilter::GenerateInputRequestedRegion() */
  virtual void GenerateInputRequestedRegion() throw(InvalidRequestedRegionError);

  /** GradientMagnitudeRecursiveGaussianImageFilter produces all of
   * the output.  Therefore, it needs to provide an implementation of
   * EnlargeOutputRequestedRegion(). */
  void EnlargeOutputRequestedRegion(DataObject *output);

private:
  GradientMagnitudeRecursiveGaussianImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
  
  GaussianFilterPointer         m_SmoothingFilters[ImageDimension-1];
  DerivativeFilterPointer       m_DerivativeFilter;

  /** Normalize the image across scale space */
  bool m_NormalizeAcrossScale; 


};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkGradientMagnitudeRecursiveGaussianImageFilter.txx"
#endif

#endif




