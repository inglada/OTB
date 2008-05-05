/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkMeanSquaresPointSetToImageMetric.txx,v $
  Language:  C++
  Date:      $Date: 2008-02-03 04:05:29 $
  Version:   $Revision: 1.19 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itkMeanSquaresPointSetToImageMetric_txx
#define _itkMeanSquaresPointSetToImageMetric_txx

#include "itkMeanSquaresPointSetToImageMetric.h"
#include "itkImageRegionConstIteratorWithIndex.h"

namespace itk
{

/*
 * Constructor
 */
template <class TFixedPointSet, class TMovingImage> 
MeanSquaresPointSetToImageMetric<TFixedPointSet,TMovingImage>
::MeanSquaresPointSetToImageMetric()
{
}

/*
 * Get the match Measure
 */
template <class TFixedPointSet, class TMovingImage> 
typename MeanSquaresPointSetToImageMetric<TFixedPointSet,TMovingImage>::MeasureType
MeanSquaresPointSetToImageMetric<TFixedPointSet,TMovingImage>
::GetValue( const TransformParametersType & parameters ) const
{

  FixedPointSetConstPointer fixedPointSet = this->GetFixedPointSet();

  if( !fixedPointSet ) 
    {
    itkExceptionMacro( << "Fixed point set has not been assigned" );
    }

  PointIterator pointItr = fixedPointSet->GetPoints()->Begin();
  PointIterator pointEnd = fixedPointSet->GetPoints()->End();

  PointDataIterator pointDataItr = fixedPointSet->GetPointData()->Begin();
  PointDataIterator pointDataEnd = fixedPointSet->GetPointData()->End();

  MeasureType measure = NumericTraits< MeasureType >::Zero;

  this->m_NumberOfPixelsCounted = 0;

  this->SetTransformParameters( parameters );

  typedef  typename NumericTraits< MeasureType >::AccumulateType AccumulateType;

  while( pointItr != pointEnd && pointDataItr != pointDataEnd )
    {
    InputPointType  inputPoint;
    inputPoint.CastFrom( pointItr.Value() );
    OutputPointType transformedPoint = 
      this->m_Transform->TransformPoint( inputPoint );

    if( this->m_Interpolator->IsInsideBuffer( transformedPoint ) )
      {
      const RealType movingValue  = this->m_Interpolator->Evaluate( transformedPoint );
      const RealType fixedValue   = pointDataItr.Value();
      const RealType diff = movingValue - fixedValue; 
      measure += diff * diff; 
      this->m_NumberOfPixelsCounted++;
      }

    ++pointItr;
    ++pointDataItr;
    }

  if( !this->m_NumberOfPixelsCounted )
    {
    itkExceptionMacro(<<"All the points mapped to outside of the moving image");
    }
  else
    {
    measure /= this->m_NumberOfPixelsCounted;
    }


  return measure;

}





/*
 * Get the Derivative Measure
 */
template < class TFixedPointSet, class TMovingImage> 
void
MeanSquaresPointSetToImageMetric<TFixedPointSet,TMovingImage>
::GetDerivative( const TransformParametersType & parameters,
                 DerivativeType & derivative ) const
{

  if( !this->GetGradientImage() )
    {
    itkExceptionMacro(<<"The gradient image is null, maybe you forgot to call Initialize()");
    }

  FixedPointSetConstPointer fixedPointSet = this->GetFixedPointSet();

  if( !fixedPointSet ) 
    {
    itkExceptionMacro( << "Fixed image has not been assigned" );
    }

  this->m_NumberOfPixelsCounted = 0;

  this->SetTransformParameters( parameters );

  typedef  typename NumericTraits< MeasureType >::AccumulateType AccumulateType;

  const unsigned int ParametersDimension = this->GetNumberOfParameters();
  derivative = DerivativeType( ParametersDimension );
  derivative.Fill( NumericTraits<ITK_TYPENAME DerivativeType::ValueType>::Zero );

  PointIterator pointItr = fixedPointSet->GetPoints()->Begin();
  PointIterator pointEnd = fixedPointSet->GetPoints()->End();

  PointDataIterator pointDataItr = fixedPointSet->GetPointData()->Begin();
  PointDataIterator pointDataEnd = fixedPointSet->GetPointData()->End();

  while( pointItr != pointEnd && pointDataItr != pointDataEnd )
    {
    InputPointType  inputPoint;
    inputPoint.CastFrom( pointItr.Value() );
    OutputPointType transformedPoint = 
      this->m_Transform->TransformPoint( inputPoint );

    if( this->m_Interpolator->IsInsideBuffer( transformedPoint ) )
      {
      const RealType movingValue  = this->m_Interpolator->Evaluate( transformedPoint );
      const RealType fixedValue   = pointDataItr.Value();

      this->m_NumberOfPixelsCounted++;
      const RealType diff = movingValue - fixedValue; 


      // Now compute the derivatives
      const TransformJacobianType & jacobian =
        this->m_Transform->GetJacobian( inputPoint ); 

      // Get the gradient by NearestNeighboorInterpolation: 
      // which is equivalent to round up the point components.
      typedef typename OutputPointType::CoordRepType CoordRepType;
      typedef ContinuousIndex<CoordRepType,MovingImageType::ImageDimension>
        MovingImageContinuousIndexType;

      MovingImageContinuousIndexType tempIndex;
      this->m_MovingImage->TransformPhysicalPointToContinuousIndex( transformedPoint, tempIndex );

      typename MovingImageType::IndexType mappedIndex; 
      mappedIndex.CopyWithRound( tempIndex );
      
      const GradientPixelType gradient = 
        this->GetGradientImage()->GetPixel( mappedIndex );

      for(unsigned int par=0; par<ParametersDimension; par++)
        {
        RealType sum = NumericTraits< RealType >::Zero;
        for(unsigned int dim=0; dim<Self::FixedPointSetDimension; dim++)
          {
          sum += 2.0 * diff * jacobian( dim, par ) * gradient[dim];
          }
        derivative[par] += sum;
        }
      }

    ++pointItr;
    ++pointDataItr;
    }

  if( !this->m_NumberOfPixelsCounted )
    {
    itkExceptionMacro(<<"All the points mapped to outside of the moving image");
    }
  else
    {
    for(unsigned int i=0; i<ParametersDimension; i++)
      {
      derivative[i] /= this->m_NumberOfPixelsCounted;
      }
    }
}


/*
 * Get both the match Measure and theDerivative Measure 
 */
template <class TFixedPointSet, class TMovingImage> 
void
MeanSquaresPointSetToImageMetric<TFixedPointSet,TMovingImage>
::GetValueAndDerivative(const TransformParametersType & parameters, 
                        MeasureType & value, DerivativeType  & derivative) const
{

  if( !this->GetGradientImage() )
    {
    itkExceptionMacro(<<"The gradient image is null, maybe you forgot to call Initialize()");
    }

  FixedPointSetConstPointer fixedPointSet = this->GetFixedPointSet();

  if( !fixedPointSet ) 
    {
    itkExceptionMacro( << "Fixed image has not been assigned" );
    }

  this->m_NumberOfPixelsCounted = 0;
  MeasureType measure = NumericTraits< MeasureType >::Zero;

  this->SetTransformParameters( parameters );

  typedef  typename NumericTraits< MeasureType >::AccumulateType AccumulateType;

  const unsigned int ParametersDimension = this->GetNumberOfParameters();
  derivative = DerivativeType( ParametersDimension );
  derivative.Fill( NumericTraits<ITK_TYPENAME DerivativeType::ValueType>::Zero );

  PointIterator pointItr = fixedPointSet->GetPoints()->Begin();
  PointIterator pointEnd = fixedPointSet->GetPoints()->End();

  PointDataIterator pointDataItr = fixedPointSet->GetPointData()->Begin();
  PointDataIterator pointDataEnd = fixedPointSet->GetPointData()->End();

  while( pointItr != pointEnd && pointDataItr != pointDataEnd )
    {
    InputPointType  inputPoint;
    inputPoint.CastFrom( pointItr.Value() );
    OutputPointType transformedPoint = 
      this->m_Transform->TransformPoint( inputPoint );

    if( this->m_Interpolator->IsInsideBuffer( transformedPoint ) )
      {
      const RealType movingValue  = this->m_Interpolator->Evaluate( transformedPoint );
      const RealType fixedValue   = pointDataItr.Value();

      this->m_NumberOfPixelsCounted++;

      // Now compute the derivatives
      const TransformJacobianType & jacobian =
        this->m_Transform->GetJacobian( inputPoint ); 

      const RealType diff = movingValue - fixedValue; 
  
      measure += diff * diff;

      // Get the gradient by NearestNeighboorInterpolation: 
      // which is equivalent to round up the point components.
      typedef typename OutputPointType::CoordRepType CoordRepType;
      typedef ContinuousIndex<CoordRepType,MovingImageType::ImageDimension>
        MovingImageContinuousIndexType;

      MovingImageContinuousIndexType tempIndex;
      this->m_MovingImage->TransformPhysicalPointToContinuousIndex( transformedPoint, tempIndex );

      typename MovingImageType::IndexType mappedIndex; 
      mappedIndex.CopyWithRound( tempIndex );

      const GradientPixelType gradient = 
        this->GetGradientImage()->GetPixel( mappedIndex );

     for(unsigned int par=0; par<ParametersDimension; par++)
        {
        RealType sum = NumericTraits< RealType >::Zero;
        for(unsigned int dim=0; dim<Self::FixedPointSetDimension; dim++)
          {
          sum += 2.0 * diff * jacobian( dim, par ) * gradient[dim];
          }
        derivative[par] += sum;
        }

      }

    ++pointItr;
    ++pointDataItr;
    }

 if( !this->m_NumberOfPixelsCounted )
    {
    itkExceptionMacro(<<"All the points mapped to outside of the moving image");
    }
  else
    {
    for(unsigned int i=0; i<ParametersDimension; i++)
      {
      derivative[i] /= this->m_NumberOfPixelsCounted;
      }
    measure /= this->m_NumberOfPixelsCounted;
    }

  value = measure;


}

} // end namespace itk


#endif
