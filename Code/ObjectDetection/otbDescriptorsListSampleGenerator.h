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
#ifndef __otbDescriptorsListSampleGenerator_h
#define __otbDescriptorsListSampleGenerator_h

#include <vector>

#include "itkVariableLengthVector.h"
#include "itkImageRegion.h"

#include "otbListSampleSource.h"
#include "otbVectorData.h"

#include "itkDataObject.h"
#include "itkDataObjectDecorator.h"
#include "itkSimpleDataObjectDecorator.h"

#include "otbPersistentImageFilter.h"
#include "otbPersistentFilterStreamingDecorator.h"

namespace otb
{

template <class TOutputPrecision>
class DefaultDescriptorsType
{
public:
  typedef itk::VariableLengthVector<TOutputPrecision> Type;
};

/** \class PersistentDescriptorsListSampleGenerator
 *  \brief
 *
 *
 */
template <class TInputImage, class TVectorData, class TFunctionType, class TListSample, class TLabelListSample>
class ITK_EXPORT PersistentDescriptorsListSampleGenerator :
  public PersistentImageFilter<TInputImage, TInputImage>
{
public:
  /** Standard Self typedef */
  typedef PersistentDescriptorsListSampleGenerator        Self;
  typedef PersistentImageFilter<TInputImage, TInputImage> Superclass;
  typedef itk::SmartPointer<Self>                         Pointer;
  typedef itk::SmartPointer<const Self>                   ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(PersistentDescriptorsListSampleGenerator, PersistentImageFilter);

  /** Image related typedefs. */
  typedef TInputImage                      InputImageType;
  typedef typename TInputImage::Pointer    InputImagePointer;
  typedef typename TInputImage::RegionType RegionType;
  typedef typename TInputImage::SizeType   SizeType;
  typedef typename TInputImage::IndexType  IndexType;
  typedef typename TInputImage::PixelType  PixelType;

  itkStaticConstMacro(InputImageDimension, unsigned int,
                      TInputImage::ImageDimension);

  /** Image related typedefs. */
  itkStaticConstMacro(ImageDimension, unsigned int,
                      TInputImage::ImageDimension);

  /** Smart Pointer type to a DataObject. */
  typedef typename itk::DataObject::Pointer DataObjectPointer;

  /** Input VectorData */
  typedef TVectorData                                     VectorDataType;
  typedef itk::DataObjectDecorator<VectorDataType>        VectorDataObjectType;
  typedef typename VectorDataObjectType::Pointer          VectorDataObjectPointerType;
  typedef typename VectorDataType::DataNodeType           VectorDataNodeType;
  typedef typename VectorDataType::DataNodePointerType    VectorDataNodePointerType;
  typedef typename VectorDataType::DataTreeType           VectorDataTreeType;
  typedef typename VectorDataType::DataTreePointerType    VectorDataTreePointerType;
  typedef typename VectorDataNodeType::PointType          VectorDataPointType;
  typedef typename VectorDataNodeType::LineType           VectorDataLineType;
  typedef typename VectorDataNodeType::PolygonType        VectorDataPolygonType;
  typedef itk::PreOrderTreeIterator<VectorDataTreeType>   VectorDataTreeIteratorType;

  /** Function type for descriptors extraction */
  typedef TFunctionType                                   DescriptorsFunctionType;
  typedef typename DescriptorsFunctionType::Pointer       DescriptorsFunctionPointerType;
  typedef typename DescriptorsFunctionType::InputType     DescriptorsFunctionPointType;

  typedef itk::ContinuousIndex
        <typename DescriptorsFunctionPointType::ValueType,
         itkGetStaticConstMacro(InputImageDimension)>     ContinuousIndexType;

  /** ListSample output */
  typedef TListSample                                     ListSampleType;
  typedef typename ListSampleType::Pointer                ListSamplePointerType;
  typedef itk::DataObjectDecorator<ListSampleType>        ListSampleObjectType;
  typedef typename ListSampleObjectType::Pointer          ListSampleObjectPointerType;
  typedef typename ListSampleType::MeasurementVectorType  SampleMeasurementVectorType;
  typedef typename ListSampleType::MeasurementType        SampleMeasurementType;

  /** LabelListSample output */
  typedef TLabelListSample                                LabelListSampleType;
  typedef typename LabelListSampleType::Pointer           LabelListSamplePointerType;
  typedef itk::DataObjectDecorator<LabelListSampleType>   LabelListSampleObjectType;
  typedef typename LabelListSampleObjectType::Pointer     LabelListSampleObjectPointerType;
  typedef typename LabelListSampleType::MeasurementVectorType LabelMeasurementVectorType;
  typedef typename LabelListSampleType::MeasurementType   LabelMeasurementType;

  /** ListSamplePositions output */
  typedef std::vector<DescriptorsFunctionPointType>               SamplesPositionType;
  typedef itk::SimpleDataObjectDecorator<SamplesPositionType>     SamplesPositionObjectType;
  typedef typename SamplesPositionObjectType::Pointer             SamplesPositionObjectPointerType;

  void SetSamplesLocations(VectorDataType * input);
  VectorDataType * GetSamplesLocations(void);

  /** The function to evaluate */
  itkSetObjectMacro(DescriptorsFunction,      DescriptorsFunctionType);
  itkGetObjectMacro(DescriptorsFunction,      DescriptorsFunctionType);
  itkGetConstObjectMacro(DescriptorsFunction, DescriptorsFunctionType);

  /** Output sample list */
  ListSampleType*             GetListSample();
  ListSampleObjectType*       GetListSampleObject();

  /** Output label list */
  LabelListSampleType*        GetLabelListSample();
  LabelListSampleObjectType*  GetLabelListSampleObject();

  /** Output sample position list */
  SamplesPositionType&             GetSamplesPositions();
  SamplesPositionObjectType*       GetSamplesPositionsObject();

  itkSetMacro( NeighborhoodRadius, unsigned int );
  itkGetConstReferenceMacro( NeighborhoodRadius, unsigned int );

  /** Make a DataObject of the correct type to be used as the specified
   * output. */
  itk::DataObject::Pointer MakeOutput(unsigned int idx);

  void AllocateOutputs();
  void GenerateOutputInformation();
  void Reset(void);
  void Synthetize(void);

  void AddInput(itk::DataObject * dataObject)
  {
    Superclass::AddInput(dataObject);
  }

protected:
  PersistentDescriptorsListSampleGenerator();
  virtual ~PersistentDescriptorsListSampleGenerator();
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  void GenerateInputRequestedRegion();

  void BeforeThreadedGenerateData();

  /** Multi-thread version GenerateData. */
  void  ThreadedGenerateData(const RegionType& outputRegionForThread,
                             int threadId);

private:
  PersistentDescriptorsListSampleGenerator(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  bool IsInsideWithNeighborhoodRadius(const RegionType& region, const ContinuousIndexType &index) const
    {
    typedef typename RegionType::IndexType     IndexType;
    typedef typename IndexType::IndexValueType IndexValueType;
    typedef typename ContinuousIndexType::ValueType ContinuousIndexValueType;

    for(unsigned int i=0; i<ImageDimension; i++)
      {
#ifdef ITK_USE_CENTERED_PIXEL_COORDINATES_CONSISTENTLY
      if( itk::Math::RoundHalfIntegerUp<IndexValueType>(index[i]) < static_cast<IndexValueType>( region.GetIndex(i) ) + static_cast<IndexValueType>(m_NeighborhoodRadius) )
#else
      if( index[i] < static_cast<IndexValueType>( region.GetIndex(i) ) + m_NeighborhoodRadius )
#endif
        {
        return false;
        }
      // bound is the last valid pixel location
#ifdef ITK_USE_CENTERED_PIXEL_COORDINATES_CONSISTENTLY
      const ContinuousIndexValueType bound = static_cast<ContinuousIndexValueType>(
          region.GetIndex(i) + region.GetSize(i) - 0.5);
#else
      const ContinuousIndexValueType bound = static_cast<ContinuousIndexValueType>(
          region.GetIndex(i) + static_cast<IndexValueType>(region.GetSize(i)) - 1);
#endif

      if( index[i] > bound - m_NeighborhoodRadius )
        {
        return false;
        }
      }
    return true;
    }
  typedef std::vector<ListSamplePointerType>      ListSampleArray;
  typedef std::vector<LabelListSamplePointerType> LabelListSampleArray;
  typedef std::vector<SamplesPositionType>        SamplesPositionArray;

  ListSampleArray                m_ThreadListSample;
  LabelListSampleArray           m_ThreadLabelListSample;
  SamplesPositionArray           m_ThreadSamplesPosition;

  DescriptorsFunctionPointerType m_DescriptorsFunction;

  unsigned int m_NeighborhoodRadius;
};


/** \class DescriptorsListSampleGenerator
 *  \brief
 *
 *  
 *
 */
template <class TInputImage, class TVectorData, class TListSample, class TLabelListSample, class TOutputPrecision = double, class TCoordRep = double>
class ITK_EXPORT DescriptorsListSampleGenerator :
    public PersistentFilterStreamingDecorator<
              PersistentDescriptorsListSampleGenerator< TInputImage,
                                                        TVectorData,
                                                        itk::FunctionBase< itk::Point<TCoordRep, 2>,
                                                                           typename DefaultDescriptorsType<TOutputPrecision>::Type >,
                                                        TListSample,
                                                        TLabelListSample > >
{
public:
    /** Standard Self typedef */
    typedef DescriptorsListSampleGenerator      Self;
    typedef PersistentFilterStreamingDecorator
              < PersistentDescriptorsListSampleGenerator
                  <TInputImage,
                   TVectorData,
                   itk::FunctionBase< itk::Point<TCoordRep, 2>, typename DefaultDescriptorsType<TOutputPrecision>::Type >,
                   TListSample,
                   TLabelListSample> >          Superclass;
    typedef itk::SmartPointer<Self>             Pointer;
    typedef itk::SmartPointer<const Self>       ConstPointer;

    /** Type macro */
    itkNewMacro(Self);

    /** Creation through object factory macro */
    itkTypeMacro(DescriptorsListSampleGenerator, PersistentFilterStreamingDecorator);

    typedef TInputImage                              InputImageType;
    typedef TVectorData                              InputVectorDataType;
    typedef TListSample                              ListSampleType;
    typedef TLabelListSample                         LabelListSampleType;
    typedef TCoordRep                                CoordRepType;
    typedef TOutputPrecision                         OutputPrecision;

    /** The coordinates used when evaluating function */
    typedef itk::Point<TCoordRep, 2> PointType;

    /** The return value of the function */
    typedef typename DefaultDescriptorsType<TOutputPrecision>::Type DescriptorType;

    /** The function evaluated on the input image at locations specified by the vector data */
    typedef itk::FunctionBase<PointType, DescriptorType> DescriptorsFunctionType;
    typedef typename DescriptorsFunctionType::Pointer    DescriptorsFunctionPointerType;

    typedef typename Superclass::FilterType                           PersistentFilterType;
    typedef typename PersistentFilterType::ListSampleObjectType       ListSampleObjectType;
    typedef typename PersistentFilterType::LabelListSampleObjectType  LabelListSampleObjectType;
    typedef typename PersistentFilterType::SamplesPositionObjectType  SamplesPositionObjectType;
    typedef typename PersistentFilterType::SamplesPositionType        SamplesPositionType;


    /** Input image to extract feature */
    void SetInputImage(InputImageType * input)
    {
      this->GetFilter()->SetInput(input);
    }

    /** Input image to extract feature */
    InputImageType * GetInputImage()
    {
      return this->GetFilter()->GetInput();
    }

    void AddInput(itk::DataObject * dataObject)
      {
        this->GetFilter()->AddInput(dataObject);
      }

    /** Sample locations as a VectorData of points. The label is in the ClassKey feature */
    void SetSamplesLocations(InputVectorDataType * input)
    {
      this->GetFilter()->SetSamplesLocations(input);
    }

    /** Sample locations as a VectorData of points. The label is in the ClassKey feature  */
    InputImageType * GetSamplesLocations()
    {
      return this->GetFilter()->GetSamplesLocations();
    }

    /** The function to evaluate */
    void SetDescriptorsFunction(DescriptorsFunctionType * input)
    {
      this->GetFilter()->SetDescriptorsFunction(input);
    }

    /** The function to evaluate */
    DescriptorsFunctionType * GetDescriptorsFunction()
    {
      return this->GetFilter()->GetDescriptorsFunction();
    }

    /** Final sample list */
    ListSampleType*       GetListSample()
    {
      return this->GetFilter()->GetListSample();
    }

    ListSampleObjectType*       GetListSampleObject()
    {
      return this->GetFilter()->GetListSampleObject();
    }

    /** Final label list */
    LabelListSampleType*        GetLabelListSample()
    {
      return this->GetFilter()->GetLabelListSample();
    }

    LabelListSampleObjectType*        GetLabelListSampleObject()
    {
      return this->GetFilter()->GetLabelListSampleObject();
    }

    /** Final label list */
    SamplesPositionType&        GetSamplesPositions()
    {
      return this->GetFilter()->GetSamplesPositions();
    }

    SamplesPositionObjectType*  GetSamplesPositionsObject()
    {
      return this->GetFilter()->GetSamplesPositionsObject();
    }

    otbSetObjectMemberMacro(Filter, NeighborhoodRadius, unsigned int);
    otbGetObjectMemberMacro(Filter, NeighborhoodRadius, unsigned int);

  protected:
    /** Constructor */
    DescriptorsListSampleGenerator();

    /** Destructor */
    virtual ~DescriptorsListSampleGenerator();

  private:
    DescriptorsListSampleGenerator(const Self &); //purposely not implemented
    void operator =(const Self&); //purposely not implemented
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbDescriptorsListSampleGenerator.txx"
#endif

#endif
