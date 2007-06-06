/*=========================================================================

  Program:   MetaIO
  Module:    $RCSfile: metaImage.cxx,v $
  Language:  C++
  Date:      $Date: 2006/12/18 18:52:55 $
  Version:   $Revision: 1.64 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "metaImage.h"

#include <stdio.h>
#include <ctype.h>
#include <string>
#include <string.h> // for memcpy
#include <math.h>

#if (METAIO_USE_NAMESPACE)
namespace METAIO_NAMESPACE {
#endif


//
// MetaImage Constructors
//
MetaImage::
MetaImage()
:MetaObject()
  {
  if(META_DEBUG) 
    {
    METAIO_STREAM::cout << "MetaImage()" << METAIO_STREAM::endl;
    }

  Clear();
  }

//
MetaImage::
MetaImage(const char *_headerName)
:MetaObject()
  {
  if(META_DEBUG) 
    {
    METAIO_STREAM::cout << "MetaImage()" << METAIO_STREAM::endl;
    }

  Clear();

  Read(_headerName);
  }

//
MetaImage::
MetaImage(MetaImage *_im)
:MetaObject()
  {
  if(META_DEBUG)
    {
    METAIO_STREAM::cout << "MetaImage()" << METAIO_STREAM::endl;
    }

  Clear();

  InitializeEssential(_im->NDims(), 
                      _im->DimSize(),
                      _im->ElementSpacing(),
                      _im->ElementType(),
                      _im->ElementNumberOfChannels(),
                      _im->ElementData(),
                      false);
  CopyInfo(_im);
  }

//
MetaImage::
MetaImage(int _nDims, 
          const int * _dimSize,
          const float * _elementSpacing,
          MET_ValueEnumType _elementType,
          int _elementNumberOfChannels,
          void *_elementData)
:MetaObject()
  {
  if(META_DEBUG)
    {
    METAIO_STREAM::cout << "MetaImage()" << METAIO_STREAM::endl;
    }

  Clear();

  if(_elementData == NULL)
    {
    InitializeEssential(_nDims, 
                        _dimSize, 
                        _elementSpacing,
                        _elementType,
                        _elementNumberOfChannels,
                        NULL, true);
    }
  else
    {
    InitializeEssential(_nDims, 
                        _dimSize, 
                        _elementSpacing,
                        _elementType,
                        _elementNumberOfChannels,
                        _elementData, false);
    }

  }

//
MetaImage::
MetaImage(int _x, int _y, 
          float _elementSpacingX, float _elementSpacingY,
          MET_ValueEnumType _elementType,
          int _elementNumberOfChannels, void *_elementData)
:MetaObject()
  {
  if(META_DEBUG)
    {
    METAIO_STREAM::cout << "MetaImage()" << METAIO_STREAM::endl;
    }

  Clear();

  int ds[2];
  ds[0] = _x;
  ds[1] = _y;

  float es[2];
  es[0] = _elementSpacingX;
  es[1] = _elementSpacingY;

  if(_elementData == NULL)
    {
    InitializeEssential(2,
                        ds,
                        es,
                        _elementType, 
                        _elementNumberOfChannels,
                        NULL,
                        true);
    }
  else
    {
    InitializeEssential(2,
                        ds,
                        es,
                        _elementType, 
                        _elementNumberOfChannels,
                        _elementData,
                        false);
    }
  }

//
MetaImage::
MetaImage(int _x, int _y, int _z, 
          float _elementSpacingX, 
          float _elementSpacingY,
          float _elementSpacingZ, 
          MET_ValueEnumType _elementType,
          int _elementNumberOfChannels, 
          void *_elementData)
:MetaObject()
  {
  if(META_DEBUG)
    {
    METAIO_STREAM::cout << "MetaImage()" << METAIO_STREAM::endl;
    }

  Clear();

  int ds[3];
  ds[0] = _x;
  ds[1] = _y;
  ds[2] = _z;

  float es[3];
  es[0] = _elementSpacingX;
  es[1] = _elementSpacingY;
  es[2] = _elementSpacingZ;

  if(_elementData == NULL)
    {
    InitializeEssential(3,
                        ds,
                        es,
                        _elementType, 
                        _elementNumberOfChannels,
                        NULL,
                        true);
    }
  else
    {
    InitializeEssential(3,
                        ds,
                        es,
                        _elementType, 
                        _elementNumberOfChannels,
                        _elementData,
                        false);
    }
  }

//
MetaImage::
~MetaImage()
  {
  M_Destroy();
  }

//
void MetaImage::
PrintInfo() const
  {
  int i;

  MetaObject::PrintInfo();

  char s[255];
  MET_ImageModalityToString(m_Modality, s);
  METAIO_STREAM::cout << "Modality = " << s << METAIO_STREAM::endl;

  METAIO_STREAM::cout << "DimSize = ";
  for(i=0; i<m_NDims; i++)
    {
    METAIO_STREAM::cout << m_DimSize[i] << " ";
    }
  METAIO_STREAM::cout << METAIO_STREAM::endl;
  METAIO_STREAM::cout << "SubQuantity = ";
  for(i=0; i<m_NDims; i++)
    {
    METAIO_STREAM::cout << m_SubQuantity[i] << " ";
    }
  METAIO_STREAM::cout << METAIO_STREAM::endl;

  METAIO_STREAM::cout << "Quantity = " << m_Quantity << METAIO_STREAM::endl;


  METAIO_STREAM::cout << "HeaderSize = " << m_HeaderSize << METAIO_STREAM::endl;

  METAIO_STREAM::cout << "SequenceID = ";
  for(i=0; i<m_NDims; i++)
    {
    METAIO_STREAM::cout << m_SequenceID[i] << " ";
    }
  METAIO_STREAM::cout << METAIO_STREAM::endl;

  METAIO_STREAM::cout << "ElementSizeValid = " << (int)m_ElementSizeValid
            << METAIO_STREAM::endl;
  METAIO_STREAM::cout << "ElementSize = ";
  for(i=0; i<m_NDims; i++)
    {
    METAIO_STREAM::cout << m_ElementSize[i] << " ";
    }
  METAIO_STREAM::cout << METAIO_STREAM::endl;

  char str[255];
  MET_TypeToString(m_ElementType, str);
  METAIO_STREAM::cout << "ElementType = " << str << METAIO_STREAM::endl;

  METAIO_STREAM::cout << "ElementNumberOfChannels = "
                      << m_ElementNumberOfChannels << METAIO_STREAM::endl;

  if(m_ElementMinMaxValid)
    {
    METAIO_STREAM::cout << "Min and Max are valid" << METAIO_STREAM::endl;
    METAIO_STREAM::cout << "   Min = " << m_ElementMin << METAIO_STREAM::endl;
    METAIO_STREAM::cout << "   Max = " << m_ElementMax << METAIO_STREAM::endl;
    }
  else
    {
    METAIO_STREAM::cout << "Min and Max are not valid" << METAIO_STREAM::endl;
    }

  METAIO_STREAM::cout << "ElementToIntensityFunctionSlope = " 
                      << m_ElementToIntensityFunctionSlope 
                      << METAIO_STREAM::endl;
  METAIO_STREAM::cout << "ElementToIntensityFunctionOffset = " 
                      << m_ElementToIntensityFunctionOffset 
                      << METAIO_STREAM::endl;
  

  METAIO_STREAM::cout << "AutoFreeElementData = " 
                      << ((m_AutoFreeElementData)?"True":"False") 
                      << METAIO_STREAM::endl;

  METAIO_STREAM::cout << "ElementData = " 
                      << ((m_ElementData==NULL)?"NULL":"Valid")
                      << METAIO_STREAM::endl;

  METAIO_STREAM::cout << "ElementDataFileName = " 
                      << m_ElementDataFileName << METAIO_STREAM::endl;

  }

void MetaImage::
CopyInfo(const MetaObject * _object)
  {
  MetaObject::CopyInfo(_object);

  if(_object)
    {
    const MetaImage * im;
    try
      {
      im = dynamic_cast<const MetaImage *>(_object);
      }
    catch( ... )
      {
      return;
      }

    if( im )
      {
      Modality(im->Modality());
    
      HeaderSize(im->HeaderSize());
    
      SequenceID(im->SequenceID());
    
      ElementSizeValid(im->ElementSizeValid());
      if(im->ElementSizeValid())
        {
        ElementSize(im->ElementSize());
        }
    
      ElementMinMaxValid(im->ElementMinMaxValid());
      if(im->ElementMinMaxValid())
        {
        ElementMin(im->ElementMin());
        ElementMax(im->ElementMax());
        }
    
      ElementToIntensityFunctionSlope(im->ElementToIntensityFunctionSlope());
      ElementToIntensityFunctionOffset(im->ElementToIntensityFunctionOffset());
      }
    }
  }

    
void MetaImage::
Clear(void)
  {
  if(META_DEBUG)
    {
    METAIO_STREAM::cout << "MetaImage: Clear" << METAIO_STREAM::endl;
    }

  m_Modality = MET_MOD_UNKNOWN;

  m_DimSize[0] = 0;
  m_SubQuantity[0] = 0;
  m_Quantity = 0;

  m_HeaderSize = 0;

  memset(m_SequenceID, 0, 4*sizeof(float));

  m_ElementSizeValid = false;
  memset(m_ElementSize, 0, 10*sizeof(float));

  m_ElementType = MET_NONE;

  m_ElementNumberOfChannels = 1;

  m_ElementMinMaxValid = false;
  m_ElementMin = 0;
  m_ElementMax = 0;

  m_ElementToIntensityFunctionSlope = 1;
  m_ElementToIntensityFunctionOffset = 0;

  m_AutoFreeElementData = true;

  m_ElementData = NULL;

  strcpy(m_ElementDataFileName, "");

  MetaObject::Clear();

  // Change the default for this object
  m_BinaryData = true;
  }
        
bool MetaImage::
InitializeEssential(int _nDims,
                    const int * _dimSize,
                    const float * _elementSpacing, 
                    MET_ValueEnumType _elementType,
                    int _elementNumberOfChannels,
                    void * _elementData,
                    bool _allocElementMemory)
  {
  if(META_DEBUG)
    {
    METAIO_STREAM::cout << "MetaImage: Initialize" << METAIO_STREAM::endl;
    }

  MetaObject::InitializeEssential(_nDims);

  int i;
  m_SubQuantity[0] = 1;
  m_Quantity = 1;
  m_ElementSizeValid = false;
  for(i=0; i<m_NDims; i++)
    {
    m_DimSize[i] = _dimSize[i];
    m_Quantity *= _dimSize[i];
    if(i>0)
      {
      m_SubQuantity[i] = m_SubQuantity[i-1]*m_DimSize[i-1];
      }
    m_ElementSpacing[i] = _elementSpacing[i];
    if(m_ElementSize[i] == 0)
      {
      m_ElementSize[i] = m_ElementSpacing[i];
      }
    else
      {
      m_ElementSizeValid = true;
      }
    }

  m_ElementType = _elementType;

  m_ElementNumberOfChannels = _elementNumberOfChannels;

  if(_elementData != NULL)
    {
    m_AutoFreeElementData = false;
    m_ElementData = (void *)_elementData;
    }
  else if(_allocElementMemory)
    {
    m_AutoFreeElementData = true;
    MET_SizeOfType(m_ElementType, &i);
    m_ElementData = new char[m_Quantity*m_ElementNumberOfChannels*i]; 
    if(m_ElementData == NULL)
      {
      METAIO_STREAM::cerr << "MetaImage:: M_Allocate:: Insufficient memory" 
                          << METAIO_STREAM::endl;
      return false;
      }
    }
  else
    {
    m_AutoFreeElementData = true;
    m_ElementData = NULL;
    }

  return true;
  }

        
//
//
//
int MetaImage::
HeaderSize(void) const
  {
  return m_HeaderSize;
  }

void MetaImage::
HeaderSize(int _headerSize)
  {
  m_HeaderSize = _headerSize;
  }

//
//
//
MET_ImageModalityEnumType MetaImage::
Modality(void) const
  {
  return m_Modality;
  }

void MetaImage::
Modality(MET_ImageModalityEnumType _modality)
  {
  m_Modality = _modality;
  }

//
//
//
const int * MetaImage::
DimSize(void) const
  {
  return m_DimSize;
  }

int MetaImage:: 
DimSize(int _i) const
  {
  return m_DimSize[_i];
  }

//
//
//
int MetaImage::
Quantity(void) const
  {
  return m_Quantity;
  }

//
//
//
const int * MetaImage::
SubQuantity(void) const
  {
  return m_SubQuantity;
  }

int MetaImage::
SubQuantity(int _i) const
  {
  return m_SubQuantity[_i];
  }

//
//
//
const float * MetaImage::
SequenceID(void) const
  {
  return m_SequenceID;
  }

float MetaImage::
SequenceID(int _i) const
  {
  return m_SequenceID[_i];
  }

void MetaImage::
SequenceID(const float *_sequenceID)
  {
  memcpy(m_SequenceID, _sequenceID, m_NDims*sizeof(float));
  }

void MetaImage::
SequenceID(int _i, float _value)
  {
  m_SequenceID[_i] = _value;
  }

//
//
//
bool MetaImage::
ElementSizeValid(void) const
  {
  return m_ElementSizeValid;
  }

void MetaImage::
ElementSizeValid(bool _elementSizeValid)
  {
  m_ElementSizeValid = _elementSizeValid;
  }

const float * MetaImage::
ElementSize(void) const
  {
  return m_ElementSize;
  }

float MetaImage::
ElementSize(int _i) const
  {
  return m_ElementSize[_i];
  }

void MetaImage::
ElementSize(const float *_elementSize)
  {
  memcpy(m_ElementSize, _elementSize, m_NDims*sizeof(float));
  m_ElementSizeValid = true;
  }

void MetaImage::
ElementSize(int _i, float _value)
  {
  m_ElementSize[_i] = _value;
  m_ElementSizeValid = true;
  }

//
//
//
MET_ValueEnumType MetaImage::
ElementType(void) const
  {
  return m_ElementType;
  }

void MetaImage::
ElementType(MET_ValueEnumType _elementType)
  {
  m_ElementType = _elementType;
  }

//
//
//
int MetaImage::
ElementNumberOfChannels(void) const
  {
  return m_ElementNumberOfChannels;
  }

void MetaImage::
ElementNumberOfChannels(int _elementNumberOfChannels)
  {
  m_ElementNumberOfChannels = _elementNumberOfChannels;
  }

//
//
//
void MetaImage::
ElementByteOrderSwap(void)
  {
  if(META_DEBUG)
    {
    METAIO_STREAM::cout << "MetaImage: ElementByteOrderSwap" 
                        << METAIO_STREAM::endl;
    }

  int eSize;
  MET_SizeOfType(m_ElementType, &eSize);    
  switch(eSize)
    {
    default:
    case 0:
    case 1: 
      {
      break;
      }
    case 2:
      {
      int i;
      for(i=0; i<m_Quantity*m_ElementNumberOfChannels; i++)
        {
        ((MET_USHORT_TYPE *)m_ElementData)[i] = 
              MET_ByteOrderSwapShort(((MET_USHORT_TYPE *)m_ElementData)[i]);
        }
      break;
      }
    case 4:
      {
      int i;
      for(i=0; i<m_Quantity*m_ElementNumberOfChannels; i++)
        {
        ((MET_UINT_TYPE *)m_ElementData)[i] =
              MET_ByteOrderSwapLong(((MET_UINT_TYPE *)m_ElementData)[i]);
        }
      break;
      }
    case 8:
      {
      int i;
      char* data = (char*)m_ElementData;
      for(i=0; i<m_Quantity*m_ElementNumberOfChannels; i++)
        {
        MET_ByteOrderSwap8(data);
        data += 8;
        }
      break;
      }
    }
  m_BinaryDataByteOrderMSB = !m_BinaryDataByteOrderMSB;
  }

bool MetaImage::
ElementByteOrderFix(void)
  {
  if(m_BinaryDataByteOrderMSB != MET_SystemByteOrderMSB())
    {
    ElementByteOrderSwap();
    return true;
    }
  return true;
  }

//
//
//
bool MetaImage::
ElementMinMaxValid(void) const
  {
  return m_ElementMinMaxValid;
  }

void MetaImage::
ElementMinMaxValid(bool _elementMinMaxValid)
  {
  m_ElementMinMaxValid = _elementMinMaxValid;
  }

void MetaImage::
ElementMinMaxRecalc(void)
  {
  int i;
  double tf;
  
  if(m_ElementData == NULL)
    return;

  ElementByteOrderFix();

  MET_ValueToDouble(m_ElementType, m_ElementData, 0, &tf);
  m_ElementMin = tf;
  m_ElementMax = tf;
  for(i=1; i<m_Quantity*m_ElementNumberOfChannels; i++)
    {
    MET_ValueToDouble(m_ElementType, m_ElementData, i, &tf);
    if(tf<m_ElementMin)
      {
      m_ElementMin = tf;
      }
    else if(tf>m_ElementMax)
      {
      m_ElementMax = tf;
      }
    }

  m_ElementMinMaxValid = true;
  }

double MetaImage::
ElementMin(void) const
  {
  return m_ElementMin;
  }

void MetaImage::
ElementMin(double _elementMin)
  {
  m_ElementMin = _elementMin;
  }

double MetaImage::
ElementMax(void) const
  {
  return m_ElementMax;
  }

void MetaImage::
ElementMax(double _elementMax)
  {
  m_ElementMax = _elementMax;
  }

//
//
//
double MetaImage::
ElementToIntensityFunctionSlope(void) const
  {
  return m_ElementToIntensityFunctionSlope;
  }

void MetaImage::
ElementToIntensityFunctionSlope(double _elementToIntensityFunctionSlope)
  {
  m_ElementToIntensityFunctionSlope = _elementToIntensityFunctionSlope;
  }

double MetaImage::
ElementToIntensityFunctionOffset(void) const
  {
  return m_ElementToIntensityFunctionOffset;
  }

void MetaImage::
ElementToIntensityFunctionOffset(double _elementOffset)
  {
  m_ElementToIntensityFunctionOffset = _elementOffset;
  }

//
//
//
bool MetaImage::
AutoFreeElementData(void) const
  {
  return m_AutoFreeElementData;
  }

void MetaImage::
AutoFreeElementData(bool _autoFreeElementData)
  {
  m_AutoFreeElementData = _autoFreeElementData;
  }

//
//
//
const char * MetaImage::
ElementDataFileName(void) const
  {
  return m_ElementDataFileName;
  }

void MetaImage::
ElementDataFileName(const char * _elementDataFileName)
  {
  strcpy(m_ElementDataFileName, _elementDataFileName);
  }

//
//
//
void * MetaImage::
ElementData(void)
  {
  return m_ElementData;
  }

double MetaImage::
ElementData(int _i) const
  {
  double tf = 0;
  MET_ValueToDouble(m_ElementType, m_ElementData, _i, &tf);

  return tf;
  }

bool MetaImage::
ElementData(int _i, double _v)
  {
  if(_i<m_Quantity)
    {
    MET_DoubleToValue(_v, m_ElementType, m_ElementData, _i);
    return true;
    }
  return false;
  }

void MetaImage::
ElementData(void * _elementData, bool _autoFreeElementData)
  {
  if(m_AutoFreeElementData)
    {
    delete [] (char *)m_ElementData;
    }
  m_ElementData = _elementData;
  m_AutoFreeElementData = _autoFreeElementData;
  }

//
//
//
bool MetaImage::
ConvertElementDataTo(MET_ValueEnumType _elementType,
                     double _toMin, double _toMax)
  {
  int eSize;
  MET_SizeOfType(_elementType, &eSize);
  void * newElementData = new char[m_Quantity*m_ElementNumberOfChannels*eSize];

  ElementByteOrderFix();
  if(!ElementMinMaxValid())
    {
    ElementMinMaxRecalc();
    }

  int i;
  for(i=0; i<m_Quantity*m_ElementNumberOfChannels; i++)
    {
    MET_ValueToValue(m_ElementType, m_ElementData, i, _elementType, 
                     newElementData, m_ElementMin, m_ElementMax,
                     _toMin, _toMax);
    }

  if(m_AutoFreeElementData)
    {
    delete [] (char *)m_ElementData;
    }
  m_ElementData = newElementData;
  m_ElementType = _elementType;
  m_ElementMinMaxValid = true;
  m_ElementMin = _toMin;
  m_ElementMax = _toMax;
  m_AutoFreeElementData = true;

  return true;
  }

bool MetaImage::
ConvertElementDataToIntensityData(MET_ValueEnumType _elementType)
  {
  ElementByteOrderFix();
  if(!ElementMinMaxValid())
    {
    ElementMinMaxRecalc();
    }

  double toMin = m_ElementMin + m_ElementToIntensityFunctionOffset;
  double toMax = (m_ElementMax-m_ElementMin) 
                   * m_ElementToIntensityFunctionSlope 
                   + m_ElementMin;

  return ConvertElementDataTo(_elementType, toMin, toMax);
  }

bool MetaImage::
ConvertIntensityDataToElementData(MET_ValueEnumType _elementType)
  {
  ElementByteOrderFix();
  if(!ElementMinMaxValid())
    {
    ElementMinMaxRecalc();
    }

  double toMin = m_ElementMin - m_ElementToIntensityFunctionOffset;
  double toMax = (m_ElementMax - m_ElementMin) 
                   / m_ElementToIntensityFunctionSlope 
                   + toMin;

  return ConvertElementDataTo(_elementType, toMin, toMax);
  }

//
//
//
bool MetaImage::
CanRead(const char *_headerName) const
  {
  // First check the extension
  METAIO_STL::string fname = _headerName;
  if(  fname == "" )
    {
    return false;
    }

  bool extensionFound = false;

  METAIO_STL::string::size_type stringPos = fname.rfind(".mhd");
  if ((stringPos != METAIO_STL::string::npos)
      && (stringPos == fname.length() - 4))
    {
    extensionFound = true;
    }

  stringPos = fname.rfind(".mha");
  if ((stringPos != METAIO_STL::string::npos)
      && (stringPos == fname.length() - 4))
    {
    extensionFound = true;
    }
  
  if( !extensionFound )
    {
    return false;
    }

  // Now check the file content
  METAIO_STREAM::ifstream inputStream;

  inputStream.open( fname.c_str(), METAIO_STREAM::ios::in |
                                   METAIO_STREAM::ios::binary );

  if( inputStream.fail() )
    {
    return false;
    }

  METAIO_STL::string key;

  inputStream >> key;

  if( inputStream.eof() )
    {
    inputStream.close();
    return false;
    }

  stringPos = fname.find("NDims");
  if( stringPos != METAIO_STL::string::npos )
    {
    inputStream.close();
    return true;
    }

  stringPos = fname.find("ObjectType");
  if( stringPos != METAIO_STL::string::npos )
    {
    inputStream.close();
    return true;
    }

  stringPos = fname.find("Comment");
  if( stringPos != METAIO_STL::string::npos )
    {
    inputStream.close();
    return true;
    }

  inputStream.close();
  return false;
  }

bool MetaImage::
Read(const char *_headerName, bool _readElements, void * _buffer)
  {
  M_Destroy();

  Clear();

  M_SetupReadFields();

  if(_headerName != NULL)
    {
    strcpy(m_FileName, _headerName);
    }
  
  M_PrepareNewReadStream();

  METAIO_STREAM::ifstream * tmpReadStream = new METAIO_STREAM::ifstream;

  tmpReadStream->open(m_FileName, METAIO_STREAM::ios::binary |
                                  METAIO_STREAM::ios::in);

  if(!tmpReadStream->is_open())
    {
    delete tmpReadStream;
    return false;
    }

  if( !this->ReadStream(0, tmpReadStream, _readElements, _buffer) )
    {
    tmpReadStream->close();
    delete tmpReadStream;
    return false;
    }

  tmpReadStream->close();

  delete tmpReadStream;

  return true;
  }

bool MetaImage::
CanReadStream(METAIO_STREAM::ifstream * _stream) const
  {
  if(!strncmp(MET_ReadType(*_stream).c_str(), "Image", 5))
    {
    return true;
    }
  return false;
  }

bool MetaImage::
ReadStream(int _nDims,
           METAIO_STREAM::ifstream * _stream,
           bool _readElements,
           void * _buffer)
  {
  if(!MetaObject::ReadStream(_nDims, _stream))
    {
    METAIO_STREAM::cerr << "MetaImage: Read: Cannot parse file" 
                        << METAIO_STREAM::endl;
    return false;
    }

  if(_readElements)
    {
    if(_buffer == NULL)
      {
      InitializeEssential(m_NDims, 
                          m_DimSize, 
                          m_ElementSpacing,
                          m_ElementType, 
                          m_ElementNumberOfChannels, 
                          NULL, true);
      }
    else
      {
      InitializeEssential(m_NDims, 
                          m_DimSize, 
                          m_ElementSpacing,
                          m_ElementType, 
                          m_ElementNumberOfChannels, 
                          _buffer, false);
      }

    int i, j;
    bool usePath;
    char pathName[255];
    char fName[255];
    usePath = MET_GetFilePath(m_FileName, pathName);

    if(!strcmp("Local", m_ElementDataFileName) || 
       !strcmp("LOCAL", m_ElementDataFileName) ||
       !strcmp("local", m_ElementDataFileName))
      {
      M_ReadElements(_stream, m_ElementData, m_Quantity);
      }
    else if(!strncmp("LIST", m_ElementDataFileName,4))
      {
      int fileImageDim = m_NDims - 1;
      int nWrds;
      char **wrds;
      MET_StringToWordArray(m_ElementDataFileName, &nWrds, &wrds);
      if(nWrds > 1)
        {
        fileImageDim = (int)atof(wrds[1]);
        }
      for(i=0; i<nWrds; i++)
        {
        delete [] wrds[i++];
        }
      if ( (fileImageDim == 0) || (fileImageDim > m_NDims) )
        {
        // if optional file dimension size is not give or is larger than
        // overall dimension then default to a size of m_NDims - 1.
        fileImageDim = m_NDims-1;
        }
      char s[1024];
      METAIO_STREAM::ifstream* readStreamTemp = new METAIO_STREAM::ifstream;
      int elementSize;
      MET_SizeOfType(m_ElementType, &elementSize);
      elementSize *= m_ElementNumberOfChannels;
      int totalFiles = 1;
      for (i = m_NDims; i > fileImageDim; i--)
        {
        totalFiles *= m_DimSize[i-1];
        }
      for(i=0; i< totalFiles && !_stream->eof(); i++)
        {
        _stream->getline(s, 1024);
        if(!_stream->eof())
          {
          j = strlen(s)-1;
          while(j>0 && (isspace(s[j]) || !isprint(s[j])))
            {
            s[j--] = '\0';
            }
          if(usePath)
            {
            sprintf(fName, "%s%s", pathName, s);
            }
          else
            {
            strcpy(fName, s);
            }

          readStreamTemp->open(fName, METAIO_STREAM::ios::binary |
                                      METAIO_STREAM::ios::in);
          if(!readStreamTemp->is_open())
            {
            METAIO_STREAM::cerr << "MetaImage: Read: cannot open slice" 
                                << METAIO_STREAM::endl;
            continue;
            }
          M_ReadElements(readStreamTemp,
                       &(((char *)m_ElementData)[i*m_SubQuantity[fileImageDim]*
                                                 elementSize]),
                       m_SubQuantity[fileImageDim]);
          readStreamTemp->close();
          }
        }
      delete readStreamTemp;
      }
    else if(strstr(m_ElementDataFileName, "%"))
      {
      int elementSize;
      MET_SizeOfType(m_ElementType, &elementSize);
      elementSize *= m_ElementNumberOfChannels;

      int nWrds;
      char **wrds;
      int minV = 1;
      int maxV = m_DimSize[m_NDims-1];
      int stepV = 1;
      char s[255];
      METAIO_STREAM::ifstream* readStreamTemp = new METAIO_STREAM::ifstream;
      MET_StringToWordArray(m_ElementDataFileName, &nWrds, &wrds);
      if(nWrds >= 2)
        {
        minV = (int)atof(wrds[1]);
        maxV = minV + m_DimSize[m_NDims-1] - 1;
        }
      if(nWrds >= 3)
        {
        maxV = (int)atof(wrds[2]);
        stepV = (maxV-minV)/(m_DimSize[m_NDims-1]);
        }
      if(nWrds >= 4)
        {
        stepV = (int)atof(wrds[3]);
        }
      int cnt = 0;
      for(i=minV; i<=maxV; i += stepV)
        {
        sprintf(s, wrds[0], i);
        if(usePath)
          {
          sprintf(fName, "%s%s", pathName, s);
          }
        else
          {
          strcpy(fName, s);
          }
        readStreamTemp->open(fName, METAIO_STREAM::ios::binary 
                                    | METAIO_STREAM::ios::in);
        if(!readStreamTemp->is_open())
          {
          METAIO_STREAM::cerr << "MetaImage: Read: cannot construct file" 
                              << METAIO_STREAM::endl;
          continue;
          }
        
        M_ReadElements(readStreamTemp,
                       &(((char *)m_ElementData)[cnt*m_SubQuantity[m_NDims-1]*
                                                 elementSize]),
                       m_SubQuantity[m_NDims-1]);
        cnt++;
  
        readStreamTemp->close();
        }
      delete readStreamTemp;
      }
    else
      {
      if(usePath)
        {
        sprintf(fName, "%s%s", pathName, m_ElementDataFileName);
        }
      else
        {
        strcpy(fName, m_ElementDataFileName);
        }

      METAIO_STREAM::ifstream* readStreamTemp = new METAIO_STREAM::ifstream;
      readStreamTemp->open(fName, METAIO_STREAM::ios::binary |
                                  METAIO_STREAM::ios::in);
      if(!readStreamTemp->is_open())
        {
        METAIO_STREAM::cerr << "MetaImage: Read: Cannot open data file" 
                            << METAIO_STREAM::endl;
        m_ReadStream->close();
        return false;
        }

      M_ReadElements(readStreamTemp, m_ElementData, m_Quantity);

      readStreamTemp->close();
      delete readStreamTemp;
      }
    }
  
  return true;
  }


//
//
//
bool MetaImage::
Write(const char *_headName,
      const char *_dataName,
      bool _writeElements,
      const void * _constElementData,
      bool _append)
  {
  if(_headName != NULL)
    {
    FileName(_headName);
    }

  bool userDataFileName = true;
  if(_dataName == NULL && strlen(m_ElementDataFileName)==0)
    {
    userDataFileName = false;
    int sPtr = 0;
    MET_GetFileSuffixPtr(m_FileName, &sPtr);
    if(!strcmp(&m_FileName[sPtr], "mha"))
      {
      ElementDataFileName("LOCAL");
      }
    else
      {
      if(!_append)
        {
        MET_SetFileSuffix(m_FileName, "mhd");
        }
      strcpy(m_ElementDataFileName, m_FileName);
      if(m_CompressedData)
        {
        MET_SetFileSuffix(m_ElementDataFileName, "zraw");
        }
      else
        {
        MET_SetFileSuffix(m_ElementDataFileName, "raw");
        }
      }
    }
  else if(_dataName != NULL)
    {
    userDataFileName = false;
    ElementDataFileName(_dataName);
    }

  // make sure suffix is valid
  if(!_append)
    {
    if(!strcmp(m_ElementDataFileName, "LOCAL"))
      {
      MET_SetFileSuffix(m_FileName, "mha");
      }
     else
      {
      MET_SetFileSuffix(m_FileName, "mhd");
      }
    }

  char pathName[255];
  bool usePath = MET_GetFilePath(m_FileName, pathName);
  if(usePath)
    {
    char elementPathName[255];
    MET_GetFilePath(m_ElementDataFileName, elementPathName);
    if(!strcmp(pathName, elementPathName))
      {
      strcpy(elementPathName, &m_ElementDataFileName[strlen(pathName)]);
      strcpy(m_ElementDataFileName, elementPathName);
      }
    }

  METAIO_STREAM::ofstream * tmpWriteStream = new METAIO_STREAM::ofstream;

// Some older sgi compilers have a error in the ofstream constructor
// that requires a file to exist for output
#ifdef __sgi
  {
  METAIO_STREAM::ofstream tFile(m_FileName, METAIO_STREAM::ios::binary |
                                            METAIO_STREAM::ios::out);
  tFile.close();                    
  }
#endif

  if(!_append)
    {
    tmpWriteStream->open(m_FileName, METAIO_STREAM::ios::binary |
                                     METAIO_STREAM::ios::out);
    }
  else
    {
    tmpWriteStream->open(m_FileName, METAIO_STREAM::ios::binary |
                                     METAIO_STREAM::ios::app |
                                     METAIO_STREAM::ios::out);
    }

  if(!tmpWriteStream->is_open())
    {
    if(!userDataFileName)
      {
      ElementDataFileName("");
      }

    delete tmpWriteStream;

    return false;
    }

  bool result = MetaImage::WriteStream(tmpWriteStream,
                                       _writeElements,
                                       _constElementData);

  if(!userDataFileName)
    {
    ElementDataFileName("");
    }
      
  tmpWriteStream->close();
  delete tmpWriteStream;

  return result;
  }
  
bool MetaImage::
WriteStream(METAIO_STREAM::ofstream * _stream,
            bool _writeElements,
            const void * _constElementData)
  {
  if(m_WriteStream != NULL)
    {
    METAIO_STREAM::cerr << "MetaArray: WriteStream: two files open?" 
                        << METAIO_STREAM::endl;
    delete m_WriteStream;
    }

  m_WriteStream = _stream;

  unsigned char * compressedElementData = NULL;
  if(m_BinaryData && m_CompressedData && !strstr(m_ElementDataFileName, "%")) 
    // compressed & !slice/file
    {
    int elementSize;
    MET_SizeOfType(m_ElementType, &elementSize);
    int elementNumberOfBytes = elementSize*m_ElementNumberOfChannels;

    if(_constElementData == NULL)
      {
      compressedElementData = MET_PerformCompression( 
                                  (const unsigned char *)m_ElementData,
                                  m_Quantity * elementNumberOfBytes,
                                  & m_CompressedDataSize );
      }
    else
      {
      compressedElementData = MET_PerformCompression( 
                                  (const unsigned char *)_constElementData,
                                  m_Quantity * elementNumberOfBytes,
                                  & m_CompressedDataSize );
      }
    }

  M_SetupWriteFields();

  M_Write();

  if(_writeElements)
    {
    if(m_BinaryData && m_CompressedData && !strstr(m_ElementDataFileName, "%")) 
      // compressed & !slice/file
      {
      M_WriteElements(m_WriteStream,
                      compressedElementData,
                      m_CompressedDataSize);

      delete [] compressedElementData;
      m_CompressedDataSize = 0;
      }
    else
      {
      if(_constElementData == NULL)
        {
        M_WriteElements(m_WriteStream,
                        m_ElementData,
                        m_Quantity);
        }
      else
        {
        M_WriteElements(m_WriteStream,
                        _constElementData, 
                        m_Quantity);
        }
      }
    }

  m_WriteStream = NULL;

  return true;
  }
  
bool MetaImage::
Append(const char *_headName)
  {
  if(META_DEBUG)
   {
   METAIO_STREAM::cout << "MetaImage: Append" << METAIO_STREAM::endl;
   }

  return this->Write(_headName, NULL, true, NULL, true);
  }

void MetaImage::
M_Destroy(void)
  {
  if(m_AutoFreeElementData && m_ElementData != NULL)
    {
    delete [] (char *)m_ElementData; 
    }

  m_ElementData = NULL;

  MetaObject::M_Destroy();
  }

void MetaImage::
M_SetupReadFields(void)
  {
  if(META_DEBUG)
    {
    METAIO_STREAM::cout << "MetaImage: M_SetupReadFields" 
                        << METAIO_STREAM::endl;
    }

  MetaObject::M_SetupReadFields();

  MET_FieldRecordType * mF;

  int nDimsRecNum = MET_GetFieldRecordNumber("NDims", &m_Fields);

  mF = new MET_FieldRecordType;
  MET_InitReadField(mF, "DimSize", MET_INT_ARRAY, true, nDimsRecNum);
  mF->required = true;
  m_Fields.push_back(mF);

  mF = new MET_FieldRecordType;
  MET_InitReadField(mF, "HeaderSize", MET_INT, false);
  m_Fields.push_back(mF);

  mF = new MET_FieldRecordType;
  MET_InitReadField(mF, "Modality", MET_STRING, false);
  m_Fields.push_back(mF);

  mF = new MET_FieldRecordType;
  MET_InitReadField(mF, "ImagePosition", MET_FLOAT_ARRAY, false, nDimsRecNum);
  m_Fields.push_back(mF);

  mF = new MET_FieldRecordType;
  MET_InitReadField(mF, "SequenceID", MET_INT_ARRAY, false, nDimsRecNum);
  m_Fields.push_back(mF);

  mF = new MET_FieldRecordType;
  MET_InitReadField(mF, "ElementMin", MET_FLOAT, false);
  m_Fields.push_back(mF);

  mF = new MET_FieldRecordType;
  MET_InitReadField(mF, "ElementMax", MET_FLOAT, false);
  m_Fields.push_back(mF);

  mF = new MET_FieldRecordType;
  MET_InitReadField(mF, "ElementNumberOfChannels", MET_INT, false);
  m_Fields.push_back(mF);

  mF = new MET_FieldRecordType;
  MET_InitReadField(mF, "ElementSize", MET_FLOAT_ARRAY, false, nDimsRecNum);
  m_Fields.push_back(mF);

  mF = new MET_FieldRecordType;  // Set but not used...
  MET_InitReadField(mF, "ElementNBits", MET_INT, false);
  m_Fields.push_back(mF);

  mF = new MET_FieldRecordType;  // Used by ConvertElementToIntensity funcs
  MET_InitReadField(mF, "ElementToIntensityFunctionSlope", MET_FLOAT, false);
  m_Fields.push_back(mF);

  mF = new MET_FieldRecordType;  // Used by ConvertElementToIntensity funcs
  MET_InitReadField(mF, "ElementToIntensityFunctionOffset", MET_FLOAT, false);
  m_Fields.push_back(mF);

  mF = new MET_FieldRecordType;
  MET_InitReadField(mF, "ElementType", MET_STRING, true);
  mF->required = true;
  m_Fields.push_back(mF);

  mF = new MET_FieldRecordType;
  MET_InitReadField(mF, "ElementDataFile", MET_STRING, true);
  mF->required = true;
  mF->terminateRead = true;
  m_Fields.push_back(mF);
  }

void MetaImage::
M_SetupWriteFields(void)
  {
  strcpy(m_ObjectTypeName,"Image");
  MetaObject::M_SetupWriteFields();

  MET_FieldRecordType * mF;

  mF = new MET_FieldRecordType;
  MET_InitWriteField(mF, "DimSize", MET_INT_ARRAY, m_NDims, m_DimSize);
  m_Fields.push_back(mF);

  char s[255];
  if(m_HeaderSize > 0 || m_HeaderSize == -1)
    {
    mF = new MET_FieldRecordType;
    MET_InitWriteField(mF, "HeaderSize", MET_INT);
    m_Fields.push_back(mF);
    }

  int i;
  if(m_Modality != MET_MOD_UNKNOWN)
    {
    mF = new MET_FieldRecordType;
    strcpy(s, MET_ValueTypeName[m_Modality]);
    MET_InitWriteField(mF, "Modality", MET_STRING, strlen(s), s);
    m_Fields.push_back(mF);
    }

  i = MET_GetFieldRecordNumber("AnatomicalOrientation", &m_Fields);
  if(i < 0)
    {
    const char * str = AnatomicalOrientationAcronym();
    mF = new MET_FieldRecordType;
    MET_InitWriteField(mF, "AnatomicalOrientation",
                       MET_STRING, strlen(str), str);
    m_Fields.push_back(mF);
    }

  bool valid = false;
  for(i=0; i<4; i++)
    {
    if(m_SequenceID[i] != 0)
      {
      valid = true;
      break;
      }
    }
  if(valid)
    {
    mF = new MET_FieldRecordType;
    MET_InitWriteField(mF, "SequenceID", MET_FLOAT_ARRAY, m_NDims,
                       m_SequenceID);
    m_Fields.push_back(mF);
    }

  if(m_ElementMinMaxValid)
    {
    mF = new MET_FieldRecordType;
    MET_InitWriteField(mF, "ElementMin", MET_FLOAT, m_ElementMin);
    m_Fields.push_back(mF);

    mF = new MET_FieldRecordType;
    MET_InitWriteField(mF, "ElementMax", MET_FLOAT, m_ElementMax);
    m_Fields.push_back(mF);
    }

  if(m_ElementNumberOfChannels>1)
    {
    mF = new MET_FieldRecordType;
    MET_InitWriteField(mF, "ElementNumberOfChannels", MET_INT, 
                       m_ElementNumberOfChannels);
    m_Fields.push_back(mF);
    }

  if(m_ElementSizeValid)
    {
    mF = new MET_FieldRecordType;
    MET_InitWriteField(mF, "ElementSize", MET_FLOAT_ARRAY, m_NDims,
                       m_ElementSize);
    m_Fields.push_back(mF);
    }

  if(m_ElementToIntensityFunctionSlope != 1 ||
     m_ElementToIntensityFunctionOffset != 0)
    {
    mF = new MET_FieldRecordType;
    MET_InitWriteField(mF, "ElementToIntensityFunctionSlope",
                       MET_FLOAT, m_ElementToIntensityFunctionSlope);
    m_Fields.push_back(mF);
    mF = new MET_FieldRecordType;
    MET_InitWriteField(mF, "ElementToIntensityFunctionOffset",
                       MET_FLOAT, m_ElementToIntensityFunctionOffset);
    m_Fields.push_back(mF);
    }

  mF = new MET_FieldRecordType;
  MET_TypeToString(m_ElementType, s);
  MET_InitWriteField(mF, "ElementType", MET_STRING, strlen(s), s);
  m_Fields.push_back(mF);

  mF = new MET_FieldRecordType;
  MET_InitWriteField(mF, "ElementDataFile", MET_STRING, 
                     strlen(m_ElementDataFileName),
                     m_ElementDataFileName);
  mF->terminateRead = true;
  m_Fields.push_back(mF);
  }

//
//
//
bool MetaImage::
M_Read(void)
  {
  if(META_DEBUG) 
    {
    METAIO_STREAM::cout << "MetaImage: M_Read: Loading Header" 
                        << METAIO_STREAM::endl;
    }
  if(!MetaObject::M_Read())
    {
    METAIO_STREAM::cerr << "MetaImage: M_Read: Error parsing file" 
                        << METAIO_STREAM::endl;
    return false;
    }

  if(META_DEBUG) 
    {
    METAIO_STREAM::cout << "MetaImage: M_Read: Parsing Header" 
                        << METAIO_STREAM::endl;
    }
  MET_FieldRecordType * mF;
     
  if(META_DEBUG) 
    {
    METAIO_STREAM::cout << "metaImage: M_Read: elementSpacing[" << 0 << "] = "
                        << m_ElementSpacing[0] << METAIO_STREAM::endl;
    }
  mF = MET_GetFieldRecord("DimSize", &m_Fields);
  if(mF && mF->defined)
    {
    int i;
    for(i=0; i<m_NDims; i++)
      {
      m_DimSize[i] = (int)mF->value[i];
      }
    }

  mF = MET_GetFieldRecord("HeaderSize", &m_Fields);
  if(mF && mF->defined)
    {
    m_HeaderSize = (int)mF->value[0];
    }

  mF = MET_GetFieldRecord("Modality", &m_Fields);
  if(mF && mF->defined)
    {
    MET_StringToImageModality((char *)mF->value, &m_Modality);
    }

  mF = MET_GetFieldRecord("SequenceID", &m_Fields);
  if(mF && mF->defined)
    {
    int i;
    for(i=0; i<m_NDims; i++)
      {
      m_SequenceID[i] = (float)(mF->value[i]);
      }
    }

  mF = MET_GetFieldRecord("ImagePosition", &m_Fields);
  if(mF && mF->defined)
    {
    int i;
    for(i=0; i<m_NDims; i++)
      {
      m_Offset[i] = static_cast<double>(mF->value[i]);
      }
    }

  mF = MET_GetFieldRecord("ElementMin", &m_Fields);
  if(mF && mF->defined)
    {
    m_ElementMin = mF->value[0];
    }

  mF = MET_GetFieldRecord("ElementMax", &m_Fields);
  if(mF && mF->defined)
    {
    m_ElementMax = mF->value[0];
    }

  mF = MET_GetFieldRecord("ElementNumberOfChannels", &m_Fields);
  if(mF && mF->defined)
    {
    m_ElementNumberOfChannels = (int)mF->value[0];
    }


  mF = MET_GetFieldRecord("ElementSize", &m_Fields);
  if(mF && mF->defined)
    {
    m_ElementSizeValid = true;
    int i;
    for(i=0; i<m_NDims; i++)
      {
      m_ElementSize[i] = (float)(mF->value[i]);
      }
    mF = MET_GetFieldRecord("ElementSpacing", &m_Fields);
    if(mF && !mF->defined)
      {
      for(i=0; i<m_NDims; i++)
        {
        m_ElementSpacing[i] = m_ElementSize[i];
        }
      }
    }
  else
    {
    int i;
    m_ElementSizeValid = false;
    for(i=0; i<m_NDims; i++)
      {
      m_ElementSize[i] = m_ElementSpacing[i];
      }
    }

  m_ElementToIntensityFunctionSlope = 1;
  m_ElementToIntensityFunctionOffset = 0;
  mF = MET_GetFieldRecord("ElementToIntensityFunctionSlope", &m_Fields);
  if(mF && mF->defined)
    {
    m_ElementToIntensityFunctionSlope = mF->value[0];
    }
  mF = MET_GetFieldRecord("ElementToIntensityFunctionOffset", &m_Fields);
  if(mF && mF->defined)
    {
    m_ElementToIntensityFunctionOffset = mF->value[0];
    }

  mF = MET_GetFieldRecord("ElementType", &m_Fields);
  if(mF && mF->defined)
    {
    MET_StringToType((char *)(mF->value), &m_ElementType);
    }

  mF = MET_GetFieldRecord("ElementDataFile", &m_Fields);
  if(mF && mF->defined)
    {
    strcpy(m_ElementDataFileName, (char *)(mF->value));
    }

  return true;
  }

//
//
//
bool MetaImage::
M_ReadElements(METAIO_STREAM::ifstream * _fstream, void * _data,
               int _dataQuantity)
  {
  if(META_DEBUG) 
    {
    METAIO_STREAM::cout << "MetaImage: M_ReadElements" << METAIO_STREAM::endl;
    }

  if(m_HeaderSize>(int)0)
    {
    _fstream->seekg(m_HeaderSize, METAIO_STREAM::ios::beg);
    if(!_fstream->good())
      {
      METAIO_STREAM::cerr << "MetaImage: Read: header not read correctly" 
                          << METAIO_STREAM::endl;
      return false;
      }
    }

  int elementSize;
  MET_SizeOfType(m_ElementType, &elementSize);
  int readSize = _dataQuantity*m_ElementNumberOfChannels*elementSize;
  if(META_DEBUG)
    {
    METAIO_STREAM::cout << "MetaImage: M_ReadElements: ReadSize = " 
                        << readSize << METAIO_STREAM::endl;
    }

  if(m_HeaderSize == -1)
    {
    if(META_DEBUG) 
      {
      METAIO_STREAM::cout << "MetaImage: M_ReadElements: Skipping header" 
                          << METAIO_STREAM::endl;
      }
    _fstream->seekg(-readSize, METAIO_STREAM::ios::end);
    }

  // If compressed we inflate
  if(m_BinaryData && m_CompressedData)
    {
    // if m_CompressedDataSize is not defined we assume the size of the
    // file is the size of the compressed data
    if(m_CompressedDataSize==0)
      {
      _fstream->seekg(0, METAIO_STREAM::ios::end);
      m_CompressedDataSize = _fstream->tellg();
      _fstream->seekg(0, METAIO_STREAM::ios::beg);
      }

    unsigned char* compr = new unsigned char[m_CompressedDataSize];
    _fstream->read((char *)compr, m_CompressedDataSize);
    
    MET_PerformUncompression(compr, m_CompressedDataSize,
                             (unsigned char *)_data, readSize);

    delete [] compr;
    }
  else // if not compressed
    {
    if(!m_BinaryData)
      {
      double tf;
      MET_SizeOfType(m_ElementType, &elementSize);
      for(int i=0; i<_dataQuantity; i++)
        {
        *_fstream >> tf;
        MET_DoubleToValue(tf, m_ElementType, _data, i);
        _fstream->get();
        }
      }
    else
      {
      _fstream->read((char *)_data, readSize);
      int gc = _fstream->gcount();
      if(gc != readSize)
        {
        METAIO_STREAM::cerr
                  << "MetaImage: M_ReadElements: data not read completely" 
                  << METAIO_STREAM::endl;
        METAIO_STREAM::cerr << "   ideal = " << readSize << " : actual = " << gc 
                  << METAIO_STREAM::endl;
        return false;
        }
      }
    }

  return true;
  }

bool MetaImage::
M_WriteElements(METAIO_STREAM::ofstream * _fstream,
                const void * _data,
                int _dataQuantity)
  {

  if(!strcmp(m_ElementDataFileName, "LOCAL"))
    {
    MetaImage::M_WriteElementData(_fstream, _data, _dataQuantity);
    }
  else // write the data in a separate file
    {
    char dataFileName[255];
    char pathName[255];
    bool usePath = MET_GetFilePath(m_FileName, pathName);
    if(usePath)
      {
      sprintf(dataFileName, "%s%s", pathName, m_ElementDataFileName);
      }
    else
      {
      strcpy(dataFileName, m_ElementDataFileName);
      }

    if(strstr(dataFileName, "%")) // write slice by slice
      {
      int i;
      char fName[255];
      int elementSize;
      MET_SizeOfType(m_ElementType, &elementSize);
      int elementNumberOfBytes = elementSize*m_ElementNumberOfChannels;
      int sliceNumberOfBytes = m_SubQuantity[m_NDims-1]*elementNumberOfBytes;

      METAIO_STREAM::ofstream* writeStreamTemp = new METAIO_STREAM::ofstream;
      for(i=1; i<=m_DimSize[m_NDims-1]; i++)
        {
        sprintf(fName, dataFileName, i);

// Some older sgi compilers have a error in the ofstream constructor
// that requires a file to exist for output
#ifdef __sgi
        {
        METAIO_STREAM::ofstream tFile(fName, METAIO_STREAM::ios::binary |
                                             METAIO_STREAM::ios::out);
        tFile.close();                    
        }
#endif

        writeStreamTemp->open(fName, METAIO_STREAM::ios::binary |
                                     METAIO_STREAM::ios::out);

        if(!m_CompressedData)
          {
          MetaImage::M_WriteElementData(writeStreamTemp, 
                             &(((const char *)_data)[(i-1)*sliceNumberOfBytes]),
                             sliceNumberOfBytes);
          }
        else
          {
          unsigned char * compressedData = NULL;
          unsigned int compressedDataSize = 0;

          // Compress the data slice by slice
          compressedData = MET_PerformCompression( 
                  &(((const unsigned char *)_data)[(i-1)*sliceNumberOfBytes]),
                  sliceNumberOfBytes,
                  & compressedDataSize );

          // Write the compressed data
          MetaImage::M_WriteElementData( writeStreamTemp,
                              compressedData,
                              compressedDataSize );

          delete [] compressedData;
          }

        writeStreamTemp->close();
        }

      delete writeStreamTemp;
      }
    else // write the image in one unique other file
      {
// Some older sgi compilers have a error in the ofstream constructor
// that requires a file to exist for output
#ifdef __sgi
      {
      METAIO_STREAM::ofstream tFile(dataFileName, METAIO_STREAM::ios::binary |
                                                  METAIO_STREAM::ios::out);
      tFile.close();                    
      }
#endif

      METAIO_STREAM::ofstream* writeStreamTemp = new METAIO_STREAM::ofstream;
      writeStreamTemp->open(dataFileName, METAIO_STREAM::ios::binary |
                                          METAIO_STREAM::ios::out);
 
      MetaImage::M_WriteElementData(writeStreamTemp, _data, _dataQuantity);
        
      writeStreamTemp->close();
      delete writeStreamTemp; 
      }
    }

  return true;
  }


bool MetaImage::
M_WriteElementData(METAIO_STREAM::ofstream * _fstream,
                   const void * _data,
                   int _dataQuantity)
  {
  if(!m_BinaryData)
    {
    double tf;
    for(int i=0; i<_dataQuantity; i++)
      {
      MET_ValueToDouble(m_ElementType, _data, i, &tf);
      if((i+1)/10 == (double)(i+1.0)/10.0)
        {
        (*_fstream) << tf << METAIO_STREAM::endl;
        }
      else
        {
        (*_fstream) << tf << " ";
        }
      }
    }
  else
    {
    if(m_CompressedData)
      {
      _fstream->write( (const char *)_data, _dataQuantity );
      }
    else
      {
      int elementSize;
      MET_SizeOfType(m_ElementType, &elementSize);
      int elementNumberOfBytes = elementSize*m_ElementNumberOfChannels;

      _fstream->write( (const char *)_data, 
                       _dataQuantity * elementNumberOfBytes );
      }
    }
    
  return true;
  }

#if (METAIO_USE_NAMESPACE)
};
#endif

