/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkOptimizer.cxx,v $
  Language:  C++
  Date:      $Date: 2004/03/12 22:26:41 $
  Version:   $Revision: 1.11 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itkOptimizer_txx
#define _itkOptimizer_txx

#include "itkOptimizer.h"

namespace itk
{

/**
 * Constructor
 */

Optimizer
::Optimizer()
{
  m_ScalesInitialized = false;
}

/**
 * Set scaling as an array of factors
 */
void 
Optimizer
::SetScales(const ScalesType & scales)
{
  itkDebugMacro("setting scales to " <<  scales);
  m_Scales = scales;
  m_ScalesInitialized = true;
  this->Modified();
}

/**
 * Set the initial position
 */
void 
Optimizer
::SetInitialPosition( const ParametersType & param )
{
  m_InitialPosition = param;
  this->Modified();
}


/**
 * Set the current position
 */
void 
Optimizer
::SetCurrentPosition( const ParametersType &  param )
{
  m_CurrentPosition = param;
  this->Modified();
}


/**
 * Print Self method
 */
void
Optimizer
::PrintSelf( std::ostream& os, Indent indent ) const
{
  Superclass::PrintSelf(os,indent);

  os << indent << "InitialPosition: "
     << m_InitialPosition << std::endl;
  os << indent << "CurrentPosition: "
     << m_CurrentPosition << std::endl;

  if(m_ScalesInitialized)
    {
    os << indent << "Scales: "
       << m_Scales << std::endl;
    }
  else
    {
    os << indent << "Scales: not defined (default 1)" 
       << std::endl;
    }
}



} // end namespace itk

#endif
