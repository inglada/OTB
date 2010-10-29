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

#ifndef __otbSarRadiometricCalibrationFunction_txx
#define __otbSarRadiometricCalibrationFunction_txx

#include "otbSarRadiometricCalibrationFunction.h"
#include "otbSarParametricMapFunction.h"
#include "itkNumericTraits.h"

namespace otb
{

/**
 * Constructor
 */
template <class TInputImage, class TCoordRep>
SarRadiometricCalibrationFunction<TInputImage, TCoordRep>
::SarRadiometricCalibrationFunction():
  m_Scale(1.0)
{
  m_Noise = ParametricFunctionType::New();
  m_AntennaPatternNewGain = ParametricFunctionType::New();
  m_AntennaPatternOldGain = ParametricFunctionType::New();
  m_IncidenceAngle = ParametricFunctionType::New();
  m_RangeSpreadLoss = ParametricFunctionType::New();

  m_Noise->SetConstantValue(0.0);
  m_AntennaPatternNewGain->SetConstantValue(1.0);
  m_AntennaPatternOldGain->SetConstantValue(1.0);
  m_IncidenceAngle->SetConstantValue(CONST_PI_2);
  m_RangeSpreadLoss->SetConstantValue(1.0);
}

/**
 * Initialize by setting the input image
 */
template <class TInputImage, class TCoordRep>
void
SarRadiometricCalibrationFunction<TInputImage, TCoordRep>
::SetInputImage(
  const InputImageType * ptr )
{
  Superclass::SetInputImage(ptr);
  m_Noise->SetInputImage(ptr);
  m_IncidenceAngle->SetInputImage(ptr);
  m_AntennaPatternNewGain->SetInputImage(ptr);
  m_AntennaPatternOldGain->SetInputImage(ptr);
  m_RangeSpreadLoss->SetInputImage(ptr);
}

/**
 *
 */
template <class TInputImage, class TCoordRep>
void
SarRadiometricCalibrationFunction<TInputImage, TCoordRep>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  this->Superclass::PrintSelf(os, indent);
}

/**
 *
 */
template <class TInputImage, class TCoordRep>
typename SarRadiometricCalibrationFunction<TInputImage, TCoordRep>
::OutputType
SarRadiometricCalibrationFunction<TInputImage, TCoordRep>
::EvaluateAtIndex(const IndexType& index) const
{
  RealType result;
  result = itk::NumericTraits<RealType>::Zero;

  if (!this->GetInputImage())
    {
    return (itk::NumericTraits<OutputType>::max());
    }

  if (!this->IsInsideBuffer(index))
    {
    return (itk::NumericTraits<OutputType>::max());
    }


  FunctorRealType noise;
  FunctorRealType antennaPatternNewGain;
  FunctorRealType antennaPatternOldGain;
  FunctorRealType incidenceAngle;
  FunctorRealType rangeSpreadLoss;

  noise = static_cast<FunctorRealType>(m_Noise->EvaluateAtIndex(index));
  antennaPatternNewGain = static_cast<FunctorRealType>(m_AntennaPatternNewGain->EvaluateAtIndex(index));
  antennaPatternOldGain = static_cast<FunctorRealType>(m_AntennaPatternOldGain->EvaluateAtIndex(index));
  incidenceAngle = static_cast<FunctorRealType>(m_IncidenceAngle->EvaluateAtIndex(index));
  rangeSpreadLoss = static_cast<FunctorRealType>(m_RangeSpreadLoss->EvaluateAtIndex(index));

  FunctorType functor;
  functor.SetNoise(noise);
  functor.SetScale(m_Scale);
  functor.SetAntennaPatternNewGain(antennaPatternNewGain);
  functor.SetAntennaPatternOldGain(antennaPatternOldGain);
  functor.SetIncidenceAngle(incidenceAngle);
  functor.SetRangeSpreadLoss(rangeSpreadLoss);

  const RealType value = static_cast<RealType>(vcl_abs(this->GetInputImage()->GetPixel(index)));
  result = functor(value);

  return static_cast<OutputType>(result);
}

} // end namespace otb

#endif
