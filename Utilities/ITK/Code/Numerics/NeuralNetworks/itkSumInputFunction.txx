/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkSumInputFunction.txx,v $
  Language:  C++
  Date:      $Date: 2007/08/17 13:10:57 $
  Version:   $Revision: 1.3 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkSumInputFunction_txx
#define __itkSumInputFunction_txx

#include "itkSumInputFunction.h"

namespace itk
{
namespace Statistics
{

/** Constructor */
template<class TMeasurementVector, class ScalarType>
SumInputFunction<TMeasurementVector,ScalarType>
::SumInputFunction()
{
  m_Size = 0;
}

/** Destructor */
template<class TMeasurementVector, class ScalarType>
SumInputFunction<TMeasurementVector,ScalarType>
::~SumInputFunction()
{
}

/** Set the size */
template<class TMeasurementVector, class ScalarType>
void
SumInputFunction<TMeasurementVector,ScalarType>
::SetSize(unsigned int n)
{
  m_Size = n;
  this->Modified();
}

/** Evaluate */
template<class TMeasurementVector, class ScalarType>
ScalarType
SumInputFunction<TMeasurementVector,ScalarType>
::Evaluate(const TMeasurementVector& input)  const
{
  vnl_vector<ScalarType> temp(input, m_Size);
  return temp.sum();
}

/** Print the object */
template<class TMeasurementVector, class ScalarType>
void  
SumInputFunction <TMeasurementVector,ScalarType>
::PrintSelf( std::ostream& os, Indent indent ) const 
{ 
  os << indent << "SumInputFunction(" << this << ")" << std::endl;
  os << indent << "m_Size = " << m_Size << std::endl;
  Superclass::PrintSelf( os, indent ); 
}

} // end namespace Statistics
} // end namespace itk

#endif
