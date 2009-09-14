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
#ifndef __otbRadarFunctors_txx
#define __otbRadarFunctors_txx

#include "otbRadarFunctors.h"


namespace otb
{
/**************************************************************************
 ***************** TerraSarCalibrationImageFunctor *******************
 **************************************************************************/
namespace Functor
{
/** Constructor */
template <class TInputIt, class TOutput>
TerraSarCalibrationImageFunctor<TInputIt, TOutput>
::TerraSarCalibrationImageFunctor()
{
    m_CalFactor = 1.;
    m_NoiseRangeValidityMin = 0.;
    m_NoiseRangeValidityMax = 0.;
    m_NoiseRangeValidityRef = 0.;
    m_LocalIncidentAngle = 0.;
    m_NoisePolynomialCoefficientsList.clear();
    m_ImageSize.Fill(0);
    m_UseFastCalibrationMethod = true;
    m_TimeUTC.clear();
    m_PRF = 1.;
}

  
template <class TInputIt, class TOutput>
double
TerraSarCalibrationImageFunctor<TInputIt, TOutput>
::ComputeCurrentNoise( unsigned int colId )
  {
    double curRange = 0.;
    double width_2 = static_cast<double>(m_ImageSize[0])/2.;

    // Use +1 because image start index is 0
    if( colId < static_cast<unsigned int>(width_2) )
      {
	curRange = m_NoiseRangeValidityMin + ( m_NoiseRangeValidityRef-m_NoiseRangeValidityMin )/width_2 * static_cast<double>(colId+1);
      }
    else
      {
	curRange = m_NoiseRangeValidityRef + ( m_NoiseRangeValidityMax-m_NoiseRangeValidityRef )/width_2 * (static_cast<double>(colId+1) - width_2 );
      }
 
    return curRange;
  }

   
template <class TInputIt, class TOutput>
typename TerraSarCalibrationImageFunctor<TInputIt, TOutput>::DoubleVectorType
TerraSarCalibrationImageFunctor<TInputIt, TOutput>
::ComputeCurrentCoeffs( unsigned int lineId )
  {
    DoubleVectorType curCoeffs;
    if(m_UseFastCalibrationMethod)
      { 
	curCoeffs = m_NoisePolynomialCoefficientsList[0];
      }
    else
      {
	// m_ImageSize[1]-(lineId+1) because the first acquisition line is the last image one.
	// line+1 because image starts to 0.
	double interval =  static_cast<double>(m_ImageSize[1]) / static_cast<double>(m_NoisePolynomialCoefficientsList.size());
	// compute utc time of the line
	double currTimeUTC = m_TimeUTC[0] + static_cast<double>(m_ImageSize[1]-(lineId-1))*m_InvPRF;
	unsigned int id = 0;
	bool go = true;
	// deduct the corresponding noise acquisition index
	while( id<m_TimeUTC.size() && go)
	  { 
	    if( currTimeUTC < m_TimeUTC[id] )
		go = false;
	    id++;
	  }
	id--;

	double timeCoef = 1. / (m_TimeUTC[id]- m_TimeUTC[id-1]) * (currTimeUTC-m_TimeUTC[id-1]);
	for(unsigned int j=0; j<m_NoisePolynomialCoefficientsList.size(); j++)
	  {
	    curCoeffs.push_back( m_NoisePolynomialCoefficientsList[id-1][j] + (m_NoisePolynomialCoefficientsList[id][j] - m_NoisePolynomialCoefficientsList[id-1][j]) * timeCoef );
	  }
    }


    return curCoeffs;
  }

template <class TInputIt, class TOutput>
inline TOutput
TerraSarCalibrationImageFunctor<TInputIt, TOutput>
::operator() (const TInputIt & inIt)
{
  double diffCurRange = ComputeCurrentNoise( static_cast<unsigned int>(inIt.GetIndex()[0]) ) - m_NoiseRangeValidityRef;
  DoubleVectorType curCoeff = ComputeCurrentCoeffs( static_cast<unsigned int>(inIt.GetIndex()[1]) );
  
  TOutput outRadBr = m_RadarBrightness( inIt.GetCenterPixel() );

  double NEBN = 0.;
  for(int i=0; i<curCoeff.size(); i++)
    {
      NEBN += curCoeff[i]*vcl_pow( diffCurRange, i);
    }
  double sigma = ( outRadBr - m_CalFactor*NEBN ) * m_SinLocalIncidentAngle;
  
  return static_cast<TOutput>(sigma);
}
  
}// namespace Functor
  
} // namespace otb
#endif
