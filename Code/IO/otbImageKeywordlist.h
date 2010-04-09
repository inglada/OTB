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
#ifndef __otbImageKeywordlist_h
#define __otbImageKeywordlist_h

#include <iostream>
#include "itkObject.h"
#include "itkObjectFactory.h"
#include "base/ossimString.h"
#include "base/ossimKeywordlist.h"

#include <map>

namespace otb
{

/** \class ImageKeywordlist
 * \brief Storage and conversion for OSSIM metadata
 *
 * \sa ImageSeriesReader
 * \sa ImageIOBase
 *
 * \ingroup Projections
 *
 */
class ITK_EXPORT ImageKeywordlist
{
public:
  /** Standard class typedefs. */
  typedef ImageKeywordlist Self;

  //virtual const char *GetNameOfClass() const
  //{return "ImageKeywordlist";}

  typedef std::map<ossimString, ossimString> KeywordlistMap;

  /** Run-time type information (and related methods). */
  itkTypeMacro(ImageKeywordlist, ImageKeywordlist);

  /** Get the internal map container */
  const KeywordlistMap& GetKeywordlist() const
  {
    return m_Keywordlist;
  }

  void SetKeywordlist(const ossimKeywordlist& kwl)
  {
    m_Keywordlist = kwl.getMap();
  }

  void Clear(void)
  {
    m_Keywordlist.clear();
  }

  unsigned int GetSize(void) const
  {
    return m_Keywordlist.size();
  }

  /** Get the Data object descriptor corresponding to the given key */
  const ossimString& GetMetadataByKey(const ossimString& key) const;

  virtual void convertToOSSIMKeywordlist(ossimKeywordlist& kwl) const;

  virtual void Print(std::ostream& os, itk::Indent indent = 0) const;

  ImageKeywordlist();
  virtual ~ImageKeywordlist();

  ImageKeywordlist(const Self &);
  void operator =(const Self&);

protected:
  /** Methods invoked by Print() to print information about the object
   * Typically not called by the user (use Print()
   * instead) but used in the hierarchical print process to combine the
   * output of several classes.  */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:

  /** Geo informations are in this map */
  KeywordlistMap m_Keywordlist;

//  char m_Delimiter;

//  void operator=(const Self&); //purposely not implemented

};

extern std::ostream & operator <<(std::ostream& os, const ImageKeywordlist& kwl);

} //namespace otb

#endif // __otbImageKeywordlist_h
