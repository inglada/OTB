/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkSegmentationBorder.cxx,v $
  Language:  C++
  Date:      $Date: 2006/02/06 22:01:58 $
  Version:   $Revision: 1.4 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkSegmentationBorder.h"

namespace itk
{


SegmentationBorder
::SegmentationBorder(void):
  m_BorderLength(0)
{
}


SegmentationBorder
::~SegmentationBorder()
{

}

/**
 * PrintSelf
 */
void
SegmentationBorder
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);
  os << indent << "Segmentation border object" << std::endl;
  os << indent << "Length of the border: " << m_BorderLength << std::endl;
}

} // end namespace itk

