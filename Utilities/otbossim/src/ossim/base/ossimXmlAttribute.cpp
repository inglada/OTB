//*******************************************************************
// Copyright (C) 2001 ImageLinks Inc.  All rights reserved.
//
// License:  See top level LICENSE.txt file.
// 
// Author:  Oscar Kramer (ossim port by D. Burken)
//
// Description:  
//
// Contains definition of class ossimXmlAttribute.
// 
//*****************************************************************************
// $Id: ossimXmlAttribute.cpp 12886 2008-05-21 18:00:43Z gpotts $

#include <iostream>
#include <sstream>

#include <ossim/base/ossimXmlAttribute.h>
#include <ossim/base/ossimNotifyContext.h>

RTTI_DEF2(ossimXmlAttribute, "ossimXmlAttribute", ossimObject, ossimErrorStatusInterface)

static std::istream& xmlskipws(std::istream& in)
{
   int c = in.peek();
   while((!in.fail())&&
         ((c == ' ') ||
          (c == '\t') ||
          (c == '\n')||
          (c == '\r')))
   {
      in.ignore(1);
      c = in.peek();
   }
   
   return in;
}

ossimXmlAttribute::ossimXmlAttribute(ossimString& spec)
{
   std::stringstream in(spec);

   read(in);
}

ossimXmlAttribute::ossimXmlAttribute(const ossimXmlAttribute& src)
   :theName(src.theName),
    theValue(src.theValue)
{
}

bool ossimXmlAttribute::read(std::istream& in)
{
   in >> xmlskipws;
   if(in.fail()) return false;
   if(readName(in))
   {
      in >> xmlskipws;
      if((in.peek() != '=')||
         (in.fail()))
      {
         setErrorStatus();
         return false;
      }
      in.ignore(1);
      if(readValue(in))
      {
         return true;
      }
      else
      {
         setErrorStatus();
         return false;
      }
   }
   return false;
   
#if 0   
   //
   // Pull out name:
   //
   theName = spec.before('=');
   theName = theName.trim();
   if (theName.empty())
   {
      ossimNotify(ossimNotifyLevel_FATAL) << "ossimXmlAttribute::ossimXmlAttribute \n"
                                           << "Bad attribute format encountered near:\n\""<< spec<<"\"\n"
                                           << "Parsing aborted...\n";
      setErrorStatus();
      
      return;
   }
   spec = spec.after('=');

   //***
   // Read value:
   //***
   char quote_char = spec[0];
   spec = spec.after(quote_char);  // after first quote
   theValue = spec.before(quote_char); // before second quote

   //
   // Reposition attribute specification to the start of next attribute or end
   // of tag:
   //
   spec = spec.after(quote_char);  // after second quote
   ossimString next_entry ("-?[+0-9A-Za-z<>]+");
   spec = spec.fromRegExp(next_entry.c_str());
#endif
}

ossimXmlAttribute::~ossimXmlAttribute()
{
}

ossimXmlAttribute::ossimXmlAttribute()
{
}

ossimXmlAttribute::ossimXmlAttribute(const ossimString& name,
                                     const ossimString& value)
{
   setNameValue(name, value);
}

const ossimString& ossimXmlAttribute::getName()  const
{
   return theName;
}

const ossimString& ossimXmlAttribute::getValue() const
{
   return theValue;
}

void ossimXmlAttribute::setNameValue(const ossimString& name,
                                     const ossimString& value)
{
   theName  = name;
   theValue = value;
}

void ossimXmlAttribute::setName(const ossimString& name)
{
   theName = name;
}

void ossimXmlAttribute::setValue(const ossimString& value)
{
   theValue = value;
}

std::ostream& operator << (std::ostream& os, const ossimXmlAttribute* xml_attr) 
{
   os << " " << xml_attr->theName << "=\"" << xml_attr->theValue << "\"";

   return os;
}


bool ossimXmlAttribute::readName(std::istream& in)
{
   in >>xmlskipws;
   theName = "";
   char c = in.peek();
   while((c != ' ')&&
         (c != '\n')&&
	 (c != '\r')&&
         (c != '\t')&&
         (c != '=')&&
         (c != '<')&&
         (c != '/')&&
         (c != '>')&&
         (!in.fail()))
   {
      theName += (char)in.get();
      c = in.peek();
   }

   return ((!in.fail())&&
           (theName != ""));
}

bool ossimXmlAttribute::readValue(std::istream& in)
{
   in >>xmlskipws;
   if(in.fail()) return false;
   theValue = "";
   char c = in.peek();
   bool done = false;
	char startQuote = '\0';
   if((c == '\'')||
      (c == '"'))
   {
		startQuote = c;
      theValue += in.get();
      while(!done&&!in.fail())
      {
         c = in.peek();
         if(c==startQuote)
         {
            theValue += c;
            done = true;
            in.ignore(1);
            
         }
         else if(c == '\n')
         {
            done = true;
         }
         else
         {
            theValue += in.get();
         }
      }
   }

   if(theValue != "")
   {
      ossimString::iterator startIter = theValue.begin();
      ossimString::iterator endIter   = --theValue.end();
      if(*startIter == startQuote)
      {
         ++startIter;
      }
      else
      {
         return false;
         setErrorStatus();
      }
      if(*endIter != startQuote)
      {
         return false;
         setErrorStatus();
      }
      theValue = ossimString(startIter, endIter);
   }
   return ((!in.bad())&&
           (theValue !=""));
}
