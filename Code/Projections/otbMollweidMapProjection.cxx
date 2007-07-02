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

#include "otbMollweidMapProjection.h"

namespace otb
{ /************************************/
  /*          Constructeurs           */
  /************************************/
  
// Constructeur par d�fault
  MollweidMapProjection::MollweidMapProjection()
{
 m_MollweidProjection = new ossimMollweidProjection();
}

// Destructeur 
 MollweidMapProjection::~MollweidMapProjection()
{
delete m_MollweidProjection;
}
/******************************************/
/*        D�claration des m�thodes:       */
/******************************************/
///Sp�cification du false easting
void    MollweidMapProjection::SetFalseEasting(double falseEasting) 
{
m_MollweidProjection->setFalseEasting(falseEasting);
}

///Sp�cification du false Northing
void    MollweidMapProjection::SetFalseNorthing(double falseNorthing) 
{
m_MollweidProjection->setFalseNorthing(falseNorthing);
}

///Instanciation avec les param�tres par d�faut
void    MollweidMapProjection::SetDefaults() 
{
m_MollweidProjection->setDefaults();
}

///M�thode pour r�cup�rer le FalseNorthing(pour �viter les coordonn�es n�gatives)
 double MollweidMapProjection::GetFalseNorthing() const
{
double falseNorthing=m_MollweidProjection->getFalseNorthing();
return falseNorthing;
}

///M�thode pour r�cup�rer le FalseEasting(pour �viter les coordonn�es n�gatives)
double MollweidMapProjection::GetFalseEasting() const
{
double falseEasting=m_MollweidProjection->getFalseEasting();
return falseEasting;
}
}//fin namespace

