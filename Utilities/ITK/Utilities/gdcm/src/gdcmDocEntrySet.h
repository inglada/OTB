/*=========================================================================
                                                                                
  Program:   gdcm
  Module:    $RCSfile: gdcmDocEntrySet.h,v $
  Language:  C++
  Date:      $Date: 2006/07/10 20:08:21 $
  Version:   $Revision: 1.6 $
                                                                                
  Copyright (c) CREATIS (Centre de Recherche et d'Applications en Traitement de
  l'Image). All rights reserved. See Doc/License.txt or
  http://www.creatis.insa-lyon.fr/Public/Gdcm/License.html for details.
                                                                                
     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.
                                                                                
=========================================================================*/

#ifndef GDCMDOCENTRYSET_H
#define GDCMDOCENTRYSET_H

#include "gdcmBase.h"
#include <fstream>

namespace gdcm 
{
//-----------------------------------------------------------------------------
class DocEntry;
class ValEntry;
class BinEntry;
class SeqEntry;
class DictEntry;

typedef TagKey BaseTagKey;

//-----------------------------------------------------------------------------
/**
 * \brief
 * \ref DocEntrySet is an abstract base class for \ref ElementSet
 * and \ref SQItem which are both containers for DocEntries.
 *  - \ref ElementSet is based on the STL map<> container
 * (see \ref ElementSet::TagHT)
 *  - \ref SQItem is based on an STL list container (see \ref ListDocEntry).
 * 
 * Since the syntax for adding a new element to a map<> or a list<>
 * differ, \ref DocEntrySet is designed as an adapter to unify the
 * interfaces of \ref DocEntrySet and \ref ElementSet.
 * 
 * As an illustration of this design, please refer to the implementation
 * of \ref AddEntry (or any pure virtual method) in both derived classes.
 * This adapter unification of interfaces enables the parsing of a
 * DICOM header containing (optionaly heavily nested) sequences to be
 * written recursively [see \ref Document::ParseDES 
 * which calls \ref Document::ParseSQ, which in turn calls 
 * \ref Document::ParseDES ].
 *
 * \note Developpers should strongly resist to the temptation of adding
 *       members to this class since this class is designed as an adapter 
 *       in the form of an abstract base class.
 */
class GDCM_EXPORT DocEntrySet : public Base
{
public:
   /// Canonical Constructor
   DocEntrySet();
   /// Canonical Destructor
   virtual ~DocEntrySet() {}

   /// \brief write any type of entry to the entry set
   virtual void WriteContent (std::ofstream *fp, FileType filetype) = 0;

   /// \brief Remove all Entry of the current set
   virtual void ClearEntry() = 0;
   /// \brief adds any type of entry to the current set
   virtual bool AddEntry(DocEntry *entry) = 0;
   /// \brief Removes any type of entry out of the entry set, and destroys it
   virtual bool RemoveEntry(DocEntry *entryToRemove) = 0;
   /// \brief Removes any type of entry out of the entry set, DOESN'T destroy it
   virtual bool RemoveEntryNoDestroy(DocEntry *entryToRemove) = 0;
   /// \brief Gets the first entry (of any type) of the current set
   virtual DocEntry *GetFirstEntry()=0;
   /// \brief Gets the next entry (of any type) of the current set
   virtual DocEntry *GetNextEntry()=0;

   virtual std::string GetEntryValue(uint16_t group, uint16_t elem);
   virtual void *GetEntryBinArea(uint16_t group, uint16_t elem);
   
   virtual int GetEntryLength(uint16_t group, uint16_t elem);
   virtual std::string GetEntryVR(uint16_t group, uint16_t elem);
   virtual std::string GetEntryForcedAsciiValue(uint16_t group, uint16_t elem);
   /// \brief Gets any type of DocEntry, identified by its (group,elem)
   virtual DocEntry *GetDocEntry(uint16_t group, uint16_t elem) = 0;
   /// \brief Gets a ValEntry, identified by its (group, elem)
   ValEntry *GetValEntry(uint16_t group, uint16_t elem);
   /// \brief Gets a BinEntry, identified by its (group,elem)
   BinEntry *GetBinEntry(uint16_t group, uint16_t elem);
   /// \brief Gets a SeqEntry, identified by its (group,elem)
   SeqEntry *GetSeqEntry(uint16_t group, uint16_t elem);

   bool SetValEntry(std::string const &content,
                    uint16_t group, uint16_t elem);
   bool SetBinEntry(uint8_t *content, int lgth,
                    uint16_t group, uint16_t elem);
   bool SetValEntry(std::string const &content, ValEntry *entry);
   bool SetBinEntry(uint8_t *content, int lgth, BinEntry *entry);

   ValEntry *InsertValEntry(std::string const &value,
                                    uint16_t group, uint16_t elem,
                                    TagName const &vr = GDCM_UNKNOWN);
   BinEntry *InsertBinEntry(uint8_t *binArea, int lgth,
                                    uint16_t group, uint16_t elem,
                                    TagName const &vr = GDCM_UNKNOWN);
   SeqEntry *InsertSeqEntry(uint16_t group, uint16_t elem);
   /// \brief Tells us if the set contains no entry
   virtual bool IsEmpty() = 0;
   virtual bool CheckIfEntryExist(uint16_t group, uint16_t elem);

// DocEntry  related utilities 
   ValEntry *NewValEntry(uint16_t group,uint16_t elem,
                         TagName const &vr = GDCM_UNKNOWN);
   BinEntry *NewBinEntry(uint16_t group, uint16_t elem,
                         TagName const &vr = GDCM_UNKNOWN);
   SeqEntry *NewSeqEntry(uint16_t group, uint16_t elem);

// DictEntry  related utilities 
   DictEntry *NewVirtualDictEntry(uint16_t group,uint16_t elem,
                                  TagName const &vr     = GDCM_UNKNOWN,
                                  TagName const &vm     = GDCM_UNKNOWN,
                                  TagName const &name   = GDCM_UNKNOWN );

protected:
// DictEntry  related utilities
   DictEntry *GetDictEntry(uint16_t group, uint16_t elem);
   DictEntry *GetDictEntry(uint16_t group, uint16_t elem,
                           TagName const &vr);
   /// To be able to backtrack (Private Sequence, Implicit VR related pb)
   DocEntry *PreviousDocEntry;

private:
};

} // end namespace gdcm
//-----------------------------------------------------------------------------
#endif

