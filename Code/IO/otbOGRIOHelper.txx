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
#ifndef __otbOGRIOHelper_txx
#define __otbOGRIOHelper_txx

#include "itkTimeProbe.h"

#include "otbOGRIOHelper.h"

namespace otb
{

template<class TVectorData>
void
OGRIOHelper<TVectorData>
::ConvertGeometryToPointNode(const OGRGeometry * ogrGeometry, DataNodePointerType node) const
{
  OGRPoint * ogrPoint = (OGRPoint *) ogrGeometry;

  if (ogrPoint == NULL)
    {
    itkGenericExceptionMacro(<< "Failed to convert OGRGeometry to OGRPoint");
    }

  PointType otbPoint;
  otbPoint.Fill(0);
  otbPoint[0] = static_cast<typename DataNodeType::PrecisionType>(ogrPoint->getX());
  otbPoint[1] = static_cast<typename DataNodeType::PrecisionType>(ogrPoint->getY());

  if (DataNodeType::Dimension > 2)
    {
    if (PointType::PointDimension != 3)
      {
      itkGenericExceptionMacro(<< "OTB vector data can't contain the OGR information (2D instead of 2.5D)");
      }
    otbPoint[2] = static_cast<typename DataNodeType::PrecisionType>(ogrPoint->getZ());
    }

  node->SetPoint(otbPoint);
}

template<class TVectorData>
void
OGRIOHelper<TVectorData>
::ConvertGeometryToLineNode(const OGRGeometry * ogrGeometry, DataNodePointerType node) const
{
  OGRLineString * ogrLine = (OGRLineString *) ogrGeometry;

  if (ogrLine == NULL)
    {
    itkGenericExceptionMacro(<< "Failed to convert OGRGeometry to OGRLine");
    }

  LinePointerType line = LineType::New();

  OGRPoint * ogrTmpPoint = (OGRPoint *) OGRGeometryFactory::createGeometry(wkbPoint);

  for (int pIndex = 0; pIndex < ogrLine->getNumPoints(); ++pIndex)
    {

    ogrLine->getPoint(pIndex, ogrTmpPoint);

    typename LineType::VertexType vertex;

    vertex[0] = ogrTmpPoint->getX();
    vertex[1] = ogrTmpPoint->getY();

    if (DataNodeType::Dimension > 2)
      {
      if (LineType::VertexType::PointDimension != 3)
        {
        itkGenericExceptionMacro(<< "OTB vector data can't contain the OGR information (2D instead of 2.5D)");
        }
      vertex[2] = ogrTmpPoint->getZ();
      }

    line->AddVertex(vertex);
    }
  OGRGeometryFactory::destroyGeometry(ogrTmpPoint);

  node->SetLine(line);
}

template<class TVectorData>
inline void
OGRIOHelper<TVectorData>
::ConvertGeometryToPolygonNode(const OGRGeometry * ogrGeometry, DataNodePointerType node) const
{
  OGRPolygon * ogrPolygon = (OGRPolygon *) ogrGeometry;

  if (ogrPolygon == NULL)
    {
    itkGenericExceptionMacro(<< "Failed to convert OGRGeometry to OGRPolygon");
    }

  OGRPoint *      ogrTmpPoint = (OGRPoint *) OGRGeometryFactory::createGeometry(wkbPoint);
  OGRLinearRing * ogrRing = ogrPolygon->getExteriorRing();

  PolygonPointerType extRing = PolygonType::New();

  for (int pIndex = 0; pIndex < ogrRing->getNumPoints(); ++pIndex)
    {
    ogrRing->getPoint(pIndex, ogrTmpPoint);
    typename PolygonType::VertexType vertex;
    vertex[0] = ogrTmpPoint->getX();
    vertex[1] = ogrTmpPoint->getY();

    if (DataNodeType::Dimension > 2)
      {
      if (PolygonType::VertexType::PointDimension != 3)
        {
        itkGenericExceptionMacro(<< "OTB vector data can't contain the OGR information (2D instead of 2.5D)");
        }
      vertex[2] = ogrTmpPoint->getZ();
      }

    extRing->AddVertex(vertex);
    }

  PolygonListPointerType intRings = PolygonListType::New();

  for (int intRingIndex = 0; intRingIndex < ogrPolygon->getNumInteriorRings(); ++intRingIndex)
    {
    PolygonPointerType ring = PolygonType::New();
    ogrRing = ogrPolygon->getInteriorRing(intRingIndex);
    for (int pIndex = 0; pIndex < ogrRing->getNumPoints(); ++pIndex)
      {
      ogrRing->getPoint(pIndex, ogrTmpPoint);
      typename PolygonType::VertexType vertex;

      vertex[0] = ogrTmpPoint->getX();
      vertex[1] = ogrTmpPoint->getY();
      if (DataNodeType::Dimension > 2)
        {
        if (PolygonType::VertexType::PointDimension != 3)
          {
          itkGenericExceptionMacro(<< "OTB vector data can't contain the OGR information (2D instead of 2.5D)");
          }
        vertex[2] = ogrTmpPoint->getZ();
        }
      ring->AddVertex(vertex);
      }
    intRings->PushBack(ring);
    }

  OGRGeometryFactory::destroyGeometry(ogrTmpPoint);

  node->SetPolygonExteriorRing(extRing);
  node->SetPolygonInteriorRings(intRings);
}


template<class TVectorData>
OGRIOHelper<TVectorData>
::OGRIOHelper()
{}

template<class TVectorData>
OGRIOHelper<TVectorData>
::~OGRIOHelper()
{}

template<class TVectorData>
void OGRIOHelper<TVectorData>
::ConvertOGRLayerToDataTreeNode(OGRLayer * layer, InternalTreeNodeType * documentPtr) const
{
  /** Temporary pointer to store the feature */
  OGRFeature * feature;

  layer->ResetReading();

  unsigned int   counter = 0;
  itk::TimeProbe chrono;

  while ((feature = layer->GetNextFeature()) != NULL)
    {
    chrono.Start();

    // A pointer to the current multi-geometry
    typename InternalTreeNodeType::Pointer multiPtr;

    /** Temporary geometry container */
    OGRGeometry * geometry = feature->GetGeometryRef();

    if (geometry == NULL)
      {
      OGRFeature::DestroyFeature(feature);
      chrono.Stop();
      ++counter;
      continue;
      }

    otb::VectorDataKeywordlist kwl;
    for (int fieldNum = 0; fieldNum < feature->GetFieldCount(); ++fieldNum)
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
      DataNodePointerType dataNode = DataNodeType::New();
      ConvertGeometryToPointNode(geometry, dataNode);
      newNode->Set(dataNode);
      //Reach the DataNode inside the tree node
      itk::MetaDataDictionary& dict = newNode->Get()->GetMetaDataDictionary();
      itk::EncapsulateMetaData<VectorDataKeywordlist>(dict,
                                                      MetaDataKey::VectorDataKeywordlistKey,
                                                      kwl);
      documentPtr->AddChild(newNode);
      break;
      }
      case wkbPoint25D:
      {
      typename InternalTreeNodeType::Pointer newNode = InternalTreeNodeType::New();
      DataNodePointerType dataNode = DataNodeType::New();
      ConvertGeometryToPointNode(geometry, dataNode);
      newNode->Set(dataNode);
      //Reach the DataNode inside the tree node
      itk::MetaDataDictionary& dict = newNode->Get()->GetMetaDataDictionary();
      itk::EncapsulateMetaData<VectorDataKeywordlist>(dict,
                                                      MetaDataKey::VectorDataKeywordlistKey,
                                                      kwl);
      documentPtr->AddChild(newNode);
      break;
      }
      case wkbLineString:
      {
      typename InternalTreeNodeType::Pointer newNode = InternalTreeNodeType::New();
      DataNodePointerType dataNode = DataNodeType::New();
      ConvertGeometryToLineNode(geometry, dataNode);
      newNode->Set(dataNode);
      //Reach the DataNode inside the tree node
      itk::MetaDataDictionary& dict = newNode->Get()->GetMetaDataDictionary();
      itk::EncapsulateMetaData<VectorDataKeywordlist>(dict,
                                                      MetaDataKey::VectorDataKeywordlistKey,
                                                      kwl);
      documentPtr->AddChild(newNode);
      break;
      }
      case wkbLineString25D:
      {
      typename InternalTreeNodeType::Pointer newNode = InternalTreeNodeType::New();
      DataNodePointerType dataNode = DataNodeType::New();
      ConvertGeometryToLineNode(geometry, dataNode);
      newNode->Set(dataNode);
      //Reach the DataNode inside the tree node
      itk::MetaDataDictionary& dict = newNode->Get()->GetMetaDataDictionary();
      itk::EncapsulateMetaData<VectorDataKeywordlist>(dict,
                                                      MetaDataKey::VectorDataKeywordlistKey,
                                                      kwl);
      documentPtr->AddChild(newNode);
      break;
      }
      case wkbPolygon:
      {
      typename InternalTreeNodeType::Pointer newNode = InternalTreeNodeType::New();
      DataNodePointerType dataNode = DataNodeType::New();
      ConvertGeometryToPolygonNode(geometry, dataNode);
      newNode->Set(dataNode);
      //Reach the DataNode inside the tree node
      itk::MetaDataDictionary& dict = newNode->Get()->GetMetaDataDictionary();
      itk::EncapsulateMetaData<VectorDataKeywordlist>(dict,
                                                      MetaDataKey::VectorDataKeywordlistKey,
                                                      kwl);
      documentPtr->AddChild(newNode);
      break;
      }
      case wkbPolygon25D:
      {
      typename InternalTreeNodeType::Pointer newNode = InternalTreeNodeType::New();
      DataNodePointerType dataNode = DataNodeType::New();
      ConvertGeometryToPolygonNode(geometry, dataNode);
      newNode->Set(dataNode);
      //Reach the DataNode inside the tree node
      itk::MetaDataDictionary& dict = newNode->Get()->GetMetaDataDictionary();
      itk::EncapsulateMetaData<VectorDataKeywordlist>(dict,
                                                      MetaDataKey::VectorDataKeywordlistKey,
                                                      kwl);
      documentPtr->AddChild(newNode);
      break;
      }
      case wkbMultiPoint:
      {

      DataNodePointerType multi = DataNodeType::New();
      multi->SetNodeType(FEATURE_MULTIPOINT);
      multiPtr = InternalTreeNodeType::New();
      multiPtr->Set(multi);
      documentPtr->AddChild(multiPtr);

      OGRMultiPoint * ogrMulti = (OGRMultiPoint *) geometry;

      for (int geoIndex = 0; geoIndex < ogrMulti->getNumGeometries(); ++geoIndex)
        {
        typename InternalTreeNodeType::Pointer newNode = InternalTreeNodeType::New();
        DataNodePointerType dataNode = DataNodeType::New();
        ConvertGeometryToPointNode(ogrMulti->getGeometryRef(geoIndex), dataNode);
        newNode->Set(dataNode);
        itk::MetaDataDictionary& dict = newNode->Get()->GetMetaDataDictionary();
        itk::EncapsulateMetaData<VectorDataKeywordlist>(dict,
                                                        MetaDataKey::VectorDataKeywordlistKey,
                                                        kwl);
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
      documentPtr->AddChild(multiPtr);

      OGRMultiPoint * ogrMulti = (OGRMultiPoint *) geometry;

      for (int geoIndex = 0; geoIndex < ogrMulti->getNumGeometries(); ++geoIndex)
        {
        typename InternalTreeNodeType::Pointer newNode = InternalTreeNodeType::New();
        DataNodePointerType dataNode = DataNodeType::New();
        ConvertGeometryToPointNode(ogrMulti->getGeometryRef(geoIndex), dataNode);
        newNode->Set(dataNode);
        itk::MetaDataDictionary& dict = newNode->Get()->GetMetaDataDictionary();
        itk::EncapsulateMetaData<VectorDataKeywordlist>(dict,
                                                        MetaDataKey::VectorDataKeywordlistKey,
                                                        kwl);
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
      documentPtr->AddChild(multiPtr);

      OGRMultiLineString * ogrMulti = (OGRMultiLineString *) geometry;

      for (int geoIndex = 0; geoIndex < ogrMulti->getNumGeometries(); ++geoIndex)
        {
        typename InternalTreeNodeType::Pointer newNode = InternalTreeNodeType::New();
        DataNodePointerType dataNode = DataNodeType::New();
        ConvertGeometryToLineNode(ogrMulti->getGeometryRef(geoIndex), dataNode);
        newNode->Set(dataNode);
        itk::MetaDataDictionary& dict = newNode->Get()->GetMetaDataDictionary();
        itk::EncapsulateMetaData<VectorDataKeywordlist>(dict,
                                                        MetaDataKey::VectorDataKeywordlistKey,
                                                        kwl);
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
      documentPtr->AddChild(multiPtr);

      OGRMultiLineString * ogrMulti = (OGRMultiLineString *) geometry;

      for (int geoIndex = 0; geoIndex < ogrMulti->getNumGeometries(); ++geoIndex)
        {
        typename InternalTreeNodeType::Pointer newNode = InternalTreeNodeType::New();
        DataNodePointerType dataNode = DataNodeType::New();
        ConvertGeometryToLineNode(ogrMulti->getGeometryRef(geoIndex), dataNode);
        newNode->Set(dataNode);
        itk::MetaDataDictionary& dict = newNode->Get()->GetMetaDataDictionary();
        itk::EncapsulateMetaData<VectorDataKeywordlist>(dict,
                                                        MetaDataKey::VectorDataKeywordlistKey,
                                                        kwl);
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
      documentPtr->AddChild(multiPtr);

      OGRMultiPolygon * ogrMulti = (OGRMultiPolygon *) geometry;

      for (int geoIndex = 0; geoIndex < ogrMulti->getNumGeometries(); ++geoIndex)
        {
        typename InternalTreeNodeType::Pointer newNode = InternalTreeNodeType::New();
        DataNodePointerType dataNode = DataNodeType::New();
        ConvertGeometryToPolygonNode(ogrMulti->getGeometryRef(geoIndex), dataNode);
        newNode->Set(dataNode);
        itk::MetaDataDictionary& dict = newNode->Get()->GetMetaDataDictionary();
        itk::EncapsulateMetaData<VectorDataKeywordlist>(dict,
                                                        MetaDataKey::VectorDataKeywordlistKey,
                                                        kwl);
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
      documentPtr->AddChild(multiPtr);

      OGRMultiPolygon * ogrMulti = (OGRMultiPolygon *) geometry;

      for (int geoIndex = 0; geoIndex < ogrMulti->getNumGeometries(); ++geoIndex)
        {
        typename InternalTreeNodeType::Pointer newNode = InternalTreeNodeType::New();
        DataNodePointerType dataNode = DataNodeType::New();
        ConvertGeometryToPolygonNode(ogrMulti->getGeometryRef(geoIndex), dataNode);
        newNode->Set(dataNode);
        itk::MetaDataDictionary& dict = newNode->Get()->GetMetaDataDictionary();
        itk::EncapsulateMetaData<VectorDataKeywordlist>(dict,
                                                        MetaDataKey::VectorDataKeywordlistKey,
                                                        kwl);
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
      documentPtr->AddChild(multiPtr);

      OGRGeometryCollection * ogrMulti = (OGRGeometryCollection *) geometry;

      for (int geoIndex = 0; geoIndex < ogrMulti->getNumGeometries(); ++geoIndex)
        {
        switch (ogrMulti->getGeometryRef(geoIndex)->getGeometryType())
          {
          case wkbPoint:
          {
          typename InternalTreeNodeType::Pointer newNode = InternalTreeNodeType::New();
          DataNodePointerType dataNode = DataNodeType::New();
          ConvertGeometryToPointNode(ogrMulti->getGeometryRef(geoIndex), dataNode);
          newNode->Set(dataNode);
          itk::MetaDataDictionary& dict = newNode->Get()->GetMetaDataDictionary();
          itk::EncapsulateMetaData<VectorDataKeywordlist>(dict,
                                                          MetaDataKey::VectorDataKeywordlistKey,
                                                          kwl);
          multiPtr->AddChild(newNode);
          break;
          }
          case wkbPoint25D:
          {
          typename InternalTreeNodeType::Pointer newNode = InternalTreeNodeType::New();
          DataNodePointerType dataNode = DataNodeType::New();
          ConvertGeometryToPointNode(ogrMulti->getGeometryRef(geoIndex), dataNode);
          newNode->Set(dataNode);
          itk::MetaDataDictionary& dict = newNode->Get()->GetMetaDataDictionary();
          itk::EncapsulateMetaData<VectorDataKeywordlist>(dict,
                                                          MetaDataKey::VectorDataKeywordlistKey,
                                                          kwl);
          multiPtr->AddChild(newNode);
          break;
          }
          case wkbLineString:
          {
          typename InternalTreeNodeType::Pointer newNode = InternalTreeNodeType::New();
          DataNodePointerType dataNode = DataNodeType::New();
          ConvertGeometryToLineNode(ogrMulti->getGeometryRef(geoIndex), dataNode);
          newNode->Set(dataNode);
          itk::MetaDataDictionary& dict = newNode->Get()->GetMetaDataDictionary();
          itk::EncapsulateMetaData<VectorDataKeywordlist>(dict,
                                                          MetaDataKey::VectorDataKeywordlistKey,
                                                          kwl);
          multiPtr->AddChild(newNode);
          break;
          }
          case wkbLineString25D:
          {
          typename InternalTreeNodeType::Pointer newNode = InternalTreeNodeType::New();
          DataNodePointerType dataNode = DataNodeType::New();
          ConvertGeometryToLineNode(ogrMulti->getGeometryRef(geoIndex), dataNode);
          newNode->Set(dataNode);
          itk::MetaDataDictionary& dict = newNode->Get()->GetMetaDataDictionary();
          itk::EncapsulateMetaData<VectorDataKeywordlist>(dict,
                                                          MetaDataKey::VectorDataKeywordlistKey,
                                                          kwl);
          multiPtr->AddChild(newNode);
          break;
          }
          case wkbPolygon:
          {
          typename InternalTreeNodeType::Pointer newNode = InternalTreeNodeType::New();
          DataNodePointerType dataNode = DataNodeType::New();
          ConvertGeometryToPolygonNode(ogrMulti->getGeometryRef(geoIndex), dataNode);
          newNode->Set(dataNode);
          itk::MetaDataDictionary& dict = newNode->Get()->GetMetaDataDictionary();
          itk::EncapsulateMetaData<VectorDataKeywordlist>(dict,
                                                          MetaDataKey::VectorDataKeywordlistKey,
                                                          kwl);
          multiPtr->AddChild(newNode);
          break;
          }
          case wkbPolygon25D:
          {
          typename InternalTreeNodeType::Pointer newNode = InternalTreeNodeType::New();
          DataNodePointerType dataNode = DataNodeType::New();
          ConvertGeometryToPolygonNode(ogrMulti->getGeometryRef(geoIndex), dataNode);
          newNode->Set(dataNode);
          itk::MetaDataDictionary& dict = newNode->Get()->GetMetaDataDictionary();
          itk::EncapsulateMetaData<VectorDataKeywordlist>(dict,
                                                          MetaDataKey::VectorDataKeywordlistKey,
                                                          kwl);
          multiPtr->AddChild(newNode);
          break;
          }
          default:
          {
          std::cout << "Geometry type not found: " << ogrMulti->getGeometryRef(geoIndex)->getGeometryType() <<
            std::endl;
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
      documentPtr->AddChild(multiPtr);

      OGRGeometryCollection * ogrMulti = (OGRGeometryCollection *) geometry;

      for (int geoIndex = 0; geoIndex < ogrMulti->getNumGeometries(); ++geoIndex)
        {
        switch (ogrMulti->getGeometryRef(geoIndex)->getGeometryType())
          {
          case wkbPoint:
          {
          typename InternalTreeNodeType::Pointer newNode = InternalTreeNodeType::New();
          DataNodePointerType dataNode = DataNodeType::New();
          ConvertGeometryToPointNode(ogrMulti->getGeometryRef(geoIndex), dataNode);
          multiPtr->AddChild(newNode);
          break;
          }
          case wkbPoint25D:
          {
          typename InternalTreeNodeType::Pointer newNode = InternalTreeNodeType::New();
          DataNodePointerType dataNode = DataNodeType::New();
          ConvertGeometryToPointNode(ogrMulti->getGeometryRef(geoIndex), dataNode);
          multiPtr->AddChild(newNode);
          break;
          }
          case wkbLineString:
          {
          typename InternalTreeNodeType::Pointer newNode = InternalTreeNodeType::New();
          DataNodePointerType dataNode = DataNodeType::New();
          ConvertGeometryToLineNode(ogrMulti->getGeometryRef(geoIndex), dataNode);
          multiPtr->AddChild(newNode);
          break;
          }
          case wkbLineString25D:
          {
          typename InternalTreeNodeType::Pointer newNode = InternalTreeNodeType::New();
          DataNodePointerType dataNode = DataNodeType::New();
          ConvertGeometryToLineNode(ogrMulti->getGeometryRef(geoIndex), dataNode);
          multiPtr->AddChild(newNode);
          break;
          }
          case wkbPolygon:
          {
          typename InternalTreeNodeType::Pointer newNode = InternalTreeNodeType::New();
          DataNodePointerType dataNode = DataNodeType::New();
          ConvertGeometryToPolygonNode(ogrMulti->getGeometryRef(geoIndex), dataNode);
          multiPtr->AddChild(newNode);
          break;
          }
          case wkbPolygon25D:
          {
          typename InternalTreeNodeType::Pointer newNode = InternalTreeNodeType::New();
          DataNodePointerType dataNode = DataNodeType::New();
          ConvertGeometryToPolygonNode(ogrMulti->getGeometryRef(geoIndex), dataNode);
          multiPtr->AddChild(newNode);
          break;
          }
          default:
          {
          std::cout << "Geometry type not found: " << ogrMulti->getGeometryRef(geoIndex)->getGeometryType() <<
            std::endl;
          break;
          }
          }
        }
      break;
      }
      default:
      {
      std::cout << "Geometry not handled: " << geometry->getGeometryName() << std::endl;
      break;
      }
      }

      
    OGRFeature::DestroyFeature(feature);
    chrono.Stop();
    ++counter;
    } //end While feature
  otbMsgDevMacro(
    << layer->GetFeatureCount() << " features read, average insertion time " << chrono.GetMeanTime() << " s");
}

template<class TVectorData>
unsigned int OGRIOHelper<TVectorData>
::ProcessNodeWrite(InternalTreeNodeType * source, OGRDataSource * m_DataSource, OGRGeometryCollection * ogrCollection,
                   OGRLayer * ogrCurrentLayer, OGRSpatialReference * oSRS)
{
  unsigned int kept = 0;
  // Get the children list from the input node
  typedef typename InternalTreeNodeType::ChildrenListType ChildrenListType;
  ChildrenListType children = source->GetChildrenList();

  // For each child
  for (typename ChildrenListType::iterator it = children.begin(); it != children.end(); ++it)
    {
    DataNodePointerType dataNode = (*it)->Get();
    //otbMsgDevMacro(<< "Type of node " << dataNode->GetNodeType() << " id " << dataNode->GetNodeId());
    ++kept;

    // Get the kwl
    otb::VectorDataKeywordlist kwl;
    itk::ExposeMetaData<VectorDataKeywordlist>(dataNode->GetMetaDataDictionary(),
                                               MetaDataKey::VectorDataKeywordlistKey,
                                               kwl);
    
    // Create the field once
    if (ogrCurrentLayer != NULL && ogrCurrentLayer->GetFeatureCount() == 0)
      {
      // Take into account the fields stored in the
      // vectordatakeywordlist
      for (unsigned int fieldIdx  = 0; fieldIdx < kwl.GetNumberOfFields(); fieldIdx++)
        {
        if ( std::string(kwl.GetNthField(fieldIdx).first->GetNameRef()) != "FID" )
          {
          otbMsgDevMacro(<< " CreateField '" << kwl.GetNthField(fieldIdx).first->GetNameRef() << "'");
          if (ogrCurrentLayer->CreateField(kwl.GetNthField(fieldIdx).first) != OGRERR_NONE )
            {
            itkExceptionMacro(<< "Failed to create Field "<<kwl.GetNthField(fieldIdx).first->GetNameRef());
            }
          }
         else
          {
          otbMsgDevMacro(<< "WARNING: Skipping OGR field 'FID'");
          }
        }
      }

    switch (dataNode->GetNodeType())
      {
      case ROOT:
      {
      break;
      }
      case DOCUMENT:
      {
      ogrCurrentLayer = m_DataSource->CreateLayer(dataNode->GetNodeId(), oSRS, wkbUnknown, NULL);
      if (ogrCurrentLayer == NULL)
        {
        //itkExceptionMacro(<<"Failed to create layer "<<dataNode->GetNodeId());
        std::cout << "Failed to create layer " << dataNode->GetNodeId() << std::endl;
        }
      ProcessNodeWrite(*it, m_DataSource, ogrCollection, ogrCurrentLayer, oSRS);
      break;
      }
      case FOLDER:
      {
      ProcessNodeWrite(*it, m_DataSource, ogrCollection, ogrCurrentLayer, oSRS);
      break;
      }
      case FEATURE_POINT:
      {
      //Build the ogrObject
      OGRPoint ogrPoint;
      ogrPoint.setX(dataNode->GetPoint()[0]);
      ogrPoint.setY(dataNode->GetPoint()[1]);

      if (DataNodeType::Dimension > 2)
        {
        ogrPoint.setZ(dataNode->GetPoint()[2]);
        }

      //Save it in the structure
      if (ogrCollection == NULL)
        {
        OGRFeature *ogrFeature;
        ogrFeature = OGRFeature::CreateFeature(ogrCurrentLayer->GetLayerDefn());
    
        // Add the fields to the features
        for (unsigned int i  = 0; i < kwl.GetNumberOfFields(); i++)
          {
          // Get the key of the Nth OGRFieldRefn
          const char * key = kwl.GetNthField(i).first->GetNameRef();
          
          if (std::string(key) != "FID")
            {
            // Edit the value of the field and add it to the current feature
            ogrFeature->SetField(ogrFeature->GetFieldIndex(key) , kwl.GetFieldAsString(key).c_str());
            }
          }
    
//        ogrFeature->SetField("Name", dataNode->GetNodeId());
        ogrFeature->SetGeometry(&ogrPoint);

        if (ogrCurrentLayer->CreateFeature(ogrFeature) != OGRERR_NONE)
          {
          itkExceptionMacro(<< "Failed to create feature in shapefile.");
          //std::cout << "Failed to create feature in shapefile."
          }

        OGRFeature::DestroyFeature(ogrFeature);
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
      OGRLineString              ogrLine;
      VertexListConstPointerType vertexList = dataNode->GetLine()->GetVertexList();

      typename VertexListType::ConstIterator vIt = vertexList->Begin();

      while (vIt != vertexList->End())
        {
        OGRPoint ogrPoint;
        ogrPoint.setX(vIt.Value()[0]);
        ogrPoint.setY(vIt.Value()[1]);
        if (DataNodeType::Dimension > 2)
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
        ogrFeature = OGRFeature::CreateFeature(ogrCurrentLayer->GetLayerDefn());

        // Add the fields to the features
        for (unsigned int i  = 0; i < kwl.GetNumberOfFields(); i++)
          {
          // Get the key of the Nth OGRFieldRefn
          const char * key = kwl.GetNthField(i).first->GetNameRef();
          // Edit the value of the field and add it to the current feature
          ogrFeature->SetField(ogrFeature->GetFieldIndex(key) , kwl.GetFieldAsString(key).c_str());
          }
    
//        ogrFeature->SetField("Name", dataNode->GetNodeId());
        ogrFeature->SetGeometry(&ogrLine);

        if (ogrCurrentLayer->CreateFeature(ogrFeature) != OGRERR_NONE)
          {
          itkExceptionMacro(<< "Failed to create feature in shapefile.");
          }

        OGRFeature::DestroyFeature(ogrFeature);

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
      OGRPolygon *               ogrPolygon = (OGRPolygon *) OGRGeometryFactory::createGeometry(wkbPolygon);
      OGRLinearRing *            ogrExternalRing = (OGRLinearRing *) OGRGeometryFactory::createGeometry(wkbLinearRing);
      VertexListConstPointerType vertexList = dataNode->GetPolygonExteriorRing()->GetVertexList();

      typename VertexListType::ConstIterator vIt = vertexList->Begin();

      while (vIt != vertexList->End())
        {
        OGRPoint ogrPoint;
        ogrPoint.setX(vIt.Value()[0]);
        ogrPoint.setY(vIt.Value()[1]);
        if (DataNodeType::Dimension > 2)
          {
          ogrPoint.setZ(vIt.Value()[2]);
          }

        ogrExternalRing->addPoint(&ogrPoint);
        ++vIt;
        }
      ogrPolygon->addRing(ogrExternalRing);
      // Close the polygon
      ogrPolygon->closeRings();
      OGRGeometryFactory::destroyGeometry(ogrExternalRing);

      // Retrieving internal rings as well
      for (typename PolygonListType::Iterator pIt = dataNode->GetPolygonInteriorRings()->Begin();
           pIt != dataNode->GetPolygonInteriorRings()->End(); ++pIt)
        {
        OGRLinearRing * ogrInternalRing = (OGRLinearRing *) OGRGeometryFactory::createGeometry(wkbLinearRing);
        vertexList = pIt.Get()->GetVertexList();
        vIt = vertexList->Begin();

        while (vIt != vertexList->End())
          {
          OGRPoint ogrPoint;
          ogrPoint.setX(vIt.Value()[0]);
          ogrPoint.setY(vIt.Value()[1]);
          if (DataNodeType::Dimension > 2)
            {
            ogrPoint.setZ(vIt.Value()[2]);
            }
          ogrInternalRing->addPoint(&ogrPoint);
          ++vIt;
          }
        ogrPolygon->addRing(ogrInternalRing);
        OGRGeometryFactory::destroyGeometry(ogrInternalRing);
        }

      //Save it in the structure
      if (ogrCollection == NULL)
        {
        OGRFeature *ogrFeature;
        ogrFeature = OGRFeature::CreateFeature(ogrCurrentLayer->GetLayerDefn());
    
        // Add the fields to the features
        for (unsigned int i  = 0; i < kwl.GetNumberOfFields(); i++)
          {
          // Get the key of the Nth OGRFieldRefn
          const char * key = kwl.GetNthField(i).first->GetNameRef();
          // Edit the value of the field and add it to the current feature
          ogrFeature->SetField(ogrFeature->GetFieldIndex(key) , kwl.GetFieldAsString(key).c_str());
          }
    
        ogrFeature->SetGeometry(ogrPolygon);
    
        if (ogrCurrentLayer->CreateFeature(ogrFeature) != OGRERR_NONE)
          {
          itkExceptionMacro(<< "Failed to create feature in shapefile.");
          }
    
        OGRFeature::DestroyFeature(ogrFeature);
        }
      else
        {
        ogrCollection->addGeometry(ogrPolygon);
        }

      OGRGeometryFactory::destroyGeometry(ogrPolygon);
      break;
      }
      case FEATURE_MULTIPOINT:
      {
      if (ogrCollection != NULL)
        {
        itkExceptionMacro(<< "Problem while creating multipoint.");
        }

      OGRMultiPoint* ogrMultiPoint = (OGRMultiPoint*) OGRGeometryFactory::createGeometry(wkbMultiPoint);
      OGRFeature *   ogrFeature;

      ogrFeature = OGRFeature::CreateFeature(ogrCurrentLayer->GetLayerDefn());
//      ogrFeature->SetField("Name", dataNode->GetNodeId());
      ogrFeature->GetDefnRef()->SetGeomType(wkbMultiPoint);
      ogrFeature->SetGeometry(ogrMultiPoint);

      if (ogrCurrentLayer->CreateFeature(ogrFeature) != OGRERR_NONE)
        {
        itkExceptionMacro(<< "Failed to create feature in shapefile.");
        }

      ProcessNodeWrite(*it, m_DataSource, ogrCollection, ogrCurrentLayer, oSRS);
      break;
      }
      case FEATURE_MULTILINE:
      {
      if (ogrCollection != NULL)
        {
        itkExceptionMacro(<< "Problem while creating multiline.");
        }

      // Instanciate a new  ogrMultiLineString feature
      OGRMultiLineString* ogrMultiLineString = (OGRMultiLineString*) OGRGeometryFactory::createGeometry(
        wkbMultiLineString);

      OGRFeature *ogrFeature;

      ogrFeature = OGRFeature::CreateFeature(ogrCurrentLayer->GetLayerDefn());
//      ogrFeature->SetField("Name", dataNode->GetNodeId());
      ogrFeature->GetDefnRef()->SetGeomType(wkbMultiLineString);
      ogrFeature->SetGeometry(ogrMultiLineString);

      if (ogrCurrentLayer->CreateFeature(ogrFeature) != OGRERR_NONE)
        {
        itkExceptionMacro(<< "Failed to create feature in shapefile.");
        }
      ProcessNodeWrite(*it, m_DataSource, ogrCollection, ogrCurrentLayer, oSRS);
      break;
      }
      case FEATURE_MULTIPOLYGON:
      {
      if (ogrCollection != NULL)
        {
        itkExceptionMacro(<< "Problem while creating multipolygon.");
        }

      // Instanciate a new multipolygon feature
      OGRMultiPolygon* ogrMultiPolygon = (OGRMultiPolygon*) OGRGeometryFactory::createGeometry(wkbMultiPolygon);
      OGRFeature *     ogrFeature;

      ogrFeature = OGRFeature::CreateFeature(ogrCurrentLayer->GetLayerDefn());
//      ogrFeature->SetField("Name", dataNode->GetNodeId());
      ogrFeature->GetDefnRef()->SetGeomType(wkbMultiPolygon);
      ogrFeature->SetGeometry(ogrMultiPolygon);

      if (ogrCurrentLayer->CreateFeature(ogrFeature) != OGRERR_NONE)
        {
        itkExceptionMacro(<< "Failed to create feature in shapefile.");
        }
      ProcessNodeWrite(*it, m_DataSource, ogrCollection, ogrCurrentLayer, oSRS);
      break;
      }
      case FEATURE_COLLECTION:
      {
      if (ogrCollection != NULL)
        {
        itkExceptionMacro(<< "Problem while creating collection.");
        }

      OGRGeometryCollection* ogrCollectionGeometry = (OGRGeometryCollection*) OGRGeometryFactory::createGeometry(
        wkbGeometryCollection);

      OGRFeature *ogrFeature;

      ogrFeature = OGRFeature::CreateFeature(ogrCurrentLayer->GetLayerDefn());
//      ogrFeature->SetField("Name", dataNode->GetNodeId());
      ogrFeature->GetDefnRef()->SetGeomType(wkbGeometryCollection);
      ogrFeature->SetGeometry(ogrCollectionGeometry);

      if (ogrCurrentLayer->CreateFeature(ogrFeature) != OGRERR_NONE)
        {
        itkExceptionMacro(<< "Failed to create feature in shapefile.");
        }

      ProcessNodeWrite(*it, m_DataSource, ogrCollection, ogrCurrentLayer, oSRS);
      break;
      }
      }
    }

  return kept;
}
} // end namespace otb

#endif
