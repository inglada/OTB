//----------------------------------------------------------------------------
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: Base class for Info object.
// 
//----------------------------------------------------------------------------
// $Id$

#include <ossim/support_data/ossimInfoBase.h>
#include <ossim/base/ossimIoStream.h> /* for ossimIOMemoryStream */
#include <ossim/base/ossimKeywordlist.h>

ossimInfoBase::ossimInfoBase()
   : theOverviewFlag(true)
{}

ossimInfoBase::~ossimInfoBase()
{}

void ossimInfoBase::setProcessOverviewFlag(bool flag)
{
   theOverviewFlag = flag;
}

bool ossimInfoBase::getProcessOverviewFlag() const
{
   return theOverviewFlag;
}


bool ossimInfoBase::getKeywordlist(ossimKeywordlist& kwl)
{  
   // Do a print to a memory stream.
   ossimIOMemoryStream memStr;
   print(memStr);

   // Give the result to the keyword list.
   return kwl.parseStream(memStr);
}
