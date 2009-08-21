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


#include <PlatformPosition.h>
#include <Ephemeris.h>
#include <HermiteInterpolator.h>
#include <ossim/base/ossimKeywordlist.h>

#include <iostream>
#include <string>
#include <cmath>

namespace ossimplugins
{


static const char NUMBER_PLATFORM_POSITIONS_KW[] = "platform_positions_count";

PlatformPosition::PlatformPosition():
   _nbrData(0),
   _data(NULL)
{
}

PlatformPosition::~PlatformPosition()
{
   Clear();
}

void PlatformPosition::Clear()
{
   if(_data != NULL)
   {
      for (int i=0;i<_nbrData;i++)
      {
         delete _data[i];
      }
      delete [] _data;
   }
   _data = NULL;
   _nbrData = 0;
}

PlatformPosition::PlatformPosition(const PlatformPosition& rhs)
{
   _data = new Ephemeris*[rhs._nbrData];
   _nbrData = rhs._nbrData;
   for (int i=0;i<rhs._nbrData;i++)
   {
      _data[i] = rhs._data[i]->Clone();
   }
}

PlatformPosition& PlatformPosition::operator=(const PlatformPosition& rhs)
{
   Clear();
   _data = new Ephemeris*[rhs._nbrData];
   _nbrData = rhs._nbrData;
   for (int i=0;i<rhs._nbrData;i++)
   {
      _data[i] = rhs._data[i]->Clone();
   }

   return *this;
}

PlatformPosition::PlatformPosition(Ephemeris** data, int nbrData):
   _nbrData(nbrData)
{
   _data = new Ephemeris*[_nbrData];
   for (int i=0;i<_nbrData;i++)
   {
      _data[i] = data[i]->Clone();
   }
}

Ephemeris* PlatformPosition::Interpolate(JSDDateTime date)
{
   const double JOURCIVIL_LENGTH = 86400.0 ;
   Ephemeris* ephem = NULL;
   if (_nbrData<=1)
   {
      std::cout << "a..." << std::endl;
      ephem = NULL;
   }
   else
   {
      /*
       * The first element of the list is cloned to ensure that the output ephemeris is expressed in the same coordinate system as input ones
       */
      ephem = _data[0]->Clone();

      /* NORMAL CASE */
      /*------------*/
      double * x = new double[_nbrData];
      double * y = new double[_nbrData];
      double * yd = new double[_nbrData];
      double dt = 0.0;
      bool echIrreg = false;
      double d;

      x[0] = 0.0 ;
      for (int i = 1 ; i < _nbrData ; i++)
      {
         x[i] =   (_data[i]->get_date().get_day0hTU().get_julianDate() - _data[0]->get_date().get_day0hTU().get_julianDate())
            * JOURCIVIL_LENGTH
            + _data[i]->get_date().get_second()   - _data[0]->get_date().get_second()
            + _data[i]->get_date().get_decimal()     - _data[0]->get_date().get_decimal();
         d = x[i] - x[i-1] ;

         /*
          * Non increasing time
          */
         if (d <= 0.0)/* Non increasing time */
         {
            delete ephem;
            ephem = NULL;
         }
         else if (i == 1)
         {
            dt = d ;
         }
         else if (fabs (d-dt) >= 1.0e-4) /* Irregular sampling */
         {
            echIrreg = true;
         }
      }

      if (ephem != NULL)
      {
         ephem->set_date(date);

         dt =  (date.get_day0hTU().get_julianDate() - _data[0]->get_date().get_day0hTU().get_julianDate()) * JOURCIVIL_LENGTH
            + date.get_second()   - _data[0]->get_date().get_second()
            + date.get_decimal()     - _data[0]->get_date().get_decimal();

         /* If nPts odd or if the searched date is not situated in the middle of the ephemeris list -> Lagrange */
         d = (dt - x[_nbrData/2-1]) / (x[_nbrData/2] - x[_nbrData/2-1]) ;


         /* Computation by Everett  */
         /*---------------------*/
         double pos[3];
         double vit[3];
         for (int j = 0 ; j < 3 ; j++)
         {
            for (int i = 0 ; i < _nbrData ; i++)
            {
               y[i] = _data[i]->get_position()[j] ;
               yd[i] = _data[i]->get_vitesse()[j] ;
            }
            HermiteInterpolator interpolator(_nbrData,x,y,yd);
            interpolator.Interpolate(dt, pos[j], vit[j]);

         }
         ephem->set_position(pos);
         ephem->set_vitesse(vit);

      }

      delete[] x;
      delete[] y;
      delete[] yd;

   }
   return ephem;
}

void PlatformPosition::setData(Ephemeris** data, int nbrData)
{
   Clear();
   _data = data;
   _nbrData = nbrData;
}

bool PlatformPosition::saveState(ossimKeywordlist& kwl,
                                 const char* prefix) const
{
   kwl.add(prefix, NUMBER_PLATFORM_POSITIONS_KW, _nbrData);

   std::string s1;
   if (prefix)
   {
      s1 = prefix;
   }

   for (int i = 0; i < _nbrData; ++i)
   {
      std::string s2 = s1;
      s2 += "platform_position[";
      s2 += ossimString::toString(i);
      s2+= "]";
      _data[i]->saveState(kwl, s2.c_str());
   }

   return true;
}


bool PlatformPosition::loadState(const ossimKeywordlist& kwl,
                                 const char* prefix)
{
   bool result = true;

   Clear();

   const char* lookup = 0;
   lookup = kwl.find(prefix, NUMBER_PLATFORM_POSITIONS_KW);
   if (lookup)
   {
      ossimString s = lookup;
      _nbrData = s.toInt();

      if (_nbrData)
      {
         std::string s1;
         if (prefix)
         {
            s1 = prefix;
         }

         _data = new  Ephemeris*[_nbrData];
         for (int i = 0; i < _nbrData; ++i)
         {
            std::string s2 = s1;
            s2 += "platform_position[";
            s2 += ossimString::toString(i);
            s2+= "]";

            _data[i] = new Ephemeris();
            _data[i]->loadState(kwl, s2.c_str());
         }
      }
   }
   else
   {
      result = false;
   }

   return result;
}
}
