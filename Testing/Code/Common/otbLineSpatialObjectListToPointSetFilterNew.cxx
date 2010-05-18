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

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "itkPointSet.h"
#include "otbLineSpatialObjectList.h"
#include <iostream>

#include "otbLineSpatialObjectListToPointSetFilter.h"

int otbLineSpatialObjectListToPointSetFilterNew(int argc, char* argv[])
{
  typedef float                         InputPixelType;
  typedef otb::LineSpatialObjectList    LinesListType;
  typedef itk::PointSet<InputPixelType> PointSetType;

  typedef otb::LineSpatialObjectListToPointSetFilter<LinesListType,
                                                     PointSetType>   FilterType;

  FilterType::Pointer filter = FilterType::New();

  return EXIT_SUCCESS;
}
