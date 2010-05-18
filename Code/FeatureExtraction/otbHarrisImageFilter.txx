/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbHarrisImageFilter_txx
#define __otbHarrisImageFilter_txx

#include "otbHarrisImageFilter.h"

namespace otb
{

/**
 *
 */
template <class TInputImage, class TOutputImage>
HarrisImageFilter<TInputImage, TOutputImage>
::HarrisImageFilter()
{
  m_SigmaD = 1.0;
  m_SigmaI = 1.0;
  m_Alpha  = 1.0;

  m_HessianFilter         = HessianFilterType::New();
  m_GaussianFilter        = GaussianFilterType::New();
  m_HessianToScalarFilter = HessianToScalarFilterType::New();
  m_MultiplyScalarFilter  = MultiplyScalarFilterType::New();
}

template <class TInputImage, class TOutputImage>
void
HarrisImageFilter<TInputImage, TOutputImage>
::GenerateData()
{
  m_HessianFilter->SetInput(this->GetInput());
  m_HessianFilter->SetSigma(this->m_SigmaD);
  m_HessianFilter->SetNormalizeAcrossScale(false);

  m_GaussianFilter->SetInput(m_HessianFilter->GetOutput());
  m_GaussianFilter->SetSigma(this->m_SigmaI);
  m_GaussianFilter->SetOrder(GaussianFilterType::ZeroOrder);
  m_GaussianFilter->SetNormalizeAcrossScale(false);

  m_HessianToScalarFilter->SetInput(m_GaussianFilter->GetOutput());
  m_HessianToScalarFilter->SetAlpha(this->m_Alpha);

  m_MultiplyScalarFilter->SetInput(m_HessianToScalarFilter->GetOutput());
  m_MultiplyScalarFilter->SetCoef((m_SigmaD * m_SigmaD));

  m_MultiplyScalarFilter->GraftOutput(this->GetOutput());
  m_MultiplyScalarFilter->Update();
  this->GraftOutput(m_MultiplyScalarFilter->GetOutput());
}

/**
 * Standard "PrintSelf" method
 */
template <class TInputImage, class TOutput>
void
HarrisImageFilter<TInputImage, TOutput>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Sigma_D : " << this->m_SigmaD << std::endl;
  os << indent << "Sigma_I : " << this->m_SigmaI << std::endl;
  os << indent << "Alpha   : " << this->m_Alpha  << std::endl;
}

} // end namespace otb

#endif
