/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkMahalanobisDistanceThresholdImageFunction.txx,v $
  Language:  C++
  Date:      $Date: 2007-01-30 19:49:52 $
  Version:   $Revision: 1.8 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkMahalanobisDistanceThresholdImageFunction_txx
#define __itkMahalanobisDistanceThresholdImageFunction_txx

#include "itkMahalanobisDistanceThresholdImageFunction.h"

namespace itk
{

template <class TInputImage, class TCoordRep>
MahalanobisDistanceThresholdImageFunction<TInputImage,TCoordRep>
::MahalanobisDistanceThresholdImageFunction()
{
  m_Threshold = NumericTraits<double>::Zero;
  m_MahalanobisDistanceMembershipFunction = 
          MahalanobisDistanceFunctionType::New();
}


template <class TInputImage, class TCoordRep>
void
MahalanobisDistanceThresholdImageFunction<TInputImage,TCoordRep>
::SetMean(const MeanVectorType & mean)
{
  m_MahalanobisDistanceMembershipFunction->SetMean( mean );
}



template <class TInputImage, class TCoordRep>
void
MahalanobisDistanceThresholdImageFunction<TInputImage,TCoordRep>
::SetCovariance(const CovarianceMatrixType & covariance)
{
  m_MahalanobisDistanceMembershipFunction->SetCovariance( covariance );
}



template <class TInputImage, class TCoordRep>
const typename 
MahalanobisDistanceThresholdImageFunction<TInputImage,TCoordRep>::MeanVectorType &
MahalanobisDistanceThresholdImageFunction<TInputImage,TCoordRep>
::GetMean() const
{
  return m_MahalanobisDistanceMembershipFunction->GetMean();
}


template <class TInputImage, class TCoordRep>
const typename 
MahalanobisDistanceThresholdImageFunction<TInputImage,TCoordRep>::CovarianceMatrixType &
MahalanobisDistanceThresholdImageFunction<TInputImage,TCoordRep>
::GetCovariance() const
{
  return m_MahalanobisDistanceMembershipFunction->GetCovariance();
}



template <class TInputImage, class TCoordRep>
bool 
MahalanobisDistanceThresholdImageFunction<TInputImage,TCoordRep>
::Evaluate( const PointType& point ) const
{
  IndexType index;
  this->ConvertPointToNearestIndex( point, index );
  return ( this->EvaluateAtIndex( index ) );
}




template <class TInputImage, class TCoordRep>
bool 
MahalanobisDistanceThresholdImageFunction<TInputImage,TCoordRep>
::EvaluateAtContinuousIndex( const ContinuousIndexType & index ) const
{
  IndexType nindex;
  this->ConvertContinuousIndexToNearestIndex (index, nindex);
  return this->EvaluateAtIndex(nindex);
}



template <class TInputImage, class TCoordRep>
bool 
MahalanobisDistanceThresholdImageFunction<TInputImage,TCoordRep>
::EvaluateAtIndex( const IndexType & index ) const
{
  double mahalanobisDistance = this->EvaluateDistanceAtIndex( index );
  return ( mahalanobisDistance <= m_Threshold );
}



template <class TInputImage, class TCoordRep>
double 
MahalanobisDistanceThresholdImageFunction<TInputImage,TCoordRep>
::EvaluateDistance( const PointType& point ) const
{
  IndexType index;
  this->ConvertPointToNearestIndex( point, index );
  const double mahalanobisDistance = this->EvaluateDistanceAtIndex( index );
  return  mahalanobisDistance;
}


template <class TInputImage, class TCoordRep>
double 
MahalanobisDistanceThresholdImageFunction<TInputImage,TCoordRep>
::EvaluateDistanceAtIndex( const IndexType& index ) const
{
  double mahalanobisDistanceSquared = 
    m_MahalanobisDistanceMembershipFunction->Evaluate( 
                                  this->GetInputImage()->GetPixel( index ) );

  double mahalanobisDistance;

  // Deal with cases that are barely negative. 
  // In theory they should never appear, but
  // they may happen and would produce NaNs
  // in the vcl_sqrt
  if( mahalanobisDistanceSquared < 0.0 )
    {
    mahalanobisDistance = 0.0;
    }
  else
    {
    mahalanobisDistance = vcl_sqrt( mahalanobisDistanceSquared );
    }

  return  mahalanobisDistance;
}





template <class TInputImage, class TCoordRep>
void 
MahalanobisDistanceThresholdImageFunction<TInputImage,TCoordRep>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf( os, indent );

  os << indent << "Threshold: " << m_Threshold << std::endl;
  os << indent << "MahalanobisDistanceMembershipFunction: " << m_MahalanobisDistanceMembershipFunction << std::endl;
}

} // end namespace itk

#endif
