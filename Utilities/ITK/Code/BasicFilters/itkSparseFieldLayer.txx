/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkSparseFieldLayer.txx,v $
  Language:  C++
  Date:      $Date: 2009-04-06 14:13:03 $
  Version:   $Revision: 1.11 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkSparseFieldLayer_txx
#define __itkSparseFieldLayer_txx
#include "itkSparseFieldLayer.h"
#include <math.h>
#include "vcl_cmath.h"

namespace itk {

template<class TNodeType>
SparseFieldLayer<TNodeType>
::SparseFieldLayer()
{
  m_HeadNode = new NodeType;
  m_HeadNode->Next = m_HeadNode;
  m_HeadNode->Previous = m_HeadNode;
  m_Size = 0;
}

template<class TNodeType>
SparseFieldLayer<TNodeType>
::~SparseFieldLayer()
{
  delete m_HeadNode;
}
  
template<class TNodeType>
void
SparseFieldLayer<TNodeType>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);
  
  os << indent << "m_HeadNode:  " << m_HeadNode << std::endl;
  os << indent << "Empty? : " << this->Empty() << std::endl;
}

template<class TNodeType>
unsigned int
SparseFieldLayer<TNodeType>
::Size() const
{
  return m_Size;
}

template<class TNodeType>
typename SparseFieldLayer<TNodeType>::RegionListType
SparseFieldLayer<TNodeType>
::SplitRegions (int num) const
{
  std::vector<RegionType> regionlist;
  unsigned int size, regionsize;
  size=Size();
  regionsize=static_cast<unsigned int>(
    vcl_ceil(static_cast<float>(size)/static_cast<float>(num)));
  ConstIterator position=Begin();
  ConstIterator last=End();
  
  for (int i=0;i<num;i++) 
    {
    unsigned int j=0;
    RegionType region;
    region.first=position;
    while ((j<regionsize)&&(position!=last)) 
      {
      j++;
      ++position;
      }
    region.last=position;
    regionlist.push_back(region);
    }
  
  return regionlist;
}

}// end namespace itk

#endif
