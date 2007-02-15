/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkSimpleDataObjectDecorator.txx,v $
  Language:  C++
  Date:      $Date: 2004/03/18 19:24:08 $
  Version:   $Revision: 1.2 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

  Portions of this code are covered under the VTK copyright.
  See VTKCopyright.txt or http://www.kitware.com/VTKCopyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itkSimpleDataObjectDecorator_txx
#define _itkSimpleDataObjectDecorator_txx

#include "itkSimpleDataObjectDecorator.h"

namespace itk
{

/**
 *
 */
template<class T>
SimpleDataObjectDecorator<T>
::SimpleDataObjectDecorator() 
{
  m_Component = ComponentType(); // initialize here to avoid Purify UMR
}


/**
 *
 */
template<class T>
SimpleDataObjectDecorator<T>
::~SimpleDataObjectDecorator()
{
}


/**
 *
 */
template<class T>
void
SimpleDataObjectDecorator<T>
::Set(const T& val)
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
void 
SimpleDataObjectDecorator<T>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);

  os << indent << "Component: " << typeid(m_Component).name() << std::endl;
}

} // end namespace itk

#endif
