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
#ifndef __otbLabelMapToVectorDataFilter_h
#define __otbLabelMapToVectorDataFilter_h

#include "otbVectorDataSource.h"
#include "itkLabelMap.h"
#include "otbLabelObjectToPolygonFunctor.h"
// #include "otbSimplifyPathFunctor.h"
// #include "otbClosePathFunctor.h"
#include "otbCorrectPolygonFunctor.h"
#include "otbDataNode.h"

#include <string>
#include <sstream>

namespace otb {

/** \class LabelMapToVectorDataFilter
   * \brief This class vectorizes a LabelObject to a VectorData.
   *
   * LabelMapToVectorDataFilter converts a LabelMap to a
   * VectorData where all the pixels get the attribute value of the label object they belong.
   * It uses the class otbLabelObjectToPolygonFunctor which follows a finite states machine described in
   *
   * "An algorithm for the rapid computation of boundaries of run-length
   * encoded regions", Francis K. H. Queck, in Pattern Recognition 33
   * (2000), p 1637-1649.
 *
   * \sa VectorDataSource
 * \ingroup GeoSpatialAnalysis
 */
template<class TLabelMap, class TVectorData>
class ITK_EXPORT LabelMapToVectorDataFilter :
  public VectorDataSource<TVectorData>
{
public:
  /** Standard class typedefs. */
  typedef LabelMapToVectorDataFilter Self;
  typedef VectorDataSource<TVectorData>
  Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Some convenient typedefs. */
  typedef TLabelMap                                   InputLabelMapType;
  typedef TVectorData                                 OutputVectorDataType;
  typedef typename OutputVectorDataType::Pointer      OutputVectorDataPointer;
  typedef typename OutputVectorDataType::ConstPointer OutputVectorDataConstPointer;

  typedef typename InputLabelMapType::LabelObjectType LabelObjectType;

  typedef typename OutputVectorDataType::DataNodeType DataNodeType;
  typedef typename DataNodeType::Pointer              DataNodePointerType;
  typedef typename DataNodeType::PolygonType          PolygonType;
  typedef typename PolygonType::Pointer               PolygonPointerType;
  /** Some typedefs specific to functors*/
  typedef otb::Functor::LabelObjectToPolygonFunctor<LabelObjectType, PolygonType> FunctorType;
  typedef otb::CorrectPolygonFunctor<PolygonType>                                 CorrectFunctorType;
//   typedef otb::SimplifyPathFunctor<PolygonType,PolygonType> SimplifyFunctorType;
//   typedef ClosePathFunctor <PolygonType,PolygonType> CloseFunctorType;
/** ImageDimension constants */

  /** Standard New method. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(LabelMapToVectorDataFilter,
               VectorDataSource);

  /** Set/Get the LabelMap input of this process object.  */
  virtual void SetInput(const InputLabelMapType *input);
  virtual void SetInput(unsigned int idx, const InputLabelMapType *input);
  const InputLabelMapType * GetInput(void);
  const InputLabelMapType * GetInput(unsigned int idx);

protected:
  LabelMapToVectorDataFilter();
  virtual ~LabelMapToVectorDataFilter() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** LabelMapToAttributeImageFilter needs the entire input be
   * available. Thus, it needs to provide an implementation of
   * GenerateInputRequestedRegion(). */
  void GenerateInputRequestedRegion();

  /** LabelMapToAttributeImageFilter will produce the entire output. */
  //void EnlargeOutputRequestedRegion(DataObject *itkNotUsed(output));

  /** Single-threaded version of GenerateData.  The functor is not thread safe. */
  void GenerateData();

private:
  LabelMapToVectorDataFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

}; // end of class

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "otbLabelMapToVectorDataFilter.txx"
#endif

#endif
