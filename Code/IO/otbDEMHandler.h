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
#ifndef __otbDEMHandler_h
#define __otbDEMHandler_h

#include "itkIndent.h"
#include "itkImageSource.h"
#include "otbImage.h"
#include <iostream>
#include <stdio.h>

#include "itkImageRegionIteratorWithIndex.h"
#include "itkIndent.h"

class ossimElevManager;

namespace otb
{
/** \class DEMHandler
 *
 * \brief Class for Reading a DEM data
 *
 * This class is based on ossimElevManager.
 * It allows to obtain height above MSL(Mean Sea Level) of a geographic point
 * Handle DTED and SRTM formats.
 * \ingroup Images
 *
 */

class ITK_EXPORT DEMHandler: public itk::Object
{
public :
  /** Standard class typedefs. */
  typedef DEMHandler                            Self;
  typedef itk::Object                           Superclass;
  typedef itk::SmartPointer<Self>               Pointer;
  typedef itk::SmartPointer<const Self>         ConstPointer;

  typedef itk::Point<double, 2>                 PointType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(DEMHandler,Object);

  /** Try to open the DEM directory. */
  virtual void OpenDEMDirectory(const char* DEMDirectory);

  /** Open geoid file. */
  virtual void OpenGeoidFile(const char* geoidFile);

  /** Compute the height above MSL(Mean Sea Level) of a geographic point. */
  virtual double GetHeightAboveMSL(const PointType& geoPoint) const;

  /** Compute the height above ellipsoid of a geographic point. */
  virtual double GetHeightAboveEllipsoid(const PointType& geoPoint) const;

protected:
  DEMHandler();
  virtual ~DEMHandler() {};

  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  ossimElevManager* m_ElevManager;

};

} // namespace otb


#endif
