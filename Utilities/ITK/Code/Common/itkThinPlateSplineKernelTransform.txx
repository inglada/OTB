/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkThinPlateSplineKernelTransform.txx,v $
  Language:  C++
  Date:      $Date: 2004/12/12 22:05:03 $
  Version:   $Revision: 1.16 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itkThinPlateSplineKernelTransform_txx
#define _itkThinPlateSplineKernelTransform_txx
#include "itkThinPlateSplineKernelTransform.h"

namespace itk
{

template <class TScalarType, unsigned int NDimensions>
const typename ThinPlateSplineKernelTransform<TScalarType, NDimensions>::GMatrixType &
ThinPlateSplineKernelTransform<TScalarType, NDimensions>::
ComputeG(const InputVectorType & x) const
{

  const TScalarType r = x.GetNorm();
  this->m_GMatrix.fill( NumericTraits< TScalarType >::Zero );
  for(unsigned int i=0; i<NDimensions; i++)
    {
    this->m_GMatrix[i][i] = r;
    }
  return this->m_GMatrix;
}



template <class TScalarType, unsigned int NDimensions>
void
ThinPlateSplineKernelTransform<TScalarType, NDimensions>::
ComputeDeformationContribution( const InputPointType  & thisPoint,
                                OutputPointType & result     ) const
{

  unsigned long numberOfLandmarks = this->m_SourceLandmarks->GetNumberOfPoints();

  PointsIterator sp  = this->m_SourceLandmarks->GetPoints()->Begin();

  for(unsigned int lnd=0; lnd < numberOfLandmarks; lnd++ )
    {
    InputVectorType position = thisPoint - sp->Value();
    const TScalarType r = position.GetNorm();

    for(unsigned int odim=0; odim < NDimensions; odim++ )
      {
      result[ odim ] += r * this->m_DMatrix(odim,lnd);
      }
    ++sp;
    }

}


} // namespace itk
#endif
