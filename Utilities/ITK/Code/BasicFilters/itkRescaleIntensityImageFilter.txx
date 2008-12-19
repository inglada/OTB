/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkRescaleIntensityImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2008-10-17 16:30:53 $
  Version:   $Revision: 1.12 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

  Portions of this code are covered under the VTK copyright.
  See VTKCopyright.txt or http://www.kitware.com/VTKCopyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkRescaleIntensityImageFilter_txx
#define __itkRescaleIntensityImageFilter_txx

#include "itkRescaleIntensityImageFilter.h"
#include "itkMinimumMaximumImageCalculator.h"

namespace itk
{

/**
 *
 */
template <class TInputImage, class TOutputImage>
RescaleIntensityImageFilter<TInputImage, TOutputImage>
::RescaleIntensityImageFilter()
{
  m_OutputMaximum   = NumericTraits<OutputPixelType>::max();
  m_OutputMinimum   = NumericTraits<OutputPixelType>::NonpositiveMin();

  m_InputMaximum   = NumericTraits<InputPixelType>::Zero;
  m_InputMinimum   = NumericTraits<InputPixelType>::max();
  
  m_Scale = 1.0;
  m_Shift = 0.0;
}


/**
 *
 */
template <class TInputImage, class TOutputImage>
void 
RescaleIntensityImageFilter<TInputImage, TOutputImage>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);

  os << indent << "Output Minimum: "
     << static_cast<typename NumericTraits<OutputPixelType>::PrintType>(m_OutputMinimum)
     << std::endl;
  os << indent << "Output Maximum: "
     << static_cast<typename NumericTraits<OutputPixelType>::PrintType>(m_OutputMaximum)
     << std::endl;
}

/**
 *
 */
template <class TInputImage, class TOutputImage>
void 
RescaleIntensityImageFilter<TInputImage, TOutputImage>
::BeforeThreadedGenerateData()
{

  if ( m_OutputMinimum > m_OutputMaximum )
    {
    itkExceptionMacro(<<"Minimum output value cannot be greater than Maximum output value.");
    return;
    }

  typedef MinimumMaximumImageCalculator< TInputImage >  CalculatorType;
  
  typename CalculatorType::Pointer calculator = CalculatorType::New();

  calculator->SetImage( this->GetInput() );

  calculator->Compute();

  m_InputMinimum = calculator->GetMinimum();
  m_InputMaximum = calculator->GetMaximum();

  if (m_InputMinimum != m_InputMaximum)
    {
    m_Scale = 
      (static_cast<RealType>( m_OutputMaximum )
       - static_cast<RealType>( m_OutputMinimum )) /
      (static_cast<RealType>( m_InputMaximum )
       - static_cast<RealType>( m_InputMinimum ));
    }
  else if (m_InputMaximum != NumericTraits<InputPixelType>::Zero)
    {
    m_Scale = 
      (static_cast<RealType>( m_OutputMaximum )
       - static_cast<RealType>( m_OutputMinimum )) /
      static_cast<RealType>( m_InputMaximum );
    }
  else
    {
    m_Scale = 0.0; 
    }

  m_Shift =
    static_cast<RealType>( m_OutputMinimum ) - 
    static_cast<RealType>( m_InputMinimum ) * m_Scale;
  
  // set up the functor values
  this->GetFunctor().SetMinimum( m_OutputMinimum );
  this->GetFunctor().SetMaximum( m_OutputMaximum );
  this->GetFunctor().SetFactor( m_Scale );
  this->GetFunctor().SetOffset( m_Shift );
  
}


} // end namespace itk

#endif
