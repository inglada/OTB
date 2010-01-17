/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkSmapsFileParser.txx,v $
  Language:  C++
  Date:      $Date: 2008-10-24 18:11:22 $
  Version:   $Revision: 1.1 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkSmapsFileParser_txx
#define __itkSmapsFileParser_txx

#include "itkSmapsFileParser.h"

#if defined(_WIN32) && !defined(__CYGWIN__)
# include <io.h>
# include <process.h>
#else
# include <unistd.h>
#endif

#include <fstream>  // std::ifstream
#include <numeric>  // std::accumulate
#include <algorithm> // std::find

namespace itk
{  
  

template<class TMapDataType>
MapFileParser<TMapDataType>
::~MapFileParser()
{
}

template<class TMapDataType>
bool 
MapFileParser<TMapDataType>::Update()
{
  this->ReadFile(m_MapFilePath);
  return m_MapFilePath.empty();
}

template<class TMapDataType>
typename MapFileParser<TMapDataType>::MemoryLoadType 
MapFileParser<TMapDataType>
::GetHeapUsage()
{
  if ( m_MapData.Empty() )
    {
    std::cerr << "Read a map file before quering memory usage";
    }
  return m_MapData.GetHeapUsage();
}

template<class TMapDataType>
typename MapFileParser<TMapDataType>::MemoryLoadType 
MapFileParser<TMapDataType>
::GetStackUsage()
{
  if ( m_MapData.Empty() )
    {
    std::cerr<< "Read a map file before quering memory usage";
    }
  return m_MapData.GetStackUsage();
}


template<class TMapDataType>
typename MapFileParser<TMapDataType>::MemoryLoadType 
MapFileParser<TMapDataType>
::GetTotalMemoryUsage( )
{ 
  if ( m_MapData.Empty() )
    {
    std::cerr<< "Read a map file before quering memory usage";
    }
  return m_MapData.GetTotalMemoryUsage();
}

template<class TMapDataType>
typename MapFileParser<TMapDataType>::MemoryLoadType 
MapFileParser<TMapDataType>
::GetMemoryUsage( const char * filter, const char * token )
{
  if ( m_MapData.Empty() )
    {
    std::cerr << "Read a map file before quering memory usage";
    }
  return m_MapData.GetMemoryUsage(filter,token);
}


//---------------------------------
//      SmapsFileParser
//---------------------------------


template<class TSmapsDataType>
SmapsFileParser<TSmapsDataType>
::~SmapsFileParser()
{
}

/* SmapsFileParser implementation */
template<class TSmapsDataType>
void SmapsFileParser<TSmapsDataType>::ReadFile( const std::string &mapFileLocation)
{
  std::stringstream filename;
  filename << mapFileLocation;

  // if location is empty (default parameter), use the regular linux smaps file.
  if ( filename.str().empty() )
    {
#if defined(WIN32) || defined (_WIN32)
    itkGenericExceptionMacro( << "smaps files don't exist on Windows" );
#else
    int pid = getpid();
    filename << "/proc/" << pid << "/smaps";
#endif
    }

  // open the Smaps file
  std::ifstream inputFile( filename.str().c_str() );

  // can't find or open the Smaps file
  if ( inputFile.is_open() == false )
    {
    std::cerr<< "The smaps file " << filename.str() << " could not be open";
    return;
    }

  try
    {
    //load the file
    inputFile >> this->m_MapData;
    }
  catch( ExceptionObject excp )
    {
    // propagate the exception
    itkGenericExceptionMacro( << "The smaps file " << filename.str() << " is an invalid file or contains errors" );
    }
  this->m_MapFilePath = filename.str();
}


template<class TVMMapDataType>
VMMapFileParser<TVMMapDataType>
::~VMMapFileParser()
{
}

/* VMapFileParser implementation */
template<class TVMMapDataType>
void VMMapFileParser<TVMMapDataType>::ReadFile( const std::string &mapFileLocation )
{
  try
    {
    if ( !mapFileLocation.empty() )
      {
      // open the VMap file
      std::ifstream inputFile( mapFileLocation.c_str() );
      // can't find or open the VMap file
      if ( inputFile.is_open() == false )
        {
        itkGenericExceptionMacro( << "The VMap file " << mapFileLocation << " could not be open" );
        return;
        }
      //load the file
      inputFile >> this->m_MapData;
      this->m_MapFilePath = mapFileLocation;
      }
    else
      {
#if defined(WIN32) || defined (_WIN32)
      itkGenericExceptionMacro( << "VMMap files don't exist on Windows" );
#else
      std::stringstream vmmapCommand;
      vmmapCommand << "vmmap " << getpid();

      FILE * vmmapCommandOutput = NULL;
      if ( (vmmapCommandOutput = popen(vmmapCommand.str().c_str(), "r")) == NULL)
        {
        itkGenericExceptionMacro( << "Error using pmap. Can execute pmap command" );
        }

      // Not optimal solution: copy the output into an std::istream object. 
      std::stringstream vmmapFile;
      char buf[256];
      while ( !feof(vmmapCommandOutput) )
        {
        fgets(buf,256,vmmapCommandOutput);
        vmmapFile << buf;
        }

      fclose(vmmapCommandOutput);
      //fill the data
      vmmapFile >> this->m_MapData;
      this->m_MapFilePath = "";
#endif
      }
    }
  catch( ExceptionObject excp )
    {
    // propagate the exception
    itkGenericExceptionMacro( << "The vmmap file is an invalid file or contains errors" );
    }
}


} //end namespace itk

#endif //__itkSmapsFileParser_txx
