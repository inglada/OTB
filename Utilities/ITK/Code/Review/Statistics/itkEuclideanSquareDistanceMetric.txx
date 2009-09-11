/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkEuclideanSquareDistanceMetric.txx,v $
  Language:  C++
  Date:      $Date: 2009-05-23 04:17:05 $
  Version:   $Revision: 1.2 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkEuclideanSquareDistanceMetric_txx
#define __itkEuclideanSquareDistanceMetric_txx


namespace itk  { 
namespace Statistics  {

template< class TVector >
inline double
EuclideanSquareDistanceMetric< TVector >
::Evaluate(const MeasurementVectorType &x) const
{
  MeasurementVectorSizeType 
    measurementVectorSize = this->GetMeasurementVectorSize();
  if(measurementVectorSize == 0) 
    {
    itkExceptionMacro( << "Please set the MeasurementVectorSize first" );
    }
  MeasurementVectorTraits::Assert( this->GetOrigin(),measurementVectorSize, 
    "EuclideanSquareDistanceMetric::Evaluate Origin and input vector have different lengths");
  
  double temp, distance = NumericTraits< double >::Zero;
  
  for(unsigned int i = 0; i < measurementVectorSize; i++)
    {
    temp = this->GetOrigin()[i] - x[i];
    distance += temp * temp;
    }
  
  return distance;
}

template< class TVector >
inline double
EuclideanSquareDistanceMetric< TVector >
::Evaluate(const MeasurementVectorType &x1, const MeasurementVectorType &x2) const
{
  MeasurementVectorSizeType measurementVectorSize = MeasurementVectorTraits::GetLength( x1 ); 
  if (measurementVectorSize != MeasurementVectorTraits::GetLength(x2))
    {
    itkExceptionMacro( << "EuclideanSquareDistanceMetric:: The two measurement vectors have unequal size" );
    }

  double temp, distance = NumericTraits< double >::Zero;
  for(unsigned int i = 0; i < measurementVectorSize; i++)
    {
    temp = x1[i] - x2[i];
    distance += temp * temp;
    }
  
  return distance;
}


} // end of namespace Statistics 
} // end of namespace itk

#endif
