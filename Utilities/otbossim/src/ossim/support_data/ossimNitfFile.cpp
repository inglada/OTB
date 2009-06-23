//*******************************************************************
//
//  License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
// 
// Description: Nitf support class
// 
//********************************************************************
// $Id: ossimNitfFile.cpp 14241 2009-04-07 19:59:23Z dburken $

#include <fstream>
#include <iostream>
#include <iomanip>

#include <ossim/support_data/ossimNitfFile.h>
#include <ossim/support_data/ossimNitfFileHeader.h>
#include <ossim/support_data/ossimNitfImageHeader.h>
#include <ossim/support_data/ossimNitfFileHeaderV2_0.h>
#include <ossim/support_data/ossimNitfFileHeaderV2_1.h>
#include <ossim/support_data/ossimNitfImageHeaderV2_0.h>
#include <ossim/support_data/ossimNitfImageHeaderV2_1.h>
#include <ossim/support_data/ossimNitfTagFactoryRegistry.h>
#include <ossim/support_data/ossimNitfRegisteredTag.h>
#include <ossim/support_data/ossimRpfToc.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimNotifyContext.h>



// Static trace for debugging
static ossimTrace traceDebug("ossimNitfFile:debug");

std::ostream& operator <<(std::ostream& out, const ossimNitfFile& data)
{
   return data.print(out);
}

std::ostream& ossimNitfFile::print(std::ostream& out,
                                   const std::string& prefix) const
{
   if(theNitfFileHeader.valid())
   {
      std::string pfx = prefix;
      pfx += "nitf.";
      theNitfFileHeader->print(out, pfx);
      
      ossim_int32 n = theNitfFileHeader->getNumberOfImages();
      for(ossim_int32 idx = 0; idx < n; ++idx)
      {
         ossimNitfImageHeader* ih = getNewImageHeader(idx);
         if(ih)
         {
            // Add our prefix onto prefix.
            std::string s = pfx;
            s += "image";
            s += ossimString::toString(idx);
            s += ".";
            
            ih->print(out, s);
            
            delete ih;
            ih = 0;
         }
      }

      //---
      // Check for RPF stuff:
      //---
      ossimNitfTagInformation info; 
      theNitfFileHeader->getTag(info, "RPFHDR");
      if(info.getTagName() == "RPFHDR")
      {
         // Open of the a.toc.
         ossimRpfToc* toc = new ossimRpfToc;
         if ( toc->parseFile(getFilename()) ==
              ossimErrorCodes::OSSIM_OK )
         {
            pfx += "rpf.";
            toc->print(out, pfx);
         }
         delete toc;
         toc = 0;
      }
      
   } // matches:  if(theNitfFileHeader.valid())

   return out;
   
}

ossimNitfFile::ossimNitfFile()
   : theFilename(""),
     theNitfFileHeader(0)
{
}

ossimNitfFile::~ossimNitfFile()
{
   theNitfFileHeader = 0;
}

bool ossimNitfFile::parseFile(const ossimFilename& file)
{
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "DEBUG ossimNitfFile::parseFile: "
         << "endtered......"
         << std::endl;
   }
   std::ifstream in(file.c_str(), std::ios::in|std::ios::binary);
   if (in.fail())
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "DEBUG ossimNitfFile::parseFile: "
            << "Could not open file:  " << file.c_str()
            << "\nReturning..." << std::endl;
      }
      return false;
   }
      
   if(theNitfFileHeader.valid())
   {
      theNitfFileHeader = 0;
   }

   char temp[10];
   in.read(temp, 9);
   in.seekg(0, std::ios::beg);
   temp[9] ='\0';
   
   theFilename = file;

   ossimString s(temp);
   if(s == "NITF02.00")
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "DEBUG: NITF Version 2.0"
            << std::endl;
      }
      theNitfFileHeader = new ossimNitfFileHeaderV2_0;
   }
   else if ( (s == "NITF02.10") || (s == "NSIF01.00") )
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "DEBUG: NITF Version 2.1"
            << std::endl;
      }
      theNitfFileHeader = new ossimNitfFileHeaderV2_1;
   }
   else
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "DEBUG ossimNitfFile::parseFile: "
            << "Not an NITF file!"
            << std::endl;
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "DEBUG ossimNitfFile::parseFile: returning...........false"
            << "endtered......"
            << std::endl;
      }
      return false;
   }

   if(theNitfFileHeader.valid())
   {
      try
      {
         theNitfFileHeader->parseStream(in);
      }
      catch(std::exception& e)
      {
         if (traceDebug())
         {
            ossimNotify(ossimNotifyLevel_WARN)
               << "ossimNitfFile::parseFile caught exception:\n"
               << e.what()
               << std::endl;
         }
         return false;
      }
   }

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "DEBUG ossimNitfFile::parseFile: returning...........true"
         << std::endl;
   }
   return true;
}

const ossimRefPtr<ossimNitfFileHeader> ossimNitfFile::getHeader() const
{
   return theNitfFileHeader;
}

ossimIrect ossimNitfFile::getImageRect()const
{
   if(theNitfFileHeader.valid())
   {
      return theNitfFileHeader->getImageRect();
   }

   return ossimIrect(ossimIpt(0,0), ossimIpt(0,0));
}

ossimNitfImageHeader* ossimNitfFile::getNewImageHeader(long imageNumber)const
{
   if(theNitfFileHeader.valid())
   {
      std::ifstream in(theFilename.c_str(), std::ios::in|std::ios::binary);

      return theNitfFileHeader->getNewImageHeader(imageNumber,
                                                  in);
      in.close();
   }
   
   return 0;
}

ossimNitfSymbolHeader* ossimNitfFile::getNewSymbolHeader(long symbolNumber)const
{
   if(theNitfFileHeader.valid())
   {
      std::ifstream in(theFilename.c_str(), std::ios::in|std::ios::binary);

      return theNitfFileHeader->getNewSymbolHeader(symbolNumber,
                                                   in);
      in.close();
   }
   
   return 0;
}

ossimNitfLabelHeader* ossimNitfFile::getNewLabelHeader(long labelNumber)const
{
   if(theNitfFileHeader.valid())
   {
      std::ifstream in(theFilename.c_str(), std::ios::in|std::ios::binary);

      return theNitfFileHeader->getNewLabelHeader(labelNumber,
                                                  in);
      in.close();
   }
   
   return 0;
}

ossimNitfTextHeader* ossimNitfFile::getNewTextHeader(long textNumber)const
{
   if(theNitfFileHeader.valid())
   {
      std::ifstream in(theFilename.c_str(), std::ios::in|std::ios::binary);

      return theNitfFileHeader->getNewTextHeader(textNumber,
                                                 in);
      in.close();
   }
   
   return 0;
}

ossimNitfDataExtensionSegment* ossimNitfFile::getNewDataExtensionSegment(long dataExtNumber)const
{
   if(theNitfFileHeader.valid())
   {
      std::ifstream in(theFilename.c_str(), std::ios::in|std::ios::binary);

      return theNitfFileHeader->getNewDataExtensionSegment(dataExtNumber,
                                                           in);
      in.close();
   }
   
   return 0;
}

ossimString ossimNitfFile::getVersion()const
{
   if(theNitfFileHeader.valid())
   {
      return ossimString(theNitfFileHeader->getVersion());
   }
   
   return ossimString("");
}

ossimFilename ossimNitfFile::getFilename() const
{
   return theFilename;
}
