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
#ifndef __otbOGRVectorDataIO_h
#define __otbOGRVectorDataIO_h

#include <string>

#include "otbVectorDataIOBase.h"
#include "otbVectorData.h"

class OGRDataSource;

namespace otb
{

/** \class OGRVectorDataIO
 *
 * \brief ImageIO object for reading and writing OGR format vector data
 *
 * ImageIO object for reading and writing OGR format vector data: it can
 * handle Esri Shapefile (shp), MapInfo (map) and Geography Markup Language (GML)
 * and possibly other OGR formats.
 *
 */
class ITK_EXPORT OGRVectorDataIO
  : public VectorDataIOBase
{
public:

  /** Standard class typedefs. */
  typedef OGRVectorDataIO               Self;
  typedef VectorDataIOBase              Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(OGRVectorDataIO, VectorDataIOBase);

  /** Byte order typedef */
  typedef Superclass::ByteOrder ByteOrder;

  /** Data typedef */
  typedef VectorData<double, 2>                  VectorDataType;
  typedef VectorDataType::DataTreeType           DataTreeType;
  typedef DataTreeType::TreeNodeType             InternalTreeNodeType;
  typedef InternalTreeNodeType::ChildrenListType ChildrenListType;
  typedef DataTreeType::Pointer                  DataTreePointerType;
  typedef DataTreeType::ConstPointer             DataTreeConstPointerType;
  typedef VectorDataType::DataNodeType           DataNodeType;
  typedef DataNodeType::Pointer                  DataNodePointerType;
  typedef DataNodeType::PointType                PointType;
  typedef DataNodeType::LineType                 LineType;
  typedef LineType::VertexListType               VertexListType;
  typedef VertexListType::ConstPointer           VertexListConstPointerType;
  typedef LineType::Pointer                      LinePointerType;
  typedef DataNodeType::PolygonType              PolygonType;
  typedef PolygonType::Pointer                   PolygonPointerType;
  typedef DataNodeType::PolygonListType          PolygonListType;
  typedef PolygonListType::Pointer               PolygonListPointerType;
  typedef VectorDataType::Pointer                VectorDataPointerType;
  typedef VectorDataType::ConstPointer           VectorDataConstPointerType;
  typedef Superclass::SpacingType                SpacingType;
  typedef Superclass::PointType                  OriginType;

  /*-------- This part of the interface deals with reading data. ------ */

  /** Determine the file type. Returns true if this VectorDataIO can read the
   * file specified. */
  virtual bool CanReadFile(const char*) const;

  /** Reads the data from disk into the memory buffer provided. */
  virtual void Read(itk::DataObject* data);

  /*-------- This part of the interfaces deals with writing data. ----- */

  /** Determine the file type. Returns true if this ImageIO can read the
   * file specified. */
  virtual bool CanWriteFile(const char*) const;

  /** Writes the data to disk from the memory buffer provided */
  virtual void Write(const itk::DataObject* data,  char ** papszOptions = NULL);

protected:
  /** Constructor.*/
  OGRVectorDataIO();
  /** Destructor.*/
  virtual ~OGRVectorDataIO();

  /*   virtual void InternalReadVectorDataInformation(){}; */

  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  OGRVectorDataIO(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  std::string GetOGRDriverName(std::string name) const;

  OGRDataSource * m_DataSource;

};

} // end namespace otb

#endif // __otbOGRVectorDataIO_h
