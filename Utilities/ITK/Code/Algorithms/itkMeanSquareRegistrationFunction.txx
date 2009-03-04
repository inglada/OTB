/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkMeanSquareRegistrationFunction.txx,v $
  Language:  C++
  Date:      $Date: 2009-01-24 20:02:59 $
  Version:   $Revision: 1.18 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkMeanSquareRegistrationFunction_txx
#define __itkMeanSquareRegistrationFunction_txx

#include "itkMeanSquareRegistrationFunction.h"
#include "itkExceptionObject.h"
#include "vnl/vnl_math.h"

namespace itk {

/**
 * Default constructor
 */
template <class TFixedImage, class TMovingImage, class TDeformationField>
MeanSquareRegistrationFunction<TFixedImage,TMovingImage,TDeformationField>
::MeanSquareRegistrationFunction()
{

  RadiusType r;
  unsigned int j;
  for( j = 0; j < ImageDimension; j++ )
    {
    r[j] = 0;
    }
  this->SetRadius(r);

  this->SetEnergy(0.0);
  m_TimeStep = 1.0;
  m_DenominatorThreshold = 1e-9;
  m_IntensityDifferenceThreshold = 0.001;
  this->SetMovingImage(NULL);
  this->SetFixedImage(NULL);
  m_FixedImageGradientCalculator = GradientCalculatorType::New();


  typename DefaultInterpolatorType::Pointer interp =
    DefaultInterpolatorType::New();

  m_MovingImageInterpolator = static_cast<InterpolatorType*>(
    interp.GetPointer() );


}


/*
 * Standard "PrintSelf" method.
 */
template <class TFixedImage, class TMovingImage, class TDeformationField>
void
MeanSquareRegistrationFunction<TFixedImage,TMovingImage,TDeformationField>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
/*
  os << indent << "MovingImageIterpolator: ";
  os << m_MovingImageInterpolator.GetPointer() << std::endl;
  os << indent << "FixedImageGradientCalculator: ";
  os << m_FixedImageGradientCalculator.GetPointer() << std::endl;
  os << indent << "DenominatorThreshold: ";
  os << m_DenominatorThreshold << std::endl;
  os << indent << "IntensityDifferenceThreshold: ";
  os << m_IntensityDifferenceThreshold << std::endl;
*/
}


/*
 * Set the function state values before each iteration
 */
template <class TFixedImage, class TMovingImage, class TDeformationField>
void
MeanSquareRegistrationFunction<TFixedImage,TMovingImage,TDeformationField>
::InitializeIteration()
{
  if( !this->GetMovingImage() || !this->GetFixedImage() || !m_MovingImageInterpolator )
    {
    itkExceptionMacro( << "MovingImage, FixedImage and/or Interpolator not set" );
    }

  // cache fixed image information
  m_FixedImageSpacing    = this->GetFixedImage()->GetSpacing();

  // setup gradient calculator
  m_FixedImageGradientCalculator->SetInputImage( this->GetFixedImage() );

  // setup moving image interpolator
  m_MovingImageInterpolator->SetInputImage( this->GetMovingImage() );

  this->SetEnergy(0.0);
}


/**
 * Compute update at a non boundary neighbourhood
 */
template <class TFixedImage, class TMovingImage, class TDeformationField>
typename MeanSquareRegistrationFunction<TFixedImage,TMovingImage,TDeformationField>
::PixelType
MeanSquareRegistrationFunction<TFixedImage,TMovingImage,TDeformationField>
::ComputeUpdate(const NeighborhoodType &it, void * itkNotUsed(globalData),
                const FloatOffsetType& itkNotUsed(offset))
{
  // Get fixed image related information
  // Note: no need to check the index is within
  // fixed image buffer. This is done by the external filter.
  const IndexType index = it.GetIndex();
  const double fixedValue = (double) this->GetFixedImage()->GetPixel( index );
  const CovariantVectorType fixedGradient = m_FixedImageGradientCalculator->EvaluateAtIndex( index );
  double fixedGradientSquaredMagnitude = 0;
  for(unsigned int j = 0; j < ImageDimension; j++ )
    {
    fixedGradientSquaredMagnitude += vnl_math_sqr( fixedGradient[j] ) * m_FixedImageSpacing[j];
    }

  // Get moving image related information
  const DeformationFieldPixelType itvec = this->GetDeformationField()->GetPixel(index);
  PointType mappedPoint;
  this->GetFixedImage()->TransformIndexToPhysicalPoint(index, mappedPoint);
  for(unsigned int j = 0; j < ImageDimension; j++ )
    {
      mappedPoint[j] += itvec[j];
    }
  double movingValue=0.0;
  if( m_MovingImageInterpolator->IsInsideBuffer( mappedPoint ) )
    {
    movingValue = m_MovingImageInterpolator->Evaluate( mappedPoint );
    }

  // Compute update
  const double speedValue = fixedValue - movingValue;
  this->m_Energy += speedValue * speedValue;

  const bool normalizemetric=this->GetNormalizeGradient();
  double denominator = 1.0;
  if (normalizemetric)
    {
    denominator = speedValue*speedValue *fixedGradientSquaredMagnitude;
    denominator = vcl_sqrt(denominator);
    }
  if (denominator == 0)
    {
    denominator=1.0;
    }
  PixelType update;
  if ( vnl_math_abs(speedValue) < m_IntensityDifferenceThreshold ||
    denominator < m_DenominatorThreshold )
    {
    update.Fill(0.0);
    return update;
    }

  for(unsigned int j = 0; j < ImageDimension; j++ )
    {
    update[j] = speedValue * fixedGradient[j] * vnl_math_sqr(m_FixedImageSpacing[j]) /
      denominator*this->m_GradientStep;
    }
  return update;
}

} // end namespace itk

#endif
