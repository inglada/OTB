/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkImageToImageMetric.h,v $
  Language:  C++
  Date:      $Date: 2009-08-20 09:08:41 $
  Version:   $Revision: 1.26 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkImageToImageMetric_h
#define __itkImageToImageMetric_h

// First make sure that the configuration is available.
// This line can be removed once the optimized versions
// gets integrated into the main directories.
#include "itkConfigure.h"

#ifdef ITK_USE_OPTIMIZED_REGISTRATION_METHODS
#include "itkOptImageToImageMetric.h"
#else

#include "itkImageBase.h"
#include "itkTransform.h"
#include "itkInterpolateImageFunction.h"
#include "itkSingleValuedCostFunction.h"
#include "itkExceptionObject.h"
#include "itkGradientRecursiveGaussianImageFilter.h"
#include "itkSpatialObject.h"

namespace itk
{
  
/** \class ImageToImageMetric
 * \brief Computes similarity between regions of two images.
 *
 * This Class is templated over the type of the two input images.
 * It expects a Transform and an Interpolator to be plugged in.
 * This particular class is the base class for a hierarchy of 
 * similarity metrics.
 *
 * This class computes a value that measures the similarity 
 * between the Fixed image and the transformed Moving image.
 * The Interpolator is used to compute intensity values on 
 * non-grid positions resulting from mapping points through 
 * the Transform.
 * 
 *
 * \ingroup RegistrationMetrics
 *
 */

template <class TFixedImage,  class TMovingImage> 
class ITK_EXPORT ImageToImageMetric : public SingleValuedCostFunction 
{
public:
  /** Standard class typedefs. */
  typedef ImageToImageMetric           Self;
  typedef SingleValuedCostFunction     Superclass;
  typedef SmartPointer<Self>           Pointer;
  typedef SmartPointer<const Self>     ConstPointer;

  /** Type used for representing point components  */
  typedef typename Superclass::ParametersValueType CoordinateRepresentationType;

  /** Run-time type information (and related methods). */
  itkTypeMacro(ImageToImageMetric, SingleValuedCostFunction);

  /**  Type of the moving Image. */
  typedef TMovingImage                               MovingImageType;
  typedef typename TMovingImage::PixelType           MovingImagePixelType;
  typedef typename MovingImageType::ConstPointer     MovingImageConstPointer;

  /**  Type of the fixed Image. */
  typedef TFixedImage                                FixedImageType;
  typedef typename FixedImageType::ConstPointer      FixedImageConstPointer;
  typedef typename FixedImageType::RegionType        FixedImageRegionType;

  /** Constants for the image dimensions */
  itkStaticConstMacro(MovingImageDimension, 
                      unsigned int,
                      TMovingImage::ImageDimension);
  itkStaticConstMacro(FixedImageDimension, 
                      unsigned int,
                      TFixedImage::ImageDimension);
  
  /**  Type of the Transform Base class */
  typedef Transform<CoordinateRepresentationType, 
                    itkGetStaticConstMacro(MovingImageDimension),
                    itkGetStaticConstMacro(FixedImageDimension)> 
                                                     TransformType;

  typedef typename TransformType::Pointer            TransformPointer;
  typedef typename TransformType::InputPointType     InputPointType;
  typedef typename TransformType::OutputPointType    OutputPointType;
  typedef typename TransformType::ParametersType     TransformParametersType;
  typedef typename TransformType::JacobianType       TransformJacobianType;

  /**  Type of the Interpolator Base class */
  typedef InterpolateImageFunction<
    MovingImageType,
    CoordinateRepresentationType > InterpolatorType;


  /** Gaussian filter to compute the gradient of the Moving Image */
  typedef typename NumericTraits<MovingImagePixelType>::RealType 
                                                     RealType;
  typedef CovariantVector<RealType,
                          itkGetStaticConstMacro(MovingImageDimension)>
                                                     GradientPixelType;
  typedef Image<GradientPixelType,
                itkGetStaticConstMacro(MovingImageDimension)> 
                                                     GradientImageType;
  typedef SmartPointer<GradientImageType>            GradientImagePointer;
  typedef GradientRecursiveGaussianImageFilter< MovingImageType,
                                                GradientImageType >
                                                     GradientImageFilterType;  
  typedef typename GradientImageFilterType::Pointer  GradientImageFilterPointer;


  typedef typename InterpolatorType::Pointer         InterpolatorPointer;


  /**  Type for the mask of the fixed image. Only pixels that are "inside"
       this mask will be considered for the computation of the metric */
  typedef SpatialObject< itkGetStaticConstMacro(FixedImageDimension) >
                                                     FixedImageMaskType;
  typedef typename FixedImageMaskType::Pointer       FixedImageMaskPointer;
  typedef typename FixedImageMaskType::ConstPointer  FixedImageMaskConstPointer;


  /**  Type for the mask of the moving image. Only pixels that are "inside"
       this mask will be considered for the computation of the metric */
  typedef SpatialObject< itkGetStaticConstMacro(MovingImageDimension) >
                                                     MovingImageMaskType;
  typedef typename MovingImageMaskType::Pointer      MovingImageMaskPointer;
  typedef typename MovingImageMaskType::ConstPointer MovingImageMaskConstPointer;


  /**  Type of the measure. */
  typedef typename Superclass::MeasureType                    MeasureType;

  /**  Type of the derivative. */
  typedef typename Superclass::DerivativeType                 DerivativeType;

  /**  Type of the parameters. */
  typedef typename Superclass::ParametersType                 ParametersType;

  /** Connect the Fixed Image.  */
  itkSetConstObjectMacro( FixedImage, FixedImageType );

  /** Get the Fixed Image. */
  itkGetConstObjectMacro( FixedImage, FixedImageType );

  /** Connect the Moving Image.  */
  itkSetConstObjectMacro( MovingImage, MovingImageType );

  /** Get the Moving Image. */
  itkGetConstObjectMacro( MovingImage, MovingImageType );

  /** Connect the Transform. */
  itkSetObjectMacro( Transform, TransformType );

  /** Get a pointer to the Transform.  */
  itkGetConstObjectMacro( Transform, TransformType );
 
  /** Connect the Interpolator. */
  itkSetObjectMacro( Interpolator, InterpolatorType );

  /** Get a pointer to the Interpolator.  */
  itkGetConstObjectMacro( Interpolator, InterpolatorType );

  /** Get the number of pixels considered in the computation. */
  itkGetConstReferenceMacro( NumberOfPixelsCounted, unsigned long );

  /** Set the region over which the metric will be computed */
  itkSetMacro( FixedImageRegion, FixedImageRegionType );

  /** Get the region over which the metric will be computed */
  itkGetConstReferenceMacro( FixedImageRegion, FixedImageRegionType );
 
  /** Set/Get the moving image mask. */
  itkSetObjectMacro( MovingImageMask, MovingImageMaskType );
#ifdef ITK_LEGACY_REMOVE
  itkSetConstObjectMacro( MovingImageMask, MovingImageMaskType );
#else
  virtual void SetMovingImageMask( const MovingImageMaskType* mask )
    { this->SetMovingImageMask(const_cast<MovingImageMaskType*>(mask)); }
#endif
  itkGetConstObjectMacro( MovingImageMask, MovingImageMaskType );

  /** Set/Get the fixed image mask. */
  itkSetObjectMacro( FixedImageMask, FixedImageMaskType );
#ifdef ITK_LEGACY_REMOVE
  itkSetConstObjectMacro( FixedImageMask, FixedImageMaskType );
#else
  virtual void SetFixedImageMask( const FixedImageMaskType* mask )
    { this->SetFixedImageMask(const_cast<FixedImageMaskType*>(mask)); }
#endif
  itkGetConstObjectMacro( FixedImageMask, FixedImageMaskType );

  /** Set/Get gradient computation. */
  itkSetMacro( ComputeGradient, bool);
  itkGetConstReferenceMacro( ComputeGradient, bool);
  itkBooleanMacro(ComputeGradient);

  /** Computes the gradient image and assigns it to m_GradientImage */
  virtual void ComputeGradient();

  /** Get Gradient Image. */
  itkGetConstObjectMacro( GradientImage, GradientImageType );

  /** Set the parameters defining the Transform. */
  void SetTransformParameters( const ParametersType & parameters ) const;

  /** Return the number of parameters required by the Transform */
  unsigned int GetNumberOfParameters(void) const 
    { return m_Transform->GetNumberOfParameters(); }

  /** Initialize the Metric by making sure that all the components
   *  are present and plugged together correctly     */
  virtual void Initialize(void) throw ( ExceptionObject );

protected:
  ImageToImageMetric();
  virtual ~ImageToImageMetric();
  void PrintSelf(std::ostream& os, Indent indent) const;

  mutable unsigned long       m_NumberOfPixelsCounted;

  FixedImageConstPointer      m_FixedImage;
  MovingImageConstPointer     m_MovingImage;

  mutable TransformPointer    m_Transform;
  InterpolatorPointer         m_Interpolator;

  bool                        m_ComputeGradient;
  GradientImagePointer        m_GradientImage;

#ifdef ITK_LEGACY_REMOVE
  FixedImageMaskConstPointer  m_FixedImageMask;
  MovingImageMaskConstPointer m_MovingImageMask;
#else
  mutable FixedImageMaskPointer   m_FixedImageMask;
  mutable MovingImageMaskPointer  m_MovingImageMask;
#endif

private:
  ImageToImageMetric(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
  
  FixedImageRegionType        m_FixedImageRegion;  

};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkImageToImageMetric.txx"
#endif

#endif

#endif
