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

#include "otbLUMImageIO.h"
#include "itkExceptionObject.h"
#include <iostream>

int otbLUMImageIOTestCanRead(int argc, char* argv[])
{
  otb::LUMImageIO::Pointer lLUMImageIO = otb::LUMImageIO::New();
  bool lCanRead = lLUMImageIO->CanReadFile(argv[1]);
  if ( lCanRead == false)
    {
      std::cerr << "Erreur otb::LUMImageIO : impossible d'ouvrir l'image LUM "<<argv[1]<<"."<<std::endl;
      return EXIT_FAILURE;
    }

  
  return EXIT_SUCCESS;
}
