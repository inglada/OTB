/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkImageToSpatialObjectMetric.h,v $
  Language:  C++
  Date:      $Date: 2006-04-18 14:50:07 $
  Version:   $Revision: 1.10 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkImageToSpatialObjectMetric_h
#define __itkImageToSpatialObjectMetric_h

#include "itkSingleValuedCostFunction.h"
#include "itkMinimumMaximumImageCalculator.h"
#include "itkLinearInterpolateImageFunction.h"
#include "vnl/vnl_vector_fixed.h"

namespace itk
{
  
/** \class ImageToSpatialObjectMetric
 * \brief Computes similarity between a moving spatial obejct 
 *        and an Image to be registered
 *
 *  The ImageToSpatialObjectMetric is different from the rest of the
 *  registration framework in ITK regarding the interpretation of the Transform
 *  with respect to the Fixed and Moving objects. In most of the ITK
 *  registration framework, the Transform computed by the optimizer is the one
 *  that maps points from the space of the Fixed object into the space of the
 *  Moving object. This direction of the transform is the one that makes easier
 *  to resample the Moving object into the space of the Fixed object.
 *
 *  In the particular case of the ImageToSpatialObject registration, the
 *  Transform to be computed is the one mapping points from the SpatialObject
 *  into the Image, despite the fact that the SpatialObject is called the
 *  "Moving" object and the image is called the "Fixed" object. This change of
 *  reference system is the consequence of using this type of registration in
 *  applications that are based on Visualization. In the context of such
 *  visualizations it is simpler to think in terms of the Transform that can be
 *  used for displaying the SpatialObject in the appropriate position with
 *  respect to the image. Since this process does not involve resampling, but
 *  providing a Transform to a visualization routine, it is usually more
 *  natural to use the Transform that maps points from the SpatialObject space
 *  the image space.
 *
 *  A full discussion of the Transform directions in the ITK registration
 *  framework can be found in the ITK Software Guide.
 *
 */

template < class TFixedImage, class TMovingSpatialObject> 
class ITK_EXPORT ImageToSpatialObjectMetric 
  : public SingleValuedCostFunction
{
public:
  /** Standard "Self" typedef. */
  typedef ImageToSpatialObjectMetric  Self;
  /** Standard "Superclass" typedef. */
  typedef SingleValuedCostFunction  Superclass;
  /** Smart pointer typedef support   */
  typedef SmartPointer<Self>   Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Type of the fixed image */
  typedef TFixedImage          FixedImageType;
 
  /** Type of the MovingSpatialObject */
  typedef TMovingSpatialObject  MovingSpatialObjectType;

  /** Type used for representing point components  */
  typedef Superclass::ParametersValueType CoordinateRepresentationType;

  /** Image dimension enumeration. */
  itkStaticConstMacro(ImageDimension, unsigned int,
                      FixedImageType::ImageDimension);
 
  /** Object dimension enumeration. */
  itkStaticConstMacro(ObjectDimension, unsigned int,
                      MovingSpatialObjectType::ObjectDimension);
 
  /**  Type of the Transform Base class */
  typedef Transform<CoordinateRepresentationType, 
                    itkGetStaticConstMacro(ObjectDimension),
                    itkGetStaticConstMacro(ImageDimension) > TransformType;

  typedef typename TransformType::Pointer            TransformPointer;
  typedef typename TransformType::InputPointType     InputPointType;
  typedef typename TransformType::OutputPointType    OutputPointType;
  typedef typename TransformType::ParametersType     TransformParametersType;
  typedef typename TransformType::JacobianType       TransformJacobianType;

  /**  Type of the Interpolator Base class */
  typedef LinearInterpolateImageFunction<
    TFixedImage,
    CoordinateRepresentationType > InterpolatorType;

  typedef typename InterpolatorType::Pointer         InterpolatorPointer;

  /** Typede of the vector type to return derivatives */
  typedef vnl_vector_fixed<double,
                           itkGetStaticConstMacro(ObjectDimension)> VectorType;

  /**  Type of the match measure */
  typedef Superclass::MeasureType          MeasureType;

  /** Type of the derivative of the match measure */
  typedef Superclass::DerivativeType   DerivativeType; 

  /** Pointer type for the FixedImage  */
  typedef typename FixedImageType::Pointer    FixedImagePointer;

  /** Pointer type for the MovingSpatialObject */
  typedef typename MovingSpatialObjectType::Pointer  
  MovingSpatialObjectPointer;

  /** Const pointer type for the FixedImage */
  typedef typename FixedImageType::ConstPointer   FixedImageConstPointer;
  
  /** Const pointer type for the MovingSpatialObject */
  typedef typename MovingSpatialObjectType::ConstPointer    
  MovingSpatialObjectConstPointer;

  /**  ParametersType typedef.
   *  It defines a position in the optimization search space. */
  typedef Superclass::ParametersType         ParametersType;

  /** Run-time type information (and related methods).*/
  itkTypeMacro(ImageToSpatialObjectMetric, Object);

  /** Connect the FixedImage. */
  itkSetConstObjectMacro( FixedImage, FixedImageType );
  
  /** Get the FixedImage. */
  itkGetConstObjectMacro( FixedImage, FixedImageType );

  /** Connect the MovingSpatialObject */
  itkSetConstObjectMacro( MovingSpatialObject, MovingSpatialObjectType );
  
  /** Get the MovingSpatialObject. */
  itkGetConstObjectMacro( MovingSpatialObject, MovingSpatialObjectType );

  /** Connect the Interpolator. */
  itkSetObjectMacro( Interpolator, InterpolatorType );
  
  /** Get the Interpolator. */
  itkGetObjectMacro( Interpolator, InterpolatorType );

  /** Get the Derivatives of the Match Measure */
  virtual void GetDerivative( const ParametersType & parameters,
                              DerivativeType & derivative ) const = 0;

  /** Get the Value for SingleValue Optimizers */
  virtual MeasureType    GetValue( const ParametersType & parameters ) const =0;

  /** Get Value and Derivatives for MultipleValuedOptimizers */
  virtual void GetValueAndDerivative( const ParametersType & parameters,
                                      MeasureType & Value, 
                                      DerivativeType  & Derivative ) const =0;

  /** Initialize the metric */
  virtual void Initialize(void) throw (ExceptionObject);
 
  /** Get the last transformation parameters visited by 
   * the optimizer. This function overload the superclass's one */
  itkGetConstReferenceMacro( LastTransformParameters, ParametersType );

  /** Set/Get the Transform. */
  itkSetObjectMacro( Transform, TransformType );

protected:

  ImageToSpatialObjectMetric();
  virtual ~ImageToSpatialObjectMetric() {};
  ImageToSpatialObjectMetric(const Self&) {}
  void operator=(const Self&) {}
  void PrintSelf(std::ostream& os, Indent indent) const;

  MeasureType                 m_MatchMeasure;
  DerivativeType              m_MatchMeasureDerivatives;
  mutable TransformPointer    m_Transform;
  InterpolatorPointer         m_Interpolator;

  MovingSpatialObjectConstPointer   m_MovingSpatialObject;
  FixedImageConstPointer            m_FixedImage;
  ParametersType                    m_LastTransformParameters;

};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkImageToSpatialObjectMetric.txx"
#endif

#endif


