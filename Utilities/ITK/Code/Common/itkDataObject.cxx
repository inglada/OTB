/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkDataObject.cxx,v $
  Language:  C++
  Date:      $Date: 2007-12-23 17:59:28 $
  Version:   $Revision: 1.58 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

  Portions of this code are covered under the VTK copyright.
  See VTKCopyright.txt or http://www.kitware.com/VTKCopyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkDataObject.h"
#include "itkProcessObject.h"
#include "itkObjectFactory.h"
#include "itkSmartPointerForwardReference.txx"

// Manual instantiation is necessary to prevent link errors
template class itk::SmartPointerForwardReference<itk::ProcessObject>;

namespace itk
{
  
// after use by filter
bool DataObject::m_GlobalReleaseDataFlag = false;

DataObjectError
::DataObjectError()
  : ExceptionObject(), m_DataObject(0)
{
}
  
DataObjectError
::DataObjectError(const char *file, unsigned int lineNumber)
  : ExceptionObject(file, lineNumber), m_DataObject(0)
{
}

DataObjectError
::DataObjectError(const std::string& file, unsigned int lineNumber)
  : ExceptionObject(file, lineNumber), m_DataObject(0)
{
}  

DataObjectError
::DataObjectError(const DataObjectError &orig)
  : ExceptionObject( orig )
{
  m_DataObject = orig.m_DataObject;
}

DataObjectError&
DataObjectError
::operator=( const DataObjectError& orig)
{
  ExceptionObject::operator= (orig);
  m_DataObject = orig.m_DataObject;
  return *this;
}

void
DataObjectError
::SetDataObject(DataObject *dobj)
{
  m_DataObject = dobj;
}

DataObject*
DataObjectError
::GetDataObject()
{
  return m_DataObject;
}


void
DataObjectError
::PrintSelf(std::ostream& os, Indent indent) const
{
  ExceptionObject::Print( os);
    
  os << indent << "Data object: ";
  if (m_DataObject)
    {
    os << std::endl;
    m_DataObject->PrintSelf(os, indent.GetNextIndent());
    }
  else
    {
    os << "(None)" << std::endl;
    }
}


InvalidRequestedRegionError
::InvalidRequestedRegionError()
  : DataObjectError()
{
}

InvalidRequestedRegionError
::InvalidRequestedRegionError(const char *file, unsigned int lineNumber)
  : DataObjectError(file, lineNumber)
{
}

InvalidRequestedRegionError
::InvalidRequestedRegionError(const std::string& file, unsigned int lineNumber)
  : DataObjectError(file, lineNumber)
{
}  

InvalidRequestedRegionError
::InvalidRequestedRegionError(const InvalidRequestedRegionError &orig)
  : DataObjectError( orig )
{
}

InvalidRequestedRegionError&
InvalidRequestedRegionError
::operator=( const InvalidRequestedRegionError& orig)
{
  DataObjectError::operator= (orig);
  return *this;
}

void
InvalidRequestedRegionError
::PrintSelf(std::ostream& os, Indent indent) const
{
  DataObjectError::PrintSelf( os, indent );
}


//----------------------------------------------------------------------------
DataObject::
DataObject() : m_UpdateMTime()
{
  m_Source = 0;
  m_SourceOutputIndex = 0;
  m_ReleaseDataFlag = false;

  // We have to assume that if a user is creating the data on their own,
  // then they will fill it with valid data.
  m_DataReleased = false;

  m_PipelineMTime = 0;
}

//----------------------------------------------------------------------------
DataObject
::~DataObject()
{
}


//----------------------------------------------------------------------------
void 
DataObject
::Initialize()
{
// We don't modify ourselves because the "ReleaseData" methods depend upon
// no modification when initialized.
//
}

//----------------------------------------------------------------------------
void 
DataObject
::SetGlobalReleaseDataFlag(bool val)
{
  if (val == m_GlobalReleaseDataFlag)
    {
    return;
    }
  m_GlobalReleaseDataFlag = val;
}

//----------------------------------------------------------------------------
bool 
DataObject
::GetGlobalReleaseDataFlag()
{
  return m_GlobalReleaseDataFlag;
}

//----------------------------------------------------------------------------
void 
DataObject
::ReleaseData()
{
  this->Initialize();
  m_DataReleased = true;
}

//----------------------------------------------------------------------------
bool 
DataObject
::ShouldIReleaseData() const
{
  return ( m_GlobalReleaseDataFlag || m_ReleaseDataFlag );
}

//----------------------------------------------------------------------------
// Set the process object that generates this data object.
//
void 
DataObject
::DisconnectPipeline() 
{
  itkDebugMacro( "disconnecting from the pipeline." );

  // disconnect ourselves from the current process object
  if (m_Source)
    {
    m_Source->SetNthOutput(m_SourceOutputIndex, 0);
    }

  // set our release data flag to off by default (purposely done after
  // we have disconnected from the pipeline so the new output of the
  // source can copy our original ReleaseDataFlag)
  this->ReleaseDataFlagOff();

  // reset our PipelineMTime (there is now nothing upstream from us)
  m_PipelineMTime = 0;

  this->Modified(); 
}


bool
DataObject
::DisconnectSource(ProcessObject *arg, unsigned long idx) const
{
  if ( m_Source == arg && m_SourceOutputIndex == idx)
    {
    itkDebugMacro( "disconnecting source  " << arg
                   << ", source output index " << idx);

    m_Source = 0;
    m_SourceOutputIndex = 0;
    this->Modified();
    return true;
    }
  else
    {
    itkDebugMacro( "could not disconnect source  " << arg
                   << ", source output index " << idx);
    return false;
    }
}

bool
DataObject
::ConnectSource(ProcessObject *arg, unsigned long idx) const
{
  if ( m_Source != arg || m_SourceOutputIndex != idx)
    {
    itkDebugMacro( "connecting source  " << arg
                   << ", source output index " << idx);

    m_Source = arg;
    m_SourceOutputIndex = idx;
    this->Modified();
    return true;
    }
  else
    {
    itkDebugMacro( "could not connect source  " << arg
                   << ", source output index " << idx);

    return false;
    }
}


//----------------------------------------------------------------------------

SmartPointerForwardReference<ProcessObject>
DataObject
::GetSource() const
{
  itkDebugMacro("returning Source address " << m_Source.GetPointer() );
  return m_Source.GetPointer();
}

unsigned int
DataObject
::GetSourceOutputIndex() const
{
  itkDebugMacro("returning Source index " << m_SourceOutputIndex );
  return m_SourceOutputIndex;
}


//----------------------------------------------------------------------------
void 
DataObject
::PrintSelf(std::ostream& os, Indent indent) const
{
  Object::PrintSelf(os,indent);

  if ( m_Source )
    {
    os << indent << "Source: (" << m_Source.GetPointer() << ") \n";
    os << indent << "Source output index: " << m_SourceOutputIndex << "\n";
    }
  else
    {
    os << indent << "Source: (none)\n";
    os << indent << "Source output index:  0\n";
    }

  os << indent << "Release Data: " 
     << (m_ReleaseDataFlag ? "On\n" : "Off\n");

  os << indent << "Data Released: " 
     << (m_DataReleased ? "True\n" : "False\n");
  
  os << indent << "Global Release Data: " 
     << (m_GlobalReleaseDataFlag ? "On\n" : "Off\n");

  os << indent << "PipelineMTime: " << m_PipelineMTime << std::endl;
  os << indent << "UpdateMTime: " << m_UpdateMTime << std::endl;
}

// The following methods are used for updating the data processing pipeline.
//

//----------------------------------------------------------------------------
void 
DataObject
::Update()
{
  this->UpdateOutputInformation();
  this->PropagateRequestedRegion();
  this->UpdateOutputData();
}


void
DataObject
::UpdateOutputInformation()
{
  if (this->GetSource())
    {
    this->GetSource()->UpdateOutputInformation();
    }
}

void
DataObject
::ResetPipeline()
{
  this->PropagateResetPipeline();
}

void
DataObject
::PropagateResetPipeline()
{
  if (m_Source)
    {
    m_Source->PropagateResetPipeline();
    }
}


//----------------------------------------------------------------------------
void 
DataObject
::PropagateRequestedRegion() throw (InvalidRequestedRegionError)
{
  // If we need to update due to PipelineMTime, or the fact that our
  // data was released, then propagate the update region to the source 
  // if there is one.
  if ( m_UpdateMTime < m_PipelineMTime || m_DataReleased ||
       this->RequestedRegionIsOutsideOfTheBufferedRegion() )
    {
    if ( m_Source )
      {
      m_Source->PropagateRequestedRegion(this);
      }
    }
  
  // Check that the requested region lies within the largest possible region
  if ( ! this->VerifyRequestedRegion() )
    {
    // invalid requested region, throw an exception
    InvalidRequestedRegionError e(__FILE__, __LINE__);
    e.SetLocation(ITK_LOCATION);
    e.SetDescription("Requested region is (at least partially) outside the largest possible region.");
    e.SetDataObject(this);
    
    throw e;
    // return;
    }
}

//----------------------------------------------------------------------------
void 
DataObject
::UpdateOutputData()
{
  // If we need to update due to PipelineMTime, or the fact that our
  // data was released, then propagate the UpdateOutputData to the source
  // if there is one.
  if ( m_UpdateMTime < m_PipelineMTime || m_DataReleased ||
       this->RequestedRegionIsOutsideOfTheBufferedRegion() )
    {
    if ( m_Source )
      {
      m_Source->UpdateOutputData(this);
      } 
    } 
}

//----------------------------------------------------------------------------
void 
DataObject
::DataHasBeenGenerated()
{
  m_DataReleased = 0;
  this->Modified();
  m_UpdateMTime.Modified();
}

//----------------------------------------------------------------------------
unsigned long 
DataObject
::GetUpdateMTime() const
{
  return m_UpdateMTime.GetMTime();
}

} // end namespace itk
