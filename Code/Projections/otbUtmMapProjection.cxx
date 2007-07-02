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

#include "otbUtmMapProjection.h"

namespace otb
{ /************************************/
  /*          Constructeurs           */
  /************************************/
  
// Constructeur par d�fault
  UtmMapProjection::UtmMapProjection()
{
 m_utmprojection = new ossimUtmProjection();
}

// Destructeur 
 UtmMapProjection::~UtmMapProjection()
{
delete m_utmprojection;
}

/******************************************/
/*        D�claration des m�thodes:       */
/******************************************/
///Instanciation de UtmProjection avec une zone (2 m�thodes)
void    UtmMapProjection::SetZone(long zone)
{
delete m_utmprojection;
m_utmprojection= new ossimUtmProjection(zone);
}

void    UtmMapProjection::SetZone(const InputPointType &ground) 
{ossimGpt ossimGround;
ossimGround.lon=ground[0];
ossimGround.lat=ground[1];
m_utmprojection->setZone(ossimGround);
}
///Sp�cification de l' h�misph�re
void    UtmMapProjection::SetHemisphere(char hemisphere) 
{
m_utmprojection->setHemisphere(hemisphere);
}

///R�cup�rer la Zone
long UtmMapProjection::GetZone()
{long zone;
zone=m_utmprojection->getZone();
return zone;
}

///R�cup�rer l'h�misph�re
const char UtmMapProjection::GetHemisphere() const
{ char hemisphere=0;
hemisphere=m_utmprojection->getHemisphere();
return hemisphere;
}
}//fin namespace

