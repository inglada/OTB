/*=========================================================================

  Program:   MetaIO
  Module:    $RCSfile: metaImageUtils.cxx,v $
  Language:  C++
  Date:      $Date: 2008-04-09 01:44:28 $
  Version:   $Revision: 1.12 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include <stdio.h>

#include "metaImageTypes.h"
#include <string.h>

#if (METAIO_USE_NAMESPACE)
namespace METAIO_NAMESPACE {
#endif


bool MET_StringToImageModality(const char * _str,
                               MET_ImageModalityEnumType * _type)
  {
  int i;

  for(i=0; i<MET_NUM_IMAGE_MODALITY_TYPES; i++)
    if(!strcmp(MET_ImageModalityTypeName[i], _str))
      {
      *_type = (MET_ImageModalityEnumType)i;
      return true;
      }

  *_type = MET_MOD_UNKNOWN;

  return false;
  }

bool MET_ImageModalityToString(MET_ImageModalityEnumType _type,
                               char * _str)
  {
  strcpy(_str, MET_ImageModalityTypeName[(int)_type]);
  return true;
  }

#if (METAIO_USE_NAMESPACE)
};
#endif

