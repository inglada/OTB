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

//#define MAIN
#include "otbONERAImageIO.h"
#include "itkExceptionObject.h"
#include <iostream>

int otbONERAImageIOTestCanRead(int argc, char* argv[])
{
  try
  {
  	otb::ONERAImageIO::Pointer lONERAImageIO = otb::ONERAImageIO::New();
        bool lCanRead = lONERAImageIO->CanReadFile(argv[1]);
        if ( lCanRead == false)
        {
                std::cerr << "Erreur otb::CAIImageIO : impossible d'ouvrir l'image "<<argv[1]<<"."<<std::endl;
                return EXIT_FAILURE;
        }
  }
  catch( itk::ExceptionObject & err ) 
  { 
    std::cerr << "Exception OTB attrappee dans exception ITK !" << std::endl; 
    std::cerr << err << std::endl; 
    return EXIT_FAILURE;
  } 
  catch( ... )
  {
    std::cerr << "Exception OTB non attrappee !" << std::endl; 
    return EXIT_FAILURE;
  }
  
  return EXIT_SUCCESS;
}
