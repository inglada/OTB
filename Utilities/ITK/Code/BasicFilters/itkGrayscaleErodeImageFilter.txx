/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkGrayscaleErodeImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2008-10-16 16:45:08 $
  Version:   $Revision: 1.15 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkGrayscaleErodeImageFilter_txx
#define __itkGrayscaleErodeImageFilter_txx

// First make sure that the configuration is available.
// This line can be removed once the optimized versions
// gets integrated into the main directories.
#include "itkConfigure.h"

#ifdef ITK_USE_CONSOLIDATED_MORPHOLOGY
#include "itkOptGrayscaleErodeImageFilter.txx"
#else


#include "itkGrayscaleErodeImageFilter.h"

namespace itk {

template<class TInputImage, class TOutputImage, class TKernel>
GrayscaleErodeImageFilter<TInputImage, TOutputImage, TKernel>
::GrayscaleErodeImageFilter()
{
  m_ErodeBoundaryCondition.SetConstant( NumericTraits<PixelType>::max() );
  this->OverrideBoundaryCondition( &m_ErodeBoundaryCondition );
}

template<class TInputImage, class TOutputImage, class TKernel>
typename GrayscaleErodeImageFilter<TInputImage, TOutputImage, TKernel>::PixelType
GrayscaleErodeImageFilter<TInputImage, TOutputImage, TKernel>
::Evaluate(const NeighborhoodIteratorType &nit,
           const KernelIteratorType kernelBegin,
           const KernelIteratorType kernelEnd)
{
  unsigned int i;
  PixelType min = NumericTraits<PixelType>::max();
  PixelType temp;

  KernelIteratorType kernel_it;

  for (i=0, kernel_it=kernelBegin; kernel_it<kernelEnd; ++kernel_it, ++i)
    {
    // if structuring element is positive, use the pixel under that element
    // in the image
    if (*kernel_it > 0)
      {
      // note we use GetPixel() on the NeighborhoodIterator in order
      // to respect boundary conditions.
      temp = nit.GetPixel(i);

      if (temp < min)
        {
        min = temp;
        }
      }
    }
  
  return min;
} 
  
}// end namespace itk
#endif

#endif
