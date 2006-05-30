/*=========================================================================
                                                                                
  Program:   gdcm
  Module:    $RCSfile: gdcmSQItem.h,v $
  Language:  C++
  Date:      $Date: 2006/02/18 12:36:39 $
  Version:   $Revision: 1.5 $
                                                                                
  Copyright (c) CREATIS (Centre de Recherche et d'Applications en Traitement de
  l'Image). All rights reserved. See Doc/License.txt or
  http://www.creatis.insa-lyon.fr/Public/Gdcm/License.html for details.
                                                                                
     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.
                                                                                
=========================================================================*/
#ifndef GDCMSQITEM_H
#define GDCMSQITEM_H

#include "gdcmDocEntrySet.h"
#include "gdcmElementSet.h"

#include <list>
#include <fstream>

namespace gdcm 
{
class DocEntry;

//-----------------------------------------------------------------------------
typedef std::list<DocEntry *> ListDocEntry;

//-----------------------------------------------------------------------------
/**
 * \brief a SeqEntry is composed by a set of SQItems.
 *        Each SQItem is composed by a set of DocEntry
 *        A DocEntry may be a SeqEntry
 *        ... and so forth 
 */ 
class GDCM_EXPORT SQItem : public DocEntrySet 
{
public:
   SQItem(int depthLevel);
   ~SQItem();

   virtual void Print(std::ostream &os = std::cout, std::string const &indent = "" ); 
   void WriteContent(std::ofstream *fp, FileType filetype);
   uint32_t ComputeFullLength();

   bool AddEntry(DocEntry *Entry); // add to the List
   bool RemoveEntry(DocEntry *EntryToRemove);
   bool RemoveEntryNoDestroy(DocEntry *EntryToRemove);
   void ClearEntry();
   void ClearEntryNoDestroy();
   void MoveObject(SQItem *source);
  
   DocEntry *GetFirstEntry();
   DocEntry *GetNextEntry();

   DocEntry *GetDocEntry(uint16_t group, uint16_t elem);

   bool IsEmpty() { return DocEntries.empty(); }

   /// \brief   returns the ordinal position of a given SQItem
   int GetSQItemNumber() { return SQItemNumber; }
   /// \brief   Sets the ordinal position of a given SQItem
   void SetSQItemNumber(int itemNumber) { SQItemNumber = itemNumber; }

   ///  \brief Accessor on \ref SQDepthLevel.
   int GetDepthLevel() { return SQDepthLevel; }                                                                             

   ///  \brief Accessor on \ref SQDepthLevel.
   void SetDepthLevel(int depth) { SQDepthLevel = depth; }

protected:
// Variables that need to be accessed in subclasses
   /// \brief Chained list of Doc Entries
   ListDocEntry DocEntries;
   /// Iterator, used to visit the entries
   ListDocEntry::iterator ItDocEntries;
   /// Iterator, used to visit the Val Entries (for Python users)
   ListDocEntry::iterator ItValEntries;
  
private:
   /// \brief Sequences can be nested. This \ref SQDepthLevel represents
   ///        the level of the nesting of instances of this class.
   ///        \ref SQDepthLevel and its \ref SeqEntry::SQDepthLevel
   ///        counterpart are only defined on printing purposes
   ///        (see \ref Print).
   int SQDepthLevel;

   /// \brief SQ Item ordinal number 
   int SQItemNumber;
};
} // end namespace gdcm
//-----------------------------------------------------------------------------
#endif
