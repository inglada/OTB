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
#ifndef __otbTerraSarBrightnessImageFilter_txx
#define __otbTerraSarBrightnessImageFilter_txx

#include "otbTerraSarBrightnessImageFilter.h"
#include "otbImageMetadataInterfaceFactory.h"
#include "otbImageMetadataInterfaceBase.h"


namespace otb
{

/**
 * Constructor
 */
template <class TInputImage, class TOutputImage>
TerraSarBrightnessImageFilter<TInputImage,TOutputImage>
::TerraSarBrightnessImageFilter()
{
//  m_CalFactor = 1.;
}

template <class TInputImage, class TOutputImage>
void
TerraSarBrightnessImageFilter<TInputImage,TOutputImage>
::BeforeThreadedGenerateData()
{
  Superclass::BeforeThreadedGenerateData();
}

}

#endif
