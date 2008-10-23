/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkGoodnessOfFitFunctionBase.txx,v $
  Language:  C++
  Date:      $Date: 2006-03-19 04:37:20 $
  Version:   $Revision: 1.7 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkGoodnessOfFitFunctionBase_txx
#define __itkGoodnessOfFitFunctionBase_txx

#include "itkGoodnessOfFitFunctionBase.h"

namespace itk{ 
namespace Statistics{

template< class TInputHistogram >
GoodnessOfFitFunctionBase< TInputHistogram >
::GoodnessOfFitFunctionBase()
{
  m_Epsilon = 1e-20 ;
  m_LogEpsilon = vcl_log(m_Epsilon) ;

  m_UseExpectedHistogram = true ;
  m_ObservedHistogram = 0 ;
  m_ExpectedHistogram = 0 ;

  m_UseExpectedHistogram = false ;
  m_TotalObservedScale = 0 ;
  
  m_Output = 0.0 ;
}

template< class TInputHistogram >
void
GoodnessOfFitFunctionBase< TInputHistogram >
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os, indent) ;

  os << indent << "ObservedHistogram: " ;
  if ( m_ObservedHistogram != 0 )
    {
    os << m_ObservedHistogram << std::endl ;
    }
  else
    {
    os << "not set." << std::endl ;
    }

  os << indent << "ExpectedHistogram: " ;
  if ( m_ExpectedHistogram != 0 )
    {
    os << m_ExpectedHistogram << std::endl ;
    }
  else
    {
    os << "not set." << std::endl ;
    }

  os << indent << "Output: " << m_Output << std::endl ;
  os << indent << "Total Observed Scale: " << m_TotalObservedScale 
     << std::endl ;
  os << indent << "Use Expected Histogram: " << m_UseExpectedHistogram 
     << std::endl ;
  os << indent << "Epsilon: " << m_Epsilon << std::endl ;
  os << indent << "Log(Epsilon): " <<  m_LogEpsilon << std::endl ;
}

template< class TInputHistogram >
void
GoodnessOfFitFunctionBase< TInputHistogram >
::SetObservedHistogram(InputHistogramType* histogram)
{
  m_ObservedHistogram = histogram ;
  this->Modified() ;
}

template< class TInputHistogram >
void
GoodnessOfFitFunctionBase< TInputHistogram >
::SetExpectedHistogram(InputHistogramType* histogram)
{
  m_ExpectedHistogram = histogram ;
  this->Modified() ;
}

} // end of namespace Statistics 
} // end of namespace itk

#endif

