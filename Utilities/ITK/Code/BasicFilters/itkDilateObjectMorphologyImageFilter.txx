/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkDilateObjectMorphologyImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2009-04-28 14:36:19 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkDilateObjectMorphologyImageFilter_txx
#define __itkDilateObjectMorphologyImageFilter_txx

#include "itkDilateObjectMorphologyImageFilter.h"

namespace itk {

template<class TInputImage, class TOutputImage, class TKernel>
DilateObjectMorphologyImageFilter<TInputImage, TOutputImage, TKernel>
::DilateObjectMorphologyImageFilter()
{
  m_DilateBoundaryCondition.SetConstant( 
                                NumericTraits<PixelType>::NonpositiveMin() );
  this->OverrideBoundaryCondition( &m_DilateBoundaryCondition );

}


template<class TInputImage, class TOutputImage, class TKernel>
void
DilateObjectMorphologyImageFilter<TInputImage, TOutputImage, TKernel>
::Evaluate(OutputNeighborhoodIteratorType &nit, const KernelType &kernel)
{
  unsigned int i;
  KernelIteratorType kernel_it;
  const KernelIteratorType kernelEnd = kernel.End();

  bool valid = true;
  for( i=0, kernel_it=kernel.Begin(); kernel_it<kernelEnd; ++kernel_it, ++i )
    {
    if( *kernel_it > NumericTraits< KernelPixelType >::Zero )
      {
      nit.SetPixel(i, this->GetObjectValue(), valid);
      }
    }
} 

template<class TInputImage, class TOutputImage, class TKernel>
void
DilateObjectMorphologyImageFilter<TInputImage, TOutputImage, TKernel>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

}// end namespace itk
#endif
