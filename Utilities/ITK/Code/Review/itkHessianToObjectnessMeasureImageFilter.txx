/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkHessianToObjectnessMeasureImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2010-06-24 12:35:05 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkHessianToObjectnessMeasureImageFilter_txx
#define __itkHessianToObjectnessMeasureImageFilter_txx

#include "itkHessianToObjectnessMeasureImageFilter.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkSymmetricEigenAnalysis.h"
#include "itkProgressReporter.h"

#include "vnl/vnl_math.h"

namespace itk
{

/**
 * Constructor
 */
template < typename TInputImage, typename TOutputImage >
HessianToObjectnessMeasureImageFilter< TInputImage, TOutputImage>
::HessianToObjectnessMeasureImageFilter()
{
  m_Alpha = 0.5;
  m_Beta = 0.5;
  m_Gamma = 5.0;


  m_ScaleObjectnessMeasure = true;

  // by default extract bright lines (equivalent to vesselness)
  m_ObjectDimension = 1;
  m_BrightObject = true;
}


template < typename TInputImage, typename TOutputImage >
void
HessianToObjectnessMeasureImageFilter< TInputImage, TOutputImage>
::BeforeThreadedGenerateData( void )
{
  if (m_ObjectDimension >= ImageDimension)
    {
    itkExceptionMacro( "ObjectDimension must be lower than ImageDimension." );
    }

}

template < typename TInputImage, typename TOutputImage >
void
HessianToObjectnessMeasureImageFilter< TInputImage, TOutputImage>
::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                       int threadId)
{
  typename OutputImageType::Pointer output = this->GetOutput();
  typename InputImageType::ConstPointer input = this->GetInput();

  // support progress methods/callbacks
  ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels(), 1000 / this->GetNumberOfThreads() );

  // calculator for computation of the eigen values
  typedef SymmetricEigenAnalysis< InputPixelType, EigenValueArrayType > CalculatorType;
  CalculatorType eigenCalculator( ImageDimension );


  // walk the region of eigen values and get the objectness measure
  ImageRegionConstIterator<InputImageType> it(input, outputRegionForThread);
  ImageRegionIterator<OutputImageType> oit(output, outputRegionForThread );

  oit.GoToBegin();
  it.GoToBegin();

  while (!it.IsAtEnd())
    {
    // compute eigen values
    EigenValueArrayType eigenValues;
    eigenCalculator.ComputeEigenValues( it.Get(), eigenValues );

    // Sort the eigenvalues by magnitude but retain their sign
    EigenValueArrayType sortedEigenValues = eigenValues;
    std::sort( sortedEigenValues.Begin(), sortedEigenValues.End(), AbsCompare() );


    // check whether eigenvalues have the right sign
    bool signConstraintsSatisfied = true;
    for (unsigned int i=m_ObjectDimension; i<ImageDimension; i++)
      {
      if ((m_BrightObject && sortedEigenValues[i] > 0.0) ||
          (!m_BrightObject && sortedEigenValues[i] < 0.0) )
        {
        signConstraintsSatisfied = false;
        break;
        }
      }

    if (!signConstraintsSatisfied)
      {
      oit.Set(NumericTraits< OutputPixelType >::Zero);
      ++it;
      ++oit;
      progress.CompletedPixel();
      continue;
      }

    EigenValueArrayType sortedAbsEigenValues;
    for (unsigned int i=0; i<ImageDimension; i++)
      {
      sortedAbsEigenValues[i] = vnl_math_abs(sortedEigenValues[i]);
      }

    // initialize the objectness measure
    double objectnessMeasure = 1.0;

    // compute objectness from eigenvalue ratios and second-order structureness
    if (m_ObjectDimension < ImageDimension-1)
      {
      double rA = sortedAbsEigenValues[m_ObjectDimension];
      double rADenominatorBase = 1.0;
      for (unsigned int j=m_ObjectDimension+1; j<ImageDimension; j++)
        {
        rADenominatorBase *= sortedAbsEigenValues[j];
        }
      if (vcl_fabs(rADenominatorBase) > 0.0)
        {
        if ( vcl_fabs( m_Alpha ) > 0.0 )
          {
          rA /= vcl_pow(rADenominatorBase, 1.0 / (ImageDimension-m_ObjectDimension-1));
          objectnessMeasure *= 1.0 - vcl_exp(- 0.5 * vnl_math_sqr(rA) / vnl_math_sqr(m_Alpha));
          }
        }
      else
        {
        objectnessMeasure = 0.0;
        }
      }

    if (m_ObjectDimension > 0)
      {
      double rB = sortedAbsEigenValues[m_ObjectDimension-1];
      double rBDenominatorBase = 1.0;
      for (unsigned int j=m_ObjectDimension; j<ImageDimension; j++)
        {
        rBDenominatorBase *= sortedAbsEigenValues[j];
        }
      if (vcl_fabs(rBDenominatorBase) > 0.0 && vcl_fabs( m_Beta ) > 0.0 )
        {
        rB /= vcl_pow(rBDenominatorBase, 1.0 / (ImageDimension-m_ObjectDimension));

        objectnessMeasure *= vcl_exp(- 0.5 * vnl_math_sqr(rB) / vnl_math_sqr(m_Beta));

        }
      else
        {
        objectnessMeasure = 0.0;
        }
      }

    if ( vcl_fabs( m_Gamma ) > 0.0 )
      {
      double frobeniusNormSquared = 0.0;
      for (unsigned int i=0; i<ImageDimension; i++)
        {
        frobeniusNormSquared += vnl_math_sqr(sortedAbsEigenValues[i]);
        }
      objectnessMeasure *= 1.0 - vcl_exp(- 0.5 * frobeniusNormSquared / vnl_math_sqr(m_Gamma));
      }

    // in case, scale by largest absolute eigenvalue
    if (m_ScaleObjectnessMeasure)
      {
      objectnessMeasure *= sortedAbsEigenValues[ImageDimension-1];
      }


    oit.Set( static_cast< OutputPixelType >(objectnessMeasure));


    ++it;
    ++oit;
    progress.CompletedPixel();
    }
}

template < typename TInputImage, typename TOutputImage >
void
HessianToObjectnessMeasureImageFilter< TInputImage, TOutputImage>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "Alpha: " << m_Alpha << std::endl;
  os << indent << "Beta: " << m_Beta << std::endl;
  os << indent << "Gamma: " << m_Gamma << std::endl;
  os << indent << "ScaleObjectnessMeasure: " << m_ScaleObjectnessMeasure << std::endl;
  os << indent << "ObjectDimension: " << m_ObjectDimension << std::endl;
  os << indent << "BrightObject: " << m_BrightObject << std::endl;
}

} // end namespace itk

#endif
