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

#ifndef ossimEnvisatAsarModel_H
#define ossimEnvisatAsarModel_H

#include <otb/JSDDateTime.h>
#include <ossimGeometricSarSensorModel.h>
#include <ossim/projection/ossimMapProjection.h>
#include <ossim/base/ossimIpt.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimGpt.h>
#include <ossim/base/ossimDpt.h>

#include <iostream>

class PlatformPosition;
class SensorParams;
class RefPoint;
/**
 * @brief This class is able to direct localisation and indirect localisation
 * using the EnvisatAsar sensor model
 */
class ossimEnvisatAsarModel : public ossimGeometricSarSensorModel
{
public:
	/**
	 * @brief Constructor
	 */
	ossimEnvisatAsarModel();

	/**
	 * @brief Destructor
	 */
	~ossimEnvisatAsarModel();

	/**
	 * @brief This function associates an image column number to a slant range when the image is georeferenced (ground projected)
	 * @param col Column coordinate of the image point
	 */
	virtual double getSlantRangeFromGeoreferenced(double col) const;

protected:

	/**
	 *	@brief Slant Range to Ground Range (SRGR) number of coefficients sets
	 */
	int  _n_srgr;
	/**
	 * @brief SRGR coefficient sets
	 */
	std::vector< std::vector<double> > _srgr_coefset;
	/**
	 * @brief SRGR coefficient sets update times
	 */
	std::vector<JSDDateTime> _srgr_update;
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
	std::vector<double> FindSRGRSetNumber(JSDDateTime date)  const;
	TYPE_DATA
};

#endif
