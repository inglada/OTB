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

#include "otbLineSpatialObjectListToRightAnglePointSetFilter.h"
#include "otbImage.h"
#include "otbLineSpatialObjectList.h"
#include "itkPointSet.h"

int otbLineSpatialObjectListToRightAnglePointSetFilterNew(int argc, char * argv[])
{
  const unsigned int Dimension = 2;
  typedef float PixelType;

  /** Typedefs */
  typedef otb::Image<PixelType, Dimension>      ImageType;
  typedef otb::LineSpatialObjectList            LinesListType;
  typedef LinesListType::LineType               LineType;
  typedef std::vector<LineType *>               VectorLines;
  typedef itk::PointSet<VectorLines, Dimension> PointSetType;
  typedef otb::LineSpatialObjectListToRightAnglePointSetFilter<ImageType, LinesListType,
      PointSetType>    RightAngleFilterType;

  RightAngleFilterType::Pointer filter  = RightAngleFilterType::New();

  std::cout << filter << std::endl;

  return EXIT_SUCCESS;
}
