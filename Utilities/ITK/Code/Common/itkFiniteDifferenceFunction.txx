/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkFiniteDifferenceFunction.txx,v $
  Language:  C++
  Date:      $Date: 2008-03-05 17:23:43 $
  Version:   $Revision: 1.9 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkFiniteDifferenceFunction_txx_
#define __itkFiniteDifferenceFunction_txx_

#include "itkFiniteDifferenceFunction.h"

namespace itk
{
template<class TImageType>
void
FiniteDifferenceFunction<TImageType>::
PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Radius: " << m_Radius << std::endl;
  os << indent << "ScaleCoefficients: " << m_ScaleCoefficients;
}

template<class TImageType>
const typename FiniteDifferenceFunction<TImageType>::NeighborhoodScalesType
FiniteDifferenceFunction<TImageType>::
ComputeNeighborhoodScales() const
{
  NeighborhoodScalesType neighborhoodScales;
  neighborhoodScales.Fill(0.0);
  typedef typename NeighborhoodScalesType::ComponentType NeighborhoodScaleType;
  for(int i=0; i<ImageDimension; i++)
    {
    if (this->m_Radius[i] > 0)
      {
      neighborhoodScales[i] = this->m_ScaleCoefficients[i] / this->m_Radius[i];
      }
    }
  return neighborhoodScales;
}

} // end namespace itk


#endif
