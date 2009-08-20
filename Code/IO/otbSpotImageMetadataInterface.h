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

#include "otbImageMetadataInterfaceBase.h"


namespace otb
{
/** \class SpotImageMetadataInterface
 *
 * \brief Creation of an "otb" SpotImageMetadataInterface that gets metadata.
 *
 */
class ITK_EXPORT SpotImageMetadataInterface : public ImageMetadataInterfaceBase
{
public:

  typedef SpotImageMetadataInterface Self;
  typedef ImageMetadataInterfaceBase Superclass;
  typedef itk::SmartPointer<Self>  Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(SpotImageMetadataInterface, ImageMetadataInterfaceBase);

  typedef Superclass::ImageType                ImageType;
  typedef Superclass::MetaDataDictionaryType   MetaDataDictionaryType;
  typedef Superclass::VectorType               VectorType;
  typedef Superclass::VariableLengthVectorType VariableLengthVectorType;
  typedef Superclass::ImageKeywordlistType     ImageKeywordlistType;


  /** Get the radiometric bias from the ossim metadata */
  VariableLengthVectorType GetPhysicalBias( const MetaDataDictionaryType & dict ) const;
  
  /** Get the radiometric gain from the ossim metadata */
  VariableLengthVectorType GetPhysicalGain( const MetaDataDictionaryType & dict ) const;
  
  /** Get the solar irradiance from the ossim metadata */
  VariableLengthVectorType GetSolarIrradiance( const MetaDataDictionaryType & dict ) const;
  
   /** Get the imaging day from the ossim metadata */
  int GetDay( const MetaDataDictionaryType & dict ) const;
  
  /** Get the imaging month from the ossim metadata */
  int GetMonth( const MetaDataDictionaryType & dict ) const;
  
  /** Get the imaging month from the ossim metadata */
  int GetYear( const MetaDataDictionaryType & dict ) const;
    
  /** Get the sat elevation from the ossim metadata */
  double GetSatElevation( const MetaDataDictionaryType & dict ) const;
  
  /** Get the sat azimuth from the ossim metadata */
  double GetSatAzimuth( const MetaDataDictionaryType & dict ) const;
  
  bool CanRead( const MetaDataDictionaryType & dict) const;

  
protected:
  SpotImageMetadataInterface();
  virtual ~SpotImageMetadataInterface() {};

  
private:

  SpotImageMetadataInterface(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

};

} // end namespace otb

#endif
