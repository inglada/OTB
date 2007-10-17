/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkErodeObjectMorphologyImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2005/01/16 15:10:52 $
  Version:   $Revision: 1.5 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkErodeObjectMorphologyImageFilter_txx
#define __itkErodeObjectMorphologyImageFilter_txx

#include "itkErodeObjectMorphologyImageFilter.h"

namespace itk {

template<class TInputImage, class TOutputImage, class TKernel>
ErodeObjectMorphologyImageFilter<TInputImage, TOutputImage, TKernel>
::ErodeObjectMorphologyImageFilter()
{
  m_BackgroundValue = NumericTraits< PixelType >::Zero;

  m_ErodeBoundaryCondition.SetConstant( NumericTraits<PixelType>::max() );
  this->OverrideBoundaryCondition( &m_ErodeBoundaryCondition );

}

template<class TInputImage, class TOutputImage, class TKernel>
void
ErodeObjectMorphologyImageFilter<TInputImage, TOutputImage, TKernel>
::Evaluate(OutputNeighborhoodIteratorType &nit,
           const KernelType &kernel)
{
  unsigned int i;
  KernelIteratorType kernel_it;
  const KernelIteratorType kernelEnd = kernel.End();

  bool valid = true;
  for (i=0, kernel_it=kernel.Begin(); kernel_it<kernelEnd; ++kernel_it, ++i)
    {
    if(*kernel_it>0)
      {
      nit.SetPixel(i, m_BackgroundValue, valid);
      }
    }
} 

template<class TInputImage, class TOutputImage, class TKernel>
void
ErodeObjectMorphologyImageFilter<TInputImage, TOutputImage, TKernel>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "BackgroundValue : " << m_BackgroundValue << std::endl;
}

}// end namespace itk
#endif
