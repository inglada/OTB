/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkCannySegmentationLevelSetImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2003-09-10 14:28:28 $
  Version:   $Revision: 1.4 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkCannySegmentationLevelSetImageFilter_txx_
#define __itkCannySegmentationLevelSetImageFilter_txx_

#include "itkCannySegmentationLevelSetImageFilter.h"

namespace itk {

template <class TInputImage, class TFeatureImage, class TOutputPixelType>
CannySegmentationLevelSetImageFilter<TInputImage, TFeatureImage,
                                     TOutputPixelType>
::CannySegmentationLevelSetImageFilter()
{
  m_CannyFunction = CannyFunctionType::New();

  this->SetSegmentationFunction(m_CannyFunction.GetPointer());
}
  
}// end namespace itk




#endif
