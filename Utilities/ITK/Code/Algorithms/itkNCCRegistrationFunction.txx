/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkNCCRegistrationFunction.txx,v $
  Language:  C++
  Date:      $Date: 2008-12-08 01:10:41 $
  Version:   $Revision: 1.13.4.1 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itkNCCRegistrationFunction_txx_
#define _itkNCCRegistrationFunction_txx_

#include "itkNCCRegistrationFunction.h"
#include "itkExceptionObject.h"
#include "vnl/vnl_math.h"

namespace itk {

/*
 * Default constructor
 */
template <class TFixedImage, class TMovingImage, class TDeformationField>
NCCRegistrationFunction<TFixedImage,TMovingImage,TDeformationField>
::NCCRegistrationFunction()
{

  RadiusType r;
  unsigned int j;
  for( j = 0; j < ImageDimension; j++ )
    {
    r[j] = 1;
    }
  this->SetRadius(r);
  m_MetricTotal=0.0;

  m_TimeStep = 1.0;
  m_DenominatorThreshold = 1e-9;
  m_IntensityDifferenceThreshold = 0.001;
  this->SetMovingImage(NULL);
  this->SetFixedImage(NULL);
  m_FixedImageSpacing.Fill( 1.0 );
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
NCCRegistrationFunction<TFixedImage,TMovingImage,TDeformationField>
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
NCCRegistrationFunction<TFixedImage,TMovingImage,TDeformationField>
::InitializeIteration()
{
  if( !this->m_MovingImage || !this->m_FixedImage || !m_MovingImageInterpolator )
    {
    itkExceptionMacro( << "MovingImage, FixedImage and/or Interpolator not set" );
    }

  // cache fixed image information
  m_FixedImageSpacing    = this->m_FixedImage->GetSpacing();

  // setup gradient calculator
  m_FixedImageGradientCalculator->SetInputImage( this->m_FixedImage );

  // setup moving image interpolator
  m_MovingImageInterpolator->SetInputImage( this->m_MovingImage );

  std::cout << " total metric " << m_MetricTotal << " field size " <<
    this->GetDeformationField()->GetLargestPossibleRegion().GetSize()<< " image size " <<
    this->m_FixedImage->GetLargestPossibleRegion().GetSize() << std::endl;
  m_MetricTotal=0.0;
}


/*
 * Compute update at a non boundary neighbourhood
 */
template <class TFixedImage, class TMovingImage, class TDeformationField>
typename NCCRegistrationFunction<TFixedImage,TMovingImage,TDeformationField>
::PixelType
NCCRegistrationFunction<TFixedImage,TMovingImage,TDeformationField>
::ComputeUpdate(const NeighborhoodType &it, void * itkNotUsed(globalData),
                const FloatOffsetType& itkNotUsed(offset))
{
  const IndexType oindex = it.GetIndex();
  const typename FixedImageType::SizeType hradius=it.GetRadius();
  FixedImageType* img =const_cast<FixedImageType *>(this->m_FixedImage.GetPointer());
  const typename FixedImageType::SizeType imagesize=img->GetLargestPossibleRegion().GetSize();

  NeighborhoodIterator<FixedImageType>
    hoodIt( hradius , img, img->GetRequestedRegion());
  hoodIt.SetLocation(oindex);

  double sff=0.0;
  double smm=0.0;
  double sfm=0.0;

  double derivativeF[ImageDimension];
  double derivativeM[ImageDimension];
  for (unsigned int j=0; j<ImageDimension;j++){
    derivativeF[j]=0;
    derivativeM[j]=0;
  }

  unsigned int hoodlen=hoodIt.Size();
  for(unsigned int indct=0; indct<hoodlen-1; indct++)
  {
    const IndexType index=hoodIt.GetIndex(indct);
    bool inimage=true;
    for (unsigned int dd=0; dd<ImageDimension; dd++)
    {
      if ( index[dd] < 0 || index[dd] > static_cast<typename IndexType::IndexValueType>(imagesize[dd]-1) ) inimage=false;
    }
    if (inimage)
    {
      // Get fixed image related information
      // Note: no need to check the index is within
      // fixed image buffer. This is done by the external filter.
      const double fixedValue = (double) this->m_FixedImage->GetPixel( index );
      const CovariantVectorType fixedGradient = m_FixedImageGradientCalculator->EvaluateAtIndex( index );
      double fixedGradientSquaredMagnitude = 0;
      for(unsigned int j = 0; j < ImageDimension; j++ )
      {
        fixedGradientSquaredMagnitude += vnl_math_sqr( fixedGradient[j] ) * m_FixedImageSpacing[j];
      }

      // Get moving image related information
      typedef typename TDeformationField::PixelType  DeformationPixelType;
      const DeformationPixelType vec = this->GetDeformationField()->GetPixel(index);
      PointType mappedPoint;
      this->GetFixedImage()->TransformIndexToPhysicalPoint(index, mappedPoint);
      for(unsigned int j = 0; j < ImageDimension; j++ )
        {
        mappedPoint[j] += vec[j];
        }
      double movingValue=0.0;
      if( m_MovingImageInterpolator->IsInsideBuffer( mappedPoint ) )
        {
        movingValue = m_MovingImageInterpolator->Evaluate( mappedPoint );
        }

      sff+=fixedValue*fixedValue;
      smm+=movingValue*movingValue;
      sfm+=fixedValue*movingValue;

      for(unsigned int dim=0; dim<ImageDimension; dim++)
      {
        const double differential = fixedGradient[dim];
        derivativeF[dim]+= fixedValue  * differential;
        derivativeM[dim]+= movingValue * differential;
      }
    }
  }

  PixelType update;
  update.Fill(0.0);
  double updatenorm=0.0;
  if( (sff*smm) != 0.0)
  {
    const double factor = 1.0 / vcl_sqrt(sff * smm );
    for(unsigned int i=0; i<ImageDimension; i++)
    {
      update[i] = factor * ( derivativeF[i] - (sfm/smm)*derivativeM[i]);
      updatenorm+=(update[i]*update[i]);
    }
    updatenorm=vcl_sqrt(updatenorm);
    m_MetricTotal+=sfm*factor;
    this->m_Energy+=sfm*factor;
  }
  else
  {
    update.Fill(0.0);
    updatenorm=1.0;
  }

  if (this->GetNormalizeGradient() && updatenorm !=0.0 )
  {
    update/=(updatenorm);
  }
  return update*this->m_GradientStep;
}

} // end namespace itk

#endif
