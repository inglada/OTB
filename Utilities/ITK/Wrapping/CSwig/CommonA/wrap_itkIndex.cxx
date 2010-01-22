/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: wrap_itkIndex.cxx,v $
  Language:  C++
  Date:      $Date: 2005-03-25 13:17:57 $
  Version:   $Revision: 1.1 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkIndex.h"

#ifdef CABLE_CONFIGURATION
#include "itkCSwigMacros.h"
namespace _cable_
{
  const char* const group = ITK_WRAP_GROUP(itkIndex);
  namespace wrappers
  {
    typedef itk::Index<2 >::Index itkIndex2;
    typedef itk::Index<3 >::Index itkIndex3;
  }
}

#endif
