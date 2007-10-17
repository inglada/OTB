/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkEventObject.cxx,v $
  Language:  C++
  Date:      $Date: 2003/09/10 14:29:06 $
  Version:   $Revision: 1.5 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkEventObject.h"

namespace itk
{

void
EventObject
::Print(std::ostream& os) const
{
  Indent indent;
  
  this->PrintHeader(os,0); 
  this->PrintSelf(os, indent.GetNextIndent());
  this->PrintTrailer(os,0);
}  

/**
 * Define a default print header for all objects.
 */
void 
EventObject
::PrintHeader(std::ostream& os, Indent indent) const
{
  os << std::endl;
  os << indent << "itk::" << this->GetEventName() << " (" << this << ")\n";
}


/**
 * Define a default print trailer for all objects.
 */
void 
EventObject
::PrintTrailer(std::ostream& os, Indent indent) const
{
  os << indent << std::endl;
}

void
EventObject
::PrintSelf(std::ostream&, Indent) const
{
}

} // end namespace itk
