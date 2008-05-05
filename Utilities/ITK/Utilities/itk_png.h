/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itk_png.h,v $
  Language:  C++
  Date:      $Date: 2006-09-28 16:05:07 $
  Version:   $Revision: 1.1 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

  Portions of this code are covered under the VTK copyright.
  See VTKCopyright.txt or http://www.kitware.com/VTKCopyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itk_png_h
#define __itk_png_h

/* Use the png library configured for ITK.  */
#include "itkThirdParty.h"
#ifdef ITK_USE_SYSTEM_PNG
# include <png.h>
#else
# include <itkpng/png.h>
#endif

#endif
