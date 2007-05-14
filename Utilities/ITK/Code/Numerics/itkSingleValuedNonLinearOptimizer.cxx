/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkSingleValuedNonLinearOptimizer.cxx,v $
  Language:  C++
  Date:      $Date: 2005/01/21 20:22:49 $
  Version:   $Revision: 1.17 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itkSingleValuedNonLinearOptimizer_txx
#define _itkSingleValuedNonLinearOptimizer_txx

#include "itkSingleValuedNonLinearOptimizer.h"

namespace itk
{

SingleValuedNonLinearOptimizer
::SingleValuedNonLinearOptimizer()
{
  m_CostFunction = 0;
}


/**
 * Connect a Cost Function
 */
void
SingleValuedNonLinearOptimizer
::SetCostFunction( CostFunctionType * costFunction )
{
  if( m_CostFunction.GetPointer() == costFunction )
    {
    return;
    }

  itkDebugMacro("setting CostFunction  to " <<  costFunction);

  m_CostFunction = costFunction;

  if(!m_ScalesInitialized)
    {
    const unsigned int numberOfParameters = 
      m_CostFunction->GetNumberOfParameters();

    ScalesType scales( numberOfParameters );
    scales.Fill( 1.0f );
    SetScales( scales );
    m_ScalesInitialized = true;
    }

  this->Modified();
}

/**
 * Get the cost function value at the given parameters
 */
SingleValuedNonLinearOptimizer::MeasureType
SingleValuedNonLinearOptimizer
::GetValue( const ParametersType & parameters ) const
{
  itkDebugMacro("Computing CostFunction value at " <<  parameters);

  if(!m_CostFunction)
    {
    ExceptionObject ex;
    ex.SetLocation(__FILE__);
    ex.SetDescription("The costfunction must be set prior to calling GetValue");
    throw ex;
    }

  return this->GetCostFunction()->GetValue(parameters);
}


void
SingleValuedNonLinearOptimizer
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);
  if (m_CostFunction)
    {
    os << indent << "Cost Function: " << m_CostFunction.GetPointer() << std::endl;
    }
}


} // namespace itk

#endif
