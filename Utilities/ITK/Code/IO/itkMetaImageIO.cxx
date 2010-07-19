/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkMetaImageIO.cxx,v $
  Language:  C++
  Date:      $Date: 2010-06-01 11:22:47 $
  Version:   $Revision: 1.106 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifdef _MSC_VER
#pragma warning ( disable : 4786 )
#endif

#include <string>
#include <stdlib.h>
#include "itkMetaImageIO.h"
#include "itkExceptionObject.h"
#include "itkSpatialOrientation.h"
#include "itkSpatialOrientationAdapter.h"
#include "itkMetaDataObject.h"
#include "itkIOCommon.h"
#include <itksys/SystemTools.hxx>

namespace itk
{

MetaImageIO::MetaImageIO()
{
  m_FileType = Binary;
  m_SubSamplingFactor = 1;
  if(MET_SystemByteOrderMSB())
    {
    m_ByteOrder = BigEndian;
    }
  else
    {
    m_ByteOrder = LittleEndian;
    }

  this->AddSupportedWriteExtension(".mha");
  this->AddSupportedWriteExtension(".mhd");

  this->AddSupportedReadExtension(".mha");
  this->AddSupportedReadExtension(".mhd");

} 

MetaImageIO::~MetaImageIO()
{
}

void MetaImageIO::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  m_MetaImage.PrintInfo();
  os << indent << "SubSamplingFactor: " << m_SubSamplingFactor << "\n";
}


void MetaImageIO::SetDataFileName( const char* filename ) 
{ 
  m_MetaImage.ElementDataFileName( filename );
}

// This method will only test if the header looks like a
// MetaImage.  Some code is redundant with ReadImageInformation
// a StateMachine could provide a better implementation
bool MetaImageIO::CanReadFile( const char* filename ) 
{ 
  // First check the extension
  std::string fname = filename;
  if(  fname == "" )
    {
    itkDebugMacro(<<"No filename specified.");
    return false;
    }

  return m_MetaImage.CanRead(filename);
}
  

void MetaImageIO::ReadImageInformation()
{
  if(!m_MetaImage.Read(m_FileName.c_str(), false))
    {
    itkExceptionMacro("File cannot be read: "
                      << this->GetFileName() << " for reading."
                      << std::endl
                      << "Reason: "
                      << itksys::SystemTools::GetLastSystemError());
    }

  if(m_MetaImage.BinaryData())
    {
    this->SetFileType(Binary);
    }
  else
    {
    this->SetFileType(ASCII);
    }

  this->SetNumberOfComponents(m_MetaImage.ElementNumberOfChannels());

  switch(m_MetaImage.ElementType())
    {
    default:
    case MET_OTHER:
    case MET_NONE:
      this->SetPixelType( UNKNOWNPIXELTYPE );
      this->SetComponentType( UNKNOWNCOMPONENTTYPE );
      break;
    case MET_CHAR:
    case MET_ASCII_CHAR:
      this->SetPixelType( SCALAR );
      this->SetComponentType( CHAR );
      break;
    case MET_CHAR_ARRAY:
    case MET_STRING:
      this->SetPixelType( VECTOR );
      this->SetComponentType( CHAR );
      break;
    case MET_UCHAR:
      this->SetPixelType( SCALAR );
      this->SetComponentType( UCHAR );
      break;
    case MET_UCHAR_ARRAY:
      this->SetPixelType( VECTOR );
      this->SetComponentType( UCHAR );
      break;
    case MET_SHORT:
      this->SetPixelType( SCALAR );
      this->SetComponentType( SHORT );
      break;
    case MET_SHORT_ARRAY:
      this->SetPixelType( VECTOR );
      this->SetComponentType( SHORT );
      break;
    case MET_USHORT:
      this->SetPixelType( SCALAR );
      this->SetComponentType( USHORT );
      break;
    case MET_USHORT_ARRAY:
      this->SetPixelType( VECTOR );
      this->SetComponentType( USHORT );
      break;
    case MET_INT:
      this->SetPixelType( SCALAR );
      if(sizeof(int) == MET_ValueTypeSize[MET_INT])
        {
        this->SetComponentType( INT );
        }
      else if(sizeof(long) == MET_ValueTypeSize[MET_INT])
        {
        this->SetComponentType( LONG );
        }
      break;
    case MET_INT_ARRAY:
      this->SetPixelType( VECTOR );
      if(sizeof(int) == MET_ValueTypeSize[MET_INT])
        {
        this->SetComponentType( INT );
        }
      else if(sizeof(long) == MET_ValueTypeSize[MET_INT])
        {
        this->SetComponentType( LONG );
        }
      break;
    case MET_UINT:
      this->SetPixelType( SCALAR );
      if(sizeof(unsigned int) == MET_ValueTypeSize[MET_UINT])
        {
        this->SetComponentType( UINT );
        }
      else if(sizeof(unsigned long) == MET_ValueTypeSize[MET_UINT])
        {
        this->SetComponentType( ULONG );
        }
      break;
    case MET_UINT_ARRAY: 
      this->SetPixelType( VECTOR );
      if(sizeof(int) == MET_ValueTypeSize[MET_INT])
        {
        this->SetComponentType( UINT );
        }
      else if(sizeof(long) == MET_ValueTypeSize[MET_INT])
        {
        this->SetComponentType( ULONG );
        }
      break;
    case MET_LONG:
      this->SetPixelType( SCALAR );
      if(sizeof(long) == MET_ValueTypeSize[MET_LONG])
        {
        this->SetComponentType( LONG );
        }
      else if(sizeof(int) == MET_ValueTypeSize[MET_LONG])
        {
        this->SetComponentType( INT );
        }
      break;
    case MET_LONG_ARRAY:
      this->SetPixelType( VECTOR );
      if(sizeof(long) == MET_ValueTypeSize[MET_LONG])
        {
        this->SetComponentType( LONG );
        }
      else if(sizeof(int) == MET_ValueTypeSize[MET_LONG])
        {
        this->SetComponentType( INT );
        }
      break;
    case MET_ULONG:
      this->SetPixelType( SCALAR );
      if(sizeof(unsigned long) == MET_ValueTypeSize[MET_ULONG])
        {
        this->SetComponentType( ULONG );
        }
      else if(sizeof(unsigned int) == MET_ValueTypeSize[MET_ULONG])
        {
        this->SetComponentType( UINT );
        }
      break;
    case MET_ULONG_ARRAY:
      this->SetPixelType( VECTOR );
      if(sizeof(unsigned long) == MET_ValueTypeSize[MET_ULONG])
        {
        this->SetComponentType( ULONG );
        }
      else if(sizeof(unsigned int) == MET_ValueTypeSize[MET_ULONG])
        {
        this->SetComponentType( UINT );
        }
      break;
    case MET_LONG_LONG:
      this->SetPixelType( SCALAR );
      if(sizeof(long) == MET_ValueTypeSize[MET_LONG_LONG])
        {
        this->SetComponentType( LONG );
        }
      else if(sizeof(int) == MET_ValueTypeSize[MET_LONG_LONG])
        {
        this->SetComponentType( INT );
        }
      else 
        {
        this->SetComponentType( UNKNOWNCOMPONENTTYPE );
        }
      break;
    case MET_LONG_LONG_ARRAY:
      this->SetPixelType( VECTOR );
      if(sizeof(long) == MET_ValueTypeSize[MET_LONG_LONG])
        {
        this->SetComponentType( LONG );
        }
      else if(sizeof(int) == MET_ValueTypeSize[MET_LONG_LONG])
        {
        this->SetComponentType( INT );
        }
      else 
        {
        this->SetComponentType( UNKNOWNCOMPONENTTYPE );
        }
      break;
    case MET_ULONG_LONG:
      this->SetPixelType( SCALAR );
      if(sizeof(unsigned long) == MET_ValueTypeSize[MET_ULONG_LONG])
        {
        this->SetComponentType( ULONG );
        }
      else if(sizeof(unsigned int) == MET_ValueTypeSize[MET_ULONG_LONG])
        {
        this->SetComponentType( UINT );
        }
      else 
        {
        this->SetComponentType( UNKNOWNCOMPONENTTYPE );
        }
      break;
    case MET_ULONG_LONG_ARRAY:
      this->SetPixelType( VECTOR );
      if(sizeof(unsigned long) == MET_ValueTypeSize[MET_ULONG_LONG])
        {
        this->SetComponentType( ULONG );
        }
      else if(sizeof(unsigned int) == MET_ValueTypeSize[MET_ULONG_LONG])
        {
        this->SetComponentType( UINT );
        }
      else 
        {
        this->SetComponentType( UNKNOWNCOMPONENTTYPE );
        }
      break;
    case MET_FLOAT:
      this->SetPixelType( SCALAR );
      if(sizeof(float) == MET_ValueTypeSize[MET_FLOAT])
        {
        this->SetComponentType( FLOAT );
        }
      else if(sizeof(double) == MET_ValueTypeSize[MET_FLOAT])
        {
        this->SetComponentType( DOUBLE );
        }
      break;
    case MET_FLOAT_ARRAY: 
      this->SetPixelType( VECTOR );
      if(sizeof(float) == MET_ValueTypeSize[MET_FLOAT])
        {
        this->SetComponentType( FLOAT );
        }
      else if(sizeof(double) == MET_ValueTypeSize[MET_FLOAT])
        {
        this->SetComponentType( DOUBLE );
        }
      break;
    case MET_DOUBLE:
      this->SetPixelType( SCALAR );
      this->SetComponentType( DOUBLE );
      if(sizeof(double) == MET_ValueTypeSize[MET_DOUBLE])
        {
        this->SetComponentType( DOUBLE );
        }
      else if(sizeof(float) == MET_ValueTypeSize[MET_DOUBLE])
        {
        this->SetComponentType( FLOAT );
        }
      break;
    case MET_DOUBLE_ARRAY:
      this->SetPixelType( VECTOR );
      if(sizeof(double) == MET_ValueTypeSize[MET_DOUBLE])
        {
        this->SetComponentType( DOUBLE );
        }
      else if(sizeof(float) == MET_ValueTypeSize[MET_DOUBLE])
        {
        this->SetComponentType( FLOAT );
        }
      break;
    case MET_FLOAT_MATRIX:
      this->SetPixelType( VECTOR );
      if(sizeof(float) == MET_ValueTypeSize[MET_FLOAT])
        {
        this->SetComponentType( FLOAT );
        }
      else if(sizeof(double) == MET_ValueTypeSize[MET_FLOAT])
        {
        this->SetComponentType( DOUBLE );
        }
      this->SetNumberOfComponents(m_NumberOfComponents * m_NumberOfComponents);
      break;
    }

  // BUG: 8732
  // The above use to MET_*_ARRAY may not be correct, as this MetaIO
  // ElementType was not designed to indicate vectors, but something
  // else
  //
  // if the file has multiple components then we default to a vector
  // pixel type, support could be added to MetaIO format to define
  // different pixel types   
  if ( m_MetaImage.ElementNumberOfChannels() > 1 )
    {
    this->SetPixelType( VECTOR );
    }
  
  this->SetNumberOfDimensions(m_MetaImage.NDims());

  unsigned int i;
  for(i=0; i<m_NumberOfDimensions; i++)
    {
    this->SetDimensions(i,m_MetaImage.DimSize(i)/m_SubSamplingFactor);
    this->SetSpacing(i, m_MetaImage.ElementSpacing(i)*m_SubSamplingFactor);
    this->SetOrigin(i, m_MetaImage.Position(i));
    } 


#if defined(ITKIO_DEPRECATED_METADATA_ORIENTATION)
  MetaDataDictionary & thisMetaDict = this->GetMetaDataDictionary();
#endif
  /* TO - DO */
  /*
  Record type read - as in:
  EncapsulateMetaData<std::string>(thisMetaDict, ITK_OnDiskStorageTypeName,
                                        std::string(typeid(float).name()));
  */
  if(m_NumberOfDimensions == 3)
    {
    SpatialOrientation::ValidCoordinateOrientationFlags coordOrient;
    int zero = 0;
    bool coordUndefined = false;
    switch(m_MetaImage.AnatomicalOrientation(zero))
      {
      case MET_ORIENTATION_UNKNOWN:
      default:
        {
        coordUndefined = true;
        break;
        }
      case MET_ORIENTATION_RL:  
        {
        switch(m_MetaImage.AnatomicalOrientation(1))
          {
          case MET_ORIENTATION_UNKNOWN:
          default:
            {
            coordUndefined = true;
            break;
            }
          case MET_ORIENTATION_PA:
            {
            if(m_MetaImage.AnatomicalOrientation(2) == MET_ORIENTATION_IS)
              {
              coordOrient = 
                  SpatialOrientation::ITK_COORDINATE_ORIENTATION_RPI;
              }
            else
              {
              coordOrient = 
                  SpatialOrientation::ITK_COORDINATE_ORIENTATION_RPS;
              }
            break;
            }
          case MET_ORIENTATION_AP:
            {
            if(m_MetaImage.AnatomicalOrientation(2) == MET_ORIENTATION_IS)
              {
              coordOrient = 
                  SpatialOrientation::ITK_COORDINATE_ORIENTATION_RAI;
              }
            else
              {
              coordOrient = 
                  SpatialOrientation::ITK_COORDINATE_ORIENTATION_RAS;
              }
            break;
            }
          case MET_ORIENTATION_IS:
            {
            if(m_MetaImage.AnatomicalOrientation(2) == MET_ORIENTATION_PA)
              {
              coordOrient = 
                  SpatialOrientation::ITK_COORDINATE_ORIENTATION_RIP;
              }
            else
              {
              coordOrient = 
                  SpatialOrientation::ITK_COORDINATE_ORIENTATION_RIA;
              }
            break;
            }
          case MET_ORIENTATION_SI:
            {
            if(m_MetaImage.AnatomicalOrientation(2) == MET_ORIENTATION_PA)
              {
              coordOrient = 
                  SpatialOrientation::ITK_COORDINATE_ORIENTATION_RSP;
              }
            else
              {
              coordOrient = 
                  SpatialOrientation::ITK_COORDINATE_ORIENTATION_RSA;
              }
            break;
            }
          }
        break;
        }
      case MET_ORIENTATION_LR:  
        {
        switch(m_MetaImage.AnatomicalOrientation(1))
          {
          case MET_ORIENTATION_UNKNOWN:
          default:
            {
            coordUndefined = true;
            break;
            }
          case MET_ORIENTATION_PA:
            {
            if(m_MetaImage.AnatomicalOrientation(2) == MET_ORIENTATION_IS)
              {
              coordOrient = 
                  SpatialOrientation::ITK_COORDINATE_ORIENTATION_LPI;
              }
            else
              {
              coordOrient = 
                  SpatialOrientation::ITK_COORDINATE_ORIENTATION_LPS;
              }
            break;
            }
          case MET_ORIENTATION_AP:
            {
            if(m_MetaImage.AnatomicalOrientation(2) == MET_ORIENTATION_IS)
              {
              coordOrient = 
                  SpatialOrientation::ITK_COORDINATE_ORIENTATION_LAI;
              }
            else
              {
              coordOrient = 
                  SpatialOrientation::ITK_COORDINATE_ORIENTATION_LAS;
              }
            break;
            }
          case MET_ORIENTATION_IS:
            {
            if(m_MetaImage.AnatomicalOrientation(2) == MET_ORIENTATION_PA)
              {
              coordOrient = 
                  SpatialOrientation::ITK_COORDINATE_ORIENTATION_LIP;
              }
            else
              {
              coordOrient = 
                  SpatialOrientation::ITK_COORDINATE_ORIENTATION_LIA;
              }
            break;
            }
          case MET_ORIENTATION_SI:
            {
            if(m_MetaImage.AnatomicalOrientation(2) == MET_ORIENTATION_PA)
              {
              coordOrient = 
                  SpatialOrientation::ITK_COORDINATE_ORIENTATION_LSP;
              }
            else
              {
              coordOrient = 
                  SpatialOrientation::ITK_COORDINATE_ORIENTATION_LSA;
              }
            break;
            }
          }
        break;
        }
      case MET_ORIENTATION_PA:
        {
        switch(m_MetaImage.AnatomicalOrientation(1))
          {
          case MET_ORIENTATION_UNKNOWN:
          default:
            {
            coordUndefined = true;
            break;
            }
          case MET_ORIENTATION_RL:
            {
            if(m_MetaImage.AnatomicalOrientation(2) == MET_ORIENTATION_IS)
              {
              coordOrient = 
                  SpatialOrientation::ITK_COORDINATE_ORIENTATION_PRI;
              }
            else
              {
              coordOrient = 
                  SpatialOrientation::ITK_COORDINATE_ORIENTATION_PRS;
              }
            break;
            }
          case MET_ORIENTATION_LR:
            {
            if(m_MetaImage.AnatomicalOrientation(2) == MET_ORIENTATION_IS)
              {
              coordOrient = 
                  SpatialOrientation::ITK_COORDINATE_ORIENTATION_PLI;
              }
            else
              {
              coordOrient = 
                  SpatialOrientation::ITK_COORDINATE_ORIENTATION_PLS;
              }
            break;
            }
          case MET_ORIENTATION_IS:
            {
            if(m_MetaImage.AnatomicalOrientation(2) == MET_ORIENTATION_RL)
              {
              coordOrient = 
                  SpatialOrientation::ITK_COORDINATE_ORIENTATION_PIR;
              }
            else
              {
              coordOrient = 
                  SpatialOrientation::ITK_COORDINATE_ORIENTATION_PIL;
              }
            break;
            }
          case MET_ORIENTATION_SI:
            {
            if(m_MetaImage.AnatomicalOrientation(2) == MET_ORIENTATION_RL)
              {
              coordOrient = 
                  SpatialOrientation::ITK_COORDINATE_ORIENTATION_PSR;
              }
            else
              {
              coordOrient = 
                  SpatialOrientation::ITK_COORDINATE_ORIENTATION_PSL;
              }
            break;
            }
          }
        break;
        }
      case MET_ORIENTATION_AP:
        {
        switch(m_MetaImage.AnatomicalOrientation(1))
          {
          case MET_ORIENTATION_UNKNOWN:
          default:
            {
            coordUndefined = true;
            break;
            }
          case MET_ORIENTATION_RL:
            {
            if(m_MetaImage.AnatomicalOrientation(2) == MET_ORIENTATION_IS)
              {
              coordOrient = 
                  SpatialOrientation::ITK_COORDINATE_ORIENTATION_ARI;
              }
            else
              {
              coordOrient = 
                  SpatialOrientation::ITK_COORDINATE_ORIENTATION_ARS;
              }
            break;
            }
          case MET_ORIENTATION_LR:
            {
            if(m_MetaImage.AnatomicalOrientation(2) == MET_ORIENTATION_IS)
              {
              coordOrient = 
                  SpatialOrientation::ITK_COORDINATE_ORIENTATION_ALI;
              }
            else
              {
              coordOrient = 
                  SpatialOrientation::ITK_COORDINATE_ORIENTATION_ALS;
              }
            break;
            }
          case MET_ORIENTATION_IS:
            {
            if(m_MetaImage.AnatomicalOrientation(2) == MET_ORIENTATION_RL)
              {
              coordOrient = 
                  SpatialOrientation::ITK_COORDINATE_ORIENTATION_AIR;
              }
            else
              {
              coordOrient = 
                  SpatialOrientation::ITK_COORDINATE_ORIENTATION_AIL;
              }
            break;
            }
          case MET_ORIENTATION_SI:
            {
            if(m_MetaImage.AnatomicalOrientation(2) == MET_ORIENTATION_RL)
              {
              coordOrient = 
                  SpatialOrientation::ITK_COORDINATE_ORIENTATION_ASR;
              }
            else
              {
              coordOrient = 
                  SpatialOrientation::ITK_COORDINATE_ORIENTATION_ASL;
              }
            break;
            }
          }
        break;
        }
      case MET_ORIENTATION_IS:
        {
        switch(m_MetaImage.AnatomicalOrientation(1))
          {
          case MET_ORIENTATION_UNKNOWN:
          default:
            {
            coordUndefined = true;
            break;
            }
          case MET_ORIENTATION_RL:
            {
            if(m_MetaImage.AnatomicalOrientation(2) == MET_ORIENTATION_PA)
              {
              coordOrient = 
                  SpatialOrientation::ITK_COORDINATE_ORIENTATION_IRP;
              }
            else
              {
              coordOrient = 
                  SpatialOrientation::ITK_COORDINATE_ORIENTATION_IRA;
              }
            break;
            }
          case MET_ORIENTATION_LR:
            {
            if(m_MetaImage.AnatomicalOrientation(2) == MET_ORIENTATION_PA)
              {
              coordOrient =
                  SpatialOrientation::ITK_COORDINATE_ORIENTATION_ILP;
              }
            else
              {
              coordOrient =
                  SpatialOrientation::ITK_COORDINATE_ORIENTATION_ILA;
              }
            break;
            }
          case MET_ORIENTATION_PA:
            {
            if(m_MetaImage.AnatomicalOrientation(2) == MET_ORIENTATION_RL)
              {
              coordOrient = 
                  SpatialOrientation::ITK_COORDINATE_ORIENTATION_IPR;
              }
            else
              {
              coordOrient = 
                  SpatialOrientation::ITK_COORDINATE_ORIENTATION_IPL;
              }
            break;
            }
          case MET_ORIENTATION_AP:
            {
            if(m_MetaImage.AnatomicalOrientation(2) == MET_ORIENTATION_RL)
              {
              coordOrient = 
                  SpatialOrientation::ITK_COORDINATE_ORIENTATION_IAR;
              }
            else
              {
              coordOrient = 
                  SpatialOrientation::ITK_COORDINATE_ORIENTATION_IAL;
              }
            break;
            }
          }
        break;
        }
      case MET_ORIENTATION_SI:
        {
        switch(m_MetaImage.AnatomicalOrientation(1))
          {
          case MET_ORIENTATION_UNKNOWN:
          default:
            {
            coordUndefined = true;
            break;
            }
          case MET_ORIENTATION_RL:
            {
            if(m_MetaImage.AnatomicalOrientation(2) == MET_ORIENTATION_PA)
              {
              coordOrient = 
                  SpatialOrientation::ITK_COORDINATE_ORIENTATION_SRP;
              }
            else
              {
              coordOrient = 
                  SpatialOrientation::ITK_COORDINATE_ORIENTATION_SRA;
              }
            break;
            }
          case MET_ORIENTATION_LR:
            {
            if(m_MetaImage.AnatomicalOrientation(2) == MET_ORIENTATION_PA)
              {
              coordOrient = 
                  SpatialOrientation::ITK_COORDINATE_ORIENTATION_SLP;
              }
            else
              {
              coordOrient = 
                  SpatialOrientation::ITK_COORDINATE_ORIENTATION_SLA;
              }
            break;
            }
          case MET_ORIENTATION_PA:
            {
            if(m_MetaImage.AnatomicalOrientation(2) == MET_ORIENTATION_RL)
              {
              coordOrient = 
                  SpatialOrientation::ITK_COORDINATE_ORIENTATION_SPR;
              }
            else
              {
              coordOrient = 
                  SpatialOrientation::ITK_COORDINATE_ORIENTATION_SPL;
              }
            break;
            }
          case MET_ORIENTATION_AP:
            {
            if(m_MetaImage.AnatomicalOrientation(2) == MET_ORIENTATION_RL)
              {
              coordOrient = 
                  SpatialOrientation::ITK_COORDINATE_ORIENTATION_SAR;
              }
            else
              {
              coordOrient = 
                  SpatialOrientation::ITK_COORDINATE_ORIENTATION_SAL;
              }
            break;
            }
          }
        break;
        }
      }
#if defined(ITKIO_DEPRECATED_METADATA_ORIENTATION)
    if(!coordUndefined)
      {
      EncapsulateMetaData<SpatialOrientation::ValidCoordinateOrientationFlags>(
        thisMetaDict, ITK_CoordinateOrientation, coordOrient);
      }
#endif
    }

  //
  // Read direction cosines 
  //
  const double *transformMatrix = m_MetaImage.TransformMatrix();
  vnl_vector< double > directionAxis( this->GetNumberOfDimensions() );
  for( unsigned int ii=0; ii < this->GetNumberOfDimensions(); ii++)
    {
    for( unsigned int jj=0; jj < this->GetNumberOfDimensions(); jj++)
      {
      directionAxis[jj] = transformMatrix[ii*this->GetNumberOfDimensions() + jj];
      }
    this->SetDirection( ii, directionAxis );
    }

  
  //
  // Read some metadata
  //
  MetaDataDictionary & metaDict = this->GetMetaDataDictionary();

  // Look at default metaio fields
  if ( m_MetaImage.DistanceUnits() != MET_DISTANCE_UNITS_UNKNOWN )
    {
    EncapsulateMetaData<std::string>(
      metaDict, ITK_VoxelUnits, std::string(m_MetaImage.DistanceUnitsName()) );
    }

  if ( strlen( m_MetaImage.AcquisitionDate() ) > 0 )
    {
    EncapsulateMetaData<std::string>(
      metaDict, ITK_ExperimentDate, std::string(m_MetaImage.AcquisitionDate()) );
    } 
} 


void MetaImageIO::Read(void* buffer)
{ 
  const unsigned int nDims = this->GetNumberOfDimensions();
  
  // this will check to see if we are actually streaming
  // we initialize with the dimensions of the file, since if
  // largestRegion and ioRegion don't match, we'll use the streaming
  // path since the comparison will fail
  ImageIORegion largestRegion(nDims);
  for(unsigned int i=0; i<nDims; i++)
    {
    largestRegion.SetIndex(i, 0);
    largestRegion.SetSize(i, this->GetDimensions(i));
    }

  
  if(largestRegion != m_IORegion)
    {
    int* indexMin = new int[nDims];
    int* indexMax = new int[nDims];
    for(unsigned int i=0;i<nDims;i++)
      {
      if ( i < m_IORegion.GetImageDimension() )
        {
        indexMin[i] = m_IORegion.GetIndex()[i];
        indexMax[i] = indexMin[i] + m_IORegion.GetSize()[i] - 1;
        }
      else 
        {
        indexMin[i] = 0;
        // this is zero since this is a (size - 1)
        indexMax[i] = 0; 
        }
      }

    if (!m_MetaImage.ReadROI(indexMin, indexMax, 
                             m_FileName.c_str(), true, buffer,
                             m_SubSamplingFactor))
      {
      delete [] indexMin;
      delete [] indexMax;
      itkExceptionMacro("File cannot be read: "
                        << this->GetFileName() << " for reading."
                        << std::endl
                        << "Reason: "
                        << itksys::SystemTools::GetLastSystemError());
      }
    
    delete [] indexMin;
    delete [] indexMax;

    
    m_MetaImage.ElementByteOrderFix( m_IORegion.GetNumberOfPixels() );
    
    }
  else 
    {
    if(!m_MetaImage.Read(m_FileName.c_str(), true, buffer))
      {
      itkExceptionMacro("File cannot be read: "
                        << this->GetFileName() << " for reading."
                        << std::endl
                        << "Reason: "
                        << itksys::SystemTools::GetLastSystemError());
      }

    // since we are not streaming m_IORegion may not be set, so 
    m_MetaImage.ElementByteOrderFix( this->GetImageSizeInPixels() );
    }

} 

MetaImage * MetaImageIO::GetMetaImagePointer(void)
{
  return & m_MetaImage;
}


bool MetaImageIO::CanWriteFile( const char * name )
{
  std::string filename = name;
  if(  filename == "" )
    {
    return false;
    }

  std::string::size_type mhaPos = filename.rfind(".mha");
  if ((mhaPos != std::string::npos)
      && (mhaPos == filename.length() - 4))
    {
    return true;
    }

  std::string::size_type mhdPos = filename.rfind(".mhd");
  if ((mhdPos != std::string::npos)
      && (mhdPos == filename.length() - 4))
    {
    return true;
    }

  return false;
}

  
void 
MetaImageIO
::WriteImageInformation(void)
{
  MetaDataDictionary & metaDict = this->GetMetaDataDictionary();
  std::string metaDataStr;

  // Look at default metaio fields
  if ( ExposeMetaData<std::string>( metaDict, ITK_VoxelUnits, metaDataStr ) )
    {
    // Handle analyze style unit string
    if ( metaDataStr == "um. " )
      {
      m_MetaImage.DistanceUnits( MET_DISTANCE_UNITS_UM );
      }
    else if ( metaDataStr == "mm. " )
      {
      m_MetaImage.DistanceUnits( MET_DISTANCE_UNITS_MM );
      }
    else if ( metaDataStr == "cm. " )
      {
      m_MetaImage.DistanceUnits( MET_DISTANCE_UNITS_MM );
      }
    else
      {
      m_MetaImage.DistanceUnits( metaDataStr.c_str() );
      }
    }

  if ( ExposeMetaData<std::string>( metaDict, ITK_ExperimentDate, metaDataStr ) )
    {
    m_MetaImage.AcquisitionDate( metaDataStr.c_str() );
    }
}


/**
 *
 */
void 
MetaImageIO
::Write( const void* buffer) 
{
  unsigned int nDims = this->GetNumberOfDimensions();

  bool binaryData = true;
  if(this->GetFileType() == ASCII)
    {
    binaryData = false;
    }

  int nChannels = this->GetNumberOfComponents();

  MET_ValueEnumType eType = MET_OTHER;
  switch(m_ComponentType)
    {
    default:
    case UNKNOWNCOMPONENTTYPE:
      eType = MET_OTHER;
      break;
    case CHAR:
      eType = MET_CHAR;
      break;
    case UCHAR:
      eType = MET_UCHAR;
      break;
    case SHORT:
      eType = MET_SHORT;
      break;
    case USHORT:
      eType = MET_USHORT;
      break;
    case LONG:
      if(sizeof(long) == MET_ValueTypeSize[MET_LONG])
        {
        eType = MET_LONG;
        }
      else if(sizeof(long) == MET_ValueTypeSize[MET_INT])
        {
        eType = MET_INT;
        }
      else if(sizeof(long) == MET_ValueTypeSize[MET_LONG_LONG])
        {
        eType = MET_LONG_LONG;
        }
      break;
    case ULONG:
      if(sizeof(long) == MET_ValueTypeSize[MET_LONG])
        {
        eType = MET_ULONG;
        }
      else if(sizeof(long) == MET_ValueTypeSize[MET_INT])
        {
        eType = MET_UINT;
        }
      else if(sizeof(long) == MET_ValueTypeSize[MET_LONG_LONG])
        {
        eType = MET_ULONG_LONG;
        }
      break;
    case INT:
      eType = MET_INT;
      if(sizeof(int) == MET_ValueTypeSize[MET_INT])
        {
        eType = MET_INT;
        }
      else if(sizeof(int) == MET_ValueTypeSize[MET_LONG])
        {
        eType = MET_LONG;
        }
      break;
    case UINT:
      if(sizeof(int) == MET_ValueTypeSize[MET_INT])
        {
        eType = MET_UINT;
        }
      else if(sizeof(int) == MET_ValueTypeSize[MET_LONG])
        {
        eType = MET_ULONG;
        }
      break;
    case FLOAT:
      if(sizeof(float) == MET_ValueTypeSize[MET_FLOAT])
        {
        eType = MET_FLOAT;
        }
      else if(sizeof(float) == MET_ValueTypeSize[MET_DOUBLE])
        {
        eType = MET_DOUBLE;
        }
      break;
    case DOUBLE:
      if(sizeof(double) == MET_ValueTypeSize[MET_DOUBLE])
        {
        eType = MET_DOUBLE;
        }
      else if(sizeof(double) == MET_ValueTypeSize[MET_FLOAT])
        {
        eType = MET_FLOAT;
        }
      break;
    }
  
  unsigned int i;
  int * dSize = new int[nDims];
  float * eSpacing = new float[nDims];
  double * eOrigin = new double[nDims];
  for(i=0; i<nDims; i++)
    {
    dSize[i] = this->GetDimensions(i);
    eSpacing[i] = static_cast<float>(this->GetSpacing(i));
    eOrigin[i] = this->GetOrigin(i);
    }
 
  m_MetaImage.InitializeEssential(nDims, dSize, eSpacing, eType, nChannels,
                                  const_cast<void *>(buffer));
  m_MetaImage.Position(eOrigin);
  m_MetaImage.BinaryData(binaryData);

  //Write the image Information
  this->WriteImageInformation();

  if(nDims == 3)
    {
#if defined(ITKIO_DEPRECATED_METADATA_ORIENTATION)
    MetaDataDictionary & thisMetaDict = this->GetMetaDataDictionary();
#endif  
    SpatialOrientation::ValidCoordinateOrientationFlags coordOrient =
      SpatialOrientation::ITK_COORDINATE_ORIENTATION_INVALID;
#if defined(ITKIO_DEPRECATED_METADATA_ORIENTATION)
    if( !ExposeMetaData
          <SpatialOrientation::ValidCoordinateOrientationFlags>
          (thisMetaDict, ITK_CoordinateOrientation, coordOrient) )
      {
#endif
      std::vector<double> dirx, diry, dirz;
      SpatialOrientationAdapter::DirectionType dir;
      dirx = this->GetDirection(0);
      diry = this->GetDirection(1);
      dirz = this->GetDirection(2);
      for(unsigned ii = 0; ii < 3; ii++)
        {
        dir[ii][0] = dirx[ii];
        dir[ii][1] = diry[ii];
        dir[ii][2] = dirz[ii];
        }
      coordOrient = SpatialOrientationAdapter().FromDirectionCosines(dir);
#if defined(ITKIO_DEPRECATED_METADATA_ORIENTATION)
      }
#endif

    switch(coordOrient)
      {
      default:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_RPI:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_RPS:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_RAI:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_RAS:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_RIA:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_RIP:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_RSA:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_RSP:
        {
        m_MetaImage.AnatomicalOrientation(0, MET_ORIENTATION_RL);
        break;
        }
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_LPI:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_LPS:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_LAI:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_LAS:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_LIA:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_LIP:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_LSA:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_LSP:
        {
        m_MetaImage.AnatomicalOrientation(0, MET_ORIENTATION_LR);
        break;
        }
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_ALI:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_ALS:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_ARI:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_ARS:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_AIL:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_AIR:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_ASL:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_ASR:
        {
        m_MetaImage.AnatomicalOrientation(0, MET_ORIENTATION_AP);
        break;
        }
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_PLI:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_PLS:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_PRI:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_PRS:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_PIL:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_PIR:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_PSL:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_PSR:
        {
        m_MetaImage.AnatomicalOrientation(0, MET_ORIENTATION_PA);
        break;
        }
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_IPL:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_IPR:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_IAL:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_IAR:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_ILA:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_ILP:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_IRA:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_IRP:
        {
        m_MetaImage.AnatomicalOrientation(0, MET_ORIENTATION_IS);
        break;
        }
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_SPL:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_SPR:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_SAL:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_SAR:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_SLA:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_SLP:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_SRA:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_SRP:
        {
        m_MetaImage.AnatomicalOrientation(0, MET_ORIENTATION_SI);
        break;
        }
      }
    switch(coordOrient)
      {
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_PRI:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_PRS:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_ARI:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_ARS:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_IRA:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_IRP:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_SRA:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_SRP:
        {
        m_MetaImage.AnatomicalOrientation(1, MET_ORIENTATION_RL);
        break;
        }
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_PLI:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_PLS:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_ALI:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_ALS:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_ILA:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_ILP:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_SLA:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_SLP:
        {
        m_MetaImage.AnatomicalOrientation(1, MET_ORIENTATION_LR);
        break;
        }
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_LAI:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_LAS:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_RAI:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_RAS:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_IAL:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_IAR:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_SAL:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_SAR:
        {
        m_MetaImage.AnatomicalOrientation(1, MET_ORIENTATION_AP);
        break;
        }
      default:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_LPI:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_LPS:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_RPI:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_RPS:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_IPL:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_IPR:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_SPL:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_SPR:
        {
        m_MetaImage.AnatomicalOrientation(1, MET_ORIENTATION_PA);
        break;
        }
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_PIL:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_PIR:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_AIL:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_AIR:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_LIA:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_LIP:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_RIA:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_RIP:
        {
        m_MetaImage.AnatomicalOrientation(1, MET_ORIENTATION_IS);
        break;
        }
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_PSL:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_PSR:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_ASL:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_ASR:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_LSA:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_LSP:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_RSA:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_RSP:
        {
        m_MetaImage.AnatomicalOrientation(1, MET_ORIENTATION_SI);
        break;
        }
      }
    switch(coordOrient)
      {
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_PIR:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_PSR:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_AIR:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_ASR:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_IAR:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_IPR:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_SAR:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_SPR:
        {
        m_MetaImage.AnatomicalOrientation(2, MET_ORIENTATION_RL);
        break;
        }
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_PIL:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_PSL:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_AIL:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_ASL:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_IAL:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_IPL:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_SAL:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_SPL:
        {
        m_MetaImage.AnatomicalOrientation(2, MET_ORIENTATION_LR);
        break;
        }
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_LIA:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_LSA:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_RIA:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_RSA:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_ILA:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_IRA:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_SLA:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_SRA:
        {
        m_MetaImage.AnatomicalOrientation(2, MET_ORIENTATION_AP);
        break;
        }
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_LIP:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_LSP:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_RIP:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_RSP:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_ILP:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_IRP:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_SLP:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_SRP:
        {
        m_MetaImage.AnatomicalOrientation(2, MET_ORIENTATION_PA);
        break;
        }
      default:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_PLI:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_PRI:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_ALI:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_ARI:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_LAI:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_LPI:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_RAI:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_RPI:
        {
        m_MetaImage.AnatomicalOrientation(2, MET_ORIENTATION_IS);
        break;
        }
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_PLS:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_PRS:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_ALS:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_ARS:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_LAS:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_LPS:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_RAS:
      case SpatialOrientation::ITK_COORDINATE_ORIENTATION_RPS:
        {
        m_MetaImage.AnatomicalOrientation(2, MET_ORIENTATION_SI);
        break;
        }
      }

    }

  // Propagage direction cosine information .
  double *transformMatrix = 
  static_cast< double *>(malloc(this->GetNumberOfDimensions() * 
                  this->GetNumberOfDimensions() * sizeof(double)));
  for( unsigned int ii=0; ii < this->GetNumberOfDimensions(); ii++ )
    {
    for( unsigned int jj=0; jj < this->GetNumberOfDimensions(); jj++ )
      {
      transformMatrix[ ii*this->GetNumberOfDimensions() + jj ] =
                                         this->GetDirection( ii )[ jj ];
      }
    }
  m_MetaImage.TransformMatrix( transformMatrix );
  free( transformMatrix );
  
  m_MetaImage.CompressedData( m_UseCompression );

  // this is a check to see if we are actually streaming
  // we initialize with m_IORegion to match dimensions
  ImageIORegion largestRegion(m_IORegion);
  for(i=0; i<nDims; i++)
    {
    largestRegion.SetIndex(i, 0);
    largestRegion.SetSize(i, this->GetDimensions(i));
    }
  
  if( m_UseCompression && (largestRegion != m_IORegion) )
    {
    std::cout << "Compression in use: cannot stream the file writing" << std::endl;
    }
  else if(  largestRegion != m_IORegion )
    {
    int* indexMin = new int[nDims];
    int* indexMax = new int[nDims];
    for( unsigned int k=0; k<nDims; k++ )
      {
      // the dimensions of m_IORegion should match out requested
      // dimensions, but ImageIORegion will throw an
      // exception if out of bounds 
      indexMin[k] = m_IORegion.GetIndex()[k];
      indexMax[k] = m_IORegion.GetIndex()[k] + m_IORegion.GetSize()[k] - 1;
      }
      
    if (!m_MetaImage.WriteROI( indexMin, indexMax, m_FileName.c_str() ))
      {
      delete [] indexMin;
      delete [] indexMax;
      itkExceptionMacro("File ROI cannot be written: "
                      << this->GetFileName()
                      << std::endl
                      << "Reason: "
                      << itksys::SystemTools::GetLastSystemError());
      }

    delete [] indexMin;
    delete [] indexMax;
    }
  else
    {
    if ( !m_MetaImage.Write(  m_FileName.c_str() ) ) 
      {
      itkExceptionMacro("File cannot be written: "
                      << this->GetFileName()
                      << std::endl
                      << "Reason: "
                      << itksys::SystemTools::GetLastSystemError());
      }
    }

  // we leak when exceptions are thrown :(
  delete []dSize;
  delete []eSpacing;
  delete []eOrigin;
}

/** Given a requested region, determine what could be the region that we can
 * read from the file. This is called the streamable region, which will be
 * smaller than the LargestPossibleRegion and greater or equal to the 
 * RequestedRegion */
ImageIORegion 
MetaImageIO
::GenerateStreamableReadRegionFromRequestedRegion( const ImageIORegion & requestedRegion  ) const
{
  //
  // The default implementations determines that the streamable region is
  // equal to the largest possible region of the image.
  //
  ImageIORegion streamableRegion(this->m_NumberOfDimensions);
  if(!m_UseStreamedReading)
    { 
    for( unsigned int i=0; i < this->m_NumberOfDimensions; i++ )
      {
      streamableRegion.SetSize( i, this->m_Dimensions[i] );
      streamableRegion.SetIndex( i, 0 );
      }
    }
  else
    {
    streamableRegion = requestedRegion;
    }

  return streamableRegion;
}
 
unsigned int 
MetaImageIO::GetActualNumberOfSplitsForWriting(unsigned int numberOfRequestedSplits,
                                               const ImageIORegion &pasteRegion,
                                               const ImageIORegion &largestPossibleRegion) 
{ 
  if (this->GetUseCompression()) 
    {
    // we can not stream or paste with compression
    if (pasteRegion != largestPossibleRegion) 
      {
      itkExceptionMacro("Pasting and compression is not supported! Can't write:" << this->GetFileName());
      }
    else if (numberOfRequestedSplits != 1)  
      {
      itkDebugMacro("Requested streaming and compression");
      itkDebugMacro("Meta IO is not streaming now!");
      }
    return 1;
    }

  if (!itksys::SystemTools::FileExists( m_FileName.c_str() )) 
    {
    // file doesn't exits so we don't have potential problems
    }
  else if (pasteRegion != largestPossibleRegion) 
    {
    // we are going to be pasting (may be streaming too)

    // need to check to see if the file is compatible
    std::string errorMessage;
    Pointer headerImageIOReader = Self::New();

    try 
      {
      headerImageIOReader->SetFileName(m_FileName.c_str());
      headerImageIOReader->ReadImageInformation();
      }
    catch (...)
      {
      errorMessage = "Unable to read information from file: " + m_FileName;
      }

    
    
    // we now need to check that the following match:
    // 1)file is not compressed
    // 2)pixel type
    // 3)dimensions
    // 4)size/origin/spacing
    // 5)direction cosines
    // 

    if (errorMessage.size()) 
      {
      // 0) Can't read file
      }
    // 1)file is not compressed
    else if (headerImageIOReader->m_MetaImage.CompressedData()) 
      {
      errorMessage = "File is compressed: " + m_FileName;
      }
    // 2)pixel type
    // this->GetPixelType() is not verified becasue the metaio file format
    // stores all multi-component types as arrays, so it does not
    // distinguish between pixel types. Also as long as the compoent
    // and number of compoents match we should be able to paste, that
    // is the numbers should be the same it is just the interpretation
    // that is not matching
    else if ( headerImageIOReader->GetNumberOfComponents() != this->GetNumberOfComponents() ||
              headerImageIOReader->GetComponentType() != this->GetComponentType() ) 
      {
      errorMessage = "Component type does not match in file: " + m_FileName;
      }
    // 3)dimensions/size
    else if (headerImageIOReader->GetNumberOfDimensions() != this->GetNumberOfDimensions()) 
      {
      errorMessage = "Dimensions does not match in file: " + m_FileName;
      }
    else 
      {
      for (unsigned int i = 0; i < this->GetNumberOfDimensions(); ++i) 
        {
        // 4)size/origin/spacing
        if (headerImageIOReader->GetDimensions(i) != this->GetDimensions(i) ||
            headerImageIOReader->GetSpacing(i) != this->GetSpacing(i) ||
            headerImageIOReader->GetOrigin(i) != this->GetOrigin(i))
          {
          errorMessage = "Size, spacing or origin does not match in file: " + m_FileName;
          break;
          }
        // 5)direction cosines
        if (headerImageIOReader->GetDirection(i) != this->GetDirection(i)) 
          {
          errorMessage = "Direction cosines does not match in file: " + m_FileName;
          break;
          }
        }
      }
    
    if (errorMessage.size()) 
      {
      itkExceptionMacro("Unable to paste because pasting file exists and is different. " << errorMessage);
      }
    else if ( headerImageIOReader->GetPixelType() != this->GetPixelType() ) 
      {
      // since there is currently poor support for pixel types in
      // MetaIO we will just warn when it does not match
      itkWarningMacro("Pixel types does not match file, but component type and number of components do.");
      }
    }
  else if (numberOfRequestedSplits != 1)  
    {
    // we are going be streaming
    
    // need to remove the file incase the file doesn't match our
    // current header/meta data information
    if (!itksys::SystemTools::RemoveFile(m_FileName.c_str()))
      itkExceptionMacro("Unable to remove file for streaming: " << m_FileName);
    }

  return GetActualNumberOfSplitsForWritingCanStreamWrite(numberOfRequestedSplits, pasteRegion);
}


ImageIORegion 
MetaImageIO::GetSplitRegionForWriting(unsigned int ithPiece, 
                                      unsigned int numberOfActualSplits,
                                      const ImageIORegion &pasteRegion,
                                      const ImageIORegion &itkNotUsed(largestPossibleRegion) )
{

  return GetSplitRegionForWritingCanStreamWrite(ithPiece, numberOfActualSplits, pasteRegion);
}

} // end namespace itk
