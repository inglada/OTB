/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkBinaryMinMaxCurvatureFlowFunction.txx,v $
  Language:  C++
  Date:      $Date: 2008-10-20 21:28:19 $
  Version:   $Revision: 1.13 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkBinaryMinMaxCurvatureFlowFunction_txx
#define __itkBinaryMinMaxCurvatureFlowFunction_txx
#include "itkBinaryMinMaxCurvatureFlowFunction.h"

#include "vnl/vnl_math.h"
#include "itkNeighborhoodInnerProduct.h"

namespace itk {

/**
 * Constructor
 */
template<class TImage>
BinaryMinMaxCurvatureFlowFunction<TImage>
::BinaryMinMaxCurvatureFlowFunction()
{

  m_Threshold = 0.0;

}

/**
 * Update the solution at pixels which does not lie on the
 * data boundary.
 */
template<class TImage>
typename BinaryMinMaxCurvatureFlowFunction<TImage>::PixelType
BinaryMinMaxCurvatureFlowFunction<TImage>
::ComputeUpdate(const NeighborhoodType &it, void * globalData,
                const FloatOffsetType& offset)
{

  typedef CurvatureFlowFunction<TImage> CurvatureFlowFunctionType;
  PixelType update = this->CurvatureFlowFunctionType::ComputeUpdate(
    it, globalData, offset );
  
  if ( update == 0.0 )
    {
    return update;
    }


  NeighborhoodInnerProduct<ImageType> innerProduct;
  PixelType avgValue = innerProduct( it, this->m_StencilOperator );

  if ( avgValue < m_Threshold )
    {
    return ( vnl_math_min( update, NumericTraits<PixelType>::Zero ) );
    }
  else
    {
    return ( vnl_math_max( update, NumericTraits<PixelType>::Zero ) );
    }

}

} // end namespace itk

#endif
