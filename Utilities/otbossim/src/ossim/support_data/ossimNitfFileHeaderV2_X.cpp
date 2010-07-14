//----------------------------------------------------------------------------
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
// 
//----------------------------------------------------------------------------
// $Id: ossimNitfFileHeaderV2_X.cpp 16997 2010-04-12 18:53:48Z dburken $

#include <ossim/support_data/ossimNitfFileHeaderV2_X.h>
#include <ossim/support_data/ossimNitfCommon.h>

#include <iomanip>
#include <sstream>
#include <ossim/base/ossimDate.h> /* for ossimLocalTm */
#include <ossim/base/ossimDateProperty.h>
#include <ossim/base/ossimNumericProperty.h>
#include <ossim/base/ossimStringProperty.h>
#include <ossim/support_data/ossimNitfCommon.h>

RTTI_DEF1(ossimNitfFileHeaderV2_X, "ossimNitfFileHeaderV2_X", ossimNitfFileHeader);
static ossimString monthConversionTable[] = {"   ", "JAN", "FEB", "MAR", "APR", "MAY", "JUN", "JUL", "AUG", "SEP", "OCT", "NOV", "DEC"};

const ossimString ossimNitfFileHeaderV2_X::FILE_TYPE_KW  = "file_type";
const ossimString ossimNitfFileHeaderV2_X::VERSION_KW  = "version";
const ossimString ossimNitfFileHeaderV2_X::CLEVEL_KW  = "clevel";
const ossimString ossimNitfFileHeaderV2_X::STYPE_KW   = "stype";
const ossimString ossimNitfFileHeaderV2_X::OSTAID_KW  = "ostaid";
const ossimString ossimNitfFileHeaderV2_X::FDT_KW     = "fdt";  
const ossimString ossimNitfFileHeaderV2_X::FTITLE_KW  = "ftitle";
const ossimString ossimNitfFileHeaderV2_X::FSCLAS_KW  = "fsclas";
const ossimString ossimNitfFileHeaderV2_X::FSCODE_KW  = "fscode";
const ossimString ossimNitfFileHeaderV2_X::FSCTLH_KW  = "fsctlh";
const ossimString ossimNitfFileHeaderV2_X::FSREL_KW   = "fsrel";
const ossimString ossimNitfFileHeaderV2_X::FSCAUT_KW  = "fscaut";
const ossimString ossimNitfFileHeaderV2_X::FSCTLN_KW  = "fsctln";
const ossimString ossimNitfFileHeaderV2_X::FSCOP_KW   = "fscop";
const ossimString ossimNitfFileHeaderV2_X::FSCPYS_KW  = "fscpys";
const ossimString ossimNitfFileHeaderV2_X::ENCRYP_KW  = "encryp";
const ossimString ossimNitfFileHeaderV2_X::ONAME_KW   = "oname";
const ossimString ossimNitfFileHeaderV2_X::OPHONE_KW  = "ophone";

ossimNitfFileHeaderV2_X::ossimNitfFileHeaderV2_X()
{
}

void ossimNitfFileHeaderV2_X::setComplexityLevel(const ossimString& level)
{
   ossimNitfCommon::setField(theComplexityLevel, level, 2, std::ios::right, '0');
}

void ossimNitfFileHeaderV2_X::setSystemType(const ossimString& systemType)
{
   ossimNitfCommon::setField(theSystemType, systemType, 4);
}

void ossimNitfFileHeaderV2_X::setOriginatingStationId(const ossimString& originationId)
{
   ossimNitfCommon::setField(theOriginatingStationId, originationId, 10);
}

ossimString ossimNitfFileHeaderV2_X::formatDate(const ossimString& version,
                                                const ossimLocalTm& d)
{
   // Convert to ZULU as per spec for both versions.
   ossimLocalTm d1 = d.convertToGmt();
   
   std::ostringstream out;

   if(version.contains("2.1"))
   {
      out << std::setw(4)
      << std::setfill('0')
      << d1.getYear()
      << std::setw(2)
      << std::setfill('0')
      << d1.getMonth()
      << std::setw(2)
      << std::setfill('0')
      << d1.getDay()
      << std::setw(2)
      << std::setfill('0')
      << d1.getHour()
      << std::setw(2)
      << std::setfill('0')
      << d1.getMin()
      << std::setw(2)
      << std::setfill('0')
      << d1.getSec();
   }
   else
   {
      out  << std::setw(2)
      << std::setfill('0')
      << d1.getDay()
      << std::setw(2)
      << std::setfill('0')
      << d1.getHour()
      << std::setw(2)
      << std::setfill('0')
      << d1.getMin()
      << std::setw(2)
      << std::setfill('0')
      << d1.getSec()
      << "Z"
      <<monthConversionTable[d1.getMonth()]
      << std::setw(2)
      << std::setfill('0')
      <<d1.getShortYear();
      
   }
      
   return out.str();
}

void ossimNitfFileHeaderV2_X::setDate(const ossimLocalTm& d)
{
   memcpy(theDateTime, formatDate(getVersion(), d).c_str(), 14);
}

void ossimNitfFileHeaderV2_X::setDate(const ossimString& d)
{
   if(d.size()==14)
   {
      memcpy(theDateTime, d.c_str(), 14);
   }
}

void ossimNitfFileHeaderV2_X::setDate()
{
   setDate(ossimLocalTm(0));
}  

void ossimNitfFileHeaderV2_X::setTitle(const ossimString& title)
{
   ossimNitfCommon::setField(theFileTitle, title, 80);
}

void ossimNitfFileHeaderV2_X::setFileSecurityClassification(const ossimString& securityClassification)
{
   ossimNitfCommon::setField(theSecurityClassification, securityClassification, 1);
}

void ossimNitfFileHeaderV2_X::setCopyNumber(const ossimString& copyNumber)
{
   if(copyNumber.trim() == "")
   {
      memset(theCopyNumber, '0', 5);
   }
   else
   {
      ossimNitfCommon::setField(theCopyNumber, copyNumber, 5, std::ios::right, '0');
   }
}

void ossimNitfFileHeaderV2_X::setNumberOfCopies(const ossimString& numberOfCopies)
{
   if(numberOfCopies.trim() == "")
   {
      memset(theNumberOfCopies, '0', 5);
   }
   else
   {
      ossimNitfCommon::setField(theNumberOfCopies, numberOfCopies, 5, std::ios::right, '0');
   }
}

void ossimNitfFileHeaderV2_X::setEncryption(const ossimString& encryption)
{
   ossimNitfCommon::setField(theEncryption, encryption, 1);
}

ossimString ossimNitfFileHeaderV2_X::getComplexityLevel()const
{
   return theComplexityLevel;
}

ossimString ossimNitfFileHeaderV2_X::getSystemType()const
{
   return theSystemType;
}

ossimString ossimNitfFileHeaderV2_X::getOriginatingStationId()const
{
   return theOriginatingStationId;
}

ossimString ossimNitfFileHeaderV2_X::getDate()const
{
   return theDateTime;
}

ossimString ossimNitfFileHeaderV2_X::getTitle()const
{
   return theFileTitle;
}

ossimString ossimNitfFileHeaderV2_X::getSecurityClassification()const
{
   return theSecurityClassification;
}

ossimString ossimNitfFileHeaderV2_X::getCopyNumber()const
{
   return theCopyNumber;
}

ossimString ossimNitfFileHeaderV2_X::getNumberOfCopies()const
{
   return theNumberOfCopies;
}

ossimString ossimNitfFileHeaderV2_X::getEncryption()const
{
   return theEncryption;
}

void ossimNitfFileHeaderV2_X::setProperty(ossimRefPtr<ossimProperty> property)
{
   if(!property.valid()) return;

   ossimString name = property->getName();
   if(name == STYPE_KW)
   {
      setSystemType(property->valueToString());
   }
   else if(name == OSTAID_KW)
   {
      setOriginatingStationId(property->valueToString());
   }
   else if(name == FDT_KW)
   {
      setDate(property->valueToString());
   }
   else if(name == FTITLE_KW)
   {
      setTitle(property->valueToString());
   }
   else if(name == FSCLAS_KW)
   {
      setFileSecurityClassification(property->valueToString());
   }
   else if(name == FSCODE_KW)
   {
      setCodeWords(property->valueToString()); 
   }
   else if(name == FSCTLH_KW)
   {
      setControlAndHandling(property->valueToString());
   }
   else if(name == FSREL_KW)
   {
      setReleasingInstructions(property->valueToString());
   }
   else if(name == FSCAUT_KW)
   {
      setClassificationAuthority(property->valueToString()); 
   }
   else if(name == FSCTLN_KW)
   {
      setSecurityControlNumber(property->valueToString());
   }
   else if(name == FSCOP_KW)
   {
      setCopyNumber(property->valueToString());
   }
   else if(name == FSCPYS_KW)
   {
      setNumberOfCopies(property->valueToString());
   }
   else if(name == ENCRYP_KW)
   {
      setEncryption(property->valueToString());
   }
   else if(name == ONAME_KW)
   {
      setOriginatorsName(property->valueToString());
   }
   else if(name == OPHONE_KW)
   {
      setOriginatorsPhone(property->valueToString());
   }
   else
   {
      ossimNitfFileHeader::setProperty(property);
   }
}

ossimRefPtr<ossimProperty> ossimNitfFileHeaderV2_X::getProperty(const ossimString& name)const
{
   ossimRefPtr<ossimProperty> property = 0;
   
   if(name == VERSION_KW)
   {
      property = new ossimStringProperty(name, ossimString(getVersion()));
   }
   else if(name == FILE_TYPE_KW)
   {
      property = new ossimStringProperty(name, "NITF");
   }
   else if(name == CLEVEL_KW)
   {
      ossimNumericProperty* numericProperty =
         new ossimNumericProperty(name,
                                  getComplexityLevel(),
                                  1,
                                  99);
      numericProperty->setNumericType(ossimNumericProperty::ossimNumericPropertyType_INT);
      property = numericProperty;
      
   }
   else if(name == STYPE_KW)
   {
      property = new ossimStringProperty(name, getSystemType().trim());
   }
   else if(name == OSTAID_KW)
   {
      property = new ossimStringProperty(name, getOriginatingStationId().trim());
   }
   else if(name == FDT_KW)
   {
      property = new ossimStringProperty(name, getDate());
   }
   else if(name == FTITLE_KW)
   {
      property = new ossimStringProperty(name, getTitle().trim());
   }
   else if(name == FSCLAS_KW)
   {
      ossimStringProperty* stringProperty =
         new ossimStringProperty(name,
                                 getSecurityClassification().trim(),
                                 false);
      
      stringProperty->addConstraint("");
      stringProperty->addConstraint("T");
      stringProperty->addConstraint("S");
      stringProperty->addConstraint("C");
      stringProperty->addConstraint("R");
      stringProperty->addConstraint("U");
      
      property = stringProperty;
   }
   else if(name == FSCODE_KW)
   {
      property = new ossimStringProperty(name,
                                         getCodeWords().trim());
   }
   else if(name == FSCTLH_KW)
   {
      property = new ossimStringProperty(name,
                                         getControlAndHandling().trim());
   }
   else if(name == FSREL_KW)
   {
      property = new ossimStringProperty(name,
                                         getReleasingInstructions().trim());
   }
   else if(name == FSCAUT_KW)
   {
      property = new ossimStringProperty(name,
                                         getClassificationAuthority().trim());
   }
   else if(name == FSCTLN_KW)
   {
      property = new ossimStringProperty(name,
                                         getSecurityControlNumber().trim());
      
   }
   else if(name == FSCOP_KW)
   {
      property = new ossimStringProperty(name,
                                         getCopyNumber().trim());
   }
   else if(name == FSCPYS_KW)
   {
      property = new ossimStringProperty(name,
                                         getNumberOfCopies().trim());
   }
   else if(name == ENCRYP_KW)
   {
      property = new ossimStringProperty(name,
                                         getEncryption().trim(),
                                         false);
   }
   else
   {
      property = ossimNitfFileHeader::getProperty(name).get();
   }
   
   return property;
}

void ossimNitfFileHeaderV2_X::getPropertyNames(std::vector<ossimString>& propertyNames)const
{
   propertyNames.push_back(FILE_TYPE_KW);
   propertyNames.push_back(VERSION_KW);
   propertyNames.push_back(CLEVEL_KW);
   propertyNames.push_back(STYPE_KW);
   propertyNames.push_back(OSTAID_KW);
   propertyNames.push_back(FDT_KW);
   propertyNames.push_back(FTITLE_KW);
   propertyNames.push_back(FSCLAS_KW);
   propertyNames.push_back(FSCODE_KW);
   propertyNames.push_back(FSCTLH_KW);
   propertyNames.push_back(FSREL_KW);
   propertyNames.push_back(FSCAUT_KW);
   propertyNames.push_back(FSCTLN_KW);
   propertyNames.push_back(FSCOP_KW);
   propertyNames.push_back(FSCPYS_KW);
   propertyNames.push_back(ENCRYP_KW);
   propertyNames.push_back(ONAME_KW);
   propertyNames.push_back(OPHONE_KW);
}
