/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkSampleMeanShiftBlurringFilter.txx,v $
  Language:  C++
  Date:      $Date: 2005/08/11 13:39:31 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkSampleMeanShiftBlurringFilter_txx
#define __itkSampleMeanShiftBlurringFilter_txx

namespace itk{ 
namespace Statistics{

template< class TSample >
SampleMeanShiftBlurringFilter< TSample >
::SampleMeanShiftBlurringFilter()
{
  m_Output = OutputType::New() ;
  m_ModeSeeker = 0 ;
}

template< class TSample >
SampleMeanShiftBlurringFilter< TSample >
::~SampleMeanShiftBlurringFilter()
{
}

template< class TSample >
void
SampleMeanShiftBlurringFilter< TSample >
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);

  os << indent << "Output: " << m_Output << std::endl;
}

template< class TSample >
void
SampleMeanShiftBlurringFilter< TSample >
::SetMeanShiftModeSeeker(MeanShiftModeSeekerType* seeker)
{
  if ( m_ModeSeeker != seeker )
    {
    m_ModeSeeker = seeker ;
    this->Modified() ;
    }
}

template< class TSample >
typename SampleMeanShiftBlurringFilter< TSample >::OutputType*
SampleMeanShiftBlurringFilter< TSample >
::GetOutput()
{
  return m_Output.GetPointer() ;
} 

// VS6 cannot compile the following method without the following pragma
#if defined(_MSC_VER)
#pragma inline_depth(0)
#endif

template< class TSample >
void
SampleMeanShiftBlurringFilter< TSample >
::GenerateData() 
{
  typename InputSampleType::ConstIterator iter = this->GetInputSample()->Begin() ;
  typename InputSampleType::ConstIterator end = this->GetInputSample()->End() ;

  m_Output->SetMeasurementVectorSize( this->GetMeasurementVectorSize() );

  m_Output->Clear() ;
  MeasurementVectorType finalPoint;
  MeasurementVectorType sample;
  while ( iter != end )
    {
    sample = iter.GetMeasurementVector();
    finalPoint = m_ModeSeeker->Evolve( sample ) ;
    m_Output->PushBack( finalPoint  ) ;
    ++iter ;
    }
}

} // end of namespace Statistics 
} // end of namespace itk

#endif

