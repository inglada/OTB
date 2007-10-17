/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkAnisotropicFourthOrderLevelSetImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2007/04/20 13:36:35 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkAnisotropicFourthOrderLevelSetImageFilter_txx
#define __itkAnisotropicFourthOrderLevelSetImageFilter_txx

#include "itkAnisotropicFourthOrderLevelSetImageFilter.h"

namespace itk {

template <class TInputImage, class TOutputImage>
AnisotropicFourthOrderLevelSetImageFilter<TInputImage, TOutputImage>
::AnisotropicFourthOrderLevelSetImageFilter()
{
  RadiusType radius;
  for (unsigned int j=0; j<TInputImage::ImageDimension;j++)
    {
    radius[j] = 1;
    }
  
  m_Function=FunctionType::New();
  this->SetLevelSetFunction(m_Function);
  this->SetNumberOfLayers(this->GetMinimumNumberOfLayers());

  // explicitly set other parameters
  this->SetNormalProcessType (1);  // anisotropic diffusion
  this->SetNormalProcessConductance (0.2);  
  this->SetMaxNormalIteration(25);
  this->SetMaxRefitIteration(100);
  m_MaxFilterIteration = 1000;
  m_Function->Initialize(radius);
}

template <class TInputImage, class TOutputImage>
void
AnisotropicFourthOrderLevelSetImageFilter<TInputImage, TOutputImage>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "MaxFilterIteration: " << m_MaxFilterIteration << std::endl;
}

} // end namespace itk

#endif
