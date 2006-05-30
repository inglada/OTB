/*=========================================================================
                                                                                
  Program:   gdcm
  Module:    $RCSfile: gdcmGlobal.h,v $
  Language:  C++
  Date:      $Date: 2006/02/18 12:36:39 $
  Version:   $Revision: 1.4 $
                                                                                
  Copyright (c) CREATIS (Centre de Recherche et d'Applications en Traitement de
  l'Image). All rights reserved. See Doc/License.txt or
  http://www.creatis.insa-lyon.fr/Public/Gdcm/License.html for details.
                                                                                
     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.
                                                                                
=========================================================================*/

#ifndef GDCMGLOBAL_H
#define GDCMGLOBAL_H

#include "gdcmCommon.h"

namespace gdcm 
{
class DictSet;
class VR;
class TS;
class DictGroupName;
class DicomDirElement;
//-----------------------------------------------------------------------------
/**
 * \brief   This class contains all globals elements that might be
 *          instanciated only once (singletons).
 */
class GDCM_EXPORT Global
{
public:
   Global();
   ~Global();

   static DictSet *GetDicts();
   static VR *GetVR();
   static TS *GetTS();
   static DictGroupName *GetDictGroupName();
   static DicomDirElement *GetDicomDirElements();

private:
   /// Pointer to a container, holding _all_ the Dicom Dictionaries.
   static DictSet *Dicts;
   /// Pointer to a hash table containing the 'Value Representations'.
   static VR *ValRes;
   /// \brief Pointer to a hash table containing the Transfer Syntax codes 
   ///        and their english description 
   static TS *TranSyn; 
   /// \brief Pointer to a hash table containing the Group codes 
   ///        and their english name (from NIH) 
   static DictGroupName *GroupName; 
   /// \brief Pointer to the hash table containing the Dicom Elements necessary 
   ///        to describe each part of a DICOMDIR 
   static DicomDirElement *ddElem;
};
} // end namespace gdcm

//-----------------------------------------------------------------------------
#endif
