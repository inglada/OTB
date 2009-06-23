/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkOptWhiteTopHatImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2009-01-08 16:03:55 $
  Version:   $Revision: 1.5 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkOptWhiteTopHatImageFilter_txx
#define __itkOptWhiteTopHatImageFilter_txx

#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkWhiteTopHatImageFilter.h"
#include "itkGrayscaleMorphologicalOpeningImageFilter.h"
#include "itkSubtractImageFilter.h"
#include "itkProgressAccumulator.h"


namespace itk {

template <class TInputImage, class TOutputImage, class TKernel>
WhiteTopHatImageFilter<TInputImage, TOutputImage, TKernel>
::WhiteTopHatImageFilter()
{
  m_SafeBorder = true;
  m_Algorithm = HISTO;
  m_ForceAlgorithm = false;
}

template <class TInputImage, class TOutputImage, class TKernel>
void 
WhiteTopHatImageFilter<TInputImage, TOutputImage, TKernel>
::GenerateData()
{
  // Create a process accumulator for tracking the progress of this minipipeline
  ProgressAccumulator::Pointer progress = ProgressAccumulator::New();
  progress->SetMiniPipelineFilter(this);

  // Allocate the output
  this->AllocateOutputs();
  
  // Delegate to an opening filter.
  typename GrayscaleMorphologicalOpeningImageFilter<TInputImage, TInputImage, TKernel>::Pointer
    open = GrayscaleMorphologicalOpeningImageFilter<TInputImage, TInputImage, TKernel>::New();

  open->SetInput( this->GetInput() );
  open->SetKernel( this->GetKernel() );
  open->SetSafeBorder( m_SafeBorder );
  if( m_ForceAlgorithm )
    {
    open->SetAlgorithm( m_Algorithm );
    }
  else
    {
    m_Algorithm = open->GetAlgorithm();
    }
  
  // Need to subtract the opened image from the input
  typename SubtractImageFilter<TInputImage, TInputImage, TOutputImage>::Pointer
    subtract=SubtractImageFilter<TInputImage,TInputImage,TOutputImage>::New();

  subtract->SetInput1( this->GetInput() );
  subtract->SetInput2( open->GetOutput() );

  // graft our output to the subtract filter to force the proper regions
  // to be generated
  subtract->GraftOutput( this->GetOutput() );

  // run the algorithm
  progress->RegisterInternalFilter(open,.9f);
  progress->RegisterInternalFilter(subtract,.1f);

  subtract->Update();

  // graft the output of the subtract filter back onto this filter's
  // output. this is needed to get the appropriate regions passed
  // back.
  this->GraftOutput( subtract->GetOutput() );

}

template<class TInputImage, class TOutputImage, class TKernel>
void
WhiteTopHatImageFilter<TInputImage, TOutputImage, TKernel>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "Algorithm: " << m_Algorithm << std::endl;
  os << indent << "SafeBorder: " << m_SafeBorder << std::endl;
  os << indent << "ForceAlgorithm: " << m_ForceAlgorithm << std::endl;
}

}// end namespace itk
#endif
