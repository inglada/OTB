/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkAutoPointerDataObjectDecorator.txx,v $
  Language:  C++
  Date:      $Date: 2004/03/05 20:56:34 $
  Version:   $Revision: 1.2 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

  Portions of this code are covered under the VTK copyright.
  See VTKCopyright.txt or http://www.kitware.com/VTKCopyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itkAutoPointerDataObjectDecorator_txx
#define _itkAutoPointerDataObjectDecorator_txx

#include "itkAutoPointerDataObjectDecorator.h"

namespace itk
{

/**
 *
 */
template<class T>
AutoPointerDataObjectDecorator<T>
::AutoPointerDataObjectDecorator() : m_Component()
{
}


/**
 *
 */
template<class T>
AutoPointerDataObjectDecorator<T>
::~AutoPointerDataObjectDecorator()
{
}


/**
 *
 */
template<class T>
void
AutoPointerDataObjectDecorator<T>
::Set(T* val)
{
  if (m_Component.get() != val)
    {
    // store the pointer and take ownership of the memory
    ComponentPointer aPointer(val);
    m_Component = aPointer;
    this->Modified();
    }
}


/**
 *
 */
template<class T>
void 
AutoPointerDataObjectDecorator<T>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);

  os << indent << "Component: " << typeid(m_Component).name() << std::endl;
}

} // end namespace itk

#endif
