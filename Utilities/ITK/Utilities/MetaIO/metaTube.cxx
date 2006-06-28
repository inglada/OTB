#include <stdio.h>
#include <ctype.h>
#include <iostream>
#include <fstream>
#include <string>

#include "metaUtils.h"
#include "metaObject.h"
#include "metaTube.h"

/** MetaTube Constructors */
MetaTube::
MetaTube()
:MetaObject()
{
  if(META_DEBUG) std::cout << "MetaTube()" << std::endl;
  Clear();
}


MetaTube::
MetaTube(const char *_headerName)
:MetaObject()
{
  if(META_DEBUG)  std::cout << "MetaTube()" << std::endl;
  Clear();
  Read(_headerName);
}


MetaTube::
MetaTube(const MetaTube *_tube)
:MetaObject()
{
  if(META_DEBUG)  std::cout << "MetaTube()" << std::endl;
  Clear();
  CopyInfo(_tube);
}


MetaTube::
MetaTube(unsigned int dim)
:MetaObject(dim)
{
  if(META_DEBUG) std::cout << "MetaTube()" << std::endl;
  Clear();
}

/** Destructor */
MetaTube::
~MetaTube()
{
  // Delete the list of pointers to tubes.
  PointListType::iterator it = m_PointList.begin();
  while(it != m_PointList.end())
  {
    TubePnt* pnt = *it;
    it++;
    delete pnt;
  }  
  m_PointList.clear();
  M_Destroy();
}

//
void MetaTube::
PrintInfo() const
{
  MetaObject::PrintInfo();
  std::cout << "ParentPoint = " << m_ParentPoint << std::endl;
  if(m_Root)
    {
    std::cout << "Root = " << "True" << std::endl;
    }
  else
    {
    std::cout << "Root = " << "True" << std::endl;
    }
  std::cout << "PointDim = " << m_PointDim << std::endl;
  std::cout << "NPoints = " << m_NPoints << std::endl;
  char str[255];
  MET_TypeToString(m_ElementType, str);
  std::cout << "ElementType = " << str << std::endl;
}

void MetaTube::
CopyInfo(const MetaTube * _tube)
{
  MetaObject::CopyInfo(_tube);
}

    

void MetaTube::
PointDim(const char* pointDim)
{
  strcpy(m_PointDim,pointDim);
}
    
const char* MetaTube::
PointDim(void) const
{
  return m_PointDim;
}

void MetaTube::
NPoints(int npnt)
{
  m_NPoints = npnt;
}

int MetaTube::
NPoints(void) const
{
  return m_NPoints;
}

void MetaTube::
Root(bool root)
{
  m_Root = root;
}
    
bool MetaTube:: 
Root(void) const
{
  return m_Root;
}


void  MetaTube::
ParentPoint(int parentpoint)
{
  m_ParentPoint = parentpoint;
}

int MetaTube::
ParentPoint(void) const
{
  return m_ParentPoint;
}

/** Clear tube information */
void MetaTube::
Clear(void)
{
  if(META_DEBUG) std::cout << "MetaTube: Clear" << std::endl;
  MetaObject::Clear();
  // Delete the list of pointers to tubes.
  PointListType::iterator it = m_PointList.begin();
  while(it != m_PointList.end())
  {
    TubePnt* pnt = *it;
    it++;
    delete pnt;
  }  
  m_PointList.clear();

  m_ParentPoint= -1;
  m_Root = false;
  m_NPoints = 0;
  strcpy(m_PointDim, "x y z r v1x v1y v1z v2x v2y v2z tx ty tz red green blue alpha id");
  m_ElementType = MET_FLOAT;
}
        
/** Destroy tube information */
void MetaTube::
M_Destroy(void)
{
  MetaObject::M_Destroy();
}

/** Set Read fields */
void MetaTube::
M_SetupReadFields(void)
{
  if(META_DEBUG) std::cout << "MetaTube: M_SetupReadFields" << std::endl;

  MetaObject::M_SetupReadFields();

  MET_FieldRecordType * mF;

  // int nDimsRecNum = MET_GetFieldRecordNumber("NDims", &m_Fields);

  mF = new MET_FieldRecordType;
  MET_InitReadField(mF, "ParentPoint", MET_INT, false);
  m_Fields.push_back(mF);

  mF = new MET_FieldRecordType;
  MET_InitReadField(mF, "Root", MET_STRING, false);
  m_Fields.push_back(mF);

  mF = new MET_FieldRecordType;
  MET_InitReadField(mF, "PointDim", MET_STRING, true);
  m_Fields.push_back(mF);

  mF = new MET_FieldRecordType;
  MET_InitReadField(mF, "NPoints", MET_INT, true);
  m_Fields.push_back(mF);

  mF = new MET_FieldRecordType;
  MET_InitReadField(mF, "Points", MET_NONE, true);
  mF->terminateRead = true;
  m_Fields.push_back(mF);

}

void MetaTube::
M_SetupWriteFields(void)
{
  strcpy(m_ObjectTypeName,"Tube");
  MetaObject::M_SetupWriteFields();

  MET_FieldRecordType * mF;

  if(m_ParentPoint>=0 && m_ParentID>=0)
    {
    mF = new MET_FieldRecordType;
    MET_InitWriteField(mF, "ParentPoint", MET_INT,m_ParentPoint);
    m_Fields.push_back(mF);
    }

  if(m_Root)
    {
    mF = new MET_FieldRecordType;
    MET_InitWriteField(mF, "Root", MET_STRING, strlen("True"), "True");
    m_Fields.push_back(mF);
    }
  else
    {
    mF = new MET_FieldRecordType;
    MET_InitWriteField(mF, "Root", MET_STRING, strlen("False"), "False");
    m_Fields.push_back(mF);
    }

  if(strlen(m_PointDim)>0)
    {
    mF = new MET_FieldRecordType;
    MET_InitWriteField(mF, "PointDim", MET_STRING,
                           strlen(m_PointDim),m_PointDim);
    m_Fields.push_back(mF);
    }

  m_NPoints = m_PointList.size();
  mF = new MET_FieldRecordType;
  MET_InitWriteField(mF, "NPoints", MET_INT,m_NPoints);
  m_Fields.push_back(mF);

  mF = new MET_FieldRecordType;
  MET_InitWriteField(mF, "Points", MET_NONE);
  m_Fields.push_back(mF);

}



bool MetaTube::
M_Read(void)
{
  if(META_DEBUG)
    {
    std::cout << "MetaTube: M_Read: Loading Header" << std::endl;
    }

  if(!MetaObject::M_Read())
    {
    std::cout << "MetaTube: M_Read: Error parsing file" << std::endl;
    return false;
    }

  if(META_DEBUG)
    {
    std::cout << "MetaTube: M_Read: Parsing Header" << std::endl;
    }
 
  MET_FieldRecordType * mF;
 
  mF = MET_GetFieldRecord("ParentPoint", &m_Fields);
  if(mF->defined)
    {
    m_ParentPoint= (int)mF->value[0];
    }

  m_Root = false;
  mF = MET_GetFieldRecord("Root", &m_Fields);
  if(mF->defined)
    {
    if(*((char *)(mF->value)) == 'T' 
      || *((char*)(mF->value)) == 't'
      || *((char*)(mF->value)) == '1')
      {
      m_Root = true;
      }
    else
      {
      m_Root = false;
      }
    }

  mF = MET_GetFieldRecord("NPoints", &m_Fields);
  if(mF->defined)
    {
    m_NPoints= (int)mF->value[0];
    }

  mF = MET_GetFieldRecord("PointDim", &m_Fields);
  if(mF->defined)
    {
    strcpy(m_PointDim,(char *)(mF->value));
    }


  int* posDim= new int[m_NDims];
  int i;
  for(i= 0; i < m_NDims; i++)
    {
    posDim[i] = -1;
    }
  int posR = -1;
  int posV1x = -1;
  int posV1y = -1;
  int posV1z = -1;
  int posV2x = -1;
  int posV2y = -1;
  int posV2z = -1;
  int posTx = -1;
  int posTy = -1;
  int posTz = -1;
  int posRed = -1;
  int posGreen = -1;
  int posBlue = -1;
  int posAlpha = -1;
  int posID = -1;

  int pntDim;
  char** pntVal = NULL;
  MET_StringToWordArray(m_PointDim, &pntDim, &pntVal); 
 
  if(META_DEBUG)
    {
    std::cout << "MetaTube: Parsing point dim" << std::endl; 
    }

  int j;
  for(j = 0; j < pntDim; j++) 
    {
    if(!strcmp(pntVal[j], "x") || !strcmp(pntVal[j], "X"))
      {
      posDim[0] = j;
      }
    if(!strcmp(pntVal[j], "y") || !strcmp(pntVal[j], "Y"))
      {
      posDim[1] = j;
      }
    if(!strcmp(pntVal[j], "z") || !strcmp(pntVal[j], "Z"))
      {
      posDim[2] = j;
      }
    if(((char *)pntVal[j])[0] == 'w' || ((char *)pntVal[j])[0] == 'W')
      {
      posDim[(int)pntVal[j][1]+3] = j;
      }
    if(!strcmp(pntVal[j], "s") || !strcmp(pntVal[j], "S") ||
      !strcmp(pntVal[j], "r") || !strcmp(pntVal[j], "R") ||
      !strcmp(pntVal[j], "rad") || !strcmp(pntVal[j], "Rad") ||
      !strcmp(pntVal[j], "radius") || !strcmp(pntVal[j], "Radius"))
      {
      posR = j;
      }
    if(!strcmp(pntVal[j], "v1x"))
      {
      posV1x = j;
      }
    if(!strcmp(pntVal[j], "v1y"))
      {
      posV1y = j;
      }
    if(!strcmp(pntVal[j], "v1z"))
      {
      posV1z = j;
      }
    if(!strcmp(pntVal[j], "v2x"))
      {
      posV2x = j;
      }
    if(!strcmp(pntVal[j], "v2y"))
      {
      posV2y = j;
      }
    if(!strcmp(pntVal[j], "v2z"))
      {
      posV2z = j;
      }
    if(!strcmp(pntVal[j], "tx"))
      {
      posTx = j;
      }
    if(!strcmp(pntVal[j], "ty"))
      {
      posTy = j;
      }
    if(!strcmp(pntVal[j], "tz"))
      {
      posTz = j;
      }
    if(!strcmp(pntVal[j], "red"))
      {
      posRed = j;
      }
    if(!strcmp(pntVal[j], "green"))
      {
      posGreen = j;
      }
    
    if(!strcmp(pntVal[j], "blue"))
      {
      posBlue = j;
      }
    if(!strcmp(pntVal[j], "alpha"))
      {
      posAlpha = j;
      }
    if(!strcmp(pntVal[j], "id") || !strcmp(pntVal[j], "ID"))
      {
      posID = j;
      }
    }

  for(i=0;i<pntDim;i++)
    {
    delete [] pntVal[i];
    }
  delete [] pntVal;

  float v[30];
  
  if(m_Event)
    {
    m_Event->StartReading(m_NPoints);
    }

  if(m_BinaryData)
    {
    int elementSize;
    MET_SizeOfType(m_ElementType, &elementSize);
    int readSize = m_NPoints*(m_NDims*(2+m_NDims)+10)*elementSize;

    char* _data = new char[readSize];
    m_ReadStream->read((char *)_data, readSize);

    int gc = m_ReadStream->gcount();
    if(gc != readSize)
      {
      std::cout << "MetaLine: m_Read: data not read completely" 
                << std::endl;
      std::cout << "   ideal = " << readSize 
                << " : actual = " << gc << std::endl;
      return false;
      }

    i=0;
    double td;
    int d;
    for(j=0; j<m_NPoints; j++) 
      {
      TubePnt* pnt = new TubePnt(m_NDims);
      
      for(d=0; d<m_NDims; d++)
        {
        MET_ValueToDouble(m_ElementType, _data, i++, &td);
        pnt->m_X[d] = (float)td;
        }

      MET_ValueToDouble(m_ElementType, _data, i++, &td);
      pnt->m_R = (float)td;

      for(d = 0; d < m_NDims; d++)
        {
        MET_ValueToDouble(m_ElementType, _data, i++, &td);
        pnt->m_V1[d] = (float)td;
        }
    
      if(m_NDims==3)
        {
        for(d = 0; d < m_NDims; d++)
          {
          MET_ValueToDouble(m_ElementType, _data, i++, &td);
          pnt->m_V2[d] = (float)td;
          }
        }
      
      for(d = 0; d < m_NDims; d++)
        {
        MET_ValueToDouble(m_ElementType, _data, i++, &td);
        pnt->m_T[d] = (float)td;
        }
           
      for(d=0; d<4; d++)
        {
        MET_ValueToDouble(m_ElementType, _data, i++, &td);
        pnt->m_Color[d] = (float)td;
        }

      MET_ValueToDouble(m_ElementType,_data,i++,&td);
      pnt->m_ID=(int)td;

      m_PointList.push_back(pnt);
      }
    delete [] _data;
    }
  else
    {
    for(j=0; j<m_NPoints; j++) 
      {
      if(m_Event)
        {
        m_Event->SetCurrentIteration(j+1);
        }

      TubePnt* pnt = new TubePnt(m_NDims);

      for(int k=0; k<pntDim; k++)
        {
        *m_ReadStream >> v[k];
        m_ReadStream->get();
        }

      for(int d=0; d<m_NDims; d++)
        {
        pnt->m_X[d] = v[posDim[d]];
        }

      pnt->m_R = v[posR];

      if(posV1x>=0 && posV1x<pntDim)
        {
        pnt->m_V1[0] = v[posV1x]; 
        if(posV1y >= 0 && posV1y<pntDim) 
          {
          pnt->m_V1[1] = v[posV1y]; 
          }
        if(posV1z >= 0 && m_NDims>2 && posV1z<pntDim) 
          {
          pnt->m_V1[2] = v[posV1z]; 
          }
        }
      if(posV2x >= 0 && posV2x<pntDim) 
        {
        pnt->m_V2[0] = v[posV2x]; 
        if(posV2y >= 0 && posV2y<pntDim) 
          {
          pnt->m_V2[1] = v[posV2y]; 
          }
        if(posV2z >= 0 && m_NDims>2 && posV2z<pntDim) 
          {
          pnt->m_V2[2] = v[posV2z]; 
          }
        }
      if(posTx >= 0 && posTx<pntDim) 
        {
        pnt->m_T[0] = v[posTx]; 
        if(posTy >= 0 && posTy<pntDim) 
          {
          pnt->m_T[1] = v[posTy]; 
          }
        if(posTz >= 0 && m_NDims>2 && posTz<pntDim) 
          {
          pnt->m_T[2] = v[posTz]; 
          }
        }

      if(posRed >= 0 && posRed < pntDim)
        {
        pnt->m_Color[0] = v[posRed];
        }

      if(posGreen >= 0 && posGreen < pntDim)
        {
        pnt->m_Color[1] = v[posGreen];
        }
    
      if(posBlue >= 0 && posBlue < pntDim)
        {
        pnt->m_Color[2] = v[posBlue];
        }
    
      if(posAlpha >= 0 && posAlpha < pntDim)
        {
        pnt->m_Color[3] = v[posAlpha];
        }
 
      if(posID >= 0 && posID < pntDim)
        {
        pnt->m_ID = (int)v[posID];
        }
 
      m_PointList.push_back(pnt);
      }

      
    char c = ' ';
    while( (c!='\n') && (!m_ReadStream->eof()))
      {
      c = m_ReadStream->get();// to avoid unrecognize charactere
      }
    }
  
  if(m_Event)
    {
    m_Event->StopReading();
    }

  delete []posDim;
  return true;
}

MET_ValueEnumType MetaTube::
ElementType(void) const
{
  return m_ElementType;
}

void MetaTube::
ElementType(MET_ValueEnumType _elementType)
{
  m_ElementType = _elementType;
}

bool MetaTube::
M_Write(void)
{

  if(!MetaObject::M_Write())
    {
    std::cout << "MetaTube: M_Read: Error parsing file" << std::endl;
    return false;
    }

  /** Then copy all tubes points */
  if(m_BinaryData)
    {
    PointListType::const_iterator it = m_PointList.begin();
    int elementSize;
    MET_SizeOfType(m_ElementType, &elementSize);

    char* data = new char[(m_NDims*(2+m_NDims)+10)*m_NPoints*elementSize];
    int i=0;
    int d;
    while(it != m_PointList.end())
      {
      for(d = 0; d < m_NDims; d++)
        {
        MET_DoubleToValue((double)(*it)->m_X[d],m_ElementType,data,i++);  
        }

      MET_DoubleToValue((double)(*it)->m_R,m_ElementType,data,i++);  
      
      for(d = 0; d < m_NDims; d++)
        {
        MET_DoubleToValue((double)(*it)->m_V1[d],m_ElementType,data,i++);  
        }
    
      if(m_NDims==3)
        {
        for(d = 0; d < m_NDims; d++)
          {
          MET_DoubleToValue((double)(*it)->m_V2[d],m_ElementType,data,i++);  
          }
        }
      
      for(d = 0; d < m_NDims; d++)
        {
        MET_DoubleToValue((double)(*it)->m_T[d],m_ElementType,data,i++);  
        }
     
      for(d=0; d<4; d++)
        {
        MET_DoubleToValue((double)(*it)->m_Color[d],m_ElementType,data,i++);
        }

      MET_DoubleToValue((double)(*it)->m_ID,m_ElementType,data,i++);

      it++;
      }

    m_WriteStream->write((char *)data,
                         (m_NDims*(2+m_NDims)+10)*m_NPoints*elementSize);
    m_WriteStream->write("\n",1);
    delete [] data;
    }
  else
    {
    PointListType::const_iterator it = m_PointList.begin();
  
    int d;
    while(it != m_PointList.end())
      {
      for(d = 0; d < m_NDims; d++)
        {
        *m_WriteStream << (*it)->m_X[d] << " ";
        }
      
      *m_WriteStream << (*it)->m_R << " ";

      for(d = 0; d < m_NDims; d++)
        {
        *m_WriteStream << (*it)->m_V1[d] << " ";
        }
   
      if(m_NDims>=3)
        {
        for(d = 0; d < m_NDims; d++)
          {
          *m_WriteStream << (*it)->m_V2[d] << " ";
          }
        }
      
      for(d = 0; d < m_NDims; d++)
        {
        *m_WriteStream << (*it)->m_T[d] << " ";
        }

      for(d=0;d<4;d++)
        {
        *m_WriteStream << (*it)->m_Color[d] << " ";
        }

      *m_WriteStream << (*it)->m_ID << " ";

      *m_WriteStream << std::endl;
      it++;
      }
    }
  return true;
}

