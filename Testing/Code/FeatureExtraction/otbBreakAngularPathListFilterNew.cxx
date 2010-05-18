/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkExceptionObject.h"
#include "otbBreakAngularPathListFilter.h"
#include "itkPolyLineParametricPath.h"
#include <cstdlib>

int otbBreakAngularPathListFilterNew(int argc, char * argv[])
{
  const unsigned int Dimension = 2;
  typedef itk::PolyLineParametricPath<Dimension>    PathType;
  typedef otb::BreakAngularPathListFilter<PathType> BreakAngularPathListFilterType;

  // Instantiating object
  BreakAngularPathListFilterType::Pointer object = BreakAngularPathListFilterType::New();

  return EXIT_SUCCESS;
}
