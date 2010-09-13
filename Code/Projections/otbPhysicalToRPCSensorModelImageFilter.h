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
#ifndef __otbPhysicalToRPCSensorModelImageFilter_h
#define __otbPhysicalToRPCSensorModelImageFilter_h

#include "itkInPlaceImageFilter.h"
#include "otbGCPsToRPCSensorModelImageFilter.h"
#include "otbGenericRSTransform.h"


namespace otb {

/** \class PhysicalToRPCSensorModelImageFilter
 * \brief This filter estimates a RPC sensor models from a physical model.
 *
 * This filters estimates an RPC sensor model from a list of points
 * homologous point in the physical coordinate system and the WGS84
 * CS.
 * 
 * The number of points used to estimate the rpc sensor model is
 * defined by the GridSize, note that at least 16 points are needed
 * to estimate a proper RPC sensor model, there are no warning when
 * the model estimated is inaccurate.
 *
 * The UseImageGCPs flag allows to import GCPs from the image
 * metadata, if any.(TODO)
 *
 * GCPs can be passed to the filter using one of the AddGCP method
 * implementation.(TODO)
 *
 * Depending on the value of the DEMDirectory, an elevation fetched
 * from the SRT directory is used.(TODO) 
 *
 * This filter does not modify the image buffer, but only the
 * metadata. Therefore, it is implemented as an InPlaceImageFilter.
 *
 * \ingroup Projections
 */
template <class TImage>
class ITK_EXPORT PhysicalToRPCSensorModelImageFilter :
  public itk::InPlaceImageFilter<TImage>
{
public:
  /** Standard class typedefs. */
  typedef PhysicalToRPCSensorModelImageFilter Self;
  typedef itk::InPlaceImageFilter<TImage>     Superclass;
  typedef itk::SmartPointer<Self>             Pointer;
  typedef itk::SmartPointer<const Self>       ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(PhysicalToRPCSensorModelImageFilter, InPlaceImageFilter);

  /** template parameters typedef */
  typedef TImage                          ImageType;
  typedef typename ImageType::PointType   PointType;
  typedef typename ImageType::SizeType    SizeType;
  
  /** GCPsToSensor model filter */
  typedef GCPsToRPCSensorModelImageFilter<ImageType>          GCPsToSensorModelType;
  typedef typename GCPsToSensorModelType::Pointer             GCPsToSensorModelPointerType;
  typedef typename GCPsToSensorModelType::Point2DType         Point2DType;
  typedef typename GCPsToSensorModelType::Point3DType         Point3DType;
  typedef typename GCPsToSensorModelType::GCPType             GCPType;
  typedef typename GCPsToSensorModelType::GCPsContainerType   GCPsContainerType;
  typedef typename GCPsToSensorModelType::ErrorsContainerType ErrorsContainerType;

  /** Remote Sensing Transform */
  typedef GenericRSTransform<>                                RSTransformType;
  typedef typename RSTransformType::Pointer                   RSTransformPointerType;

  /** launch the process */
  virtual void GenerateOutputInformation();//GenerateData();
  
  /** Set/Get the grid size*/
  itkSetMacro(GridSize, SizeType);
  itkGetMacro(GridSize, SizeType);

  /** Set/Get the DEM directory*/
  itkSetStringMacro(DEMDirectory);
  itkGetStringMacro(DEMDirectory);

  /** Set Grid size with an unsigned int parmeter*/
  void SetGridSize(unsigned int inSize)
  {
    m_GridSize.Fill(inSize);
  }
  
protected:
  /** Constructor */
  PhysicalToRPCSensorModelImageFilter();
  /** Destructor */
  virtual ~PhysicalToRPCSensorModelImageFilter();

  /** The PrintSelf method */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /**  */
  virtual void ThreadedGenerateData(const typename TImage::RegionType&, int) {}
  
private:
  PhysicalToRPCSensorModelImageFilter(const Self &);   // purposely not implemented
  void operator =(const Self&);    // purposely not implemented
  
  /** The rpc model estimator */
  GCPsToSensorModelPointerType       m_GCPsToSensorModelFilter;
  
  std::string                        m_DEMDirectory;
  SizeType                           m_GridSize;
};
} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbPhysicalToRPCSensorModelImageFilter.txx"
#endif

#endif
