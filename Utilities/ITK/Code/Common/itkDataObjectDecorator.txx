/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkDataObjectDecorator.txx,v $
  Language:  C++
  Date:      $Date: 2007/04/25 22:25:16 $
  Version:   $Revision: 1.2 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

  Portions of this code are covered under the VTK copyright.
  See VTKCopyright.txt or http://www.kitware.com/VTKCopyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itkDataObjectDecorator_txx
#define _itkDataObjectDecorator_txx

#include "itkDataObjectDecorator.h"

namespace itk
{

/**
 *
 */
template<class T>
DataObjectDecorator<T>
::DataObjectDecorator() : m_Component()
{
}


/**
 *
 */
template<class T>
DataObjectDecorator<T>
::~DataObjectDecorator()
{
}


/**
 *
 */
template<class T>
void
DataObjectDecorator<T>
::Set( const T * val )
{
  if (m_Component != val)
    {
    m_Component = val;
    this->Modified();
    }
}

/**
 *
 */
template<class T>
const T *
DataObjectDecorator<T>
::Get() const
{ 
  return m_Component; 
}

/**
 *
 */
template<class T>
void 
DataObjectDecorator<T>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);

  os << indent << "Component: " << m_Component << std::endl;
}

} // end namespace itk

#endif
