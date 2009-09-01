/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkSparseImage.txx,v $
  Language:  C++
  Date:      $Date: 2009-03-03 15:09:33 $
  Version:   $Revision: 1.5 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkSparseImage_txx
#define __itkSparseImage_txx

#include "itkSparseImage.h"
#include "itkSparseFieldLayer.h"
#include "itkObjectStore.h"
#include "itkDataObject.h"

namespace itk {

template <class TNode, unsigned int VImageDimension>
SparseImage < TNode, VImageDimension >
::SparseImage()
{
  m_NodeList     = NodeListType::New();
  m_NodeStore    = NodeStoreType::New();
}

template <class TNode, unsigned int VImageDimension>
void
SparseImage < TNode, VImageDimension >
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

template <class TNode, unsigned int VImageDimension>
void
SparseImage < TNode, VImageDimension >
::Initialize()
{
  Superclass::Initialize();
  m_NodeList     = NodeListType::New();
  m_NodeStore    = NodeStoreType::New();
}

} // end namespace itk

#endif
