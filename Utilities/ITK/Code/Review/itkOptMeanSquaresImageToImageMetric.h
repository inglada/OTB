/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkOptMeanSquaresImageToImageMetric.h,v $
  Language:  C++
  Date:      $Date: 2008-03-17 18:33:10 $
  Version:   $Revision: 1.4 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkOptMeanSquaresImageToImageMetric_h
#define __itkOptMeanSquaresImageToImageMetric_h

#include "itkOptImageToImageMetric.h"
#include "itkCovariantVector.h"
#include "itkPoint.h"
#include "itkIndex.h"

#include "itkMultiThreader.h"

namespace itk
{

template <class TFixedImage,class TMovingImage >
class ITK_EXPORT MeanSquaresImageToImageMetric :
    public ImageToImageMetric< TFixedImage, TMovingImage >
{
public:

  /** Standard class typedefs. */
  typedef MeanSquaresImageToImageMetric                     Self;
  typedef ImageToImageMetric< TFixedImage, TMovingImage >   Superclass;
  typedef SmartPointer<Self>                                Pointer;
  typedef SmartPointer<const Self>                          ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(MeanSquaresImageToImageMetric, ImageToImageMetric);

  /** Types inherited from Superclass. */
  typedef typename Superclass::TransformType            TransformType;
  typedef typename Superclass::TransformPointer         TransformPointer;
  typedef typename Superclass::TransformJacobianType    TransformJacobianType;
  typedef typename Superclass::InterpolatorType         InterpolatorType;
  typedef typename Superclass::MeasureType              MeasureType;
  typedef typename Superclass::DerivativeType           DerivativeType;
  typedef typename Superclass::ParametersType           ParametersType;
  typedef typename Superclass::FixedImageType           FixedImageType;
  typedef typename Superclass::MovingImageType          MovingImageType;
  typedef typename Superclass::MovingImagePointType     MovingImagePointType;
  typedef typename Superclass::FixedImageConstPointer   FixedImageConstPointer;
  typedef typename Superclass::MovingImageConstPointer  MovingImageConstPointer;
  typedef typename Superclass::CoordinateRepresentationType
                                                   CoordinateRepresentationType;
  typedef typename Superclass::FixedImageSampleContainer  
                                                      FixedImageSampleContainer;
  typedef typename Superclass::ImageDerivativesType     ImageDerivativesType;
  typedef typename Superclass::WeightsValueType         WeightsValueType;
  typedef typename Superclass::IndexValueType           IndexValueType;

  // Needed for evaluation of Jacobian.
  typedef typename Superclass::FixedImagePointType     FixedImagePointType;

  /** The moving image dimension. */
  itkStaticConstMacro( MovingImageDimension, unsigned int,
                       MovingImageType::ImageDimension );

  /**
   *  Initialize the Metric by
   *  (1) making sure that all the components are present and plugged
   *      together correctly,
   *  (2) uniformly select NumberOfSpatialSamples within
   *      the FixedImageRegion, and
   *  (3) allocate memory for pdf data structures. */
  virtual void Initialize(void) throw ( ExceptionObject );

  /**  Get the value. */
  MeasureType GetValue( const ParametersType & parameters ) const;

  /** Get the derivatives of the match measure. */
  void GetDerivative( const ParametersType & parameters,
                      DerivativeType & Derivative ) const;

  /**  Get the value and derivatives for single valued optimizers. */
  void GetValueAndDerivative( const ParametersType & parameters,
                              MeasureType & Value,
                              DerivativeType & Derivative ) const;

protected:

  MeanSquaresImageToImageMetric();
  virtual ~MeanSquaresImageToImageMetric();
  void PrintSelf(std::ostream& os, Indent indent) const;

private:

  //purposely not implemented
  MeanSquaresImageToImageMetric(const Self &);
  //purposely not implemented
  void operator=(const Self &);

  inline bool GetValueThreadProcessSample( unsigned int threadID,
                                       unsigned long fixedImageSample,
                                       const MovingImagePointType & mappedPoint,
                                       double movingImageValue ) const;

  inline bool GetValueAndDerivativeThreadProcessSample( unsigned int threadID,
                                       unsigned long fixedImageSample,
                                       const MovingImagePointType & mappedPoint,
                                       double movingImageValue,
                                       const ImageDerivativesType &
                                               movingImageGradientValue ) const;

  MeasureType    * m_ThreaderMSE;
  DerivativeType * m_ThreaderMSEDerivatives;

};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkOptMeanSquaresImageToImageMetric.txx"
#endif

#endif
