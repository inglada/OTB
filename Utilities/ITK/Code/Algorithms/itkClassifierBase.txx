/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkClassifierBase.txx,v $
  Language:  C++
  Date:      $Date: 2006-02-05 20:57:46 $
  Version:   $Revision: 1.8 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itkClassifierBase_txx
#define _itkClassifierBase_txx

#include "itkClassifierBase.h"
#include "itkCommand.h"

namespace itk
{

template <class TDataContainer>
ClassifierBase<TDataContainer>
::ClassifierBase()
{
  m_NumberOfClasses = 0 ;
  m_DecisionRule = 0 ;
  m_MembershipFunctions.resize(0);
}

template <class TDataContainer>
ClassifierBase<TDataContainer>
::~ClassifierBase()
{
}

template <class TDataContainer>
void
ClassifierBase<TDataContainer>
::PrintSelf( std::ostream& os, Indent indent ) const
{
  Superclass::PrintSelf(os,indent);

  os << indent << "Number of classes: " << m_NumberOfClasses << std::endl;
  os << indent << "DecisionRule: " ;
  if ( m_DecisionRule.IsNotNull() )
    {
    os << m_DecisionRule << std::endl ;
    }
  else
    {
    os << "not set." << std::endl ;
    }

  os << indent << "MembershipFunctions: " << std::endl ;
  for ( unsigned int i = 0 ; i < m_MembershipFunctions.size() ; ++i )
    {
    os << indent << m_MembershipFunctions[i] << std::endl ;
    }
}

template <class TDataContainer>
void
ClassifierBase<TDataContainer>
::Update()
{
  if( m_NumberOfClasses == 0 )
    {
    itkExceptionMacro( "Zero class" ) ;
    return ;
    }
    
  if ( m_MembershipFunctions.size() == 0 )
    {
    itkExceptionMacro( "No membership function" ) ;
    return ;
    }
  
  if( m_NumberOfClasses != m_MembershipFunctions.size() )
    {
    itkExceptionMacro( "The number of classes and the number of membership mismatch." ) ;
    return ;
    }

  this->GenerateData() ;
}

template<class TDataContainer>
unsigned int 
ClassifierBase< TDataContainer >
::AddMembershipFunction(MembershipFunctionType * function)
{
  m_MembershipFunctions.push_back(function) ;
  return static_cast<unsigned int>( m_MembershipFunctions.size() );
}

} // namespace itk

#endif
