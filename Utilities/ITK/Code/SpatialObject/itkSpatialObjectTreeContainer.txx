/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkSpatialObjectTreeContainer.txx,v $
  Language:  C++
  Date:      $Date: 2004/09/09 22:29:20 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itkSpatialObjectTreeContainer_txx
#define _itkSpatialObjectTreeContainer_txx

#include "itkSpatialObjectTreeContainer.h"

namespace itk
{

/** Constructor */
template <unsigned int TDimension>
SpatialObjectTreeContainer<TDimension>::SpatialObjectTreeContainer()
{
}

/** Destructor */
template <unsigned int TDimension>
SpatialObjectTreeContainer<TDimension>::~SpatialObjectTreeContainer() 
{
}

/** Set the root */
template <unsigned int TDimension>
bool 
SpatialObjectTreeContainer<TDimension>::SetRoot(SpatialObjectPointer element)
{
  if(this->m_Root)
    {
    std::cout << "This tree has already a root" << std::endl;
    return false;
    }
  
  if(element->GetTreeNode())
    {
    this->m_Root = element->GetTreeNode();
    }
  else
    {
    this->m_Root = SpatialObjectTreeNode<TDimension>::New();
    this->m_Root->Set(element);
    }
  return true;
}

} // namespace itk

#endif
