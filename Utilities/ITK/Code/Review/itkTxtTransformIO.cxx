/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkTxtTransformIO.cxx,v $
  Language:  C++
  Date:      $Date: 2008-04-05 15:22:57 $
  Version:   $Revision: 1.3 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "itkTxtTransformIO.h"
#include <itksys/SystemTools.hxx>
#include <vnl/vnl_matlab_read.h>
#include <vnl/vnl_matlab_write.h>

namespace itk
{
TxtTransformIO::
TxtTransformIO()
{
}

TxtTransformIO::
~TxtTransformIO()
{
}

bool
TxtTransformIO::
CanReadFile(const char *fileName)
{
  bool recognizedExtension = false;
  recognizedExtension |= (itksys::SystemTools::GetFilenameLastExtension(fileName) == ".txt");
  recognizedExtension |= (itksys::SystemTools::GetFilenameLastExtension(fileName) == ".tfm");
  return recognizedExtension;
  
}

bool
TxtTransformIO::
CanWriteFile(const char *fileName)
{
  return (itksys::SystemTools::GetFilenameLastExtension(fileName) == ".txt");
}


std::string 
TxtTransformIO::
trim(std::string const& source, char const* delims)
{
  std::string result(source);
  std::string::size_type index = result.find_last_not_of(delims);
  if(index != std::string::npos)
    {
    result.erase(++index);
    }

  index = result.find_first_not_of(delims);
  if(index != std::string::npos)
    {
    result.erase(0, index);
    }
  else
    {
    result.erase();
    }
  return result;
}

void 
TxtTransformIO::
Read()
{  
  TransformPointer transform;
  std::ifstream in;
  in.open ( m_FileName.c_str(), std::ios::in | std::ios::binary );
  if( in.fail() )
    {
    in.close();
    itkExceptionMacro ( "The file could not be opened for read access "
                        << std::endl << "Filename: \"" << m_FileName << "\"" );
    }

  OStringStream InData;

  // in.get ( InData );
  std::filebuf *pbuf;
  pbuf=in.rdbuf();

  // get file size using buffer's members
  int size=pbuf->pubseekoff (0,std::ios::end,std::ios::in);
  pbuf->pubseekpos (0,std::ios::in);

  // allocate memory to contain file data
  char* buffer=new char[size+1];

  // get file data  
  pbuf->sgetn (buffer,size); 
  buffer[size]='\0';
  itkDebugMacro ( "Read file transform Data" );
  InData << buffer;

  delete[] buffer;
  std::string data = InData.str();
  in.close();

  // Read line by line
  vnl_vector<double> VectorBuffer;
  std::string::size_type position = 0;
  
  Array<double> TmpParameterArray;
  Array<double> TmpFixedParameterArray;
  TmpParameterArray.clear();
  TmpFixedParameterArray.clear();
  bool haveFixedParameters = false;
  bool haveParameters = false;
 
  while ( position < data.size() )
    {
    // Find the next string
    std::string::size_type end = data.find ( "\n", position );
    std::string line = trim ( data.substr ( position, end - position ) );
    position = end+1;
    itkDebugMacro ("Found line: \"" << line << "\"" );

    if ( line.length() == 0 )
      {
      continue;
      }
    if (line[0] == '#' || std::string::npos == line.find_first_not_of (" \t"))
      {
      // Skip lines beginning with #, or blank lines
      continue;
      }

    // Get the name
    end = line.find ( ":" );
    if ( end == std::string::npos )
      {
      // Throw an error
      itkExceptionMacro ( "Tags must be delimited by :" );
      }
    std::string Name = trim ( line.substr ( 0, end ) );
    std::string Value = trim ( line.substr ( end + 1, line.length() ) );
    // Push back 
    itkDebugMacro ( "Name: \"" << Name << "\"" );
    itkDebugMacro ( "Value: \"" << Value << "\"" );
    itksys_ios::istringstream parse ( Value );
    VectorBuffer.clear();
    if ( Name == "Transform" )
      {
      this->CreateTransform(transform,Value);
      m_ReadTransformList.push_back ( transform );
      }
    else if ( Name == "Parameters" || Name == "FixedParameters" )
      {
      VectorBuffer.clear();

      // Read them
      parse >> VectorBuffer;
      itkDebugMacro ( "Parsed: " << VectorBuffer );
      if ( Name == "Parameters" )
        {
        TmpParameterArray = VectorBuffer;
        itkDebugMacro ( "Setting Parameters: " << TmpParameterArray );
        if ( haveFixedParameters )
          {
          transform->SetFixedParameters ( TmpFixedParameterArray );
          itkDebugMacro ( "Set Transform Fixed Parameters" );
          transform->SetParametersByValue ( TmpParameterArray );
          itkDebugMacro ( "Set Transform Parameters" );
          TmpParameterArray.clear();
          TmpFixedParameterArray.clear(); 
          haveFixedParameters = false;
          haveParameters = false;
          }
        else
          {
          haveParameters = true;
          }   
        }
      else if ( Name == "FixedParameters" )
        {
        TmpFixedParameterArray = VectorBuffer;
        itkDebugMacro ( "Setting Fixed Parameters: " << TmpFixedParameterArray );
        if ( !transform )
          {
          itkExceptionMacro ("Please set the transform before parameters"
                             "or fixed parameters" );
          }
        if ( haveParameters )
          {
          transform->SetFixedParameters ( TmpFixedParameterArray );
          itkDebugMacro ( "Set Transform Fixed Parameters" );
          transform->SetParametersByValue ( TmpParameterArray );
          itkDebugMacro ( "Set Transform Parameters" );
          TmpParameterArray.clear();
          TmpFixedParameterArray.clear(); 
          haveFixedParameters = false;
          haveParameters = false;
          }
        else
          {
          haveFixedParameters = true;
          }
        }
      }
    }
}

void 
TxtTransformIO::
Write()
{
  ConstTransformListType::iterator it = m_WriteTransformList.begin();
  vnl_vector<double> TempArray;
  std::ofstream out;
  this->OpenStream(out,false);

  out << "#Insight Transform File V1.0" << std::endl;
  int count = 0;
  while(it != m_WriteTransformList.end())
    {
    out << "# Transform " << count << std::endl;
    out << "Transform: " << (*it)->GetTransformTypeAsString() << std::endl;

    TempArray = (*it)->GetParameters();
    out << "Parameters: " << TempArray << std::endl;
    TempArray = (*it)->GetFixedParameters();
    out << "FixedParameters: " << TempArray << std::endl;
    it++;
    count++;
    }
  out.close();
}

}
