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
#ifndef __otbVectorDataToVectorDataFilter_h
#define __otbVectorDataToVectorDataFilter_h

#include "otbVectorDataSource.h"

namespace otb
{

/** \class VectorDataToVectorDataFilter
   * \brief Base class for filters that take an VectorData as input and produce an VectorData as output.
   *
   * VectorDataToVectorDataFilter is the base class for all process objects that output
   * VectorData data and require VectorData data as input. Specifically, this class
   * defines the SetInput() method for defining the input to a filter.
   *
   *
   *
   * \ingroup VectorDataFilter
 */
template <class TInputVectorData, class TOutputVectorData >
class ITK_EXPORT VectorDataToVectorDataFilter : public otb::VectorDataSource<TOutputVectorData>
{
public:
  /** Standard class typedefs. */
  typedef VectorDataToVectorDataFilter  Self;
  typedef otb::VectorDataSource<TOutputVectorData>  Superclass;
  typedef itk::SmartPointer<Self>   Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(VectorDataToVectorDataFilter, VectorDataSource);

  /** Some typedefs. */
  typedef TInputVectorData InputVectorDataType;
  typedef TOutputVectorData OutputVectorDataType;
  typedef typename TInputVectorData::ConstPointer InputVectorDataPointer;
  typedef typename TOutputVectorData::Pointer OutputVectorDataPointer;

  typedef itk::DataObject::Pointer DataObjectPointer;


  virtual void SetInput( const InputVectorDataType *input);
  const InputVectorDataType * GetInput(void);


protected:
  /** Constructor */
  VectorDataToVectorDataFilter();
  /** Destructor */
  virtual ~VectorDataToVectorDataFilter() {};
  /**PrintSelf method */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  VectorDataToVectorDataFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

};

} // end namespace otb

#ifndef ITK_MANUAL_INSTANTIATION
#include "otbVectorDataToVectorDataFilter.txx"
#endif

#endif
