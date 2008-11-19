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
#ifndef __otbPolygonCompacityFunctor_h
#define __otbPolygonCompacityFunctor_h

#include "otbMath.h"

namespace otb
{

    /** \class PolygonCompacityFunctor
     *  \brief Select polygons according to their compacity
     *
   * This functor compute the compacity of a polygon
   * and return true is the distance is above the threshold, false otherwise.
   *
   * The compacity is defined as:
   *
   *  \f$ 4\pi \frac{S}{L}\f$
    * 
   * where \f$ S \f$ is the surface (obtained by the method GetSurface() ) 
    * and \f$ L \f$ the perimeter (obtained by the method GetLength() ).
     *
     *  \ingroup Functor
     */
  template <class TInput1>
  class PolygonCompacityFunctor
  {
    public:
      

      void SetThreshold(double threshold )
      {
        m_Threshold = threshold;
      }
      double GetThreshold(void)const
      {
        return (  m_Threshold );
      }
      
      PolygonCompacityFunctor() 
      {
        m_Threshold = 0.2;
      };
      ~PolygonCompacityFunctor() {};
      
      inline bool operator()(const TInput1 & input)
      {
        double circularityRatio = 4*M_PI*input->GetSurface()
            / M_SQUARE(input->GetLength());
      
        if (circularityRatio > m_Threshold)
        {
          return true;
        }
        else
        {
          return false;
        }
      }
      
      
      
    private:
      double m_Threshold;
  };

}

#endif
