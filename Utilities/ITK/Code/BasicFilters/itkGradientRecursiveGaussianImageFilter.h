/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkGradientRecursiveGaussianImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2005/06/19 16:41:17 $
  Version:   $Revision: 1.34 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkGradientRecursiveGaussianImageFilter_h
#define __itkGradientRecursiveGaussianImageFilter_h

#include "itkRecursiveGaussianImageFilter.h"
#include "itkNthElementImageAdaptor.h"
#include "itkImage.h"
#include "itkCovariantVector.h"
#include "itkPixelTraits.h"
#include "itkProgressAccumulator.h"


namespace itk
{

/** \class GradientRecursiveGaussianImageFilter
 * \brief Computes the gradient of an image by convolution
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
          typename TOutputImage= Image< CovariantVector< 
  ITK_TYPENAME NumericTraits< ITK_TYPENAME TInputImage::PixelType>::RealType,
  ::itk::GetImageDimension<TInputImage>::ImageDimension >,
                                        ::itk::GetImageDimension<TInputImage>::ImageDimension > >
class ITK_EXPORT GradientRecursiveGaussianImageFilter:
    public ImageToImageFilter<TInputImage,TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef GradientRecursiveGaussianImageFilter  Self;
  typedef ImageToImageFilter<TInputImage,TOutputImage> Superclass;
  typedef SmartPointer<Self>                   Pointer;
  typedef SmartPointer<const Self>        ConstPointer;
  
  
  /** Pixel Type of the input image */
  typedef TInputImage                                    InputImageType;
  typedef typename TInputImage::PixelType                PixelType;
  typedef typename NumericTraits<PixelType>::RealType    RealType;


  /** Image dimension. */
  itkStaticConstMacro(ImageDimension, unsigned int,
                      TInputImage::ImageDimension);

  /** Define the image type for internal computations 
      RealType is usually 'double' in NumericTraits. 
      Here we prefer float in order to save memory.  */

  typedef float                                            InternalRealType;
  typedef Image<InternalRealType, 
                itkGetStaticConstMacro(ImageDimension) >   RealImageType;




  /**  Output Image Nth Element Adaptor
   *  This adaptor allows to use conventional scalar 
   *  smoothing filters to compute each one of the 
   *  components of the gradient image pixels. */
  typedef NthElementImageAdaptor< TOutputImage,
                                  InternalRealType >  OutputImageAdaptorType;
  typedef typename OutputImageAdaptorType::Pointer OutputImageAdaptorPointer;

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
  typedef typename GaussianFilterType::Pointer    GaussianFilterPointer;

  /**  Pointer to a derivative filter.  */
  typedef typename DerivativeFilterType::Pointer  DerivativeFilterPointer;

  /**  Pointer to the Output Image */
  typedef typename TOutputImage::Pointer          OutputImagePointer;                                  


  /** Type of the output Image */
  typedef TOutputImage      OutputImageType;
  typedef typename          OutputImageType::PixelType      OutputPixelType;
  typedef typename PixelTraits<OutputPixelType>::ValueType  OutputComponentType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Set Sigma value. Sigma is measured in the units of image spacing.  */
  void SetSigma( RealType sigma );

  /** Define which normalization factor will be used for the Gaussian */
  void SetNormalizeAcrossScale( bool normalizeInScaleSpace );
  itkGetMacro( NormalizeAcrossScale, bool );

  /** GradientRecursiveGaussianImageFilter needs all of the input to produce an
   * output. Therefore, GradientRecursiveGaussianImageFilter needs to provide
   * an implementation for GenerateInputRequestedRegion in order to inform
   * the pipeline execution model.
   * \sa ImageToImageFilter::GenerateInputRequestedRegion() */
  virtual void GenerateInputRequestedRegion() throw(InvalidRequestedRegionError);

protected:
  GradientRecursiveGaussianImageFilter();
  virtual ~GradientRecursiveGaussianImageFilter() {};
  void PrintSelf(std::ostream& os, Indent indent) const;
  
  /** Generate Data */
  void GenerateData( void );

  // Override since the filter produces the entire dataset
  void EnlargeOutputRequestedRegion(DataObject *output);

private:
  GradientRecursiveGaussianImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
  
  GaussianFilterPointer         m_SmoothingFilters[ImageDimension-1];
  DerivativeFilterPointer       m_DerivativeFilter;
  OutputImageAdaptorPointer     m_ImageAdaptor;

  /** Normalize the image across scale space */
  bool m_NormalizeAcrossScale; 

};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkGradientRecursiveGaussianImageFilter.txx"
#endif

#endif




