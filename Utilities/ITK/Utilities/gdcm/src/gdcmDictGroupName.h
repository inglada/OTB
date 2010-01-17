/*=========================================================================
                                                                                
  Program:   gdcm
  Module:    $RCSfile: gdcmDictGroupName.h,v $
  Language:  C++
  Date:      $Date: 2006-02-18 12:36:38 $
  Version:   $Revision: 1.2 $
                                                                                
  Copyright (c) CREATIS (Centre de Recherche et d'Applications en Traitement de
  l'Image). All rights reserved. See Doc/License.txt or
  http://www.creatis.insa-lyon.fr/Public/Gdcm/License.html for details.
                                                                                
     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.
                                                                                
=========================================================================*/

#ifndef GDCMDICTGROUPNAME_H
#define GDCMDICTGROUPNAME_H

#include "gdcmCommon.h"
#include <map>
#include <string>
#include <iostream>

namespace gdcm 
{

//-----------------------------------------------------------------------------
/// Group Name Hash Table
typedef std::map<uint16_t, TagName> DictGroupNameHT;

//-----------------------------------------------------------------------------
/**
 * \brief Container for dicom 'Group Name' Hash Table.
 *        (formerly NIH defined ACR-NEMA group name)
 * \note   This is a singleton
 */
class GDCM_EXPORT DictGroupName 
{
public:
   DictGroupName(void);
   ~DictGroupName();

   void Print(std::ostream &os = std::cout, 
              std::string const &indent = "" );

   const TagName &GetName(uint16_t group);

private:
   DictGroupNameHT groupName;
};
} // end namespace gdcm

//-----------------------------------------------------------------------------
#endif
