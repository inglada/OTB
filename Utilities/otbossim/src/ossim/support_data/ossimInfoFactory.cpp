//----------------------------------------------------------------------------
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: Factory for info objects.
// 
//----------------------------------------------------------------------------
// $Id$

#include <ossim/support_data/ossimInfoFactory.h>
#include <ossim/support_data/ossimInfoBase.h>
#include <ossim/support_data/ossimCcfInfo.h>
#include <ossim/support_data/ossimDtedInfo.h>
#include <ossim/support_data/ossimNitfInfo.h>
#include <ossim/support_data/ossimTiffInfo.h>


#include <ossim/base/ossimFilename.h>

ossimInfoFactory::~ossimInfoFactory()
{}

ossimInfoFactory* ossimInfoFactory::instance()
{
   static ossimInfoFactory sharedInstance;

   return &sharedInstance;
}

ossimInfoBase* ossimInfoFactory::create(const ossimFilename& file) const
{
   ossimInfoBase* result = 0;

   result = new ossimNitfInfo();
   if ( result->open(file) )
   {
      return result;
   }
   else
   {
      delete result;
      result = 0;
   }

   result = new ossimTiffInfo();
   if ( result->open(file) )
   {
      return result;
   }
   else
   {
      delete result;
      result = 0;
   }

   result = new ossimDtedInfo();
   if ( result->open(file) )
   {
      return result;
   }
   else
   {
      delete result;
      result = 0;
   }
   
   result = new ossimCcfInfo();
   if ( result->open(file) )
   {
      return result;
   }
   else
   {
      delete result;
      result = 0;
   }
   
   return result;
}

ossimInfoFactory::ossimInfoFactory()
{}

ossimInfoFactory::ossimInfoFactory(const ossimInfoFactory& /* obj */ )
{}

const ossimInfoFactory& ossimInfoFactory::operator=(
   const ossimInfoFactory& /* rhs */)
{
   return *this;
}
