/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkIntensityWindowingImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2008-10-16 16:45:10 $
  Version:   $Revision: 1.7 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

  Portions of this code are covered under the VTK copyright.
  See VTKCopyright.txt or http://www.kitware.com/VTKCopyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkIntensityWindowingImageFilter_txx
#define __itkIntensityWindowingImageFilter_txx

#include "itkIntensityWindowingImageFilter.h"
#include "itkMinimumMaximumImageCalculator.h"

namespace itk
{

/**
 *
 */
template <class TInputImage, class TOutputImage>
IntensityWindowingImageFilter<TInputImage, TOutputImage>
::IntensityWindowingImageFilter()
{
  m_OutputMaximum   = NumericTraits<OutputPixelType>::max();
  m_OutputMinimum   = NumericTraits<OutputPixelType>::NonpositiveMin();

  m_WindowMaximum   = NumericTraits<InputPixelType>::max();
  m_WindowMinimum   = NumericTraits<InputPixelType>::NonpositiveMin();

  m_Scale = 1.0;
  m_Shift = 0.0;
}

/**
 *
 */
template <class TInputImage, class TOutputImage>
void 
IntensityWindowingImageFilter<TInputImage, TOutputImage>
::SetWindowLevel(const InputPixelType& window, const InputPixelType& level) 
{
  typedef typename NumericTraits<InputPixelType>::RealType InputRealType;
  InputRealType tmp1, tmp2;

  tmp1 = static_cast<InputRealType>(level)
    - (static_cast<InputRealType>(window)/2.0);
  if (tmp1 < NumericTraits<InputPixelType>::NonpositiveMin())
    {
    tmp1 = 0.0;
    }

  tmp2 = static_cast<InputRealType>(level)
    + (static_cast<InputRealType>(window)/2.0);
  if (tmp2 > NumericTraits<InputPixelType>::max())
    {
    tmp2 = NumericTraits<InputPixelType>::max();
    }
  
  m_WindowMinimum = static_cast<InputPixelType>(tmp1);
  m_WindowMaximum = static_cast<InputPixelType>(tmp2);
}


/**
 *
 */
template <class TInputImage, class TOutputImage>
typename IntensityWindowingImageFilter<TInputImage, TOutputImage>::InputPixelType
IntensityWindowingImageFilter<TInputImage, TOutputImage>
::GetWindow() const
{
  return m_WindowMaximum - m_WindowMinimum;
}


/**
 *
 */
template <class TInputImage, class TOutputImage>
typename IntensityWindowingImageFilter<TInputImage, TOutputImage>::InputPixelType
IntensityWindowingImageFilter<TInputImage, TOutputImage>
::GetLevel() const
{
  return (m_WindowMaximum + m_WindowMinimum) / 2;
}

/**
 *
 */
template <class TInputImage, class TOutputImage>
void 
IntensityWindowingImageFilter<TInputImage, TOutputImage>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);

  os << indent << "Output Minimum: "
     << static_cast<typename NumericTraits<OutputPixelType>::PrintType>(m_OutputMinimum)
     << std::endl;
  os << indent << "Output Maximum: "
     << static_cast<typename NumericTraits<OutputPixelType>::PrintType>(m_OutputMaximum)
     << std::endl;
  os << indent << "Window Minimum: "
     << static_cast<typename NumericTraits<InputPixelType>::PrintType>(m_WindowMinimum)
     << std::endl;
  os << indent << "Window Maximum: "
     << static_cast<typename NumericTraits<InputPixelType>::PrintType>(m_WindowMaximum)
     << std::endl;
  os << indent << "Scale Factor: "
     << static_cast<typename NumericTraits<RealType>::PrintType>(m_Scale)
     << std::endl;
  os << indent << "Shift offset: "
     << static_cast<typename NumericTraits<RealType>::PrintType>(m_Shift)
     << std::endl;

}

/**
 *
 */
template <class TInputImage, class TOutputImage>
void 
IntensityWindowingImageFilter<TInputImage, TOutputImage>
::BeforeThreadedGenerateData()
{
  m_Scale = 
    (static_cast<RealType>( m_OutputMaximum )
     - static_cast<RealType>( m_OutputMinimum )) /
    (static_cast<RealType>( m_WindowMaximum )
     - static_cast<RealType>( m_WindowMinimum ));

  m_Shift =
    static_cast<RealType>( m_OutputMinimum ) - 
    static_cast<RealType>( m_WindowMinimum ) * m_Scale;
  
  // set up the functor values
  this->GetFunctor().SetOutputMinimum( m_OutputMinimum );
  this->GetFunctor().SetOutputMaximum( m_OutputMaximum );
  
  this->GetFunctor().SetWindowMinimum( m_WindowMinimum );
  this->GetFunctor().SetWindowMaximum( m_WindowMaximum );
  
  this->GetFunctor().SetFactor( m_Scale );
  this->GetFunctor().SetOffset( m_Shift );
}


} // end namespace itk

#endif
