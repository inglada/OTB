/*=========================================================================

Program:   ORFEO Toolbox
Language:  C++
Date:      $Date$
Version:   $Revision$


Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
See OTBCopyright.txt for details.


This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbSHPVectorDataIO_txx
#define __otbSHPVectorDataIO_txx

#include "otbSHPVectorDataIO.h"

#include <itksys/SystemTools.hxx>
#include "itkExceptionObject.h"
#include "itkByteSwapper.h"
#include "otbMacro.h"
#include "otbSystem.h"
#include "otbDataNode.h"
#include "itkPreOrderTreeIterator.h"
#include "otbMetaDataKey.h"
#include "otbFileName.h"
#include "itkTimeProbe.h"
#include "otbVectorDataKeywordlist.h"

namespace otb
{
template<class TData>
SHPVectorDataIO<TData>
::SHPVectorDataIO():
  m_DataSource(NULL),
  m_Kept(0)
{
  // OGR factory registration
  OGRRegisterAll();
}

template<class TData>
SHPVectorDataIO<TData>::~SHPVectorDataIO()
{
  if (m_DataSource != NULL)
    {
    OGRDataSource::DestroyDataSource(m_DataSource);
    }
}

template<class TData>
bool
SHPVectorDataIO<TData>::CanReadFile( const char* filename )
{
  OGRDataSource * poDS = OGRSFDriverRegistrar::Open(filename, FALSE);
  if (poDS == NULL)
    {
    return false;
    }

  OGRDataSource::DestroyDataSource(poDS);
  return true;
}


// Used to print information about this object
template<class TData>
void
SHPVectorDataIO<TData>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

// Read vector data
template<class TData>
void
SHPVectorDataIO<TData>
::Read(VectorDataPointerType data)
{
  // Destroy previous opened data source
  if (m_DataSource != NULL)
  {
    OGRDataSource::DestroyDataSource(m_DataSource);
  }

  m_DataSource = OGRSFDriverRegistrar::Open(this->m_FileName.c_str(), FALSE);

  if (m_DataSource == NULL)
  {
    itkExceptionMacro(<<"Failed to open data file "<<this->m_FileName);
  }

  otbMsgDebugMacro( <<"Driver to read: OGR");
  otbMsgDebugMacro( <<"Reading  file: "<< this->m_FileName);

  // Reading layers
  otbMsgDevMacro(<<"Number of layers: "<<m_DataSource->GetLayerCount());

  // Retrieving root node
  DataTreePointerType tree = data->GetDataTree();
  DataNodePointerType root = tree->GetRoot()->Get();

  OGRSpatialReference * oSRS = NULL;
  //We take the assumption that the spatial reference is common to all layers
  oSRS = m_DataSource->GetLayer(0)->GetSpatialRef();

  if (oSRS != NULL)
  {
    char * projectionRefChar;
    oSRS->exportToWkt(&projectionRefChar);
    std::string projectionRef = projectionRefChar;
    OGRFree(projectionRefChar);
    itk::MetaDataDictionary & dict = data->GetMetaDataDictionary();
    itk::EncapsulateMetaData<std::string>(dict, MetaDataKey::ProjectionRefKey, projectionRef );
  }
  else
  {
    otbMsgDevMacro(<< "Can't retrieve the OGRSpatialReference from the shapefile");
  }



  std::string projectionRefWkt = data->GetProjectionRef();


  bool projectionInformationAvailable = !projectionRefWkt.empty();

  if (projectionInformationAvailable)
  {
    otbMsgDevMacro(<< "Projection information : " << projectionRefWkt);
  }
  else
  {
    otbMsgDevMacro(<< "Projection information unavailable: assuming WGS84");
  }

  // For each layer
  for (int layerIndex = 0; layerIndex < m_DataSource->GetLayerCount(); ++layerIndex)
  {
    /** retrieving layer and property */
    OGRLayer * layer = m_DataSource->GetLayer(layerIndex);
    otbMsgDevMacro(<<"Number of features: " << layer->GetFeatureCount());

    OGRFeatureDefn * dfn = layer->GetLayerDefn();

    /** Create the document node */
    DataNodePointerType document = DataNodeType::New();
    document->SetNodeType(DOCUMENT);
    document->SetNodeId(dfn->GetName());

    /** Retrieving the fields types */
//     OGRFieldDefn * field;
//     for (int fieldIndex = 0; fieldIndex<dfn->GetFieldCount();++fieldIndex)
//     {
//       field  = dfn->GetFieldDefn(fieldIndex);
//       document->SetField(field->GetNameRef(),OGRFieldDefn::GetFieldTypeName(field->GetType()));
//       // std::cout<<"Document "<<document->GetNodeId()<<": Adding field "<<field->GetNameRef()<<" "<<OGRFieldDefn::GetFieldTypeName(field->GetType())<<std::endl;
//     }

    /** Adding the layer to the data tree */
    tree->Add(document,root);

    /// This is not good but we do not have the choice if we want to
    /// get a hook on the internal structure
    InternalTreeNodeType * documentPtr = const_cast<InternalTreeNodeType *>(tree->GetNode(document));

    /** Temporary pointer to store the feature */
    OGRFeature * feature;

    layer->ResetReading();

    unsigned int counter = 0;
    itk::TimeProbe chrono;

    while ((feature = layer->GetNextFeature())!=NULL)
    {
      chrono.Start();

      // Creating a new folder
//       DataNodePointerType folder = DataNodeType::New();
//       folder->SetNodeType(FOLDER);
//       folder->SetNodeId(feature->GetDefnRef()->GetName());

      // Reading fields
//       for (int fieldIndex = 0; fieldIndex<dfn->GetFieldCount();++fieldIndex)
//       {
//         OGRFieldDefn * field  = dfn->GetFieldDefn(fieldIndex);
//         folder->SetField(field->GetNameRef(),feature->GetFieldAsString(fieldIndex));
//         //  std::cout<<"Folder "<<folder->GetNodeId()<<": Adding field "<<field->GetNameRef()<<" "<<feature->GetFieldAsString(fieldIndex)<<std::endl;
//       }


      // Add the folder to the document
//       typename InternalTreeNodeType::Pointer folderPtr = InternalTreeNodeType::New();
//       folderPtr->Set(folder);
//       documentPtr->AddChild(folderPtr);
      typename InternalTreeNodeType::Pointer folderPtr = documentPtr;//TODO: clean references to folder and replace by documentPtr

      // A pointer to the current multi-geometry
      typename InternalTreeNodeType::Pointer multiPtr;

      /** Temporary geometry container */
      OGRGeometry * geometry = feature->GetGeometryRef();

      if (geometry != NULL)
      {
        otb::VectorDataKeywordlist kwl;
        for (int fieldNum=0; fieldNum< feature->GetFieldCount(); ++fieldNum)
        {
          if (feature->IsFieldSet(fieldNum))
          {
            kwl.AddField(feature->GetFieldDefnRef(fieldNum), feature->GetRawFieldRef(fieldNum));
          }
        }


        switch (geometry->getGeometryType())
        {
          case wkbPoint:
          {
            typename InternalTreeNodeType::Pointer newNode = InternalTreeNodeType::New();
            newNode->Set(ConvertGeometryToPointNode(geometry));
            //Reach the DataNode inside the tree node
            itk::MetaDataDictionary& dict = newNode->Get()->GetMetaDataDictionary();
            itk::EncapsulateMetaData< VectorDataKeywordlist >(dict,
                MetaDataKey::VectorDataKeywordlistKey,
                kwl);
            folderPtr->AddChild(newNode);
            break;
          }
          case wkbPoint25D:
          {
            typename InternalTreeNodeType::Pointer newNode = InternalTreeNodeType::New();
            newNode->Set(ConvertGeometryToPointNode(geometry));
            //Reach the DataNode inside the tree node
            itk::MetaDataDictionary& dict = newNode->Get()->GetMetaDataDictionary();
            itk::EncapsulateMetaData< VectorDataKeywordlist >(dict,
                MetaDataKey::VectorDataKeywordlistKey,
                kwl);
            folderPtr->AddChild(newNode);
            break;
          }
          case wkbLineString:
          {
            typename InternalTreeNodeType::Pointer newNode = InternalTreeNodeType::New();
            newNode->Set(ConvertGeometryToLineNode(geometry));
            //Reach the DataNode inside the tree node
            itk::MetaDataDictionary& dict = newNode->Get()->GetMetaDataDictionary();
            itk::EncapsulateMetaData< VectorDataKeywordlist >(dict,
                MetaDataKey::VectorDataKeywordlistKey,
                kwl);
            folderPtr->AddChild(newNode);
            break;
          }
          case wkbLineString25D:
          {
            typename InternalTreeNodeType::Pointer newNode = InternalTreeNodeType::New();
            newNode->Set(ConvertGeometryToLineNode(geometry));
            //Reach the DataNode inside the tree node
            itk::MetaDataDictionary& dict = newNode->Get()->GetMetaDataDictionary();
            itk::EncapsulateMetaData< VectorDataKeywordlist >(dict,
                MetaDataKey::VectorDataKeywordlistKey,
                kwl);
            folderPtr->AddChild(newNode);
            break;
          }
          case wkbPolygon:
          {
            typename InternalTreeNodeType::Pointer newNode = InternalTreeNodeType::New();
            newNode->Set(ConvertGeometryToPolygonNode(geometry));
            //Reach the DataNode inside the tree node
            itk::MetaDataDictionary& dict = newNode->Get()->GetMetaDataDictionary();
            itk::EncapsulateMetaData< VectorDataKeywordlist >(dict,
                MetaDataKey::VectorDataKeywordlistKey,
                kwl);
            folderPtr->AddChild(newNode);
            break;
          }
          case wkbPolygon25D:
          {
            typename InternalTreeNodeType::Pointer newNode = InternalTreeNodeType::New();
            newNode->Set(ConvertGeometryToPolygonNode(geometry));
            //Reach the DataNode inside the tree node
            itk::MetaDataDictionary& dict = newNode->Get()->GetMetaDataDictionary();
            itk::EncapsulateMetaData< VectorDataKeywordlist >(dict,
                MetaDataKey::VectorDataKeywordlistKey,
                kwl);
            folderPtr->AddChild(newNode);
            break;
          }
          case wkbMultiPoint:
          {

            DataNodePointerType multi = DataNodeType::New();
            multi->SetNodeType(FEATURE_MULTIPOINT);
            multiPtr = InternalTreeNodeType::New();
            multiPtr->Set(multi);
            folderPtr->AddChild(multiPtr);

            OGRMultiPoint * ogrMulti = (OGRMultiPoint *) geometry;

            for (int geoIndex = 0;geoIndex<ogrMulti->getNumGeometries();++geoIndex)
            {
              typename InternalTreeNodeType::Pointer newNode = InternalTreeNodeType::New();
              newNode->Set(ConvertGeometryToPointNode(ogrMulti->getGeometryRef(geoIndex)));
              multiPtr->AddChild(newNode);
            }
            break;
          }
          case wkbMultiPoint25D:
          {
            DataNodePointerType multi = DataNodeType::New();
            multi->SetNodeType(FEATURE_MULTIPOINT);

            multiPtr = InternalTreeNodeType::New();
            multiPtr->Set(multi);
            folderPtr->AddChild(multiPtr);

            OGRMultiPoint * ogrMulti = (OGRMultiPoint *) geometry;

            for (int geoIndex = 0;geoIndex<ogrMulti->getNumGeometries();++geoIndex)
            {
              typename InternalTreeNodeType::Pointer newNode = InternalTreeNodeType::New();
              newNode->Set(ConvertGeometryToPointNode(ogrMulti->getGeometryRef(geoIndex)));
              multiPtr->AddChild(newNode);
            }
            break;
          }
          case wkbMultiLineString:
          {
            DataNodePointerType multi = DataNodeType::New();
            multi->SetNodeType(FEATURE_MULTILINE);

            multiPtr = InternalTreeNodeType::New();
            multiPtr->Set(multi);
            folderPtr->AddChild(multiPtr);

            OGRMultiLineString * ogrMulti = (OGRMultiLineString *) geometry;

            for (int geoIndex = 0;geoIndex<ogrMulti->getNumGeometries();++geoIndex)
            {
              typename InternalTreeNodeType::Pointer newNode = InternalTreeNodeType::New();
              newNode->Set(ConvertGeometryToLineNode(ogrMulti->getGeometryRef(geoIndex)));
              multiPtr->AddChild(newNode);
            }
            break;
          }
          case wkbMultiLineString25D:
          {
            DataNodePointerType multi = DataNodeType::New();
            multi->SetNodeType(FEATURE_MULTILINE);


            multiPtr = InternalTreeNodeType::New();
            multiPtr->Set(multi);
            folderPtr->AddChild(multiPtr);

            OGRMultiLineString * ogrMulti = (OGRMultiLineString *) geometry;

            for (int geoIndex = 0;geoIndex<ogrMulti->getNumGeometries();++geoIndex)
            {
              typename InternalTreeNodeType::Pointer newNode = InternalTreeNodeType::New();
              newNode->Set(ConvertGeometryToLineNode(ogrMulti->getGeometryRef(geoIndex)));
              multiPtr->AddChild(newNode);
            }
            break;
          }
          case wkbMultiPolygon:
          {
            DataNodePointerType multi = DataNodeType::New();
            multi->SetNodeType(FEATURE_MULTIPOLYGON);

            multiPtr = InternalTreeNodeType::New();
            multiPtr->Set(multi);
            folderPtr->AddChild(multiPtr);

            OGRMultiPolygon * ogrMulti = (OGRMultiPolygon *) geometry;

            for (int geoIndex = 0;geoIndex<ogrMulti->getNumGeometries();++geoIndex)
            {
              typename InternalTreeNodeType::Pointer newNode = InternalTreeNodeType::New();
              newNode->Set(ConvertGeometryToPolygonNode(ogrMulti->getGeometryRef(geoIndex)));
              multiPtr->AddChild(newNode);
            }
            break;
          }
          case wkbMultiPolygon25D:
          {
            DataNodePointerType multi = DataNodeType::New();
            multi->SetNodeType(FEATURE_MULTIPOLYGON);

            multiPtr = InternalTreeNodeType::New();
            multiPtr->Set(multi);
            folderPtr->AddChild(multiPtr);

            OGRMultiPolygon * ogrMulti = (OGRMultiPolygon *) geometry;

            for (int geoIndex = 0;geoIndex<ogrMulti->getNumGeometries();++geoIndex)
            {
              typename InternalTreeNodeType::Pointer newNode = InternalTreeNodeType::New();
              newNode->Set(ConvertGeometryToPolygonNode(ogrMulti->getGeometryRef(geoIndex)));
              multiPtr->AddChild(newNode);
            }
            break;
          }
          case wkbGeometryCollection:
          {
            DataNodePointerType multi = DataNodeType::New();
            multi->SetNodeType(FEATURE_COLLECTION);

            multiPtr = InternalTreeNodeType::New();
            multiPtr->Set(multi);
            folderPtr->AddChild(multiPtr);

            OGRGeometryCollection * ogrMulti = (OGRGeometryCollection *) geometry;

            for (int geoIndex = 0;geoIndex<ogrMulti->getNumGeometries();++geoIndex)
            {
              switch (ogrMulti->getGeometryRef(geoIndex)->getGeometryType())
              {
                case wkbPoint:
                {
                  typename InternalTreeNodeType::Pointer newNode = InternalTreeNodeType::New();
                  newNode->Set(ConvertGeometryToPointNode(ogrMulti->getGeometryRef(geoIndex)));
                  multiPtr->AddChild(newNode);
                  break;
                }
                case wkbPoint25D:
                {
                  typename InternalTreeNodeType::Pointer newNode = InternalTreeNodeType::New();
                  newNode->Set(ConvertGeometryToPointNode(ogrMulti->getGeometryRef(geoIndex)));
                  multiPtr->AddChild(newNode);
                  break;
                }
                case wkbLineString:
                {
                  typename InternalTreeNodeType::Pointer newNode = InternalTreeNodeType::New();
                  newNode->Set(ConvertGeometryToLineNode(ogrMulti->getGeometryRef(geoIndex)));
                  multiPtr->AddChild(newNode);
                  break;
                }
                case wkbLineString25D:
                {
                  typename InternalTreeNodeType::Pointer newNode = InternalTreeNodeType::New();
                  newNode->Set(ConvertGeometryToLineNode(ogrMulti->getGeometryRef(geoIndex)));
                  multiPtr->AddChild(newNode);
                  break;
                }
                case wkbPolygon:
                {
                  typename InternalTreeNodeType::Pointer newNode = InternalTreeNodeType::New();
                  newNode->Set(ConvertGeometryToPolygonNode(ogrMulti->getGeometryRef(geoIndex)));
                  multiPtr->AddChild(newNode);
                  break;
                }
                case wkbPolygon25D:
                {
                  typename InternalTreeNodeType::Pointer newNode = InternalTreeNodeType::New();
                  newNode->Set(ConvertGeometryToPolygonNode(ogrMulti->getGeometryRef(geoIndex)));
                  multiPtr->AddChild(newNode);
                  break;
                }
                default:
                {
                  std::cout<<"Geometry type not found: "<<ogrMulti->getGeometryRef(geoIndex)->getGeometryType()<<std::endl;
                  break;
                }
              }
            }
            break;
          }
          case wkbGeometryCollection25D:
          {
            DataNodePointerType multi = DataNodeType::New();
            multi->SetNodeType(FEATURE_COLLECTION);

            multiPtr = InternalTreeNodeType::New();
            multiPtr->Set(multi);
            folderPtr->AddChild(multiPtr);

            OGRGeometryCollection * ogrMulti = (OGRGeometryCollection *) geometry;

            for (int geoIndex = 0;geoIndex<ogrMulti->getNumGeometries();++geoIndex)
            {
              switch (ogrMulti->getGeometryRef(geoIndex)->getGeometryType())
              {
                case wkbPoint:
                {
                  typename InternalTreeNodeType::Pointer newNode = InternalTreeNodeType::New();
                  newNode->Set(ConvertGeometryToPointNode(ogrMulti->getGeometryRef(geoIndex)));
                  multiPtr->AddChild(newNode);
                  break;
                }
                case wkbPoint25D:
                {
                  typename InternalTreeNodeType::Pointer newNode = InternalTreeNodeType::New();
                  newNode->Set(ConvertGeometryToPointNode(ogrMulti->getGeometryRef(geoIndex)));
                  multiPtr->AddChild(newNode);
                  break;
                }
                case wkbLineString:
                {
                  typename InternalTreeNodeType::Pointer newNode = InternalTreeNodeType::New();
                  newNode->Set(ConvertGeometryToLineNode(ogrMulti->getGeometryRef(geoIndex)));
                  multiPtr->AddChild(newNode);
                  break;
                }
                case wkbLineString25D:
                {
                  typename InternalTreeNodeType::Pointer newNode = InternalTreeNodeType::New();
                  newNode->Set(ConvertGeometryToLineNode(ogrMulti->getGeometryRef(geoIndex)));
                  multiPtr->AddChild(newNode);
                  break;
                }
                case wkbPolygon:
                {
                  typename InternalTreeNodeType::Pointer newNode = InternalTreeNodeType::New();
                  newNode->Set(ConvertGeometryToPolygonNode(ogrMulti->getGeometryRef(geoIndex)));
                  multiPtr->AddChild(newNode);
                  break;
                }
                case wkbPolygon25D:
                {
                  typename InternalTreeNodeType::Pointer newNode = InternalTreeNodeType::New();
                  newNode->Set(ConvertGeometryToPolygonNode(ogrMulti->getGeometryRef(geoIndex)));
                  multiPtr->AddChild(newNode);
                  break;
                }
                default:
                {
                  std::cout<<"Geometry type not found: "<<ogrMulti->getGeometryRef(geoIndex)->getGeometryType()<<std::endl;
                  break;
                }
              }
            }
            break;
          }
          default:
          {
            std::cout<<"Geometry not handled: "<<geometry->getGeometryName()<<std::endl;
            break;
          }
        }

      }
      OGRFeature::DestroyFeature( feature );
      chrono.Stop();
      ++counter;
    }//end While feature
    otbMsgDevMacro(<<layer->GetFeatureCount()<<" features read, average insertion time "<<chrono.GetMeanTime()<<" s");
  }// end For each layer

  OGRDataSource::DestroyDataSource(m_DataSource);
  m_DataSource = NULL;
}


template<class TData>
typename SHPVectorDataIO<TData>
::DataNodePointerType
SHPVectorDataIO<TData>
::ConvertGeometryToPointNode(const OGRGeometry * ogrGeometry) const
{
  OGRPoint * ogrPoint = (OGRPoint *) ogrGeometry;

  if (ogrPoint == NULL)
    {
    itkGenericExceptionMacro(<<"Failed to convert OGRGeometry to OGRPoint");
    }

  PointType otbPoint;
  otbPoint.Fill(0);
  otbPoint[0] = static_cast<typename DataNodeType::PrecisionType>(ogrPoint->getX());
  otbPoint[1] = static_cast<typename DataNodeType::PrecisionType>(ogrPoint->getY());

  if (DataNodeType::Dimension > 2)
  {
    if (PointType::PointDimension != 3)
    {
      itkGenericExceptionMacro(<<"OTB vector data can't contain the OGR information (2D instead of 2.5D)");
    }
    otbPoint[2]=static_cast<typename DataNodeType::PrecisionType>(ogrPoint->getZ());
  }

  DataNodePointerType node = DataNodeType::New();
  node->SetPoint(otbPoint);
  return node;
}

template<class TData>
typename SHPVectorDataIO<TData>
::DataNodePointerType
SHPVectorDataIO<TData>
::ConvertGeometryToLineNode(const OGRGeometry * ogrGeometry) const
{
  OGRLineString * ogrLine = (OGRLineString *)ogrGeometry;

  if (ogrLine == NULL)
    {
    itkGenericExceptionMacro(<<"Failed to convert OGRGeometry to OGRLine");
    }

  LinePointerType line = LineType::New();

  OGRPoint * ogrTmpPoint = new OGRPoint();

  for (int pIndex = 0;pIndex<ogrLine->getNumPoints();++pIndex)
    {

    ogrLine->getPoint(pIndex,ogrTmpPoint);

    typename LineType::VertexType vertex;

    vertex[0] = ogrTmpPoint->getX();
    vertex[1] = ogrTmpPoint->getY();

    if (DataNodeType::Dimension > 2)
    {
      if (LineType::VertexType::PointDimension != 3)
      {
        itkGenericExceptionMacro(<<"OTB vector data can't contain the OGR information (2D instead of 2.5D)");
      }
      vertex[2]= ogrTmpPoint->getZ();
    }

    line->AddVertex(vertex);
    }
  delete ogrTmpPoint;

  DataNodePointerType node = DataNodeType::New();
  node->SetLine(line);

  return node;
}

template<class TData>
typename SHPVectorDataIO<TData>
::DataNodePointerType
SHPVectorDataIO<TData>
::ConvertGeometryToPolygonNode(const OGRGeometry * ogrGeometry) const
{
  OGRPolygon * ogrPolygon = (OGRPolygon *)ogrGeometry;

  if (ogrPolygon == NULL)
    {
    itkGenericExceptionMacro(<<"Failed to convert OGRGeometry to OGRPolygon");
    }

  OGRPoint * ogrTmpPoint = new OGRPoint();

  OGRLinearRing *  ogrRing = ogrPolygon->getExteriorRing();

  PolygonPointerType extRing = PolygonType::New();

  for (int pIndex = 0;pIndex<ogrRing->getNumPoints();++pIndex)
  {
    ogrRing->getPoint(pIndex,ogrTmpPoint);
    typename PolygonType::VertexType vertex;
    vertex[0] = ogrTmpPoint->getX();
    vertex[1] = ogrTmpPoint->getY();

    if (DataNodeType::Dimension > 2)
    {
      if (PolygonType::VertexType::PointDimension != 3)
      {
        itkGenericExceptionMacro(<<"OTB vector data can't contain the OGR information (2D instead of 2.5D)");
      }
      vertex[2]= ogrTmpPoint->getZ();
    }

    extRing->AddVertex(vertex);
  }

  PolygonListPointerType intRings = PolygonListType::New();

  for (int intRingIndex = 0;intRingIndex<ogrPolygon->getNumInteriorRings();++intRingIndex)
  {
    PolygonPointerType ring = PolygonType::New();
    ogrRing = ogrPolygon->getInteriorRing(intRingIndex);
    for (int pIndex = 0;pIndex<ogrRing->getNumPoints();++pIndex)
    {
      ogrRing->getPoint(pIndex,ogrTmpPoint);
      typename PolygonType::VertexType vertex;

      vertex[0] = ogrTmpPoint->getX();
      vertex[1] = ogrTmpPoint->getY();
      if (DataNodeType::Dimension > 2)
      {
        if (PolygonType::VertexType::PointDimension != 3)
        {
          itkGenericExceptionMacro(<<"OTB vector data can't contain the OGR information (2D instead of 2.5D)");
        }
        vertex[2]= ogrTmpPoint->getZ();
      }
      ring->AddVertex(vertex);
    }
    intRings->PushBack(ring);
  }

  delete ogrTmpPoint;

  DataNodePointerType node = DataNodeType::New();
  node->SetPolygonExteriorRing(extRing);
  node->SetPolygonInteriorRings(intRings);

  return node;
}

template<class TData>
bool SHPVectorDataIO<TData>::CanWriteFile( const char* filename )
{
  std::string lFileName(filename);
  if ( System::IsADirName(lFileName) == true )
    {
    return false;
    }
  const std::string Extension = System::GetExtension(filename);
  if ( (Extension == "shp") || (Extension == "SHP") )
    {
    return true;
    }
  else
    {
    return false;
    }
}

template<class TData>
void SHPVectorDataIO<TData>::Write(const VectorDataConstPointerType data)
{
  itk::TimeProbe chrono;
  chrono.Start();
  //  // try to create an ogr driver
  OGRSFDriver * ogrDriver = OGRSFDriverRegistrar::GetRegistrar()->GetDriverByName("ESRI Shapefile");

  if (ogrDriver == NULL)
    {
    itkExceptionMacro(<<"No OGR driver found to write file "<<this->m_FileName);
    }

  // free an existing previous data source, if any
  if (m_DataSource != NULL)
    {
    OGRDataSource::DestroyDataSource(m_DataSource);
    }

  //if file exist, OGR can't overwrite: remove it  first
  otb::FileName filename(this->m_FileName.c_str());
  if (filename.exists())
    {
    filename.remove();
    }

  // m_DataSource = OGRSFDriverRegistrar::Open(this->m_FileName.c_str(), TRUE);
  m_DataSource = ogrDriver->CreateDataSource(this->m_FileName.c_str(),NULL);


  // check the created data source
  if (m_DataSource == NULL)
    {
    itkExceptionMacro(<<"Failed to create OGR data source for file "<<this->m_FileName<<". Since OGR can not overwrite existing file, be sure that this file does not already exist");
    }

  // Retrieve data required for georeferencing

  std::string projectionRefWkt = data->GetProjectionRef();
  bool projectionInformationAvailable = !projectionRefWkt.empty();

  if (projectionInformationAvailable)
    {
    otbMsgDevMacro(<< "Projection information : " << projectionRefWkt);
    }
  else
    {
    otbMsgDevMacro(<< "Projection information unavailable");
    }

  //TODO georeference here from OGRSpatialReference http://www.gdal.org/ogr/classOGRDataSource.html
  OGRSpatialReference * oSRS =NULL;
  if (projectionInformationAvailable)
    {
      oSRS = new OGRSpatialReference(projectionRefWkt.c_str());
    }

  // Retrieving root node
  DataTreeConstPointerType tree = data->GetDataTree();
  if (tree->GetRoot() == NULL)
    {
    itkExceptionMacro(<<"Data tree is empty: Root == NULL");
    }
  DataNodePointerType root = tree->GetRoot()->Get();

  typedef itk::PreOrderTreeIterator<DataTreeType> TreeIteratorType;

  m_Kept = 0;

  OGRLayer * ogrCurrentLayer = NULL;
//   OGRFeatureVectorType ogrFeatures;
  OGRGeometryCollection * ogrCollection = NULL;
  // OGRGeometry * ogrCurrentGeometry = NULL;

  // Get the input tree root
  InternalTreeNodeType * inputRoot = const_cast<InternalTreeNodeType *>(tree->GetRoot());
  ProcessNodeWrite(inputRoot, ogrCollection, ogrCurrentLayer, oSRS);

  OGRDataSource::DestroyDataSource( m_DataSource );
  m_DataSource = NULL;

  if (oSRS != NULL)
  {
    delete oSRS;
  }

  chrono.Stop();
  std::cout<<"SHPVectorDataIO: file saved in "<<chrono.GetMeanTime()<<" seconds. (" << m_Kept << " elements)"<<std::endl;

  otbMsgDevMacro( <<" SHPVectorDataIO::Write()  ");
}

template<class TData>
    void SHPVectorDataIO<TData>::ProcessNodeWrite(InternalTreeNodeType * source, OGRGeometryCollection * ogrCollection,
    OGRLayer * ogrCurrentLayer, OGRSpatialReference * oSRS)
{
  // Get the children list from the input node
  ChildrenListType children = source->GetChildrenList();
//   std::cout << "Found " <<  children.size() << " children" << std::endl;

  // For each child
  for(typename ChildrenListType::iterator it = children.begin(); it!=children.end(); ++it)
  {
    DataNodePointerType dataNode = (*it)->Get();
//     std::cout << "Processing " << dataNode->GetNodeType() << " with Id: " << dataNode->GetNodeId() << std::endl;
    ++m_Kept;
    switch (dataNode->GetNodeType())
    {
      case ROOT:
      {
        break;
      }
      case DOCUMENT:
      {
//         if (ogrCurrentLayer!=NULL && ogrFeatures.size()>0)
//         {
//           std::vector<OGRFeature*>::iterator fIt = ogrFeatures.begin();
//
//           while (fIt!=ogrFeatures.end())
//           {
//             if (ogrCurrentLayer->CreateFeature(*fIt) != OGRERR_NONE)
//             {
//               itkExceptionMacro(<<"Failed to create ogr feature in file "<<this->m_FileName);
//             }
//             OGRFeature::DestroyFeature(*fIt);
//             ++fIt;
//           }
//         }
//         ogrFeatures.clear();


        ogrCurrentLayer = m_DataSource->CreateLayer(dataNode->GetNodeId(),oSRS,wkbUnknown,NULL);
        if (ogrCurrentLayer == NULL)
        {
          itkExceptionMacro(<<"Failed to create layer "<<dataNode->GetNodeId());
        }
        ProcessNodeWrite(*it, ogrCollection, ogrCurrentLayer, oSRS);
        break;
      }
      case FOLDER:
      {
//         if (ogrCurrentLayer!=NULL && ogrCollection != NULL && !ogrFeatures.empty())
//         {
//           ogrFeatures.back()->SetGeometry(ogrCollection);
//           delete ogrCollection;
//           ogrCollection = NULL;
//         }

//         ogrFeatures.push_back(OGRFeature::CreateFeature(ogrCurrentLayer->GetLayerDefn()));
//         ogrFeatures.back()->SetField("Name",dataNode->GetNodeId());
//         ogrCurrentLayer = m_DataSource->CreateLayer(dataNode->GetNodeId(),oSRS,wkbUnknown,NULL);
//         if (ogrCurrentLayer == NULL)
//         {
//           itkExceptionMacro(<<"Failed to create layer "<<dataNode->GetNodeId());
//         }
        ProcessNodeWrite(*it, ogrCollection, ogrCurrentLayer, oSRS);
        break;
      }
      case FEATURE_POINT:
      {
        //Build the ogrObject
        OGRPoint ogrPoint;
        ogrPoint.setX(dataNode->GetPoint()[0]);
        ogrPoint.setY(dataNode->GetPoint()[1]);


        if (DataNodeType::Dimension>2)
        {
          ogrPoint.setZ(dataNode->GetPoint()[2]);
        }

        //Save it in the structure
        if (ogrCollection == NULL)
        {
          OGRFeature *ogrFeature;

          ogrFeature = OGRFeature::CreateFeature( ogrCurrentLayer->GetLayerDefn() );
          ogrFeature->SetField("Name",dataNode->GetNodeId());
          ogrFeature->SetGeometry( &ogrPoint );

          if( ogrCurrentLayer->CreateFeature( ogrFeature ) != OGRERR_NONE )
          {
            itkExceptionMacro(<<"Failed to create feature in shapefile.");
          }

          OGRFeature::DestroyFeature( ogrFeature );
        }
        else
        {
          ogrCollection->addGeometry(&ogrPoint);
        }

        break;
      }
      case FEATURE_LINE:
      {
        //Build the ogrObject
        OGRLineString ogrLine;
        VertexListConstPointerType vertexList = dataNode->GetLine()->GetVertexList();

        typename VertexListType::ConstIterator vIt = vertexList->Begin();

        while (vIt != vertexList->End())
        {
          OGRPoint ogrPoint;
          ogrPoint.setX(vIt.Value()[0]);
          ogrPoint.setY(vIt.Value()[1]);
          if (DataNodeType::Dimension>2)
          {
            ogrPoint.setZ(vIt.Value()[2]);
          }
          ogrLine.addPoint(&ogrPoint);
          ++vIt;
        }

        //Save it in the structure
        if (ogrCollection == NULL)
        {
          OGRFeature *ogrFeature;

          ogrFeature = OGRFeature::CreateFeature( ogrCurrentLayer->GetLayerDefn() );
          ogrFeature->SetField("Name",dataNode->GetNodeId());
          ogrFeature->SetGeometry( &ogrLine );

          if( ogrCurrentLayer->CreateFeature( ogrFeature ) != OGRERR_NONE )
          {
            itkExceptionMacro(<<"Failed to create feature in shapefile.");
          }

          OGRFeature::DestroyFeature( ogrFeature );

        }
        else
        {
          ogrCollection->addGeometry(&ogrLine);
        }

        break;
      }
      case FEATURE_POLYGON:
      {
        //Build the ogrObject
        OGRPolygon * ogrPolygon = new OGRPolygon();
        OGRLinearRing * ogrExternalRing = new OGRLinearRing();
        VertexListConstPointerType vertexList = dataNode->GetPolygonExteriorRing()->GetVertexList();

        typename VertexListType::ConstIterator vIt = vertexList->Begin();

        while (vIt != vertexList->End())
        {
          OGRPoint ogrPoint;
          ogrPoint.setX(vIt.Value()[0]);
          ogrPoint.setY(vIt.Value()[1]);
          if (DataNodeType::Dimension>2)
          {
            ogrPoint.setZ(vIt.Value()[2]);
          }

          ogrExternalRing->addPoint(&ogrPoint);
          ++vIt;
        }
        ogrPolygon->addRing(ogrExternalRing);
        delete ogrExternalRing;

      // Retrieving internal rings as well
        for (typename PolygonListType::Iterator pIt = dataNode->GetPolygonInteriorRings()->Begin();
             pIt!=dataNode->GetPolygonInteriorRings()->End();++pIt)
        {
          OGRLinearRing * ogrInternalRing = new OGRLinearRing();
          vertexList = pIt.Get()->GetVertexList();
          vIt = vertexList->Begin();

          while (vIt != vertexList->End())
          {
            OGRPoint ogrPoint;
            ogrPoint.setX(vIt.Value()[0]);
            ogrPoint.setY(vIt.Value()[1]);
            if (DataNodeType::Dimension>2)
            {
              ogrPoint.setZ(vIt.Value()[2]);
            }
            ogrInternalRing->addPoint(&ogrPoint);
            ++vIt;
          }
          ogrPolygon->addRing(ogrInternalRing);
          delete ogrInternalRing;
        }

        //Save it in the structure
        if (ogrCollection == NULL)
        {
          OGRFeature *ogrFeature;

          ogrFeature = OGRFeature::CreateFeature( ogrCurrentLayer->GetLayerDefn() );
          ogrFeature->SetField("Name",dataNode->GetNodeId());
          ogrFeature->SetGeometry( ogrPolygon );

          if( ogrCurrentLayer->CreateFeature( ogrFeature ) != OGRERR_NONE )
          {
            itkExceptionMacro(<<"Failed to create feature in shapefile.");
          }

          OGRFeature::DestroyFeature( ogrFeature );

        }
        else
        {
          ogrCollection->addGeometry(ogrPolygon);
        }

        delete ogrPolygon;
        break;
      }
      case FEATURE_MULTIPOINT:
      {
        if (ogrCollection != NULL)
        {
          itkExceptionMacro(<<"Problem while creating multipoint.");
        }
        ogrCollection = new OGRMultiPoint();

        OGRFeature *ogrFeature;

        ogrFeature = OGRFeature::CreateFeature( ogrCurrentLayer->GetLayerDefn() );
        ogrFeature->SetField("Name",dataNode->GetNodeId());
        ogrFeature->GetDefnRef()->SetGeomType(wkbMultiPoint);
        ogrFeature->SetGeometry(ogrCollection  );

        if( ogrCurrentLayer->CreateFeature( ogrFeature ) != OGRERR_NONE )
        {
          itkExceptionMacro(<<"Failed to create feature in shapefile.");
        }

        ProcessNodeWrite(*it, ogrCollection, ogrCurrentLayer, oSRS);
        break;
      }
      case FEATURE_MULTILINE:
      {
        if (ogrCollection != NULL)
        {
          itkExceptionMacro(<<"Problem while creating multiline.");
        }
        ogrCollection = new OGRMultiLineString();

        OGRFeature *ogrFeature;

        ogrFeature = OGRFeature::CreateFeature( ogrCurrentLayer->GetLayerDefn() );
        ogrFeature->SetField("Name",dataNode->GetNodeId());
        ogrFeature->GetDefnRef()->SetGeomType(wkbMultiLineString);
        ogrFeature->SetGeometry(ogrCollection  );

        if( ogrCurrentLayer->CreateFeature( ogrFeature ) != OGRERR_NONE )
        {
          itkExceptionMacro(<<"Failed to create feature in shapefile.");
        }

        ProcessNodeWrite(*it, ogrCollection, ogrCurrentLayer, oSRS);
        break;
      }
      case FEATURE_MULTIPOLYGON:
      {
        if (ogrCollection != NULL)
        {
          itkExceptionMacro(<<"Problem while creating multipolygon.");
        }
        ogrCollection = new OGRMultiPolygon();

        OGRFeature *ogrFeature;

        ogrFeature = OGRFeature::CreateFeature( ogrCurrentLayer->GetLayerDefn() );
        ogrFeature->SetField("Name",dataNode->GetNodeId());
        ogrFeature->GetDefnRef()->SetGeomType(wkbMultiPolygon);
        ogrFeature->SetGeometry(ogrCollection  );

        if( ogrCurrentLayer->CreateFeature( ogrFeature ) != OGRERR_NONE )
        {
          itkExceptionMacro(<<"Failed to create feature in shapefile.");
        }

        ProcessNodeWrite(*it, ogrCollection, ogrCurrentLayer, oSRS);
        break;
      }
      case FEATURE_COLLECTION:
      {
        if (ogrCollection != NULL)
        {
          itkExceptionMacro(<<"Problem while creating collection.");
        }
        ogrCollection = new OGRMultiPoint();//Why is is a multipoint?

        OGRFeature *ogrFeature;

        ogrFeature = OGRFeature::CreateFeature( ogrCurrentLayer->GetLayerDefn() );
        ogrFeature->SetField("Name",dataNode->GetNodeId());
        ogrFeature->GetDefnRef()->SetGeomType(wkbGeometryCollection);
        ogrFeature->SetGeometry(ogrCollection  );

        if( ogrCurrentLayer->CreateFeature( ogrFeature ) != OGRERR_NONE )
        {
          itkExceptionMacro(<<"Failed to create feature in shapefile.");
        }

        ProcessNodeWrite(*it, ogrCollection, ogrCurrentLayer, oSRS);
        break;
      }
    }
  }
}


} // end namespace otb

#endif
