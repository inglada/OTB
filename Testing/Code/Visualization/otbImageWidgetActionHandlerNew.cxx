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
#include "otbImageWidgetActionHandler.h"

int otbImageWidgetActionHandlerNew(int argc, char * argv[])
{
  typedef otb::ImageWidgetActionHandler HandlerType;
  HandlerType::Pointer handler = HandlerType::New();

  return EXIT_SUCCESS;
}
