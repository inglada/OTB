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

#include <ossimGeometricSarSensorModel.h>

#include <otb/GalileanEphemeris.h>
#include <otb/GeographicEphemeris.h>
#include <otb/GMSTDateTime.h>

#include <otb/PlatformPosition.h>
#include <otb/SensorParams.h>
#include <otb/RefPoint.h>
#include <otb/SarSensor.h>

#include <ossim/base/ossimTrace.h>

#include <cmath>
#include <string>

static const char PRODUCT_GEOREFERENCED_FLAG_KW[] =
   "product_georeferenced_flag";
static const char OPTIMIZATION_FACTOR_X_KW[] = "optimization_factor_x";
static const char OPTIMIZATION_FACTOR_Y_KW[] = "optimization_factor_y";
static const char OPTIMIZATION_BIAS_X_KW[] = "optimization_bias_x";
static const char OPTIMIZATION_BIAS_Y_KW[] = "optimization_bias_y";


RTTI_DEF1(ossimGeometricSarSensorModel, "ossimGeometricSarSensorModel", ossimSensorModel);

static ossimTrace traceDebug("ossimGeometricSarSensorModel:debug");

ossimGeometricSarSensorModel::ossimGeometricSarSensorModel()
   :
   ossimSensorModel(),
   _platformPosition(0),
   _sensor(0),
   _refPoint(0),
   _isProductGeoreferenced(false),
   _optimizationFactorX(0.0),
   _optimizationFactorY(0.0),
   _optimizationBiasX(0.0),
   _optimizationBiasY(0.0)
{
}

ossimGeometricSarSensorModel::ossimGeometricSarSensorModel(
   const ossimGeometricSarSensorModel& rhs)
   :
   ossimSensorModel(rhs),
   _platformPosition(rhs._platformPosition),
   _sensor(rhs._sensor),
   _refPoint(rhs._refPoint),
   _isProductGeoreferenced(rhs._isProductGeoreferenced),
   _optimizationFactorX(rhs._optimizationFactorX),
   _optimizationFactorY(rhs._optimizationFactorY),
   _optimizationBiasX(rhs._optimizationBiasX),
   _optimizationBiasY(rhs._optimizationBiasY)
{
}

ossimGeometricSarSensorModel::~ossimGeometricSarSensorModel()
{
   if (_platformPosition != 0)
   {
      delete _platformPosition;
   }

   if(_sensor != 0)
   {
      delete _sensor;
   }

   if(_refPoint != 0)
   {
      delete _refPoint;
   }
}

double ossimGeometricSarSensorModel::getSlantRange(double col) const
{
   const double CLUM        = 2.99792458e+8 ;

   double dist = _refPoint->get_distance()
      + _sensor->get_col_direction() * (col - (_refPoint->get_pix_col())) * ((CLUM / 2.0) * _sensor->get_nRangeLook() / _sensor->get_sf()) ;

   return  dist;
}

JSDDateTime ossimGeometricSarSensorModel::getTime(double line) const
{
   double dt =  _sensor->get_lin_direction() * (line - _refPoint->get_pix_line()) * _sensor->get_nAzimuthLook() / _sensor->get_prf() ;
   JSDDateTime time = _refPoint->get_ephemeris()->get_date();
   time.set_second(time.get_second() + dt);
   time.NormDate();

   return time;
}

void ossimGeometricSarSensorModel::lineSampleHeightToWorld(
   const ossimDpt& image_point,
   const double&   heightEllipsoid,
   ossimGpt&       worldPoint) const
{
   SarSensor sensor(_sensor,_platformPosition);
   double lon, lat;
   // const double CLUM        = 2.99792458e+8 ;

   // optimization
   double col = image_point.x - (image_point.x * _optimizationFactorX + _optimizationBiasX) ;
   double line = image_point.y - (image_point.y * _optimizationFactorY + _optimizationBiasY) ;

   // Slant range computation, depending on the product type
   double slantRange;
   if (_isProductGeoreferenced) {
      slantRange = getSlantRangeFromGeoreferenced(col) ;
   }
   else {
      slantRange = getSlantRange(col) ;
   }

   JSDDateTime azimuthTime = getTime(line) ;
   int etatLoc = sensor.ImageToWorld(slantRange, azimuthTime, heightEllipsoid, lon, lat);

   if(traceDebug())
   {
      switch (etatLoc)
      {
         case 0:
            ossimNotify(ossimNotifyLevel_DEBUG) << "successful call to lineSampleHeightToWorld" << std::endl;
            break;
         case 1:
            ossimNotify(ossimNotifyLevel_DEBUG) << "lineSampleHeightToWorld : no real root to the equation belongs to the imaging ray" << std::endl;
            break;
         case 2:
            ossimNotify(ossimNotifyLevel_DEBUG) << "lineSampleHeightToWorld : no real root to the equation" << std::endl;
            break;
         default :
            ossimNotify(ossimNotifyLevel_DEBUG) << "lineSampleHeightToWorld : unknown error case" << std::endl;
            break;
      }
   }

   worldPoint.lat = lat;
   worldPoint.lon = lon;
   worldPoint.hgt = heightEllipsoid ;
}

void ossimGeometricSarSensorModel::clearGCPlist() {
   _optimizationGCPsGroundCoordinates.clear();
   _optimizationGCPsImageCoordinates.clear();

   // optimization model update
   optimizeModel(_optimizationGCPsGroundCoordinates, _optimizationGCPsImageCoordinates) ;
}

void ossimGeometricSarSensorModel::getGCPlist(std::list<ossimGpt> & groundCoordinates, std::list<ossimDpt> & imageCoordinates) {
   groundCoordinates = _optimizationGCPsGroundCoordinates ;
   imageCoordinates = _optimizationGCPsImageCoordinates ;
}

bool ossimGeometricSarSensorModel::optimizeModel(const std::list<ossimGpt> & groundCoordinates, const std::list<ossimDpt> & imageCoordinates)
{
   if (groundCoordinates.size() != imageCoordinates.size()) return false ;

   // no optimization is used during the GCP localization error computation
   _optimizationFactorX = 0.0 ;
   _optimizationFactorY = 0.0 ;
   _optimizationBiasX = 0.0 ;
   _optimizationBiasY = 0.0 ;

   // appends the user input GCPs to the GCPs already present
   _optimizationGCPsGroundCoordinates.insert(_optimizationGCPsGroundCoordinates.end(), groundCoordinates.begin(), groundCoordinates.end()) ;
   _optimizationGCPsImageCoordinates.insert(_optimizationGCPsImageCoordinates.end(), imageCoordinates.begin(), imageCoordinates.end()) ;

   // no GCP : no optimization
   if (groundCoordinates.size() == 0) return true ;

   // Inverse projection of each Ground Control Point
   std::list<ossimGpt>::iterator itGround = _optimizationGCPsGroundCoordinates.begin() ;
   std::list<ossimDpt> inverseLocResults ;
   while (itGround != _optimizationGCPsGroundCoordinates.end())
   {
      ossimDpt itLoc ;
      this->worldToLineSample(*itGround,itLoc);
      inverseLocResults.push_back(itLoc) ;
      itGround++;
   }

   // error computation
   int nbPoints = _optimizationGCPsGroundCoordinates.size() ;
   double xErrorMean = 0.0, yErrorMean = 0.0, xActualPow = 0.0, yActualPow = 0.0, xActualMean = 0.0, yActualMean = 0.0,
      xErrorByActualMean = 0.0, yErrorByActualMean = 0.0 ;
   double xLocalError, yLocalError ;

   std::list<ossimDpt>::iterator itActualCoords = _optimizationGCPsImageCoordinates.begin() ;
   std::list<ossimDpt>::iterator itEstimatedCoords = inverseLocResults.begin() ;
   while ((itActualCoords != _optimizationGCPsImageCoordinates.end())&&(itEstimatedCoords != inverseLocResults.end())) {
      xLocalError = itActualCoords->x - itEstimatedCoords->x ;
      yLocalError = itActualCoords->y - itEstimatedCoords->y ;

      xErrorMean += xLocalError ;
      yErrorMean += yLocalError ;
      xActualMean += itActualCoords->x ;
      yActualMean += itActualCoords->y ;
      xActualPow += itActualCoords->x * itActualCoords->x ;
      yActualPow += itActualCoords->y * itActualCoords->y ;
      xErrorByActualMean += xLocalError * itActualCoords->x ;
      yErrorByActualMean += yLocalError * itActualCoords->y ;

      ++itActualCoords;
      ++itEstimatedCoords;
   }

   xErrorMean /= nbPoints ;
   yErrorMean /= nbPoints ;
   xActualMean /= nbPoints ;
   yActualMean /= nbPoints ;
   xActualPow /= nbPoints ;
   yActualPow /= nbPoints ;
   xErrorByActualMean /= nbPoints ;
   yErrorByActualMean /= nbPoints ;

   // linear regression
   if (fabs(xActualPow - xActualMean*xActualMean) > FLT_EPSILON)
      _optimizationFactorX = (xErrorByActualMean - xErrorMean * xActualMean) / (xActualPow - xActualMean*xActualMean) ;
   if (fabs(yActualPow - yActualMean*yActualMean) > FLT_EPSILON)
      _optimizationFactorY = (yErrorByActualMean - yErrorMean * yActualMean) / (yActualPow - yActualMean*yActualMean) ;
   _optimizationBiasX = xErrorMean - _optimizationFactorX * xActualMean ;
   _optimizationBiasY = yErrorMean - _optimizationFactorY * yActualMean ;

   //// Comparison between the estimated image coordinates of each GCP and its actual image coordinates
   //double lineBias = 0.0, columnBias = 0.0 ;
   //int nbPtsUsed = 0;

   //std::list<ossimDpt>::iterator itActualCoords = imageCoordinates.begin() ;
   //std::list<ossimDpt>::iterator itEstimatedCoords = inverseLocResults.begin() ;
   //while ((itActualCoords != imageCoordinates.end())&&(itEstimatedCoords != inverseLocResults.end())) {
   //
   //	columnBias += (itActualCoords->x - itEstimatedCoords->x ) ;
   //	lineBias += (itActualCoords->y - itEstimatedCoords->y ) ;
   //
   //	nbPtsUsed++;
   //	itActualCoords++;
   //	itEstimatedCoords++;
   //}

   //// Computation of bias in line and column : mean deviations
   //lineBias /= nbPtsUsed ;
   //columnBias /= nbPtsUsed ;

   //// Update of the model Reference Point
   //_refPoint->set_pix_col(_refPoint->get_pix_col() - columnBias);
   //_refPoint->set_pix_line(_refPoint->get_pix_line() - lineBias);

   return true ;
}

bool ossimGeometricSarSensorModel::saveState(ossimKeywordlist& kwl,
                                             const char* prefix) const

{
   static const char MODULE[] = "ossimGeometricSarSensorModel::saveState";

   bool result = false;

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)<< MODULE << " entered...\n";
   }

   if (_platformPosition && _sensor && _refPoint)
   {
      if ( _platformPosition->saveState(kwl, prefix) )
      {
         if ( _sensor->saveState(kwl, prefix) )
         {
            result = _refPoint->saveState(kwl, prefix);

            if (result)
            {
               kwl.add(prefix,
                       PRODUCT_GEOREFERENCED_FLAG_KW,
                       (_isProductGeoreferenced?"true":"false"));
               kwl.add(prefix,
                       OPTIMIZATION_FACTOR_X_KW,
                       _optimizationFactorX);
               kwl.add(prefix,
                       OPTIMIZATION_FACTOR_Y_KW,
                       _optimizationFactorY);
               kwl.add(prefix,
                       OPTIMIZATION_BIAS_X_KW,
                       _optimizationBiasX);
               kwl.add(prefix,
                       OPTIMIZATION_BIAS_Y_KW,
                       _optimizationBiasY);

               result = ossimSensorModel::saveState(kwl, prefix);
            }
         }
      }
   }

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " exit status = " << (result?"true":"false\n")
         << std::endl;
   }

   return result;
}

bool ossimGeometricSarSensorModel::loadState(const ossimKeywordlist &kwl,
                                             const char *prefix)
{
   static const char MODULE[] = "ossimGeometricSarSensorModel::loadState";

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " entered...\n";
   }

   bool result = true;

   // Load the base class;
   if ( ossimSensorModel::loadState(kwl, prefix) == false )
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << MODULE
            << "\nossimSensorModel::loadState failed!\n";
      }
      result = false;
   }

   // Load the platform position state.
   if ( !_platformPosition)
   {
      _platformPosition = new PlatformPosition();
   }
   if ( _platformPosition->loadState(kwl, prefix) == false )
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << MODULE
            << "\n_platformPosition->loadState failed!\n";
      }
      result = false;
   }

   // Load the sensor position state.
   if ( !_sensor)
   {
      _sensor = new SensorParams();
   }
   if ( _sensor->loadState(kwl, prefix) == false )
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << MODULE
            << "\n_sensor->loadState failed!\n";
      }
      result = false;
   }

   // Load the ref point.
   if ( !_refPoint)
   {
      _refPoint = new RefPoint();
   }
   if ( _refPoint->loadState(kwl, prefix) == false )
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << MODULE
            << "\n_refPoint->loadState failed!\n";
      }
      result = false;
   }

   const char* lookup = 0;
   ossimString s;

   lookup = kwl.find(prefix, PRODUCT_GEOREFERENCED_FLAG_KW);
   if (lookup)
   {
      s = lookup;
      _isProductGeoreferenced = s.toBool();
   }
   else
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << MODULE
            << "\nRequired keyword not found: "
            << PRODUCT_GEOREFERENCED_FLAG_KW << "\n";
      }
      result = false;
   }

   lookup = kwl.find(prefix, OPTIMIZATION_FACTOR_X_KW);
   if (lookup)
   {
      s = lookup;
      _optimizationFactorX = s.toDouble();
   }
   else
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << MODULE
            << "\nRequired keyword not found: "
            << OPTIMIZATION_FACTOR_X_KW << "\n";
      }
      result = false;
   }

   lookup = kwl.find(prefix, OPTIMIZATION_FACTOR_Y_KW);
   if (lookup)
   {
      s = lookup;
      _optimizationFactorY = s.toDouble();
   }
   else
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << MODULE
            << "\nRequired keyword not found: "
            << OPTIMIZATION_FACTOR_Y_KW << "\n";
      }
      result = false;
   }

   lookup = kwl.find(prefix,OPTIMIZATION_BIAS_X_KW);
   if (lookup)
   {
      s = lookup;
      _optimizationBiasX= s.toDouble();
   }
   else
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << MODULE
            << "\nRequired keyword not found: "
            << OPTIMIZATION_BIAS_X_KW << "\n";
      }
      result = false;
   }

   lookup = kwl.find(prefix,OPTIMIZATION_BIAS_Y_KW);
   if (lookup)
   {
      s = lookup;
      _optimizationBiasY = s.toDouble();
   }
   else
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << MODULE
            << "\nRequired keyword not found: "
            << OPTIMIZATION_BIAS_X_KW << "\n";
      }
      result = false;
   }

   // if (result && traceDebug())
//    if (result)
//    {
//       ossimNotify(ossimNotifyLevel_DEBUG)
//          << "calling saveState to verify loadState..." << endl;

//       ossimKeywordlist kwl2;
//       saveState(kwl2, 0);

//       ossimNotify(ossimNotifyLevel_DEBUG)
//          << "saveState result after loadState:"  << kwl2 << endl;
//    }

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " exit status = " << (result?"true":"false\n")
         << std::endl;
   }

   return result;
}

#if 0
   if(!InitSRGR(kwl, prefix))
   {
      return false;
   }
   if(!InitPlatformPosition(kwl, prefix))
   {
      return false;
   }
   if(!InitSensorParams(kwl, prefix))
   {
      return false;
   }
   if(!InitRefPoint(kwl, prefix))
   {
      return false;
   }
   return true;
}
#endif

std::ostream& ossimGeometricSarSensorModel::print(std::ostream& out) const
{
   out << setprecision(15) << setiosflags(ios::fixed)
       << "\nossimGeometricSarSensorModel class data members:\n";

   const char* prefix = 0;
   ossimKeywordlist kwl;
   if (_platformPosition)
   {
      _platformPosition->saveState(kwl, prefix);
   }
   if (_sensor)
   {
      _sensor->saveState(kwl, prefix);
   }
   if (_refPoint)
   {
      _refPoint->saveState(kwl, prefix);
   }

   kwl.add(prefix,
           PRODUCT_GEOREFERENCED_FLAG_KW,
           (_isProductGeoreferenced?"true":"false"));
   kwl.add(prefix,
                       OPTIMIZATION_FACTOR_X_KW,
           _optimizationFactorX);
   kwl.add(prefix,
           OPTIMIZATION_FACTOR_Y_KW,
           _optimizationFactorY);
   kwl.add(prefix,
           OPTIMIZATION_BIAS_X_KW,
           _optimizationBiasX);
   kwl.add(prefix,
           OPTIMIZATION_BIAS_Y_KW,
           _optimizationBiasY);

   out << kwl;

   return ossimSensorModel::print(out);
}
