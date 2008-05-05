/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkBrains2HeaderBase.cxx,v $
  Language:  C++
  Date:      $Date: 2008-01-24 21:02:07 $
  Version:   $Revision: 1.10 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

  Portions of this code are covered under the VTK copyright.
  See VTKCopyright.txt or http://www.kitware.com/VTKCopyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkExceptionObject.h"
#include "itkBrains2HeaderBase.h"
#include "itkBrains2HeaderFactory.h"
#include <iostream>

namespace itk {
Brains2HeaderBase::Brains2HeaderBase()
{
  //Nothing to be done here.
}
Brains2HeaderBase::~Brains2HeaderBase()
{
  this->ClearHeader();
}

void Brains2HeaderBase::ReadBrains2Header(std::string filename)
{
  std::ifstream   local_InputStream;
  local_InputStream.open( filename.c_str(), std::ios::in | std::ios::binary );
  if( !local_InputStream )
    {
    //               itk::itkExceptionMacro("Error opening image data file for reading.");
    }
  ReadBrains2Header(local_InputStream);
  local_InputStream.close();
}

void Brains2HeaderBase::ClearHeader(void)
{
  //Clear out all children, While children exist do
  while(m_Child.size() != 0)
    {
    //Get value of front of list
    Brains2HeaderBase * pi=m_Child.front();
    //remove value from list
    m_Child.pop_front();
    //delte value
    delete pi;
    }
  this->clear();
}


void Brains2HeaderBase::WriteBrains2Header(std::string filename) const
{
  std::ofstream   local_OutputStream;
  local_OutputStream.open( filename.c_str(), std::ios::out | std::ios::binary );
  if( !local_OutputStream )
    {
    //              itk::itkExceptionMacro("Error opening image data file for reading.");
    }
  WriteBrains2Header(local_OutputStream);
  local_OutputStream.close();
}

std::ifstream & Brains2HeaderBase::ReadBrains2Header(std::ifstream  & inputstream)
{
  std::string Key;
  //NOTE: tellg returns the position in number of bytes from the begining of the stream.
  const std::streampos FileStartPos = inputstream.tellg();
  //NOTE: the >> operator on a stream reads "words" at a time.
  inputstream >> Key;  //The first word to read must be "IPL_HEADER_BEGIN", with no value
  //std::cout << "Looking for " << this->GetHeaderBeginTag() <<" but found " << Key << std::endl;
  if(Key.find(this->GetHeaderBeginTag()) == std::string::npos )
    {
    //Return the file to it's position before attempting read.
    inputstream.seekg( FileStartPos );
    return inputstream;
    }
  else
    {
    this->push_back(std::list< std::pair<std::string,std::string> >::value_type(Key,""));
    }
  itk::Brains2HeaderFactory MyBrains2HdrFac;
  std::streampos PreKeyReadPosition = inputstream.tellg();
  inputstream >> Key; //Read key that follows "IPL_HEADER_BEGIN"
//DEBUG:  std::cerr << "ReadBrains2Header: GetHeaderEndTag is |" << this->GetHeaderEndTag() << "|" << std::endl;
  while(Key != this->GetHeaderEndTag() )  //If key = "IPL_HEADER_END", then there is no value
    {
//DEBUG:    std::cerr << "ReadBrains2Header: Key is |" << Key << "|" << std::endl;
    if(inputstream.eof() == true)
      {
      itkGenericExceptionMacro(<< "Unexpected end of file");
      }
    //Check for the case where the specific header type begins and ends
    //i.e. MASK_HEADER_BEGIN and MASK_HEADER_END, or IMAGE_HEADER_BEGIN and IMAGE_HEADER_END
    if ( Key.find("HEADER_BEGIN") != std::string::npos)
      {
      this->push_back(std::list< std::pair<std::string,std::string> >::value_type("--BEGIN_CHILD--",""));
      //Rewind to befor the key.
      inputstream.seekg( PreKeyReadPosition );
      //Need Factory Here to produce proper factory based on Key.
      this->m_Child.push_back(MyBrains2HdrFac.CreateBrains2HeaderReader(Key));
      if(this->m_Child.back() == NULL)
        {
        //DEBUG: Throw error
        return inputstream;
        }
      this->m_Child.back()->ReadBrains2Header(inputstream);
      PreKeyReadPosition = inputstream.tellg();
      inputstream >> Key;
      continue;
      }
    std::string Value;  //If key does not == "IPL_HEADER_END", then there must be a value
    inputstream >> Value;
    this->push_back(std::list< std::pair<std::string,std::string> >::value_type(Key,Value));
    //Read Next Key
    PreKeyReadPosition = inputstream.tellg();
    inputstream >> Key;
    }
  //pusch back the end key
  this->push_back(std::list< std::pair<std::string,std::string> >::value_type(Key,""));
  return inputstream;
}
std::ofstream & Brains2HeaderBase::WriteBrains2Header(std::ofstream & outputstream) const
{
  return outputstream;
}
void Brains2HeaderBase::PrintSelf(std::ostream &os) const
{
  std::list<Brains2HeaderBase *>::const_iterator childiterator=this->m_Child.begin();
  //For each element in internal list
  for(std::list< std::pair<std::string,std::string> >::const_iterator pi=this->begin();
      pi != this->end(); pi++)
    {
    if(pi->first == "--BEGIN_CHILD--")
      {
      //std::cout <<"Size of m_Child " << m_Child.size() << std::endl;
      (*childiterator)->PrintSelf(os);
      childiterator++;
      continue;
      }
    //os << "Key  " << pi->first << " Value " << pi->second << std::endl;
    os << pi->first << "  " << pi->second << std::endl;
    }
}

bool Brains2HeaderBase::DoesKeyExist(const std::string &KeyID) const
{
  std::list<Brains2HeaderBase *>::const_iterator childiterator=this->m_Child.begin();
  //For each element in internal list
  for(std::list< std::pair<std::string,std::string> >::const_iterator pi=this->begin();
      pi != this->end(); pi++)
    {
    if(pi->first == KeyID)
      {
      return true;
      }
    else if(pi->first == "--BEGIN_CHILD--")
      {
      if((*childiterator)->DoesKeyExist(KeyID) == true)
        {
        return true;
        }
      childiterator++;
      continue;
      }
    }
  //Return an empty string
  return false;
}


std::string Brains2HeaderBase::getString(const std::string &KeyID) const
{
  //this->PrintSelf(std::cout);
  std::list<Brains2HeaderBase *>::const_iterator childiterator=this->m_Child.begin();
  //For each element in internal list
  for(std::list< std::pair<std::string,std::string> >::const_iterator pi=this->begin();
      pi != this->end(); pi++)
    {
    if(pi->first == KeyID)
      {
      return pi->second;
      }
    else if(pi->first == "--BEGIN_CHILD--")
      {
      //std::cout <<"Size of m_Child " << m_Child.size() << std::endl;
      std::string TempStringValue=(*childiterator)->getString(KeyID);
      if(TempStringValue.length() != 0)
        {
        return TempStringValue;
        }
      childiterator++;
      continue;
      }
    }
  //Return an empty string
  return std::string("");
}

float Brains2HeaderBase::getFloat(const std::string &KeyID) const
{
  std::string TempStringValue=this->getString(KeyID);
  if(TempStringValue.length() != 0)
    {
    return static_cast< float >( atof( TempStringValue.c_str() ) );
    }
  return 0.0F;
}

int Brains2HeaderBase::getInt(const std::string &KeyID) const
{
  std::string TempStringValue=this->getString(KeyID);
  if( static_cast< int >( TempStringValue.length() ) != 0 )
    {
    return atoi(TempStringValue.c_str());
    }
  return 0;
}
} //End namespace itk
