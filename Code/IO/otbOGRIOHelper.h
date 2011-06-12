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
#ifndef __otbOGRIOHelper_h
#define __otbOGRIOHelper_h

#include <vector>

#include "otbVectorData.h"

class OGRDataSource;
class OGRGeometryCollection;
class OGRLayer;
class OGRSpatialReference;
class OGRGeometry;

namespace otb
{

/** \class OGRIOHelper
 *  \brief This class IO OGR
 *
 */
class OGRIOHelper: public itk::Object
{
public:
  /** Standard class typedefs. */
  typedef OGRIOHelper                   Self;
  typedef itk::Object                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Template parameters typedefs */
  /** Data typedef */
  typedef VectorData<>                                    VectorDataType;
  typedef VectorDataType::DataTreeType           DataTreeType;
  typedef DataTreeType::TreeNodeType             InternalTreeNodeType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(OGRIOHelper, itk::Object);

  /** Conversion tools */
  void ConvertOGRLayerToDataTreeNode(OGRLayer * layer, InternalTreeNodeType * documentPtr) const;


  unsigned int ProcessNodeWrite(InternalTreeNodeType * source,
                                OGRDataSource * m_DataSource,
                                OGRGeometryCollection * ogrCollection,
                                OGRLayer * ogrCurrentLayer,
                                OGRSpatialReference * oSRS);

protected:
  OGRIOHelper();
  ~OGRIOHelper();

private:
  OGRIOHelper(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  typedef VectorDataType::DataNodeType           DataNodeType;
  typedef DataNodeType::Pointer                  DataNodePointerType;

  typedef DataNodeType::PointType                PointType;

  typedef DataNodeType::LineType                 LineType;
  typedef LineType::Pointer                      LinePointerType;
  typedef LineType::VertexListType               VertexListType;
  typedef VertexListType::ConstPointer           VertexListConstPointerType;

  typedef DataNodeType::PolygonType              PolygonType;
  typedef PolygonType::Pointer                   PolygonPointerType;
  typedef DataNodeType::PolygonListType          PolygonListType;
  typedef PolygonListType::Pointer               PolygonListPointerType;

  void ConvertGeometryToPointNode(const OGRGeometry * ogrGeometry, DataNodePointerType node) const;

  void ConvertGeometryToLineNode(const OGRGeometry * ogrGeometry, DataNodePointerType node) const;

  void ConvertGeometryToPolygonNode(const OGRGeometry * ogrGeometry, DataNodePointerType node) const;

}; // end class OGRIOHelper

} // end namespace otb

#endif
