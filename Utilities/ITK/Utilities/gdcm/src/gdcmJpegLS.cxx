/*=========================================================================
                                                                                
  Program:   gdcm
  Module:    $RCSfile: gdcmJpegLS.cxx,v $
  Language:  C++
  Date:      $Date: 2006-02-18 12:36:39 $
  Version:   $Revision: 1.3 $
                                                                                
  Copyright (c) CREATIS (Centre de Recherche et d'Applications en Traitement de
  l'Image). All rights reserved. See Doc/License.txt or
  http://www.creatis.insa-lyon.fr/Public/Gdcm/License.html for details.
                                                                                
     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.
                                                                                
=========================================================================*/
#include "gdcmFileHelper.h"
#include "gdcmDebug.h"

#include <iostream>
#include <fstream>

namespace gdcm 
{

//-----------------------------------------------------------------------------
 /**
 * \brief   routine for JPEG decompression. WARNING LPEG-LS is NOT
 *   the 'classical' jpeg lossless. See :
 *   http://www.jpeg.org/jpeg/jpegls.html
 *   gdcm DOES deal with 'classical' jpeg lossless
 *
 * @param fp pointer to an already open file descriptor 
 *                      JPEG-LS encoded image
 * @param image_buffer to receive uncompressed pixels
 * @return 1 on success, 0 on error
 * @warning : not yet made
 */

bool gdcm_read_JPEGLS_file (std::ifstream *fp, void *image_buffer)
{
   (void)fp;
   (void)image_buffer;
   gdcmWarningMacro( "Sorry JPEG-LS File not yet taken into account" );
   return false;
}
} // end namespace gdcm

//-----------------------------------------------------------------------------

