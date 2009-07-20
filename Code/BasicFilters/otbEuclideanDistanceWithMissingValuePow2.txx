/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Copyright (c) Institut Telecom / Telecom Bretagne. All rights reserved. 
  See ITCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __otbEuclideanDistanceWithMissingValuePow2_txx
#define __otbEuclideanDistanceWithMissingValuePow2_txx

#include "itkNumericTraits.h"

namespace otb {

namespace Statistics {

template< class TVector >
inline double
EuclideanDistanceWithMissingValuePow2< TVector >
::Evaluate(const TVector &x1, const TVector &x2) const
{
  if( itk::MeasurementVectorTraits::GetLength( x1 ) != 
    itk::MeasurementVectorTraits::GetLength( x2 ) )
  {
    itkExceptionMacro( << "Vector lengths must be equal." );
  }

  double temp, distance = itk::NumericTraits< double >::Zero ;

  for(unsigned int i = 0 ; i < x1.Size(); i++ )
  {
    if ( !IsMissingValue( x1[i] ) && !IsMissingValue( x2[i] ) )
    {
      temp = x1[i] - x2[i] ;
      distance += temp * temp ;
    }
  }

  return distance ;
}

template< class TVector >
inline double
EuclideanDistanceWithMissingValuePow2< TVector >
::Evaluate(const TVector &x) const
{
  MeasurementVectorSizeType 
    measurementVectorSize = this->GetMeasurementVectorSize();
  if(measurementVectorSize == 0) 
  {
    itkExceptionMacro( << "Please set the MeasurementVectorSize first" );
  }
  itk::MeasurementVectorTraits::Assert( this->m_Origin, measurementVectorSize, 
    "EuclideanDistance::Evaluate Origin and input vector have different lengths");

  double temp, distance = itk::NumericTraits< double >::Zero ;

  for(unsigned int i = 0 ; i < measurementVectorSize ; i++ )
  {
    if ( !IsMissingValue( this->GetOrigin()[i] ) && !IsMissingValue( x[i] ) )
    {
      temp = this->GetOrigin()[i] - x[i] ;
      distance += temp * temp ;
    }
  }

  return distance ;
}

template< class TVector >
inline double
EuclideanDistanceWithMissingValuePow2< TVector >
::Evaluate( const ValueType &a, const ValueType &b ) const
{
  // FIXME throw NaN exception ??
  if ( IsMissingValue( a ) || IsMissingValue( b ) )
    return 0.0;

  double temp = a - b ;
  return temp * temp ;
}

#if defined(_MSC_VER) /* Microsoft Visual C++ */
#include <float.h>

template< class TVector >
/*static */
bool
EuclideanDistanceWithMissingValuePow2< TVector >
::IsMissingValue ( const ValueType & v) 
{
  return static_cast<bool>( _isnan( const_cast<double>( v ) ) ); 
}

#elif HAVE_IEEE_COMPARISONS

template< class TVector >
/*static */
bool
EuclideanDistanceWithMissingValuePow2< TVector >
::IsMissingValue ( const ValueType & v) 
{
  return (v!=v);
}

#else

template< class TVector >
/*static */
bool
EuclideanDistanceWithMissingValuePow2< TVector >
::IsMissingValue ( const ValueType & v) 
{
  return static_cast<bool>( isnan(v) );
}

#endif 

template< class TVector >
/* static */
void
EuclideanDistanceWithMissingValuePow2< TVector >
::SetToMissingValue ( ValueType & v )
{
  double zero = 0.0;
  v = static_cast<ValueType>( 0.0/zero );
}


} // end namespace statistics

} // end namespace otb

#endif



