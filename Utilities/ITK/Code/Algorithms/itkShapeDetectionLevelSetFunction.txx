/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkShapeDetectionLevelSetFunction.txx,v $
  Language:  C++
  Date:      $Date: 2003-09-10 14:28:38 $
  Version:   $Revision: 1.3 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkShapeDetectionLevelSetFunction_txx_
#define __itkShapeDetectionLevelSetFunction_txx_

#include "itkShapeDetectionLevelSetFunction.h"
#include "itkImageRegionIterator.h"

namespace itk {

template <class TImageType, class TFeatureImageType>
void ShapeDetectionLevelSetFunction<TImageType, TFeatureImageType>
::CalculateSpeedImage()
{
  /* copy the feature image into the speed image */
  ImageRegionConstIterator<FeatureImageType>
    fit(this->GetFeatureImage(), this->GetFeatureImage()->GetRequestedRegion());
  ImageRegionIterator<ImageType>
    sit(this->GetSpeedImage(), this->GetFeatureImage()->GetRequestedRegion());

  for ( fit = fit.Begin(), sit = sit.Begin(); ! fit.IsAtEnd(); ++sit, ++fit)
    {
    sit.Set( static_cast<ScalarValueType>( fit.Get() ) );
    }
}


} // end namespace itk


#endif
