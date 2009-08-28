/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkLabelMapToAttributeImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2005/08/23 15:09:03 $
  Version:   $Revision: 1.4 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbVectorDataToGISTableFilter_h
#define __otbVectorDataToGISTableFilter_h

//#include "itkImageToImageFilter.h"
//#include "itkAttributeLabelObject.h"
#include "otbGISTableSource.h"
#include "otbGISConnectionImplementation.h"
#include "otbVectorData.h"
#include <iostream>
#include <sstream>
#include <string>

namespace otb {

/** \class VectorDataToGISTableFilter
 * \brief Convert a LabelMap to a VectorData
 *
 * VectorDataToGISTableFilter converts  a
 * VectorData to GIS Table (PostGIS...).
 * Create first a PostGIS table (by default name=vector_data_to_gis_sample)
 * The table is not drop if it already exist
 * Structure of the PostGIS table:
 * vector_data_to_postgis (id serial, the_geom geometry)
 * with srid = -1
 * multi geometries are not handle yet 
 * \author Manuel GRIZONNET. CNES, France.
 *
 * \sa LabelMapToBinaryImageFilter, LabelMapMaskImageFilter
 * \ingroup ImageEnhancement  MathematicalMorphologyImageFilters
 */
template<class TVectorData , class TGISTable>
class ITK_EXPORT VectorDataToGISTableFilter : 
    public GISTableSource< TGISTable >
{
public:
  /** Standard class typedefs. */
  typedef VectorDataToGISTableFilter Self;
  typedef GISTableSource< TGISTable >
  Superclass;
  typedef itk::SmartPointer<Self>        Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** Some convenient typedefs. */
  typedef TVectorData InputVectorDataType;
  typedef TGISTable OutputGISTableType;
  typedef typename InputVectorDataType::Pointer         InputVectorDataPointer;
  typedef typename InputVectorDataType::ConstPointer    InputVectorDataConstPointer;
  typedef typename OutputGISTableType::Pointer        OutputGISTablePointer;
  typedef typename OutputGISTableType::ConstPointer   OutputGISTableConstPointer;
  
  typedef typename InputVectorDataType::DataTreeType::TreeNodeType    InternalTreeNodeType;
  typedef typename InternalTreeNodeType::ChildrenListType        ChildrenListType;
  typedef typename InputVectorDataType::DataNodeType  DataNodeType;
  typedef typename DataNodeType::Pointer         DataNodePointerType;
  
  
  typedef typename OutputGISTableType::ConnectionType InputGISConnectionType;
  typedef typename InputGISConnectionType::Pointer InputGISConnectionPointerType;
  
  
  /** ImageDimension constants */
  //itkStaticConstMacro(InputImageDimension, unsigned int,
  //                    TInputImage::ImageDimension);
  //itkStaticConstMacro(OutputImageDimension, unsigned int,
  //                    TOutputImage::ImageDimension);

  /** Standard New method. */
  itkNewMacro(Self);  

  /** Runtime information support. */
  //itkTypeMacro(LabelMapToAttributeImageFilter, 
  //           ImageToImageFilter);

  /**
   * Set/Get the boolean value if you do not want to create the GIS table 
   */
  itkSetMacro(DropExistingGISTable, bool);
  itkGetConstMacro(DropExistingGISTable, bool);
  
  itkSetMacro(GISTableName, std::string);
  itkGetConstMacro(GISTableName, std::string);
  
  itkGetObjectMacro(InputGISConnection, InputGISConnectionType);
  itkSetObjectMacro(InputGISConnection, InputGISConnectionType);
  
  //void setConnection 

  /** Set/Get the Vector data input of this process object.  */
  virtual void SetInput( const InputVectorDataType *input);
  virtual void SetInput( unsigned int idx, const InputVectorDataType *input);
  const InputVectorDataType * GetInput(void);
  const InputVectorDataType * GetInput(unsigned int idx);
  
protected:
  VectorDataToGISTableFilter();
  ~VectorDataToGISTableFilter() {};
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** LabelMapToAttributeImageFilter needs the entire input be
   * available. Thus, it needs to provide an implementation of
   * GenerateInputRequestedRegion(). */
  void GenerateInputRequestedRegion() ;

  /** LabelMapToAttributeImageFilter will produce the entire output. */
  //void EnlargeOutputRequestedRegion(DataObject *itkNotUsed(output));
  
  /** Single-threaded version of GenerateData.  This filter delegates
   * to GrayscaleGeodesicErodeImageFilter. */
  void GenerateData();
  
  
private:
  VectorDataToGISTableFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
  
   
  void ProcessNode(InternalTreeNodeType * source);
  
   /** Remove table before insertion if true  */
  bool m_DropExistingGISTable;
  
  /** Connection parameters to the db  */
  InputGISConnectionPointerType m_InputGISConnection;
  /** GIS table name  */
  std::string m_GISTableName;
} ; // end of class

} // end namespace otb
  
#ifndef ITK_MANUAL_INSTANTIATION
#include "otbVectorDataToGISTableFilter_old.txx"
#endif

#endif


