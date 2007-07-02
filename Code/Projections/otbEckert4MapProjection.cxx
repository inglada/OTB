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
/** Eckert4MapProjection.cxx
D�finition des m�thodes propres � Eckert4MapProjection.
**/

#include "otbEckert4MapProjection.h"

namespace otb
{ /************************************/
  /*          Constructeurs           */
  /************************************/
  
// Constructeur par d�fault
Eckert4MapProjection::Eckert4MapProjection()
{
 m_Eckert4Projection = new ossimEckert4Projection();
}

// Destructeur 
 Eckert4MapProjection::~Eckert4MapProjection()
{
delete m_Eckert4Projection;
}
/******************************************/
/*        D�claration des m�thodes:       */
/******************************************/
///Sp�cification du false easting
void    Eckert4MapProjection::SetFalseEasting(double falseEasting) 
{
m_Eckert4Projection->setFalseEasting(falseEasting);
}

///Sp�cification du false Northing
void    Eckert4MapProjection::SetFalseNorthing(double falseNorthing) 
{
m_Eckert4Projection->setFalseNorthing(falseNorthing);
}

///Instanciation avec les param�tres par d�faut
void    Eckert4MapProjection::SetDefaults() 
{
m_Eckert4Projection->setDefaults();
}

///M�thode pour r�cup�rer le FalseNorthing(pour �viter les coordonn�es n�gatives)
 double Eckert4MapProjection::GetFalseNorthing() const
{
double falseNorthing=m_Eckert4Projection->getFalseNorthing();
return falseNorthing;
}

///M�thode pour r�cup�rer le FalseEasting(pour �viter les coordonn�es n�gatives)
double Eckert4MapProjection::GetFalseEasting() const
{
double falseEasting=m_Eckert4Projection->getFalseEasting();
return falseEasting;
}

}//fin namespace

