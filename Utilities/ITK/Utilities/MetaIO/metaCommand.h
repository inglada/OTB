/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: metaCommand.h,v $
  Language:  C++
  Date:      $Date: 2006/01/01 17:31:45 $
  Version:   $Revision: 1.7 $

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __MetaCommand_H_
#define __MetaCommand_H_

#ifdef _MSC_VER
#pragma warning ( disable : 4786 )
#endif

#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <map>

class MetaCommand
{

public:

  typedef enum {INT,FLOAT,CHAR,STRING,LIST,FLAG} TypeEnumType;

  struct Field{
    std::string name;
    std::string description;
    std::string value;
    TypeEnumType type;
    bool externaldata;
    bool required;
    bool userDefined;
    };

  struct Option{
    std::string name;
    std::string description;
    std::string tag;
    std::vector<Field> fields;
    bool required;
    bool userDefined;
    bool complete;
  };

  typedef std::vector<Option>                OptionVector; 
  
  MetaCommand();
  ~MetaCommand() {}

  bool SetOption(Option option);
  bool SetOption(std::string name,
                 std::string tag,
                 bool required,
                 std::string description,
                 std::vector<Field> fields);
  bool SetOption(std::string name,
                 std::string tag,
                 bool required,
                 std::string description,
                 TypeEnumType type = FLAG,
                 std::string defVal = "");

  /** Fields are added in order */
  bool AddField(std::string name,
                std::string description,
                TypeEnumType type,
                bool externalData);
  
  /** Add a field to an option */
  bool AddOptionField(std::string optionName,
                      std::string name,
                      TypeEnumType type,
                      bool required=true,
                      std::string defVal = "",
                      std::string description = "");
  
  /** Collect all the information until the next tag 
   * \warning this function works only if the field is of type String */
  void SetOptionComplete(std::string optionName,
                         bool complete);  

  /** Get the values given the option name */
  bool GetValueAsBool(std::string optionName,std::string fieldName="");
  bool GetValueAsBool(Option option,std::string fieldName="");

  float GetValueAsFloat(std::string optionName,std::string fieldName="");
  float GetValueAsFloat(Option option,std::string fieldName="");

  int GetValueAsInt(std::string optionName,std::string fieldName="");
  int GetValueAsInt(Option option,std::string fieldName="");

  std::string GetValueAsString(std::string optionName,std::string fieldName="");
  std::string GetValueAsString(Option option,std::string fieldName="");

  std::list< std::string > GetValueAsList(std::string optionName);
  std::list< std::string > GetValueAsList(Option option);

  bool GetOptionWasSet(std::string optionName);
  bool GetOptionWasSet(Option option);

  /** List the options */
  void ListOptions();
  void ListOptionsXML();
  void ListOptionsSimplified();

  Option * GetOptionByMinusTag(std::string minusTag);
  Option * GetOptionByTag(std::string minusTag);

  bool OptionExistsByMinusTag(std::string minusTag);

  bool Parse(int argc, char* argv[]);
  
  /** Given an XML buffer fill in the command line arguments */
  bool ParseXML(const char* buffer);

  /** Extract the date from cvs date */
  std::string ExtractDateFromCVS(std::string date);

  /** Set the version of the app */
  std::string GetVersion() 
    { return m_Version; }

  void SetVersion(const char* version) 
    { m_Version=version; }
  
  /** Set the date of the app */
  std::string GetDate() 
    { return m_Date; }

  void SetDate(const char* date) 
    { m_Date=date; }

  long GetOptionId(Option* option);

  /** Return the list of options */
  const OptionVector & GetOptions() 
    { return m_OptionVector; }

  /** Return the list of parse options */
  const OptionVector & GetParsedOptions() 
    { return m_ParsedOptionVector; }

  void SetHelpCallBack(void (* newHelpCallBack)(void))
    { m_HelpCallBack = newHelpCallBack; }

protected:

  std::string TypeToString(TypeEnumType type);
  TypeEnumType StringToType(const char* type);

  /** Small XML helper */
  std::string GetXML(const char* buffer,const char* desc,unsigned long pos);

  std::string m_Version;
  std::string m_Date;

private:

  void         (* m_HelpCallBack)(void);

  OptionVector m_OptionVector;
  OptionVector m_ParsedOptionVector; // We store the parsed option in
                                     //   case we have multiple options

}; // end of class


#endif 
