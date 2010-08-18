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
#ifndef __otbSpotImageMetadataInterface_h
#define __otbSpotImageMetadataInterface_h

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "otbOpticalImageMetadataInterface.h"

namespace otb
{
/** \class SpotImageMetadataInterface
 *
 * \brief Creation of an "otb" SpotImageMetadataInterface that gets metadata.
 *
 */
class ITK_EXPORT SpotImageMetadataInterface : public OpticalImageMetadataInterface
{
public:

  typedef SpotImageMetadataInterface    Self;
  typedef OpticalImageMetadataInterface    Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(SpotImageMetadataInterface, OpticalImageMetadataInterface);

  typedef Superclass::ImageType                ImageType;
  typedef Superclass::MetaDataDictionaryType   MetaDataDictionaryType;
  typedef Superclass::VectorType               VectorType;
  typedef Superclass::VariableLengthVectorType VariableLengthVectorType;
  typedef Superclass::ImageKeywordlistType     ImageKeywordlistType;

  /** Get the radiometric bias from the ossim metadata */
  VariableLengthVectorType GetPhysicalBias() const;

  /** Get the radiometric gain from the ossim metadata */
  VariableLengthVectorType GetPhysicalGain() const;

  /** Get the solar irradiance from the ossim metadata */
  VariableLengthVectorType GetSolarIrradiance() const;

  /** Get the imaging acquisition day from the ossim metadata : IMAGING_DATE metadata variable */
  int GetDay() const;

  /** Get the imaging acquisition month from the ossim metadata : IMAGING_DATE metadata variable */
  int GetMonth() const;

  /** Get the imaging acquisition year from the ossim metadata : IMAGING_DATE metadata variable */
  int GetYear() const;

  /** Get the imaging acquisition hour from the ossim metadata : IMAGING_DATE metadata variable */
  int GetHour() const;

  /** Get the imaging acquisition year from the ossim metadata : IMAGING_DATE metadata variable */
  int GetMinute() const;

  /** Get the imaging production day from the ossim metadata : DATASET_PRODUCTION_DATE metadata variable */
  int GetProductionDay() const;

  /** Get the imaging production month from the ossim metadata : DATASET_PRODUCTION_DATE metadata variable */
  int GetProductionMonth() const;

  /** Get the imaging production year from the ossim metadata : DATASET_PRODUCTION_DATE metadata variable */
  int GetProductionYear() const;

  /** Get the sat elevation from the ossim metadata */
  double GetSatElevation() const;

  /** Get the sat azimuth from the ossim metadata */
  double GetSatAzimuth() const;

  /** Get the first wavelength for the spectral band definition */
  VariableLengthVectorType GetFirstWavelengths() const;

  /** Get the last wavelength for the spectral band definition */
  VariableLengthVectorType GetLastWavelengths() const;

  /** Get Instrument */
  std::string GetInstrument() const;

  /** Get Instrument Index */
  unsigned int GetInstrumentIndex() const;

  /** This method is to handle the permutation of the spectral band by some image provider
   * in most cases, this method won't change the value, but for SPOT data, the bands are set up as
   *  2 1 0 3 in the tiff file, this method which is overloaded for SPOT enables to retrieve the
   *  proper band. */
  virtual unsigned int BandIndexToWavelengthPosition(unsigned int i) const;

  /** Get the 3 spectral band numbers corresponding to the default display for visualization,
   *  in the order R, G, B */
  std::vector<unsigned int> GetDefaultDisplay() const;

  bool CanRead() const;

protected:
  SpotImageMetadataInterface();
  virtual ~SpotImageMetadataInterface() {}

private:

  SpotImageMetadataInterface(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

};

} // end namespace otb

#endif
