/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkGaussianGoodnessOfFitComponent.txx,v $
  Language:  C++
  Date:      $Date: 2006-03-19 04:37:20 $
  Version:   $Revision: 1.11 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkGaussianGoodnessOfFitComponent_txx
#define __itkGaussianGoodnessOfFitComponent_txx

#include "itkGaussianGoodnessOfFitComponent.h"

#include "vnl/vnl_math.h"

namespace itk{ 
namespace Statistics{

template< class TInputSample >
GaussianGoodnessOfFitComponent< TInputSample >
::GaussianGoodnessOfFitComponent()
{
  m_StandardDeviation = 0.0 ;
  m_Radius = 0.0 ;

  m_NumberOfParameters = (unsigned int)(this->GetMeasurementVectorSize() + 1) ;

  m_ProbabilityDensityFunction = ProbabilityDensityFunctionType::New() ;
  
  m_CovarianceCalculator = CovarianceCalculatorType::New() ;
  m_CovarianceCalculator->
    SetWeightFunction(m_ProbabilityDensityFunction.GetPointer()) ;

  m_ProjectionAxisCalculator = new ProjectionAxisCalculatorType() ;
  //m_ProjectionAxisCalculator->SetMatrix(&m_Covariance);

  m_LargestEigenValue = 0.0 ;
  m_LongestAxisIndex = 0;
}

template< class TInputSample >
GaussianGoodnessOfFitComponent< TInputSample >
::~GaussianGoodnessOfFitComponent()
{
  delete m_ProjectionAxisCalculator;
}

template< class TInputSample >
void
GaussianGoodnessOfFitComponent< TInputSample >
::SetInputSample( const TInputSample * sample )
{
  Superclass::SetInputSample( sample );
  const MeasurementVectorSizeType measurementVectorLength = 
    this->GetMeasurementVectorSize();
  MeasurementVectorTraits::SetLength( m_Mean, measurementVectorLength );
  MeasurementVectorTraits::SetLength( m_Center, measurementVectorLength );
  
  m_Covariance.SetSize( measurementVectorLength, measurementVectorLength );
  m_Mean.Fill(0.0) ;
  m_Covariance.Fill(0.0) ;
  
  m_CovarianceCalculator->SetMean(&m_Mean);
  m_ProbabilityDensityFunction->SetMean(&m_Mean);
  m_ProbabilityDensityFunction->SetCovariance(&m_Covariance);
}
  
  
  
template< class TInputSample >
void
GaussianGoodnessOfFitComponent< TInputSample >
::PrintSelf(std::ostream& os, Indent indent) const 
{
  Superclass::PrintSelf(os,indent) ;

  os << indent << "Mean: " << m_Mean << std::endl ;
  os << indent << "Covariance: " << m_Covariance << std::endl ;
  os << indent << "StandardDeviation: " << m_StandardDeviation << std::endl ;
  os << indent << "Center: " << m_Center << std::endl ;
  os << indent << "Radius: " << m_Radius << std::endl ;
  os << indent << "NumberOfParameters: " << m_NumberOfParameters << std::endl ;
  os << indent << "ProbabilityDensityFunction: " << m_ProbabilityDensityFunction << std::endl ;
  os << indent << "CovarianceCalculator: " << m_CovarianceCalculator << std::endl ;
  os << indent << "ProjectionAxisCalculator: " << m_ProjectionAxisCalculator << std::endl ;
  os << indent << "LongestAxisIndex: " << m_LongestAxisIndex << std::endl ;
  os << indent << "LargestEigenValue: " << m_LargestEigenValue << std::endl ;
}


template< class TInputSample >
void
GaussianGoodnessOfFitComponent< TInputSample >
::SetParameters(const ParametersType &parameters)
{
  Superclass::SetParameters(parameters) ;

  bool changed = false ;

  unsigned int i = 0 ;
  while ( i < this->GetMeasurementVectorSize() )
    {
    if ( m_Mean[i] != parameters[i] )
      {
      m_Mean[i] = parameters[i] ;
      m_Center[i] = m_Mean[i] ;
      changed = true ;
      }
    i++ ;
    }

  if ( m_StandardDeviation != parameters[i] )
    {
    m_StandardDeviation = parameters[i] ;
    changed = true ;
    }

  if ( changed )
    {
    this->Modified() ;
    }
}

template< class TInputSample >
typename GaussianGoodnessOfFitComponent< TInputSample >::CenterType*
GaussianGoodnessOfFitComponent< TInputSample >
::GetCenter()
{
  return &m_Center ;
}

template< class TInputSample >
typename GaussianGoodnessOfFitComponent< TInputSample >::RadiusType*
GaussianGoodnessOfFitComponent< TInputSample >
::GetRadius() 
{
  m_Radius = m_StandardDeviation * this->GetHistogramExtent() ;
  return &m_Radius ;
}
  
template< class TInputSample >
typename GaussianGoodnessOfFitComponent< TInputSample >::MeanType*
GaussianGoodnessOfFitComponent< TInputSample >
::GetMean()
{
  return &m_Mean ;
}

template< class TInputSample >
typename GaussianGoodnessOfFitComponent< TInputSample >::StandardDeviationType*
GaussianGoodnessOfFitComponent< TInputSample >
::GetStandardDeviation() 
{
  return &m_StandardDeviation ;
}

template< class TInputSample >
void
GaussianGoodnessOfFitComponent< TInputSample >
::CalculateProjectionAxes()
{
  unsigned int i, j ;

  m_CovarianceCalculator->SetInputSample(this->GetResampledSample()) ;
  m_Covariance.Fill(0.0) ;
  m_Covariance.GetVnlMatrix().
    fill_diagonal( m_StandardDeviation * m_StandardDeviation ) ;

  if ( this->GetResampledSample()->GetTotalFrequency() > 0 )
    {
    m_ProbabilityDensityFunction->SetCovariance(&m_Covariance) ;
    m_CovarianceCalculator->Update() ;
      
    m_Covariance = (*m_CovarianceCalculator->GetOutput()) ;
    }

  m_ProjectionAxisCalculator->SetDimension( this->GetResampledSample(
        )->GetMeasurementVectorSize() );
  EigenValuesArrayType eigenValues( this->GetMeasurementVectorSize() );
  ProjectionAxisArrayType  from;
  from.SetSize( this->GetMeasurementVectorSize(), this->GetMeasurementVectorSize());

  ProjectionAxisArrayType* to = this->GetProjectionAxes() ;

  m_ProjectionAxisCalculator->ComputeEigenValuesAndVectors( 
      m_Covariance, eigenValues, from);

  for ( i = 0 ; i < this->GetMeasurementVectorSize(); i++ )
    {
    for (j = 0 ; j < this->GetMeasurementVectorSize(); j++)
      {
      (*to)[i][j] = (from)[i][j] ;
      }
    }

  m_LongestAxisIndex = 0 ;
  m_LargestEigenValue = NumericTraits< double >::NonpositiveMin() ;
  for ( i = 0 ; i < this->GetMeasurementVectorSize(); i++ )
    {
    if ( eigenValues[i] > m_LargestEigenValue )
      {
      m_LongestAxisIndex = i ;
      m_LargestEigenValue = (eigenValues)[i] ;
      }
    }

  m_Covariance *= 
    (m_StandardDeviation * m_StandardDeviation / m_LargestEigenValue) ;
}

template< class TInputSample >
double
GaussianGoodnessOfFitComponent< TInputSample >
::GetCumulativeProbability(double x) const
{
  /* |e(x)| < 7.5e-8 : From Handbook, p491 */
  
  double mean = 0.0 ;
  double standardDeviation = 1.0 ;

  double nx = vcl_fabs(x - mean ) / standardDeviation ;
  
  double t = 1 / (1 + 0.2316419 * nx);
  double tt = t*t;
  double ttt = tt*t;
  double tttt = ttt*t;
  double ttttt = tttt*t;
  
  double z = vcl_exp(-nx*nx/2) / vcl_sqrt(2*vnl_math::pi);
  
  if ( x > mean )
    {
    return 1 - z * (0.319381530*t - 0.356563782*tt + 1.781477937*ttt -
                    1.821255978*tttt + 1.330274429*ttttt);
    }
  else
    {
    return z * (0.319381530*t - 0.356563782*tt + 1.781477937*ttt -
                1.821255978*tttt + 1.330274429*ttttt);
    }
}

template< class TInputSample >
double
GaussianGoodnessOfFitComponent< TInputSample >
::GetProbabilityDensity(MeasurementVectorType &measurements) const
{
  return m_ProbabilityDensityFunction->Evaluate(measurements) ;
}

template< class TInputSample >
void
GaussianGoodnessOfFitComponent< TInputSample >
::PrintParameters(std::ostream &os) const
{
  unsigned int i, j ;
  os << m_Mean ;
  for( i = 0 ; i < this->GetMeasurementVectorSize() ; i++)
    {
    for( j = 0 ; j < this->GetMeasurementVectorSize() ; j++)
      {
      os << " " << m_Covariance.GetVnlMatrix().get(i,j) ;
      }
    }
  os << std::endl ;
}

template< class TInputSample >
typename GaussianGoodnessOfFitComponent< TInputSample >::ParametersType
GaussianGoodnessOfFitComponent< TInputSample >
::GetFullParameters() const
{
  ParametersType params(this->GetMeasurementVectorSize() + 
                        this->GetMeasurementVectorSize() * this->GetMeasurementVectorSize()) ;

  unsigned int index = 0 ;
  while ( index < this->GetMeasurementVectorSize() )
    {
    params[index] = m_Mean[index] ;
    ++index ;
    }

  unsigned int i, j ;
  for( i = 0 ; i < this->GetMeasurementVectorSize() ; i++)
    {
    for( j = 0 ; j < this->GetMeasurementVectorSize() ; j++)
      {
      params[index] = m_Covariance.GetVnlMatrix().get(i,j) ;
      ++index ;
      }
    }

  return params ;
}

} // end of namespace Statistics 
} // end of namespace itk

#endif

