//----------------------------------------------------------------------------
//
// "Copyright Centre National d'Etudes Spatiales"
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
//----------------------------------------------------------------------------
// $Id$

#ifndef ossimRadarSatModel_H
#define ossimRadarSatModel_H

#include <otb/JSDDateTime.h>

#include <ossimGeometricSarSensorModel.h>
#include <ossim/projection/ossimMapProjection.h>
#include <ossim/base/ossimIpt.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimGpt.h>
#include <ossim/base/ossimDpt.h>

#include <iostream>

namespace ossimplugins
{


class PlatformPosition;
class SensorParams;
class RefPoint;
/**
 * @brief This class is able to direct localisation and indirect localisation using the RadarSat sensor model
 */
class ossimRadarSatModel : public ossimGeometricSarSensorModel
{
public:
	/** @brief Constructor */
	ossimRadarSatModel();

	/** @brief Destructor */
	virtual ~ossimRadarSatModel();

        /**
         * @brief Method to return the class name.
         * @return The name of this class.
         */
        virtual ossimString getClassName()   const;

	/**
	 * @brief This function associates an image column number to a slant range when the image is georeferenced (ground projected)
	 * @param col Column coordinate of the image point
	 */
	virtual double getSlantRangeFromGeoreferenced(double col) const;

protected:

	/**
	 *	@brief Slant Range for each Ground Range (SRGR) number of coefficients sets
	 */
	int   _n_srgr;
	/**
	 * @brief SRGR coefficient sets
	 */
	double _srgr_coefset[20][6];
	/**
	 * @brief SRGR coefficient sets update times
	 */
	double _srgr_update[20];
	/**
	 * @brief Pixel spacing
	 */
	double _pixel_spacing;

private:
	virtual bool InitPlatformPosition(const ossimKeywordlist &kwl, const char *prefix);
	virtual bool InitSensorParams(const ossimKeywordlist &kwl, const char *prefix);
	virtual bool InitRefPoint(const ossimKeywordlist &kwl, const char *prefix);
	/**
	 * @brief Initializes the Slant Range for each Ground Range data sets : _n_srgr,_srgr_coefset,_srgr_update,_pixel_spacing,_isProductGeoreferenced
	 */
	virtual bool InitSRGR(const ossimKeywordlist &kwl, const char *prefix);
	/**
	 * @brief Finds the SRGR data set which update time is the closest to the center scene time
	 */
	int FindSRGRSetNumber(JSDDateTime date)  const;
	TYPE_DATA

};
}

#endif
