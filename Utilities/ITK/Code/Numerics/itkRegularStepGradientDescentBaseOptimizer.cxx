/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkRegularStepGradientDescentBaseOptimizer.cxx,v $
  Language:  C++
  Date:      $Date: 2007/09/10 16:22:23 $
  Version:   $Revision: 1.24 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itkRegularStepGradientDescentBaseOptimizer_txx
#define _itkRegularStepGradientDescentBaseOptimizer_txx

#include "itkRegularStepGradientDescentBaseOptimizer.h"
#include "itkCommand.h"
#include "itkEventObject.h"
#include "vnl/vnl_math.h"

namespace itk
{

/**
 * Constructor
 */
RegularStepGradientDescentBaseOptimizer
::RegularStepGradientDescentBaseOptimizer()
{

  itkDebugMacro("Constructor");
      
  m_MaximumStepLength = 1.0;
  m_MinimumStepLength = 1e-3;
  m_GradientMagnitudeTolerance = 1e-4;
  m_NumberOfIterations = 100;
  m_CurrentIteration   =   0;
  m_Value = 0;
  m_Maximize = false;
  m_CostFunction = 0;
  m_CurrentStepLength   =   0;
  m_StopCondition = Unknown;
  m_Gradient.Fill( 0.0f );
  m_PreviousGradient.Fill( 0.0f );
  m_RelaxationFactor = 0.5;

}

/**
 * Start the optimization
 */
void
RegularStepGradientDescentBaseOptimizer
::StartOptimization( void )
{

  itkDebugMacro("StartOptimization");

  m_CurrentStepLength         = m_MaximumStepLength;
  m_CurrentIteration          = 0;

  m_StopCondition = Unknown;

  // validity check for the value of GradientMagnitudeTolerance
  if( m_GradientMagnitudeTolerance < 0.0 )
      {
      itkExceptionMacro(<< "Gradient magnitude tolerance must be"
      "greater or equal 0.0. Current value is " << m_GradientMagnitudeTolerance );
      }

  const unsigned int spaceDimension = m_CostFunction->GetNumberOfParameters();

  m_Gradient = DerivativeType( spaceDimension );
  m_PreviousGradient = DerivativeType( spaceDimension );
  m_Gradient.Fill( 0.0f );
  m_PreviousGradient.Fill( 0.0f );

  this->SetCurrentPosition( GetInitialPosition() );
  this->ResumeOptimization();

}

/**
 * Resume the optimization
 */
void
RegularStepGradientDescentBaseOptimizer
::ResumeOptimization( void )
{
  
  itkDebugMacro("ResumeOptimization");

  m_Stop = false;

  this->InvokeEvent( StartEvent() );

  while( !m_Stop ) 
    {

    if( m_CurrentIteration >= m_NumberOfIterations )
      {
      m_StopCondition = MaximumNumberOfIterations;
      this->StopOptimization();
      break;
      }

    m_PreviousGradient = m_Gradient;

    try
      {
      m_CostFunction->GetValueAndDerivative( 
                               this->GetCurrentPosition(), m_Value, m_Gradient );
      }
    catch( ExceptionObject & excp )
      {
      m_StopCondition = CostFunctionError;
      this->StopOptimization();
      throw excp;
      }

    if( m_Stop )
      {
      break;
      }

    this->AdvanceOneStep();

    m_CurrentIteration++;

    }

}

/**
 * Stop optimization
 */
void
RegularStepGradientDescentBaseOptimizer
::StopOptimization( void )
{

  itkDebugMacro("StopOptimization");
  
  m_Stop = true;
  this->InvokeEvent( EndEvent() );
}

/**
 * Advance one Step following the gradient direction
 */
void
RegularStepGradientDescentBaseOptimizer
::AdvanceOneStep( void )
{ 

  itkDebugMacro("AdvanceOneStep");

  const unsigned int  spaceDimension = m_CostFunction->GetNumberOfParameters();

  DerivativeType transformedGradient( spaceDimension );
  DerivativeType previousTransformedGradient( spaceDimension );
  ScalesType     scales = this->GetScales();

  if( m_RelaxationFactor < 0.0 )
    {
    itkExceptionMacro(<< "Relaxation factor must be positive. Current value is " << m_RelaxationFactor );
    return;
    }

  if( m_RelaxationFactor >= 1.0 )
    {
    itkExceptionMacro(<< "Relaxation factor must less than 1.0. Current value is " << m_RelaxationFactor );
    return;
    }


  // Make sure the scales have been set properly
  if (scales.size() != spaceDimension)
    {
    itkExceptionMacro(<< "The size of Scales is "
                      << scales.size()
                      << ", but the NumberOfParameters for the CostFunction is "
                      << spaceDimension
                      << ".");
    }

  for(unsigned int i = 0;  i < spaceDimension; i++)
    {
    transformedGradient[i]  = m_Gradient[i] / scales[i];
    previousTransformedGradient[i] = 
      m_PreviousGradient[i] / scales[i];
    }

  double magnitudeSquare = 0;
  for(unsigned int dim=0; dim<spaceDimension; dim++)
    {
    const double weighted = transformedGradient[dim];
    magnitudeSquare += weighted * weighted;
    }
    
  const double gradientMagnitude = vcl_sqrt( magnitudeSquare );

  if( gradientMagnitude < m_GradientMagnitudeTolerance ) 
    {
    m_StopCondition = GradientMagnitudeTolerance;
    this->StopOptimization();
    return;
    }
    
  double scalarProduct = 0;

  for(unsigned int i=0; i<spaceDimension; i++)
    {
    const double weight1 = transformedGradient[i];
    const double weight2 = previousTransformedGradient[i];
    scalarProduct += weight1 * weight2;
    }
   
  // If there is a direction change 
  if( scalarProduct < 0 ) 
    {
    m_CurrentStepLength *= m_RelaxationFactor;
    }
  
  if( m_CurrentStepLength < m_MinimumStepLength )
    {
    m_StopCondition = StepTooSmall;
    this->StopOptimization();
    return;
    }

  double direction;
  if( this->m_Maximize ) 
    {
    direction = 1.0;
    }
  else 
    {
    direction = -1.0;
    }

  const double factor = 
    direction * m_CurrentStepLength / gradientMagnitude;

  // This method StepAlongGradient() will 
  // be overloaded in non-vector spaces
  this->StepAlongGradient( factor, transformedGradient );

  this->InvokeEvent( IterationEvent() );

}

void
RegularStepGradientDescentBaseOptimizer
::PrintSelf( std::ostream& os, Indent indent ) const
{
  Superclass::PrintSelf(os,indent);
  os << indent << "MaximumStepLength: "
     << m_MaximumStepLength << std::endl;
  os << indent << "MinimumStepLength: "
     << m_MinimumStepLength << std::endl;
  os << indent << "RelaxationFactor: "
     << m_RelaxationFactor << std::endl;
  os << indent << "GradientMagnitudeTolerance: "
     << m_GradientMagnitudeTolerance << std::endl;
  os << indent << "NumberOfIterations: "
     << m_NumberOfIterations << std::endl;
  os << indent << "CurrentIteration: "
     << m_CurrentIteration   << std::endl;
  os << indent << "Value: "
     << m_Value << std::endl;
  os << indent << "Maximize: "
     << m_Maximize << std::endl;
  if (m_CostFunction)
    {
    os << indent << "CostFunction: "
       << &m_CostFunction << std::endl;
    }
  else
    {
    os << indent << "CostFunction: "
       << "(None)" << std::endl;
    }
  os << indent << "CurrentStepLength: "
     << m_CurrentStepLength << std::endl;
  os << indent << "StopCondition: "
     << m_StopCondition << std::endl;
  os << indent << "Gradient: "
     << m_Gradient << std::endl;
}
} // end namespace itk

#endif
