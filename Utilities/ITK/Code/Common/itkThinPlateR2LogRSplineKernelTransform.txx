/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkThinPlateR2LogRSplineKernelTransform.txx,v $
  Language:  C++
  Date:      $Date: 2004/12/12 22:05:03 $
  Version:   $Revision: 1.7 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itkThinPlateR2LogRSplineKernelTransform_txx
#define _itkThinPlateR2LogRSplineKernelTransform_txx
#include "itkThinPlateR2LogRSplineKernelTransform.h"

namespace itk
{

template <class TScalarType, unsigned int NDimensions>
const typename ThinPlateR2LogRSplineKernelTransform<TScalarType, NDimensions>::GMatrixType &
ThinPlateR2LogRSplineKernelTransform<TScalarType, NDimensions>::
ComputeG(const InputVectorType & x) const
{

  const TScalarType r = x.GetNorm();
  this->m_GMatrix.fill( NumericTraits< TScalarType >::Zero );
  const TScalarType R2logR = 
    ( r > 1e-8 )? r * r * log( r ) : NumericTraits<TScalarType>::Zero;

  this->m_GMatrix.fill_diagonal( R2logR );
 
  return this->m_GMatrix;
}


template <class TScalarType, unsigned int NDimensions>
void
ThinPlateR2LogRSplineKernelTransform<TScalarType, NDimensions>::
ComputeDeformationContribution( const InputPointType  & thisPoint,
                                OutputPointType & result     ) const
{

  unsigned long numberOfLandmarks = this->m_SourceLandmarks->GetNumberOfPoints();

  PointsIterator sp  = this->m_SourceLandmarks->GetPoints()->Begin();

  for(unsigned int lnd=0; lnd < numberOfLandmarks; lnd++ )
    {
    InputVectorType position = thisPoint - sp->Value();
    const TScalarType r = position.GetNorm();
    const TScalarType R2logR = 
      ( r > 1e-8 )? r * r * log( r ) : NumericTraits<TScalarType>::Zero;
    for(unsigned int odim=0; odim < NDimensions; odim++ )
      {
      result[ odim ] += R2logR * this->m_DMatrix(odim,lnd);
      }
    ++sp;
    }

}


} // namespace itk
#endif
