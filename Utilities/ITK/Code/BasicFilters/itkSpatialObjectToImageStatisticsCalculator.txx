/*=========================================================================

Program:   Insight Segmentation & Registration Toolkit
Module:    $RCSfile: itkSpatialObjectToImageStatisticsCalculator.txx,v $
Language:  C++
Date:      $Date: 2005/07/26 15:55:08 $
Version:   $Revision: 1.16 $

Copyright (c) Insight Software Consortium. All rights reserved.
See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

This software is distributed WITHOUT ANY WARRANTY; without even 
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itkSpatialObjectToImageStatisticsCalculator_txx
#define _itkSpatialObjectToImageStatisticsCalculator_txx

#include "itkSpatialObjectToImageStatisticsCalculator.h"
#include "itkImageRegionConstIteratorWithIndex.h"
#include "itkVector.h"
#include "itkListSample.h"
#include "itkMeanCalculator.h"
#include "itkCovarianceCalculator.h"

namespace itk
{ 
    
/** Constructor */
template<class TInputImage, class TInputSpatialObject, unsigned int TSampleDimension>
SpatialObjectToImageStatisticsCalculator<TInputImage,TInputSpatialObject,TSampleDimension>
::SpatialObjectToImageStatisticsCalculator()
{
  m_Image = NULL;
  m_SpatialObject = NULL;
  m_Mean.Fill(0);
  m_CovarianceMatrix.SetIdentity();
  m_SampleDirection = TSampleDimension-1;
  m_InternalImageTime = 0;
  m_InternalSpatialObjectTime = 0;
  m_Sum = 0;
  m_NumberOfPixels = 0;;
}

/** */
template<class TInputImage,class TInputSpatialObject, unsigned int TSampleDimension>
void
SpatialObjectToImageStatisticsCalculator<TInputImage,TInputSpatialObject,TSampleDimension>
::Update(void)
{
  if(!m_Image || !m_SpatialObject)
    {
    itkExceptionMacro("SpatialObjectToImageStatisticsCalculator: Please set the image AND the spatial object first.");
    }

  // Update only if the image or the spatial object has been modified
  if((m_Image->GetMTime() == m_InternalImageTime)
     &&
     (m_SpatialObject->GetMTime() == m_InternalSpatialObjectTime) 
    )
    {
    return; // No need to update
    }

  
  m_InternalImageTime = m_Image->GetMTime();
  m_InternalSpatialObjectTime = m_SpatialObject->GetMTime();

  // Get the bounding box
  typename SpatialObjectType::BoundingBoxType::Pointer boundingBox;
  m_SpatialObject->ComputeBoundingBox();
  boundingBox = m_SpatialObject->GetBoundingBox();
  
  Point<double,itkGetStaticConstMacro(ObjectDimension)> pt;
  for(unsigned int i=0;i<itkGetStaticConstMacro(ObjectDimension);i++)
    {
    pt[i]=boundingBox->GetBounds()[i*2]+(boundingBox->GetBounds()[i*2+1]-boundingBox->GetBounds()[i*2])/2;
    }

  IndexType index;
  
  // We should remove the spacing and the origin of the image since the FloodFill iterator is
  // considering them.
  for(unsigned int i=0;i<itkGetStaticConstMacro(ObjectDimension);i++)
    {
    index[i]=(long int)((static_cast<typename IndexType::IndexValueType>(pt[i])-m_Image->GetOrigin()[i])/m_Image->GetSpacing()[i]);
    }

  IteratorType it = IteratorType(m_Image,m_SpatialObject,index);
  it.SetOriginInclusionStrategy();
  it.GoToBegin();

  typedef typename ImageType::PixelType PixelType;
  typedef itk::Statistics::ListSample< VectorType > SampleType;
  typename SampleType::Pointer sample = SampleType::New();
  sample->SetMeasurementVectorSize( SampleDimension );
 
  m_NumberOfPixels = 0;

  while(!it.IsAtEnd())
    {
    IndexType ind = it.GetIndex();
    VectorType mv ;
    mv[0] = it.Get();
    for(unsigned int i=1;i<itkGetStaticConstMacro(SampleDimension);i++)
      {
      ind[m_SampleDirection] += 1;
      mv[i]= m_Image->GetPixel(ind);
      m_Sum += static_cast< AccumulateType >(mv[i]);
      }
    sample->PushBack( mv );
    m_NumberOfPixels++;
    ++it;
    }

  typedef itk::Statistics::MeanCalculator< SampleType >
    MeanAlgorithmType ;
  
  typename MeanAlgorithmType::Pointer meanAlgorithm = MeanAlgorithmType::New() ;
  meanAlgorithm->SetInputSample( sample ) ;
  meanAlgorithm->Update() ;

  typename MeanAlgorithmType::OutputType mean = 
                                *(meanAlgorithm->GetOutput());
  for( unsigned int i=0; i< SampleDimension; i++ )
    {
    m_Mean[i] = mean[i];
    }

  typedef itk::Statistics::CovarianceCalculator< SampleType >
    CovarianceAlgorithmType ;
  
  typename CovarianceAlgorithmType::Pointer covarianceAlgorithm = 
    CovarianceAlgorithmType::New() ;

  covarianceAlgorithm->SetInputSample( sample ) ;
  covarianceAlgorithm->SetMean( meanAlgorithm->GetOutput() ) ;
  covarianceAlgorithm->Update();
  
  typename CovarianceAlgorithmType::OutputType covarianceMatrix
      = *(covarianceAlgorithm->GetOutput());
  for( unsigned int i=0; i< covarianceMatrix.Rows(); i++ )
    {
    for( unsigned int j=0; j< covarianceMatrix.Rows(); j++ )
      {
      m_CovarianceMatrix(i,j) = covarianceMatrix(i,j);
      }
    }
}

template<class TInputImage,class TInputSpatialObject, unsigned int TSampleDimension>
void
SpatialObjectToImageStatisticsCalculator<TInputImage,TInputSpatialObject,TSampleDimension>
::PrintSelf( std::ostream& os, Indent indent ) const
{
  Superclass::PrintSelf(os,indent);
  os << indent << "Image: " << m_Image << std::endl;
  os << indent << "SpatialObject: " << m_SpatialObject << std::endl;
  os << indent << "Mean: " << m_Mean << std::endl;
  os << indent << "Covariance Matrix: " << m_CovarianceMatrix << std::endl;
  os << indent << "Sum: " << m_Sum << std::endl;
  os << indent << "m_NumberOfPixels: " << m_NumberOfPixels << std::endl;
  os << indent << "Internal Image Time: " << m_InternalImageTime << std::endl;
  os << indent << "Internal Spatial Object Time: " << m_InternalSpatialObjectTime << std::endl;
  os << indent << "SampleDirection: " << m_SampleDirection << std::endl;
}

} // end namespace itk

#endif
