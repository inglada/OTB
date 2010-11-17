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
#ifndef __otbObjectDetectionClassifier_h
#define __otbObjectDetectionClassifier_h

#include <vector>

#include "itkFixedArray.h"
#include "itkImageRegion.h"
#include "otbVectorData.h"

#include "itkDataObject.h"
#include "itkDataObjectDecorator.h"
#include "itkSimpleDataObjectDecorator.h"

#include "otbPersistentImageFilter.h"
#include "otbPersistentFilterStreamingDecorator.h"

#include "otbSVMModel.h"

namespace otb
{

template <class TFunctionPrecision>
class DefaultDescriptorsType
{
public:
  typedef itk::VariableLengthVector<TFunctionPrecision> Type;
};

/** \class PersistentObjectDetectionClassifier
 *  \brief
 *
 *
 */
template <class TInputImage, class TOutputVectorData, class TLabel, class TFunctionType>
class ITK_EXPORT PersistentObjectDetectionClassifier :
  public PersistentImageFilter<TInputImage, TInputImage>
{
public:
  /** Standard Self typedef */
  typedef PersistentObjectDetectionClassifier        Self;
  typedef PersistentImageFilter<TInputImage, TInputImage> Superclass;
  typedef itk::SmartPointer<Self>                         Pointer;
  typedef itk::SmartPointer<const Self>                   ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(PersistentObjectDetectionClassifier, PersistentImageFilter);

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
  typedef TOutputVectorData                               VectorDataType;
  typedef typename VectorDataType::Pointer                VectorDataObjectType;
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
  typedef typename DescriptorsFunctionType::OutputType    DescriptorType;
  typedef typename DescriptorType::ValueType              DescriptorPrecision;

  typedef itk::ContinuousIndex
        <typename DescriptorsFunctionPointType::ValueType,
         itkGetStaticConstMacro(InputImageDimension)>     ContinuousIndexType;

  /** TLabel output */
  typedef TLabel                                             LabelType;
  typedef SVMModel<DescriptorPrecision, LabelType>           SVMModelType;
  typedef typename SVMModelType::Pointer                     SVMModelPointerType;
  typedef typename SVMModelType::MeasurementType             SVMModelMeasurementType;

  typedef itk::Statistics::ListSample<DescriptorType>        ListSampleType;

  /** SVM model used for classification */
  void SetSVMModel(SVMModelType * model);

  VectorDataType* GetOutputVectorData(void);

  /** The function to evaluate */
  itkSetObjectMacro(DescriptorsFunction,      DescriptorsFunctionType);
  itkGetObjectMacro(DescriptorsFunction,      DescriptorsFunctionType);
  itkGetConstObjectMacro(DescriptorsFunction, DescriptorsFunctionType);

  itkSetMacro( NeighborhoodRadius, unsigned int );
  itkGetConstReferenceMacro( NeighborhoodRadius, unsigned int );

  itkGetConstMacro(ClassKey, std::string);
  itkSetMacro(ClassKey, std::string);

  itkGetMacro(NoClassLabel, LabelType);
  itkSetMacro(NoClassLabel, LabelType);

  /** Set/Get the Shifts for this sample list */
  itkGetMacro(Shifts, DescriptorType);
  itkSetMacro(Shifts, DescriptorType);
  itkGetMacro(Scales, DescriptorType);
  itkSetMacro(Scales, DescriptorType);

  /** Make a DataObject of the correct type to be used as the specified
   * output. */
  itk::DataObject::Pointer MakeOutput(unsigned int idx);

  void AllocateOutputs();

  void GenerateOutputInformation();

  void Reset(void);

  void Synthetize(void);

protected:
  PersistentObjectDetectionClassifier();
  virtual ~PersistentObjectDetectionClassifier();
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  void GenerateInputRequestedRegion();

  void BeforeThreadedGenerateData();

  /** Multi-thread version GenerateData. */
  void  ThreadedGenerateData(const RegionType& outputRegionForThread,
                             int threadId);

private:
  PersistentObjectDetectionClassifier(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  template <typename TCoordRepType>
  bool
  IsInsideWithNeighborhoodRadius(const RegionType& region, const ContinuousIndexType &index) const
    {
    typedef typename RegionType::IndexType     IndexType;
    typedef typename IndexType::IndexValueType IndexValueType;

    for(unsigned int i=0; i<ImageDimension; i++)
      {
#ifdef ITK_USE_CENTERED_PIXEL_COORDINATES_CONSISTENTLY
      if( itk::Math::RoundHalfIntegerUp<IndexValueType>(index[i]) < static_cast<IndexValueType>( region.GetIndex(i) ) + m_NeighborhoodRadius  + 1 )
#else
      if( index[i] < static_cast<TCoordRepType>( region.GetIndex(i) ) + m_NeighborhoodRadius )
#endif
        {
        return false;
        }
      // bound is the last valid pixel location
#ifdef ITK_USE_CENTERED_PIXEL_COORDINATES_CONSISTENTLY
      const TCoordRepType bound = static_cast<TCoordRepType>(
          region.GetIndex(i) + region.GetSize(i) - 0.5);
#else
      const TCoordRepType bound = static_cast<TCoordRepType>(
          region.GetIndex(i) + static_cast<IndexValueType>(region.GetSize(i)) - 1);
#endif

      if( index[i] > bound - m_NeighborhoodRadius - 1 )
        {
        return false;
        }
      }
    return true;
    }

  typedef std::pair<DescriptorsFunctionPointType, LabelType> DetectedPoint;
  typedef std::vector<DetectedPoint>                         PointArray;
  typedef std::vector<PointArray>                            PointArrayContainer;
  PointArrayContainer            m_ThreadPointArray;

  DescriptorsFunctionPointerType m_DescriptorsFunction;

  unsigned int   m_NeighborhoodRadius;
  std::string    m_ClassKey;
  LabelType      m_NoClassLabel;
  DescriptorType m_Shifts;
  DescriptorType m_Scales;
  DescriptorType m_InvertedScales;
};


/** \class ObjectDetectionClassifier
 *  \brief
 *
 *  
 *
 */
template <class TInputImage, class TOutputVectorData, class TLabel, class TFunctionPrecision = double, class TCoordRep = double>
class ITK_EXPORT ObjectDetectionClassifier :
    public PersistentFilterStreamingDecorator<
              PersistentObjectDetectionClassifier< TInputImage,
                                                   TOutputVectorData,
                                                   TLabel,
                                                   itk::FunctionBase< itk::Point<TCoordRep, 2>,
                                                                      typename DefaultDescriptorsType<TFunctionPrecision>::Type> >
                                                 >
{
public:
    /** Standard Self typedef */
    typedef ObjectDetectionClassifier           Self;
    typedef PersistentFilterStreamingDecorator
              < PersistentObjectDetectionClassifier
                  <TInputImage,
                  TOutputVectorData,
                  TLabel,
                  itk::FunctionBase< itk::Point<TCoordRep, 2>,
                                     typename DefaultDescriptorsType<TFunctionPrecision>::Type> >
              >                                 Superclass;
    typedef itk::SmartPointer<Self>             Pointer;
    typedef itk::SmartPointer<const Self>       ConstPointer;

    /** Type macro */
    itkNewMacro(Self);

    /** Creation through object factory macro */
    itkTypeMacro(ObjectDetectionClassifier, PersistentFilterStreamingDecorator);

    typedef TInputImage                              InputImageType;
    typedef TOutputVectorData                        OutputVectorDataType;
    typedef TLabel                                   LabelType;
    typedef TCoordRep                                CoordRepType;
    typedef TFunctionPrecision                       FunctionPrecision;

    /** The coordinates used when evaluating function */
    typedef itk::Point<TCoordRep, 2> PointType;

    /** The return value of the function */
    typedef typename DefaultDescriptorsType<FunctionPrecision>::Type DescriptorType;

    /** The function evaluated on the input image at locations specified by the vector data */
    typedef itk::FunctionBase<PointType, DescriptorType> DescriptorsFunctionType;
    typedef typename DescriptorsFunctionType::Pointer    DescriptorsFunctionPointerType;

    typedef typename Superclass::FilterType                           PersistentFilterType;

    typedef typename PersistentFilterType::SVMModelType                        SVMModelType;
    typedef typename PersistentFilterType::SVMModelPointerType                 SVMModelPointerType;

    /** Input image to extract feature */
    void SetInputImage(InputImageType* input)
    {
      this->GetFilter()->SetInput(input);
    }

    /** Sample locations as a VectorData of points. The label is in the ClassKey feature */
    OutputVectorDataType* GetOutputVectorData()
    {
      return this->GetFilter()->GetOutputVectorData();
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

    /** The function to evaluate */
    void SetSVMModel(SVMModelType* model)
    {
      this->GetFilter()->SetSVMModel(model);
    }

    /** The function to evaluate */
    SVMModelType* GetSVMModel()
    {
      return this->GetFilter()->GetSVMModel();
    }

    otbSetObjectMemberMacro(Filter, NeighborhoodRadius, unsigned int);
    otbGetObjectMemberMacro(Filter, NeighborhoodRadius, unsigned int);

    otbGetObjectMemberMacro(Filter, ClassKey, std::string);
    otbSetObjectMemberMacro(Filter, ClassKey, std::string);

    otbGetObjectMemberMacro(Filter, NoClassLabel, LabelType);
    otbSetObjectMemberMacro(Filter, NoClassLabel, LabelType);

    /** Set/Get the Shifts for this sample list */
    otbGetObjectMemberMacro(Filter, Shifts, DescriptorType);
    otbSetObjectMemberMacro(Filter, Shifts, DescriptorType);

    /** Set/Get the Scales for this sample list */
    otbGetObjectMemberMacro(Filter, Scales, DescriptorType);
    otbSetObjectMemberMacro(Filter, Scales, DescriptorType);

  protected:
    /** Constructor */
    ObjectDetectionClassifier();

    /** Destructor */
    virtual ~ObjectDetectionClassifier();

  private:
    ObjectDetectionClassifier(const Self &); //purposely not implemented
    void operator =(const Self&); //purposely not implemented
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbObjectDetectionClassifier.txx"
#endif

#endif
