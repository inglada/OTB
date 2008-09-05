/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkNormalVectorFunctionBase.txx,v $
  Language:  C++
  Date:      $Date: 2003-09-10 14:28:53 $
  Version:   $Revision: 1.4 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

     =========================================================================*/
#ifndef __itkNormalVectorFunctionBase_txx_
#define __itkNormalVectorFunctionBase_txx_

#include "itkNormalVectorFunctionBase.h"
#include "itkNumericTraits.h"

namespace itk {

template <class TSparseImageType>
NormalVectorFunctionBase <TSparseImageType>
::NormalVectorFunctionBase()
{
  m_TimeStep = NumericTraits<TimeStepType>::One;
}

template <class TSparseImageType>
void
NormalVectorFunctionBase <TSparseImageType>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "TimeStep: " << m_TimeStep << std::endl;
}

} // end namespace itk

#endif
