/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itk_tiff.h,v $
  Language:  C++
  Date:      $Date: 2006/09/28 15:30:39 $
  Version:   $Revision: 1.1 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

  Portions of this code are covered under the VTK copyright.
  See VTKCopyright.txt or http://www.kitware.com/VTKCopyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itk_tiff_h
#define __itk_tiff_h

/* Use the tiff library configured for ITK.  */
#include "itkThirdParty.h"
#ifdef ITK_USE_SYSTEM_TIFF
# include <tiffio.h>
#else
# include <itktiff/tiffio.h>
#endif

#endif
