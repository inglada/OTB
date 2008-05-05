/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkMahalanobisDistanceMembershipFunction.txx,v $
  Language:  C++
  Date:      $Date: 2008-01-15 12:37:56 $
  Version:   $Revision: 1.15 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkMahalanobisDistanceMembershipFunction_txx
#define __itkMahalanobisDistanceMembershipFunction_txx

#include "itkMahalanobisDistanceMembershipFunction.h"

namespace itk{ 
namespace Statistics{

template < class TVector >
MahalanobisDistanceMembershipFunction< TVector >
::MahalanobisDistanceMembershipFunction():
  m_NumberOfSamples(0),
  m_PreFactor(0),
  m_Epsilon( 1e-100 ),
  m_DoubleMax( 1e+20 )
{
  m_Mean.fill( 0.0f );
  m_Covariance.set_identity();
  m_InverseCovariance.set_identity();
  this->m_MeasurementVectorSize = 0;
}

template< class TVector >
void 
MahalanobisDistanceMembershipFunction< TVector >
::SetMeasurementVectorSize( const MeasurementVectorSizeType s )
{
  if( s == this->m_MeasurementVectorSize )
    {
    return;
    }
  
  if( this->m_MeasurementVectorSize != 0 )
    {  
    itkWarningMacro( << "Destructively resizing paramters of the DistanceToCentroidMembershipFunction." );
    }
  this->m_MeasurementVectorSize = s;
  m_Mean.set_size( s );
  this->Modified();
}

template < class TVector >
void 
MahalanobisDistanceMembershipFunction< TVector >
::SetMean(const MeanVectorType & mean)
{
  if( this->m_MeasurementVectorSize != 0 )
    {  
    if( mean.size() != this->m_MeasurementVectorSize )
      {
      itkExceptionMacro( << "Size of the centroid must be same as the length of"
          << " each measurement vector.");
      }
    }
  else
    {
    this->m_MeasurementVectorSize = mean.size();
    }

  m_Mean = mean ;
}


template < class TVector >
void 
MahalanobisDistanceMembershipFunction< TVector >
::SetMean(const Array< double > & mean)
{
  if( this->m_MeasurementVectorSize != 0 )
    {  
    if( mean.Size() != this->m_MeasurementVectorSize )
      {
      itkExceptionMacro( << "Size of the centroid must be same as the length of"
          << " each measurement vector.");
      }
    }
  else
    {
    this->m_MeasurementVectorSize = mean.Size();
    }

  m_Mean = dynamic_cast< MeanVectorType & >(const_cast< Array< double >& >(mean));
}


template < class TVector >
const typename
MahalanobisDistanceMembershipFunction< TVector >::MeanVectorType &
MahalanobisDistanceMembershipFunction< TVector >
::GetMean() const
{
  return m_Mean ;
}

template < class TVector >
void 
MahalanobisDistanceMembershipFunction< TVector >
::SetCovariance(const CovarianceMatrixType &cov)
{
  if( this->m_MeasurementVectorSize != 0 )
    {  
    if( cov.rows() != this->m_MeasurementVectorSize || 
        cov.cols() != this->m_MeasurementVectorSize )
      {
      itkExceptionMacro( << "Size of the centroid must be same as the length of"
          << " each measurement vector.");
      }
    }
  else
    {
    this->m_MeasurementVectorSize = cov.rows();
    }

  m_Covariance = cov; 
  this->CalculateInverseCovariance();
}



template < class TVector >
void 
MahalanobisDistanceMembershipFunction< TVector >
::SetInverseCovariance(const CovarianceMatrixType &invcov)
{
  if( this->m_MeasurementVectorSize != 0 )
    {  
    if( invcov.rows() != this->m_MeasurementVectorSize || 
        invcov.cols() != this->m_MeasurementVectorSize )
      {
      itkExceptionMacro( << "Size of the centroid must be same as the length of"
          << " each measurement vector.");
      }
    }
  else
    {
    this->m_MeasurementVectorSize = invcov.rows();
    }

  // use the inverse computation
  m_Covariance = invcov; 
  this->CalculateInverseCovariance();
  m_Covariance = m_InverseCovariance;
  m_InverseCovariance = invcov;
}



template < class TVector >
void
MahalanobisDistanceMembershipFunction< TVector >
::CalculateInverseCovariance() 
{

  // pack the cov matrix from in_model to tmp_cov_mat 
  double cov_sum = 0;
  for(unsigned int band_x = 0; band_x < m_Covariance.cols(); band_x++)
    { 
    for(unsigned int band_y = 0; band_y < m_Covariance.rows(); band_y++)
      {
      cov_sum += vnl_math_abs( m_Covariance[band_x][band_y] );
      }
    } 
  // check if it is a zero covariance, if it is, we make its
  // inverse as an identity matrix with diagonal elements as
  // a very large number; otherwise, inverse it 
  if( cov_sum < m_Epsilon ) 
    {
    m_InverseCovariance.set_size( m_Covariance.rows(), m_Covariance.cols() );
    m_InverseCovariance.set_identity();
    m_InverseCovariance *= m_DoubleMax;
    }
  else 
    {
    // check if num_bands == 1, if it is, we just use 1 to divide it
    if( m_Covariance.rows() < 2 ) 
      {
      m_InverseCovariance.set_size(1,1);
      m_InverseCovariance[0][0] = 1.0 / m_Covariance[0][0];
      }
    else 
      {
      m_InverseCovariance = vnl_matrix_inverse<double>(m_Covariance);
      }
    }// end inverse calculations

}// CalculateInverseCovariance()

template < class TVector >
double 
MahalanobisDistanceMembershipFunction< TVector >
::Evaluate(const MeasurementVectorType &measurement) const
{ 

  double temp;
  m_TempVec.set_size( 1, this->m_MeasurementVectorSize);
  m_TempMat.set_size( 1, this->m_MeasurementVectorSize);

  // Compute |y - mean |   
  for ( unsigned int i = 0; i < this->m_MeasurementVectorSize; i++ )
    {
    m_TempVec[0][i] = measurement[i] - m_Mean[i];
    }

  // Compute |y - mean | * inverse(cov) 
  m_TempMat= m_TempVec * m_InverseCovariance;

  // Compute |y - mean | * inverse(cov) * |y - mean|^T 
  temp = dot_product( m_TempMat.as_ref(), m_TempVec.as_ref() ); 
  
  return temp ;
}
  
template < class TVector >
void  
MahalanobisDistanceMembershipFunction< TVector >
::PrintSelf(std::ostream& os, Indent indent) const
{
  unsigned int i ;
  Superclass::PrintSelf(os,indent);

  if ( this->m_MeasurementVectorSize &&
       m_Mean.size() == this->m_MeasurementVectorSize )
    {
    os << indent << "Mean: [" ;
    for (i=0 ; (i + 1) < this->m_MeasurementVectorSize; i++)
      {
      os << m_Mean[i] << ", ";
      }
    os << m_Mean[i] << "]" << std::endl;
    }
  else
    {
    os << indent << "Mean: not set or size does not match" << std::endl ;
    }

  os << indent << "Number of Samples: " << m_NumberOfSamples << std::endl;
  os << indent << "Covariance:        " << std::endl;
  os << m_Covariance << std::endl;
  os << indent << "Inverse covariance:        " << std::endl;
  os << m_InverseCovariance << std::endl;
}
} // end namespace Statistics
} // end of namespace itk



#endif
