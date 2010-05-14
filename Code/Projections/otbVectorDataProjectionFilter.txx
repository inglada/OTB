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
#ifndef __otbVectorDataProjectionFilter_txx
#define __otbVectorDataProjectionFilter_txx

#include "otbVectorDataProjectionFilter.h"
#include "itkProgressReporter.h"
#include "itkMetaDataObject.h"
#include "otbMetaDataKey.h"
#include "otbDataNode.h"
#include "itkTimeProbe.h"

namespace otb
{
/**
   * Constructor
 */
template <class TInputVectorData, class TOutputVectorData>
VectorDataProjectionFilter<TInputVectorData, TOutputVectorData>
::VectorDataProjectionFilter() : m_DEMDirectory(""), m_AverageElevation(-32768.0)
{
  m_InputProjectionRef.clear();
  m_OutputProjectionRef.clear();
  m_InputKeywordList.Clear();
  m_OutputKeywordList.Clear();
  m_InputSpacing.Fill(1);
  m_InputOrigin.Fill(0);
  m_OutputSpacing.Fill(1);
  m_OutputOrigin.Fill(0);
}

//----------------------------------------------------------------------------
template <class TInputVectorData, class TOutputVectorData>
void
VectorDataProjectionFilter<TInputVectorData, TOutputVectorData>
::SetInputSpacing(const SpacingType& spacing)
{
  itkDebugMacro("setting Spacing to " << spacing);
  if (this->m_InputSpacing != spacing)
    {
    this->m_InputSpacing = spacing;
    this->Modified();
    }
}

//----------------------------------------------------------------------------
template <class TInputVectorData, class TOutputVectorData>
void
VectorDataProjectionFilter<TInputVectorData, TOutputVectorData>
::SetInputSpacing(const double spacing[2])
{
  SpacingType s(spacing);
  this->SetInputSpacing(s);
}

//----------------------------------------------------------------------------
template <class TInputVectorData, class TOutputVectorData>
void
VectorDataProjectionFilter<TInputVectorData, TOutputVectorData>
::SetInputSpacing(const float spacing[2])
{
  itk::Vector<float, 2> sf(spacing);
  SpacingType s;
  s.CastFrom(sf);
  this->SetInputSpacing(s);
}

//----------------------------------------------------------------------------
template <class TInputVectorData, class TOutputVectorData>
void
VectorDataProjectionFilter<TInputVectorData, TOutputVectorData>
::SetInputOrigin(const double origin[2])
{
  OriginType p(origin);
  this->SetInputOrigin(p);
}

//----------------------------------------------------------------------------
template <class TInputVectorData, class TOutputVectorData>
void
VectorDataProjectionFilter<TInputVectorData, TOutputVectorData>
::SetInputOrigin(const float origin[2])
{
  itk::Point<float, 2> of(origin);
  OriginType p;
  p.CastFrom(of);
  this->SetInputOrigin(p);
}

//----------------------------------------------------------------------------
template <class TInputVectorData, class TOutputVectorData>
void
VectorDataProjectionFilter<TInputVectorData, TOutputVectorData>
::SetOutputSpacing(const SpacingType& spacing)
{
  itkDebugMacro("setting Spacing to " << spacing);
  if (this->m_OutputSpacing != spacing)
    {
    this->m_OutputSpacing = spacing;
    this->Modified();
    }
}

//----------------------------------------------------------------------------
template <class TInputVectorData, class TOutputVectorData>
void
VectorDataProjectionFilter<TInputVectorData, TOutputVectorData>
::SetOutputSpacing(const double spacing[2])
{
  SpacingType s(spacing);
  this->SetOutputSpacing(s);
}

//----------------------------------------------------------------------------
template <class TInputVectorData, class TOutputVectorData>
void
VectorDataProjectionFilter<TInputVectorData, TOutputVectorData>
::SetOutputSpacing(const float spacing[2])
{
  itk::Vector<float, 2> sf(spacing);
  SpacingType s;
  s.CastFrom(sf);
  this->SetOutputSpacing(s);
}

//----------------------------------------------------------------------------
template <class TInputVectorData, class TOutputVectorData>
void
VectorDataProjectionFilter<TInputVectorData, TOutputVectorData>
::SetOutputOrigin(const double origin[2])
{
  OriginType p(origin);
  this->SetOutputOrigin(p);
}

//----------------------------------------------------------------------------
template <class TInputVectorData, class TOutputVectorData>
void
VectorDataProjectionFilter<TInputVectorData, TOutputVectorData>
::SetOutputOrigin(const float origin[2])
{
  itk::Point<float, 2> of(origin);
  OriginType p;
  p.CastFrom(of);
  this->SetOutputOrigin(p);
}

template <class TInputVectorData, class TOutputVectorData>
void
VectorDataProjectionFilter<TInputVectorData, TOutputVectorData>
::GenerateOutputInformation(void)
{
  Superclass::GenerateOutputInformation();

  OutputVectorDataPointer  output = this->GetOutput();
  itk::MetaDataDictionary& dict = output->GetMetaDataDictionary();

  itk::EncapsulateMetaData<std::string>(dict, MetaDataKey::ProjectionRefKey, m_OutputProjectionRef);

}

/**
* Convert point
 */
template <class TInputVectorData, class TOutputVectorData>
typename VectorDataProjectionFilter<TInputVectorData, TOutputVectorData>::PointType
VectorDataProjectionFilter<TInputVectorData, TOutputVectorData>
::ReprojectPoint(PointType pointCoord) const
{

  itk::Point<double, 2> point;
  point = m_Transform->TransformPoint(pointCoord);
  return point;
}

/**
 * Convert line
 */
template <class TInputVectorData, class TOutputVectorData>
typename VectorDataProjectionFilter<TInputVectorData, TOutputVectorData>::LinePointerType
VectorDataProjectionFilter<TInputVectorData, TOutputVectorData>
::ReprojectLine(LinePointerType line) const
{
  typedef typename LineType::VertexListType::ConstPointer VertexListConstPointerType;
  typedef typename LineType::VertexListConstIteratorType  VertexListConstIteratorType;
  VertexListConstPointerType  vertexList = line->GetVertexList();
  VertexListConstIteratorType it = vertexList->Begin();
  typename LineType::Pointer  newLine = LineType::New();
  while (it != vertexList->End())
    {
    itk::Point<double, 2>           point;
    itk::ContinuousIndex<double, 2> index;
    typename LineType::VertexType   pointCoord = it.Value();
    point = m_Transform->TransformPoint(pointCoord);
    index[0] = point[0];
    index[1] = point[1];
//       otbMsgDevMacro(<< "Converting: " << it.Value() << " -> " << pointCoord << " -> " << point << " -> " << index);
    newLine->AddVertex(index);
    it++;
    }

  return newLine;
}

/**
 * Convert polygon
 */
template <class TInputVectorData, class TOutputVectorData>
typename VectorDataProjectionFilter<TInputVectorData, TOutputVectorData>::PolygonPointerType
VectorDataProjectionFilter<TInputVectorData, TOutputVectorData>
::ReprojectPolygon(PolygonPointerType polygon) const
{
  typedef typename PolygonType::VertexListType::ConstPointer VertexListConstPointerType;
  typedef typename PolygonType::VertexListConstIteratorType  VertexListConstIteratorType;
  VertexListConstPointerType    vertexList = polygon->GetVertexList();
  VertexListConstIteratorType   it = vertexList->Begin();
  typename PolygonType::Pointer newPolygon = PolygonType::New();
  while (it != vertexList->End())
    {
    itk::Point<double, 2>            point;
    itk::ContinuousIndex<double, 2>  index;
    typename PolygonType::VertexType pointCoord = it.Value();
    point = m_Transform->TransformPoint(pointCoord);
    index[0] = point[0];
    index[1] = point[1];
    newPolygon->AddVertex(index);
    it++;
    }
  return newPolygon;
}

/**
* Convert polygon list
 */
template <class TInputVectorData, class TOutputVectorData>
typename VectorDataProjectionFilter<TInputVectorData, TOutputVectorData>::PolygonListPointerType
VectorDataProjectionFilter<TInputVectorData, TOutputVectorData>
::ReprojectPolygonList(PolygonListPointerType polygonList) const
{

  PolygonListPointerType newPolygonList = PolygonListType::New();
  for (typename PolygonListType::ConstIterator it = polygonList->Begin();
       it != polygonList->End(); ++it)
    {
    newPolygonList->PushBack(this->ReprojectPolygon(it.Get()));
    }
  return newPolygonList;
}

/**
 * Instanciate the transformation according to informations
 */
template <class TInputVectorData, class TOutputVectorData>
void
VectorDataProjectionFilter<TInputVectorData, TOutputVectorData>
::InstanciateTransform(void)
{

//   otbMsgDevMacro(<< "Information to instanciate transform (VectorDataProjectionFilter): ");
//   otbMsgDevMacro(<< " * Input Origin: " << m_InputOrigin);
//   otbMsgDevMacro(<< " * Input Spacing: " << m_InputSpacing);
//   otbMsgDevMacro(<< " * Input keyword list: "
//       << ((m_InputKeywordList.GetSize() == 0)?"Empty":"Full"));
//   otbMsgDevMacro(<< " * Input projection: " << m_InputProjectionRef);
//   otbMsgDevMacro(<< " * Output keyword list: "
//       << ((m_OutputKeywordList.GetSize() == 0)?"Empty":"Full"));
//   otbMsgDevMacro(<< " * Output projection: " << m_OutputProjectionRef);
//   otbMsgDevMacro(<< " * Output Origin: " << m_OutputOrigin);
//   otbMsgDevMacro(<< " * Output Spacing: " << m_OutputSpacing);

  m_Transform = InternalTransformType::New();

  InputVectorDataPointer         input = this->GetInput();
  const itk::MetaDataDictionary& inputDict = input->GetMetaDataDictionary();

  OutputVectorDataPointer  output = this->GetOutput();
  itk::MetaDataDictionary& outputDict = output->GetMetaDataDictionary();

//   m_Transform->SetInputDictionary(input->GetMetaDataDictionary());
  m_Transform->SetInputDictionary(inputDict);
  m_Transform->SetOutputDictionary(output->GetMetaDataDictionary());

  m_Transform->SetInputProjectionRef(m_InputProjectionRef);
  m_Transform->SetOutputProjectionRef(m_OutputProjectionRef);
  m_Transform->SetInputKeywordList(m_InputKeywordList);
  m_Transform->SetOutputKeywordList(m_OutputKeywordList);
  m_Transform->SetDEMDirectory(m_DEMDirectory);
  m_Transform->SetAverageElevation(m_AverageElevation);
  m_Transform->SetInputSpacing(m_InputSpacing);
  m_Transform->SetInputOrigin(m_InputOrigin);
  m_Transform->SetOutputSpacing(m_OutputSpacing);
  m_Transform->SetOutputOrigin(m_OutputOrigin);

  m_Transform->InstanciateTransform();
  // retrieve the output projection ref
  // if it is not specified and end up being geographic,
  // only the m_Transform will know
  m_OutputProjectionRef = m_Transform->GetOutputProjectionRef();

  //If the projection information for the output is provided, propagate it

  if (m_OutputKeywordList.GetSize() != 0)
    {
    ossimKeywordlist kwl;
    m_OutputKeywordList.convertToOSSIMKeywordlist (kwl);
    itk::EncapsulateMetaData<ossimKeywordlist>(outputDict, MetaDataKey::OSSIMKeywordlistKey, kwl);
    }
  if (!m_OutputProjectionRef.empty())
    {
    itk::EncapsulateMetaData<std::string>(outputDict, MetaDataKey::ProjectionRefKey, m_OutputProjectionRef);
    }
  output->SetSpacing(m_OutputSpacing);
  output->SetOrigin(m_OutputOrigin);

}

/**
   * GenerateData Performs the coordinate convertion for each element in the tree
 */
template <class TInputVectorData, class TOutputVectorData>
void
VectorDataProjectionFilter<TInputVectorData, TOutputVectorData>
::GenerateData(void)
{
  this->AllocateOutputs();
  InputVectorDataPointer  inputPtr = this->GetInput();
  OutputVectorDataPointer outputPtr = this->GetOutput();

  //Instanciate the transform
  this->InstanciateTransform();

  OutputDataTreePointerType tree = outputPtr->GetDataTree();

// Get the input tree root
  InputInternalTreeNodeType * inputRoot = const_cast<InputInternalTreeNodeType *>(inputPtr->GetDataTree()->GetRoot());

  // Create the output tree root
  OutputDataNodePointerType newDataNode = OutputDataNodeType::New();
  newDataNode->SetNodeType(inputRoot->Get()->GetNodeType());
  newDataNode->SetNodeId(inputRoot->Get()->GetNodeId());
  typename OutputInternalTreeNodeType::Pointer outputRoot = OutputInternalTreeNodeType::New();
  outputRoot->Set(newDataNode);
  tree->SetRoot(outputRoot);

  // Start recursive processing
  itk::TimeProbe chrono;
  chrono.Start();
  ProcessNode(inputRoot, outputRoot);
  chrono.Stop();
  otbMsgDevMacro(<< "VectoDataProjectionFilter: features Processed in " << chrono.GetMeanTime() << " seconds.");
}

template <class TInputVectorData, class TOutputVectorData>
void
VectorDataProjectionFilter<TInputVectorData, TOutputVectorData>
::ProcessNode(InputInternalTreeNodeType * source, OutputInternalTreeNodeType * destination)
{
  // Get the children list from the input node
  InputChildrenListType children = source->GetChildrenList();

  // For each child
  typename InputChildrenListType::iterator it = children.begin();
  while (it != children.end())
//for(typename InputChildrenListType::iterator it = children.begin(); it!=children.end();++it)
    {
    typename OutputInternalTreeNodeType::Pointer newContainer;

    // Copy input DataNode info
    InputDataNodePointerType  dataNode = (*it)->Get();
    OutputDataNodePointerType newDataNode   = OutputDataNodeType::New();
    newDataNode->SetNodeType(dataNode->GetNodeType());
    newDataNode->SetNodeId(dataNode->GetNodeId());
    newDataNode->SetMetaDataDictionary(dataNode->GetMetaDataDictionary());

    switch (dataNode->GetNodeType())
      {
    case ROOT:
      {
      newContainer = OutputInternalTreeNodeType::New();
      newContainer->Set(newDataNode);
      destination->AddChild(newContainer);
      ProcessNode((*it), newContainer);
      break;
      }
    case DOCUMENT:
      {
      newContainer = OutputInternalTreeNodeType::New();
      newContainer->Set(newDataNode);
      destination->AddChild(newContainer);
      ProcessNode((*it), newContainer);
      break;
      }
    case FOLDER:
      {
      newContainer = OutputInternalTreeNodeType::New();
      newContainer->Set(newDataNode);
      destination->AddChild(newContainer);
      ProcessNode((*it), newContainer);
      break;
      }
    case FEATURE_POINT:
      {
      newDataNode->SetPoint(this->ReprojectPoint(dataNode->GetPoint()));
      newContainer = OutputInternalTreeNodeType::New();
      newContainer->Set(newDataNode);
      destination->AddChild(newContainer);
      break;
      }
    case FEATURE_LINE:
      {
      newDataNode->SetLine(this->ReprojectLine(dataNode->GetLine()));
      newContainer = OutputInternalTreeNodeType::New();
      newContainer->Set(newDataNode);
      destination->AddChild(newContainer);
      break;
      }
    case FEATURE_POLYGON:
      {
      newDataNode->SetPolygonExteriorRing(this->ReprojectPolygon(dataNode->GetPolygonExteriorRing()));
      newDataNode->SetPolygonInteriorRings(this->ReprojectPolygonList(dataNode->GetPolygonInteriorRings()));
      newContainer = OutputInternalTreeNodeType::New();
      newContainer->Set(newDataNode);
      destination->AddChild(newContainer);
      break;
      }
    case FEATURE_MULTIPOINT:
      {
      newContainer = OutputInternalTreeNodeType::New();
      newContainer->Set(newDataNode);
      destination->AddChild(newContainer);
      ProcessNode((*it), newContainer);
      break;
      }
    case FEATURE_MULTILINE:
      {
      newContainer = OutputInternalTreeNodeType::New();
      newContainer->Set(newDataNode);
      destination->AddChild(newContainer);
      ProcessNode((*it), newContainer);
      break;
      }
    case FEATURE_MULTIPOLYGON:
      {
      newContainer = OutputInternalTreeNodeType::New();
      newContainer->Set(newDataNode);
      destination->AddChild(newContainer);
      ProcessNode((*it), newContainer);
      break;
      }
    case FEATURE_COLLECTION:
      {
      newContainer = OutputInternalTreeNodeType::New();
      newContainer->Set(newDataNode);
      destination->AddChild(newContainer);
      ProcessNode((*it), newContainer);
      break;
      }
      }
    ++it;
    }
}

} // end namespace otb

#endif
