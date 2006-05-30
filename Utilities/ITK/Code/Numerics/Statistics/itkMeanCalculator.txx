/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkMeanCalculator.txx,v $
  Language:  C++
  Date:      $Date: 2005/07/26 15:54:59 $
  Version:   $Revision: 1.15 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkMeanCalculator_txx
#define __itkMeanCalculator_txx

namespace itk{ 
namespace Statistics{

template< class TSample >
MeanCalculator< TSample >
::MeanCalculator()
{
  m_Output.Fill(0.0) ;
}

template< class TSample >
void
MeanCalculator< TSample >
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);

  os << indent << "Output: " << m_Output << std::endl;
}

template< class TSample >
typename MeanCalculator< TSample >::OutputType*
MeanCalculator< TSample >
::GetOutput()
{
  return &m_Output ;
} 

template< class TSample >
void
MeanCalculator< TSample >
::GenerateData() 
{
  typename TSample::ConstIterator iter = this->GetInputSample()->Begin() ;
  typename TSample::ConstIterator end = this->GetInputSample()->End() ;
  double totalFrequency = 0.0 ;

  const MeasurementVectorSizeType measurementVectorSize 
                        = this->GetMeasurementVectorSize();
  MeasurementVectorTraits::SetLength( m_Output, measurementVectorSize );

  while (iter != end)
    {
    double frequency = iter.GetFrequency() ;
    totalFrequency += frequency ;
    for (unsigned int dim = 0 ; dim < measurementVectorSize ; dim++)
      {
      m_Output[dim] += iter.GetMeasurementVector()[dim] * frequency ;
      }
    ++iter ;
    }
  
  m_Output /= totalFrequency ;
}

} // end of namespace Statistics 
} // end of namespace itk

#endif

