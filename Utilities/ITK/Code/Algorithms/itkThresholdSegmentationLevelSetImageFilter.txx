/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkThresholdSegmentationLevelSetImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2003-09-10 14:28:39 $
  Version:   $Revision: 1.11 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkThresholdSegmentationLevelSetImageFilter_txx_
#define __itkThresholdSegmentationLevelSetImageFilter_txx_

#include "itkThresholdSegmentationLevelSetImageFilter.h"

namespace itk {


template <class TInputImage, class TFeatureImage, class TOutputType>
ThresholdSegmentationLevelSetImageFilter<TInputImage, TFeatureImage, TOutputType>
::ThresholdSegmentationLevelSetImageFilter()
{
  m_ThresholdFunction = ThresholdFunctionType::New();
  m_ThresholdFunction->SetUpperThreshold(0);
  m_ThresholdFunction->SetLowerThreshold(0);

  this->SetSegmentationFunction(m_ThresholdFunction);
}
 
template <class TInputImage, class TFeatureImage, class TOutputType>
void
ThresholdSegmentationLevelSetImageFilter<TInputImage, TFeatureImage, TOutputType>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << "ThresholdFunction: " << m_ThresholdFunction;
}


}// end namespace itk




#endif
