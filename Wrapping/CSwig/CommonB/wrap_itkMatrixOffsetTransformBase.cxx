/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: wrap_itkMatrixOffsetTransformBase.cxx,v $
  Language:  C++
  Date:      $Date: 2005/03/25 13:17:57 $
  Version:   $Revision: 1.1 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkMatrixOffsetTransformBase.h"

#ifdef CABLE_CONFIGURATION
#include "itkCSwigMacros.h"

#define ITK_WRAP_TRANSFORM_3(x, d1, d2) \
  ITK_WRAP_OBJECT3(x, double, d1, d2, itk##x##d1##d2)
    
namespace _cable_
{
  const char* const group = ITK_WRAP_GROUP(itkMatrixOffsetTransformBase);
  namespace wrappers
  {
    ITK_WRAP_TRANSFORM_3(MatrixOffsetTransformBase, 2, 2);
    ITK_WRAP_TRANSFORM_3(MatrixOffsetTransformBase, 3, 3);
  }
}
#endif
