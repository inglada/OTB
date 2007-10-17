/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkLoggerBase.cxx,v $
  Language:  C++
  Date:      $Date: 2007/05/21 09:08:55 $
  Version:   $Revision: 1.8 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "itkLoggerBase.h"
#include "itksys/SystemTools.hxx"

namespace itk
{

LoggerBase::LoggerBase()
{
  this->m_PriorityLevel = LoggerBase::NOTSET;
  this->m_LevelForFlushing = LoggerBase::MUSTFLUSH;
  this->m_Clock = RealTimeClock::New();
  this->m_Output = MultipleLogOutput::New();
  this->m_TimeStampFormat = REALVALUE;
  this->m_HumanReadableFormat = "%Y %b %d %H:%M:%S";
}

LoggerBase::~LoggerBase()
{
//  this->m_Output->Flush();
}

/** Adds an output stream to the MultipleLogOutput for writing. */
void LoggerBase::AddLogOutput( OutputType* output )
{
  // delegates to MultipleLogOutput
  this->m_Output->AddLogOutput( output ); 
}

void LoggerBase::Write(PriorityLevelType level, std::string const & content)
{
  if( this->m_PriorityLevel >= level )
    {
    this->m_Output->Write(this->BuildFormattedEntry(level,content));
    if( this->m_LevelForFlushing >= level )
      {
      this->m_Output->Flush();
      }
    }
}

void LoggerBase::Flush()
{
  this->m_Output->Flush();
}

std::string 
LoggerBase
::BuildFormattedEntry(PriorityLevelType level, std::string const & content)
{
  static std::string m_LevelString[] = { "(MUSTFLUSH) ", "(FATAL) ", "(CRITICAL) ",
                                         "(WARNING) ", "(INFO) ", "(DEBUG) ", "(NOTSET) " };
  OStringStream s;
  switch( this->m_TimeStampFormat )
    {
    case REALVALUE:
      {
      s.precision(30);
      s << m_Clock->GetTimeStamp();
      break;
      }
    case HUMANREADABLE:
      {
      s << itksys::SystemTools::GetCurrentDateTime( this->m_HumanReadableFormat.c_str() );
      break;
      }
    }
  s << "  :  " << this->GetName() <<  "  " <<  m_LevelString[level] << content;
  
  return s.str();
}

/** Print contents of a LoggerBase */
void LoggerBase::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);

  os << indent << "Name: " << this->GetName() << std::endl;
  os << indent << "PriorityLevel: " << this->GetPriorityLevel()   << std::endl;
  os << indent << "LevelForFlushing: " << this->GetLevelForFlushing() << std::endl;
  os << indent << "TimeStampFormat: " << this->GetTimeStampFormat() << std::endl;
  os << indent << "HumanReadableFormat: " << this->GetHumanReadableFormat() << std::endl;
}

} //namespace
