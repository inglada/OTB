/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


  Some parts of this code are derived from ITK. See ITKCopyright.txt
  for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbTerraSarBrightnessFunctor_h
#define __otbTerraSarBrightnessFunctor_h

#include "otbMath.h"

#include <complex>

namespace otb
{
namespace Functor
{

/**
   * \class TerraSarBrightnessFunctor
   *  \brief Compute the radar brightness from an modulus image.
   *
   * \ingroup Functor
   * \ingroup Radiometry
 */
template <class TInput, class TOutput>
class TerraSarBrightnessFunctor
{
public:
  TerraSarBrightnessFunctor();
 virtual ~TerraSarBrightnessFunctor() {};
 
 /** Accessors */
 void SetCalibrationFactor( double val ) { m_CalibrationFactor = val; };
 double GetCalibrationFactor() { return m_CalibrationFactor; };

  /** Set results in decibels */
  void SetResultsInDecibels(bool db)
  {
    m_ResultsInDecibels = db;
  }

  /** Get the value of the flag */
  bool GetResultsInDecibels()
  {
    return m_ResultsInDecibels;
  }
 
 /** We assume that the input pixel is a scalar -> modulus image */
 inline TOutput operator() (const TInput & inPix);
 /** We assume that the input pixel is a complex -> complex image */
 inline std::complex<TOutput> operator() (const std::complex<TInput> & inPix);

private:
  /** Calibration Factor */
  double m_CalibrationFactor;

  /** Shall we compute results in decibels ? */
  bool m_ResultsInDecibels;
};

}// end namespace functor
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbTerraSarBrightnessFunctor.txx"
#endif

#endif
