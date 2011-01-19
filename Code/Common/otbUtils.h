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
#ifndef __otbUtils_h
#define __otbUtils_h

#include "itkMacro.h"

namespace otb
{

/** \class Utils
 * \brief Utils operations.
 *
 * Some usefull utilities in use across the library
 *
 */
class ITK_EXPORT Utils
{
public:

  /** Check that the values for lon, lat are reasonable */
  static bool IsLonLatValid(double lon, double lat);

};

} // namespace otb

#endif
