/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkStatisticsLabelMapFilter.txx,v $
  Language:  C++
  Date:      $Date: 2010-07-12 14:50:26 $
  Version:   $Revision: 1.5 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkStatisticsLabelMapFilter_txx
#define __itkStatisticsLabelMapFilter_txx

#include "itkStatisticsLabelMapFilter.h"
#include "itkMinimumMaximumImageCalculator.h"
#include "itkProgressReporter.h"
#include "vnl/algo/vnl_real_eigensystem.h"
#include "vnl/algo/vnl_symmetric_eigensystem.h"


namespace itk {

template <class TImage, class TFeatureImage>
StatisticsLabelMapFilter<TImage, TFeatureImage>
::StatisticsLabelMapFilter()
{
  m_NumberOfBins = 128;
  m_ComputeHistogram = true;
  this->SetNumberOfRequiredInputs(2);
}


template <class TImage, class TFeatureImage>
void
StatisticsLabelMapFilter<TImage, TFeatureImage>
::BeforeThreadedGenerateData()
{
  Superclass::BeforeThreadedGenerateData();

  // get the min and max of the feature image, to use those value as the bounds of our
  // histograms
  typedef MinimumMaximumImageCalculator< FeatureImageType > MinMaxCalculatorType;
  typename MinMaxCalculatorType::Pointer minMax = MinMaxCalculatorType::New();
  minMax->SetImage( this->GetFeatureImage() );
  minMax->Compute();

  m_Minimum = minMax->GetMinimum();
  m_Maximum = minMax->GetMaximum();

}


template <class TImage, class TFeatureImage>
void
StatisticsLabelMapFilter<TImage, TFeatureImage>
::ThreadedProcessLabelObject( LabelObjectType * labelObject )
{
  Superclass::ThreadedProcessLabelObject( labelObject );

  ImageType * output = this->GetOutput();
  const FeatureImageType * featureImage = this->GetFeatureImage();

  typedef typename LabelObjectType::HistogramType HistogramType;

  typename HistogramType::SizeType histogramSize;
#ifdef ITK_USE_REVIEW_STATISTICS  //http://www.itk.org/Wiki/Proposals:Refactoring_Statistics_Framework_2007_Migration_Users_Guide
  histogramSize.SetSize(1);
#endif
  histogramSize.Fill( m_NumberOfBins );

  typename HistogramType::MeasurementVectorType featureImageMin;
#ifdef ITK_USE_REVIEW_STATISTICS  //http://www.itk.org/Wiki/Proposals:Refactoring_Statistics_Framework_2007_Migration_Users_Guide
  featureImageMin.SetSize(1);
#endif
  featureImageMin.Fill( m_Minimum );

  typename HistogramType::MeasurementVectorType featureImageMax;
#ifdef ITK_USE_REVIEW_STATISTICS  //http://www.itk.org/Wiki/Proposals:Refactoring_Statistics_Framework_2007_Migration_Users_Guide
  featureImageMax.SetSize(1);
#endif
  featureImageMax.Fill( m_Maximum );

  typename HistogramType::Pointer histogram = HistogramType::New();
#ifdef ITK_USE_REVIEW_STATISTICS  //http://www.itk.org/Wiki/Proposals:Refactoring_Statistics_Framework_2007_Migration_Users_Guide
  histogram->SetMeasurementVectorSize(1);
#endif
  histogram->SetClipBinsAtEnds( false );
  histogram->Initialize( histogramSize, featureImageMin, featureImageMax );

  typename LabelObjectType::LineContainerType::const_iterator lit;
  typename LabelObjectType::LineContainerType & lineContainer = labelObject->GetLineContainer();

  FeatureImagePixelType min = NumericTraits< FeatureImagePixelType >::max();
  FeatureImagePixelType max = NumericTraits< FeatureImagePixelType >::NonpositiveMin();
  double sum = 0;
  double sum2 = 0;
  double sum3 = 0;
  double sum4 = 0;
  IndexType minIdx;
  minIdx.Fill( 0 );
  IndexType maxIdx;
  maxIdx.Fill( 0 );
  PointType centerOfGravity;
  centerOfGravity.Fill( 0 );
  MatrixType centralMoments;
  centralMoments.Fill( 0 );
  MatrixType principalAxes;
  principalAxes.Fill( 0 );
  VectorType principalMoments;
  principalMoments.Fill( 0 );

  // iterate over all the lines
  for( lit = lineContainer.begin(); lit != lineContainer.end(); lit++ )
    {
    const IndexType & firstIdx = lit->GetIndex();
    unsigned long length = lit->GetLength();

    typename HistogramType::MeasurementVectorType mv;
#ifdef ITK_USE_REVIEW_STATISTICS  //http://www.itk.org/Wiki/Proposals:Refactoring_Statistics_Framework_2007_Migration_Users_Guide
    mv.SetSize(1);
#endif
    long endIdx0 = firstIdx[0] + length;
    for( IndexType idx = firstIdx; idx[0]<endIdx0; idx[0]++)
      {
      const FeatureImagePixelType & v = featureImage->GetPixel( idx );
      mv[0] = v;
      histogram->IncreaseFrequency( mv, 1 );

      // update min and max
      if( v <= min )
        {
        min = v;
        minIdx = idx;
        }
      if( v >= max )
        {
        max = v;
        maxIdx = idx;
        }

      //increase the sums
      sum += v;
      sum2 += vcl_pow( (double)v, 2 );
      sum3 += vcl_pow( (double)v, 3 );
      sum4 += vcl_pow( (double)v, 4 );

      // moments
      PointType physicalPosition;
      output->TransformIndexToPhysicalPoint(idx, physicalPosition);
      for(unsigned int i=0; i<ImageDimension; i++)
        {
        centerOfGravity[i] += physicalPosition[i] * v; 
        centralMoments[i][i] += v * physicalPosition[i] * physicalPosition[i];
        for(unsigned int j=i+1; j<ImageDimension; j++)
          {
          double weight = v * physicalPosition[i] * physicalPosition[j];
          centralMoments[i][j] += weight;
          centralMoments[j][i] += weight;
          }
        }

      }
    }

  // final computations
#ifdef ITK_USE_REVIEW_STATISTICS  //http://www.itk.org/Wiki/Proposals:Refactoring_Statistics_Framework_2007_Migration_Users_Guide
  const typename HistogramType::AbsoluteFrequencyType & totalFreq = histogram->GetTotalFrequency();
#else
  const typename HistogramType::FrequencyType & totalFreq = histogram->GetTotalFrequency();
#endif
  double mean = sum / totalFreq;
  double variance = ( sum2 - ( vcl_pow( sum, 2 ) / totalFreq ) ) / ( totalFreq - 1 );
  double sigma = vcl_sqrt( variance );
  double mean2 = mean * mean;
  double skewness = ( ( sum3 - 3.0 * mean * sum2) / totalFreq + 2.0 * mean * mean2 ) / ( variance * sigma );
  double kurtosis = ( ( sum4 - 4.0 * mean * sum3 + 6.0 * mean2 * sum2) / totalFreq - 3.0 * mean2 * mean2 ) / ( variance * variance ) - 3.0;

  // the median
  double median = 0;
  double count = 0;  // will not be fully set, so do not use later !
  for( unsigned long i=0;
    i<histogram->Size();
    i++)
    {
    count += histogram->GetFrequency( i );

    if( count >= ( totalFreq / 2 ) )
      {
      median = histogram->GetMeasurementVector( i )[0];
      break;
      }
    }

  double elongation = 0;
  double flatness = 0;
  if( sum != 0 )
    {
    // Normalize using the total mass
    for(unsigned int i=0; i<ImageDimension; i++)
      {
      centerOfGravity[i] /= sum;
      for(unsigned int j=0; j<ImageDimension; j++)
        {
        centralMoments[i][j] /= sum;
        }
      }
  
    // Center the second order moments
    for(unsigned int i=0; i<ImageDimension; i++)
      {
      for(unsigned int j=0; j<ImageDimension; j++)
        {
        centralMoments[i][j] -= centerOfGravity[i] * centerOfGravity[j];
        }
      }
  
    // the normalized second order central moment of a pixel
    for(unsigned int i=0; i<ImageDimension; i++)
      {
      centralMoments[i][i] += output->GetSpacing()[i] * output->GetSpacing()[i] / 12.0;
      }

    // Compute principal moments and axes
    vnl_symmetric_eigensystem<double> eigen( centralMoments.GetVnlMatrix() );
    vnl_diag_matrix<double> pm = eigen.D;
    for(unsigned int i=0; i<ImageDimension; i++)
      {
  //    principalMoments[i] = 4 * vcl_sqrt( pm(i,i) );
      principalMoments[i] = pm(i,i);
      }
    principalAxes = eigen.V.transpose();
  
    // Add a final reflection if needed for a proper rotation,
    // by multiplying the last row by the determinant
    vnl_real_eigensystem eigenrot( principalAxes.GetVnlMatrix() );
    vnl_diag_matrix< vcl_complex<double> > eigenval = eigenrot.D;
    vcl_complex<double> det( 1.0, 0.0 );
  
    for(unsigned int i=0; i<ImageDimension; i++)
      {
      det *= eigenval( i, i );
      }
  
    for(unsigned int i=0; i<ImageDimension; i++)
      {
      principalAxes[ ImageDimension-1 ][i] *= std::real( det );
      }
  
    if( ImageDimension < 2 )
      {
      elongation = 1;
      flatness = 1;
      }
    else if( principalMoments[0] != 0 )
      {
  //    elongation = principalMoments[ImageDimension-1] / principalMoments[0];
      elongation = vcl_sqrt(principalMoments[ImageDimension-1] / principalMoments[ImageDimension-2]);
      flatness = vcl_sqrt(principalMoments[1] / principalMoments[0]);
      }
    }
  else
    {
    // can't compute anything in that case - just set everything to a default value
    // Normalize using the total mass
    for(unsigned int i=0; i<ImageDimension; i++)
      {
      centerOfGravity[i] = 0;
      principalMoments[i] = 0;
      for(unsigned int j=0; j<ImageDimension; j++)
        {
        principalAxes[i][j] = 0;
        }
      }
    }

  // finally put the values in the label object
  labelObject->SetMinimum( (double)min );
  labelObject->SetMaximum( (double)max );
  labelObject->SetSum( sum );
  labelObject->SetMean( mean );
  labelObject->SetMedian( median );
  labelObject->SetVariance( variance );
  labelObject->SetSigma( sigma );
  labelObject->SetMinimumIndex( minIdx );
  labelObject->SetMaximumIndex( maxIdx );
  labelObject->SetCenterOfGravity( centerOfGravity );
  labelObject->SetPrincipalAxes( principalAxes );
  labelObject->SetFlatness( flatness );
  labelObject->SetPrincipalMoments( principalMoments );
  // labelObject->SetCentralMoments( centralMoments );
  labelObject->SetSkewness( skewness );
  labelObject->SetKurtosis( kurtosis );
  labelObject->SetElongation( elongation );
  if( m_ComputeHistogram )
    {
    labelObject->SetHistogram( histogram );
    }

}


template <class TImage, class TFeatureImage>
void
StatisticsLabelMapFilter<TImage, TFeatureImage>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);
  
  os << indent << "ComputeHistogram: " << m_ComputeHistogram << std::endl;
  os << indent << "NumberOfBins: " << m_NumberOfBins << std::endl;
}


}// end namespace itk
#endif
