/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkSampleAlgorithmBase.txx,v $
  Language:  C++
  Date:      $Date: 2005/07/26 15:55:04 $
  Version:   $Revision: 1.8 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkSampleAlgorithmBase_txx
#define __itkSampleAlgorithmBase_txx

#include "itkSampleAlgorithmBase.h"

namespace itk{ 
namespace Statistics{

template< class TInputSample >
SampleAlgorithmBase< TInputSample >
::SampleAlgorithmBase()
{
  m_InputSample = 0;
  m_MeasurementVectorSize = 0;
}

template< class TInputSample >
void
SampleAlgorithmBase< TInputSample >
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);

  os << indent << "Input Sample: " ;
  if ( m_InputSample.IsNotNull() )
    {
    os << m_InputSample << std::endl;
    os << indent << "MeasurementVectorSize: " << m_MeasurementVectorSize << std::endl;
    }
  else
    {
    os << "not set." << std::endl ;
    }

}


template< class TInputSample >
void
SampleAlgorithmBase< TInputSample >
::GenerateData() 
{
  // subclasses should override this function.
}

} // end of namespace Statistics 
} // end of namespace itk

#endif

