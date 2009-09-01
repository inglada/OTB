/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkNarrowBand.txx,v $
  Language:  C++
  Date:      $Date: 2009-04-06 13:59:00 $
  Version:   $Revision: 1.12 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

  Portions of this code are covered under the VTK copyright.
  See VTKCopyright.txt or http://www.kitware.com/VTKCopyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkNarrowBand_txx
#define __itkNarrowBand_txx
#include "itkNarrowBand.h"
#include "math.h"
#include "vcl_cmath.h"

namespace itk
{

//template <class NodeType>
//void 
//NarrowBand<NodeType>
//::PrintSelf(std::ostream& os, Indent indent) const
//{
//  Superclass::PrintSelf(os, indent);
//}

#if !defined(CABLE_CONFIGURATION)
template <class NodeType >
std::vector< ITK_TYPENAME NarrowBand<NodeType>::RegionType>
NarrowBand<NodeType>
::SplitBand( unsigned int n)
{
  unsigned int i;
  std::vector<RegionType> regionList;
  if (n > static_cast<unsigned int>( m_NodeContainer.size() ) )
    {
    n = static_cast<unsigned int>( m_NodeContainer.size() );
    }
  unsigned int regionsize = static_cast<unsigned int> (vcl_floor(static_cast<float>(m_NodeContainer.size())/static_cast<float>( n )));
  if (regionsize == 0)
    {
    regionsize = 1;
    }
  RegionType region;
  Iterator pos = this->Begin();
  
  for (i = 0; i < n; i++)
    {
    region.Begin = pos;
    pos += regionsize;
    
    if (i != n-1)
      {
      region.End = pos;
      }
    else
      {
      region.End = this->End();
      }
    
    regionList.push_back(region);
    }
  
  return regionList;
}
#endif


} // end namespace itk

#endif
