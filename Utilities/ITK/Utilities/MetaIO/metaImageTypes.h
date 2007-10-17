/*=========================================================================

  Program:   MetaIO
  Module:    $RCSfile: metaImageTypes.h,v $
  Language:  C++
  Date:      $Date: 2007/05/31 13:53:13 $
  Version:   $Revision: 1.7 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "metaTypes.h"

#ifndef ITKMetaIO_METAIMAGETYPES_H
#define ITKMetaIO_METAIMAGETYPES_H

#if (METAIO_USE_NAMESPACE)
namespace METAIO_NAMESPACE {
#endif

typedef enum { MET_MOD_CT, MET_MOD_MR, MET_MOD_NM, MET_MOD_US, MET_MOD_OTHER,
       MET_MOD_UNKNOWN } MET_ImageModalityEnumType;

#define MET_NUM_IMAGE_MODALITY_TYPES 6

const char MET_ImageModalityTypeName[MET_NUM_IMAGE_MODALITY_TYPES][17] = {
   {'M','E','T','_','M','O','D','_','C','T','\0',' ',' ',' ',' ',' ',' '},
   {'M','E','T','_','M','O','D','_','M','R','\0',' ',' ',' ',' ',' ',' '},
   {'M','E','T','_','M','O','D','_','N','M','\0',' ',' ',' ',' ',' ',' '},
   {'M','E','T','_','M','O','D','_','U','S','\0',' ',' ',' ',' ',' ',' '},
   {'M','E','T','_','M','O','D','_','O','T','H','E','R','\0',' ',' ',' '},
   {'M','E','T','_','M','O','D','_','U','N','K','N','O','W','N','\0',' '}};

#if (METAIO_USE_NAMESPACE)
};
#endif

#endif
