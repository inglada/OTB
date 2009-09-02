/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkJointDomainImageToListAdaptor.txx,v $
  Language:  C++
  Date:      $Date: 2009-03-04 15:23:51 $
  Version:   $Revision: 1.12 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkJointDomainImageToListAdaptor_txx
#define __itkJointDomainImageToListAdaptor_txx

#include "itkJointDomainImageToListAdaptor.h"

namespace itk { 
namespace Statistics {

template < class TImage >
JointDomainImageToListAdaptor< TImage >
::JointDomainImageToListAdaptor()
{
  m_NormalizationFactors.Fill( 1.0f );
}

template < class TImage >
void
JointDomainImageToListAdaptor< TImage >
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);
}

template < class TImage >
void
JointDomainImageToListAdaptor< TImage >
::SetNormalizationFactors(NormalizationFactorsType& factors)
{
  if ( m_NormalizationFactors != factors )
    {
    m_NormalizationFactors = factors;
    this->Modified();
    }
}


} // end of namespace Statistics 
} // end of namespace itk

#endif
