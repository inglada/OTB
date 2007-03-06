/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: wrap_vnl_file_vector.cxx,v $
  Language:  C++
  Date:      $Date: 2003/09/10 14:30:12 $
  Version:   $Revision: 1.2 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "vnl/vnl_file_vector.h"

#ifdef CABLE_CONFIGURATION
#include "wrap_VXLNumerics.h"

namespace _cable_
{
  const char* const group = ITK_WRAP_GROUP(vnl_file_vector);
  namespace wrappers
  {
    typedef vnl_file_vector<double>  vnl_file_vector_double;
  }
}

void force_instantiate()
{
  using namespace _cable_::wrappers;
  sizeof(vnl_file_vector_double);
}

#endif
