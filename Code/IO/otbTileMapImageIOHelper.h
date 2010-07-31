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
#ifndef __otbTileMapImageIOHelper_h
#define __otbTileMapImageIOHelper_h

#include <string>
#include <iostream>
#include <sstream>

namespace otb
{

/** \class TileMapImageIOHelper
 *  \brief This class IO TileMapImage
 *
 */

class TileMapImageIOHelper
{
public:
  /** Correspondance between depth and scale for the quad tree paradigm*/
  const std::string ConvertDepthToScale(const unsigned int depth) const;
}; // end class TileMapImageIOHelper

} // end namespace otb

#endif
