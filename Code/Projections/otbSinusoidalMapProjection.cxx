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

#include "otbSinusoidalMapProjection.h"

namespace otb
{ 

SinusoidalMapProjection
::SinusoidalMapProjection()
{
	m_SinusoidalProjection = new ossimSinusoidalProjection();
}


SinusoidalMapProjection
::~SinusoidalMapProjection()
{
	delete m_SinusoidalProjection;
}

///Sp�cification du false easting
void SinusoidalMapProjection
::SetFalseEasting(double falseEasting) 
{
	m_SinusoidalProjection->setFalseEasting(falseEasting);
}

///Sp�cification du false Northing
void SinusoidalMapProjection
::SetFalseNorthing(double falseNorthing) 
{
	m_SinusoidalProjection->setFalseNorthing(falseNorthing);
}

///Instanciation avec les param�tres par d�faut
void SinusoidalMapProjection
::SetDefaults() 
{
	m_SinusoidalProjection->setDefaults();
}

///M�thode pour r�cup�rer le FalseNorthing(pour �viter les coordonn�es n�gatives)
double SinusoidalMapProjection
::GetFalseNorthing() const
{
	double falseNorthing=m_SinusoidalProjection->getFalseNorthing();
	
	return falseNorthing;
}

///M�thode pour r�cup�rer le FalseEasting(pour �viter les coordonn�es n�gatives)
double SinusoidalMapProjection
::GetFalseEasting() const
{
	double falseEasting=m_SinusoidalProjection->getFalseEasting();
	
	return falseEasting;
}

} // namespace otb

