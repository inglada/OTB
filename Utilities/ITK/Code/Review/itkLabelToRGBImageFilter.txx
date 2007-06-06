/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkLabelToRGBImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2007/02/25 14:28:08 $
  Version:   $Revision: 1.2 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkLabelToRGBImageFilter_txx
#define __itkLabelToRGBImageFilter_txx

#include "itkLabelToRGBImageFilter.h"

namespace itk
{

/**
 *
 */
template <class TLabelImage, class TOutputImage>
LabelToRGBImageFilter<TLabelImage, TOutputImage>
::LabelToRGBImageFilter()
{
  m_BackgroundValue = NumericTraits<LabelPixelType>::Zero;
  m_UseBackground = false;
}

template <class TLabelImage, class TOutputImage>
void
LabelToRGBImageFilter<TLabelImage, TOutputImage>
::BeforeThreadedGenerateData()
{
  this->GetFunctor().SetBackgroundValue(m_BackgroundValue);
  this->GetFunctor().SetUseBackground(m_UseBackground);
}

/**
 *
 */
template <class TLabelImage, class TOutputImage>
void 
LabelToRGBImageFilter<TLabelImage, TOutputImage>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);

  os << indent << "BackgroundValue: "
     << static_cast<typename NumericTraits<LabelPixelType>::PrintType>(m_BackgroundValue)
     << std::endl
     << indent << "UseBackground: "
     << static_cast<typename NumericTraits<bool>::PrintType>(m_UseBackground)
     << std::endl;
}


} // end namespace itk

#endif
