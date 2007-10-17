/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkTransform.txx,v $
  Language:  C++
  Date:      $Date: 2007/08/20 18:06:55 $
  Version:   $Revision: 1.26 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itkTransform_txx
#define _itkTransform_txx

// First, make sure that we include the configuration file.
// This line may be removed once the ThreadSafeTransform gets
// integrated into ITK.
#include "itkConfigure.h"

// Second, redirect to the optimized version if necessary
#ifdef ITK_USE_OPTIMIZED_REGISTRATION_METHODS
#include "itkThreadSafeTransform.txx"
#else

#include "itkTransform.h"

namespace itk
{


/**
 * Constructor
 */
template < class TScalarType,
           unsigned int NInputDimensions,
           unsigned int NOutputDimensions >
Transform< TScalarType,NInputDimensions,NOutputDimensions>
::Transform():
  m_Parameters(1),
  m_FixedParameters(1),
  m_Jacobian(NOutputDimensions,1)
{
  itkWarningMacro(<< "Using default transform constructor.  Should specify NOutputDims and NParameters as args to constructor.");
}



/**
 * Constructor
 */
template < class TScalarType,
           unsigned int NInputDimensions,
           unsigned int NOutputDimensions >
Transform< TScalarType,NInputDimensions,NOutputDimensions>
::Transform(unsigned int dimension,unsigned int numberOfParameters):
  m_Parameters(numberOfParameters),
  m_FixedParameters(numberOfParameters),
  m_Jacobian(dimension,numberOfParameters)
{
}


/**
 * GenerateName
 */
template < class TScalarType,
           unsigned int NInputDimensions,
           unsigned int NOutputDimensions >
std::string Transform< TScalarType,NInputDimensions,NOutputDimensions>
::GetTransformTypeAsString () const
{
  OStringStream n;
  n << GetNameOfClass();
  n << "_";
  if ( typeid ( TScalarType ) == typeid ( float ) )
    {
    n << "float";
    }
  else if ( typeid ( TScalarType ) == typeid ( double ) )
    {
      n << "double";
    }
  else
    {
      n << "other";
    }
  n << "_" << this->GetInputSpaceDimension() << "_" << this->GetOutputSpaceDimension();
  return n.str();
}


} // end namespace itk


#endif

#endif
