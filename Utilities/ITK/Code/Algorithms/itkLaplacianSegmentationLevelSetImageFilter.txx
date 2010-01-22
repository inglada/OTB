/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkLaplacianSegmentationLevelSetImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2009-01-24 20:02:57 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkLaplacianSegmentationLevelSetImageFilter_txx
#define __itkLaplacianSegmentationLevelSetImageFilter_txx

#include "itkLaplacianSegmentationLevelSetImageFilter.h"

namespace itk {

template <class TInputImage, class TFeatureImage, class TOutputPixelType>
LaplacianSegmentationLevelSetImageFilter<TInputImage, TFeatureImage,
                                         TOutputPixelType>
::LaplacianSegmentationLevelSetImageFilter()
{
  m_LaplacianFunction = LaplacianFunctionType::New();

  this->SetSegmentationFunction(m_LaplacianFunction);
}
  
template <class TInputImage, class TFeatureImage, class TOutputPixelType>
void
LaplacianSegmentationLevelSetImageFilter<TInputImage, TFeatureImage,
                                         TOutputPixelType>
::PrintSelf(std::ostream &, Indent) const
{
  //   Superclass::PrintSelf(os, indent);
}


}// end namespace itk

#endif
