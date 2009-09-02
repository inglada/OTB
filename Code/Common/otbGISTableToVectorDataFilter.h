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
#ifndef __otbGISTableToVectorDataFilter_h
#define __otbGISTableToVectorDataFilter_h

//#include "itkImageToImageFilter.h"
#include "itkDataObject.h"
#include "otbVectorDataSource.h"
#include "otbGISTable.h"
#include "otbVectorDataFileReader.h"
#include "otbSHPVectorDataIO.h"

namespace otb {

/** \class GISTableToVectorDataFilter
 * \brief Convert a LabelMap to a VectorData
 *
 * GISTableToVectorDataFilter converts a GIS Table (PostGIS...) to a
 * VectorData.
 * The translation is delegate to the SHPVectorDataIO class (OGRVectorDataIO)
 * which calls the OGR driver to translate GIS layer to a vectorData structure
 * \author Manuel GRIZONNET. CNES, France.
 *
 * \sa VectorDataSource
 * \ingroup Common, GeospatialAnalysis
 */
template<class TGISTable, class TVectorData >
class ITK_EXPORT GISTableToVectorDataFilter : 
    public VectorDataSource< TVectorData >
{
public:
  /** Standard class typedefs. */
  typedef GISTableToVectorDataFilter Self;
  typedef VectorDataSource< TVectorData >
  Superclass;
  typedef itk::SmartPointer<Self>        Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** Some convenient typedefs. */
  typedef TGISTable InputGISTableType;
  typedef TVectorData OutputVectorDataType;
  typedef typename OutputVectorDataType::Pointer        OutputVectorDataPointer;
  typedef typename OutputVectorDataType::ConstPointer   OutputVectorDataConstPointer;
  
  typedef typename OutputVectorDataType::DataNodeType  DataNodeType;
  typedef typename DataNodeType::Pointer         DataNodePointerType;
  
  typedef typename InputGISTableType::ConnectionType ConnectionType;
  typedef typename InputGISTableType::ConnectionPointerType ConnectionPointerType;
  
  typedef SHPVectorDataIO<OutputVectorDataType> SHPVectorDataIOType;
  typedef typename SHPVectorDataIOType::Pointer SHPVectorDataIOPointerType;
  
  typedef VectorDataFileReader<OutputVectorDataType> VectorDataFileReaderType;
  /** ImageDimension constants */
  

  /** Standard New method. */
  itkNewMacro(Self);  

  /** Runtime information support. */
  itkTypeMacro(GISTableToVectorDataFilter, 
               VectorDataSource);

  
  /** Set/Get the GISTable input of this process object.  */
  virtual void SetInput( const InputGISTableType *input);
  virtual void SetInput( unsigned int idx, const InputGISTableType *input);
  const InputGISTableType * GetInput(void);
  const InputGISTableType * GetInput(unsigned int idx);
  
protected:
  GISTableToVectorDataFilter();
  ~GISTableToVectorDataFilter() {};
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** GISTableToVectorDataFilter needs the entire input be
   * available. Thus, it needs to provide an implementation of
   * GenerateInputRequestedRegion(). */
  void GenerateInputRequestedRegion() ;

  /** Single-threaded version of GenerateData.  This filter delegates
   * to the OGR driver. */
  void GenerateData();
  

private:
  GISTableToVectorDataFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  typename VectorDataFileReaderType::Pointer m_Reader;
} ; // end of class

} // end namespace otb
  
#ifndef ITK_MANUAL_INSTANTIATION
#include "otbGISTableToVectorDataFilter.txx"
#endif

#endif