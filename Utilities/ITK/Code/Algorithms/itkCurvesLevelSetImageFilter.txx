/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkCurvesLevelSetImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2004/04/08 12:04:39 $
  Version:   $Revision: 1.3 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkCurvesLevelSetImageFilter_txx_
#define __itkCurvesLevelSetImageFilter_txx_

#include "itkCurvesLevelSetImageFilter.h"

namespace itk {


template <class TInputImage, class TFeatureImage, class TOutputType>
CurvesLevelSetImageFilter<TInputImage, TFeatureImage, TOutputType>
::CurvesLevelSetImageFilter()
{
  /* Instantiate a geodesic active contour function and set it as the segmentation function. */
  m_CurvesFunction = CurvesFunctionType::New();

  this->SetSegmentationFunction( m_CurvesFunction );

  /* Use negative features by default. */
  this->ReverseExpansionDirectionOff();

  /* Turn off interpolation. */
  this->InterpolateSurfaceLocationOff();
}
 
template <class TInputImage, class TFeatureImage, class TOutputType>
void
CurvesLevelSetImageFilter<TInputImage, TFeatureImage, TOutputType>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "CurvesFunction: \n";
  m_CurvesFunction->Print(os, indent.GetNextIndent());
}


}// end namespace itk




#endif
