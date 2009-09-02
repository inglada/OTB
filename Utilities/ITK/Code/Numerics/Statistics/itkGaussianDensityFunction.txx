/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkGaussianDensityFunction.txx,v $
  Language:  C++
  Date:      $Date: 2009-04-06 00:19:17 $
  Version:   $Revision: 1.24 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkGaussianDensityFunction_txx
#define __itkGaussianDensityFunction_txx

#include "itkGaussianDensityFunction.h"

namespace itk { 
namespace Statistics {

template < class TMeasurementVector >
GaussianDensityFunction< TMeasurementVector >
::GaussianDensityFunction()
{
  m_Mean = 0;
  m_Covariance = 0;
  m_PreFactor = 0.0;
}

template < class TMeasurementVector >
void  
GaussianDensityFunction< TMeasurementVector >
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);

  os << indent << "Mean: ";
  if ( m_Mean != 0 )
    {
    os << (*m_Mean) << std::endl;
    }
  else
    {
    os << " not set." << std::endl;
    }
  
  os << indent << "Covariance: " << std::endl;
  if ( m_Covariance != 0 )
    {
    os << m_Covariance->GetVnlMatrix();
    os << indent << "InverseCovariance: " << std::endl;
    os << indent << m_InverseCovariance.GetVnlMatrix();
    os << indent << "Prefactor: " << m_PreFactor << std::endl;
    }
  os << indent << " not set." << std::endl;
}

template < class TMeasurementVector >
void 
GaussianDensityFunction< TMeasurementVector >
::SetCovariance(const CovarianceType* cov)
{
  // Sanity check
  if( cov->GetVnlMatrix().rows() != cov->GetVnlMatrix().cols() )
    {
    itkExceptionMacro( << "Covariance matrix must be square" );
    }
  if( this->GetMeasurementVectorSize() )
    {
    if( cov->GetVnlMatrix().rows() != this->GetMeasurementVectorSize() )
      {
      itkExceptionMacro( << "Length of measurement vectors in the sample must be"
         << " the same as the size of the covariance." );
      }
    }
  else
    {
    this->SetMeasurementVectorSize( cov->GetVnlMatrix().rows() );
    }

  m_Covariance = cov;

  m_IsCovarianceZero = m_Covariance->GetVnlMatrix().is_zero();

  if ( !m_IsCovarianceZero )
    {
    // allocate the memory for m_InverseCovariance matrix   
    m_InverseCovariance.GetVnlMatrix() = 
      vnl_matrix_inverse< double >(m_Covariance->GetVnlMatrix());
      
    // the determinant of the covaraince matrix
    double det = vnl_determinant(m_Covariance->GetVnlMatrix());
      
    // calculate coefficient C of multivariate gaussian
    m_PreFactor = 1.0 / (vcl_sqrt(det) * 
                         vcl_pow(vcl_sqrt(2.0 * vnl_math::pi), double(this->GetMeasurementVectorSize())));
    }
}

template < class TMeasurementVector >
const typename GaussianDensityFunction< TMeasurementVector >::CovarianceType*
GaussianDensityFunction< TMeasurementVector >
::GetCovariance() const
{
  return m_Covariance;
}

template < class TMeasurementVector >
inline double
GaussianDensityFunction< TMeasurementVector >
::Evaluate(const MeasurementVectorType &measurement) const
{ 

  double temp;

  const MeasurementVectorSizeType measurementVectorSize = 
                          this->GetMeasurementVectorSize();
  MeanType tempVector;
  MeasurementVectorTraits::SetLength( tempVector, measurementVectorSize );
  MeanType tempVector2;
  MeasurementVectorTraits::SetLength( tempVector2, measurementVectorSize );

  if ( !m_IsCovarianceZero )
    {
    // Compute |y - mean | 
    for ( unsigned int i = 0; i < measurementVectorSize; i++)
      {
      tempVector[i] = measurement[i] - (*m_Mean)[i];
      }
      
      
    // Compute |y - mean | * inverse(cov) 
    for (unsigned int i = 0; i < measurementVectorSize; i++)
      {
      temp = 0;
      for (unsigned int j = 0; j < measurementVectorSize; j++)
        {
        temp += tempVector[j] * m_InverseCovariance.GetVnlMatrix().get(j, i);
        }
      tempVector2[i] = temp;
      }


    // Compute |y - mean | * inverse(cov) * |y - mean|^T 
    temp = 0;
    for (unsigned int i = 0; i < measurementVectorSize; i++)
      {
      temp += tempVector2[i] * tempVector[i];
      }
      
    return  m_PreFactor * vcl_exp(-0.5 * temp );
    }
  else
    {
    for ( unsigned int i = 0; i < measurementVectorSize; i++)
      {
      if ( (*m_Mean)[i] != (double) measurement[i] )
        {
        return 0;
        }
      }
    return NumericTraits< double >::max();
    }
}
  

} // end namespace Statistics
} // end of namespace itk

#endif
