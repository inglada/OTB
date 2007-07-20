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

#include "otbLambertConformalConicMapProjection.h"

namespace otb
{ 

LambertConformalConicMapProjection
::LambertConformalConicMapProjection()
{
	m_LambertConformalConicProjection = new ossimLambertConformalConicProjection();
}

LambertConformalConicMapProjection
::~LambertConformalConicMapProjection()
{
	delete m_LambertConformalConicProjection;
}


///Sp�cification du Parallel1
void LambertConformalConicMapProjection
::SetStandardParallel1(double degree) 
{
	m_LambertConformalConicProjection->setStandardParallel1(degree);
}

///Sp�cification du Parallel2
void LambertConformalConicMapProjection
::SetStandardParallel2(double degree) 
{
	m_LambertConformalConicProjection->setStandardParallel2(degree);
}

///Sp�cification des 2 parallels
void LambertConformalConicMapProjection
::SetStandardParallels(double parallel1Degree,double parallel2Degree) 
{
	m_LambertConformalConicProjection->setStandardParallels(parallel1Degree,parallel2Degree);
}

///Sp�cification du false easting
void LambertConformalConicMapProjection
::SetFalseEasting(double falseEasting) 
{
	m_LambertConformalConicProjection->setFalseEasting(falseEasting);
}

///Sp�cification du false Northing
void LambertConformalConicMapProjection
::SetFalseNorthing(double falseNorthing) 
{
	m_LambertConformalConicProjection->setFalseNorthing(falseNorthing);
}

///Instanciation de tous les param�tres
void LambertConformalConicMapProjection
::SetParameters(double parallel1Degree,double parallel2Degree,double falseEasting,double falseNorthing) 
{
	m_LambertConformalConicProjection->setParameters(parallel1Degree, parallel2Degree, falseEasting, falseNorthing);
}

///Instanciation avec les param�tres par d�faut
void LambertConformalConicMapProjection
::SetDefaults() 
{
	m_LambertConformalConicProjection->setDefaults();
}

///M�thode pour r�cup�rer le FalseNorthing(pour �viter les coordonn�es n�gatives)
double LambertConformalConicMapProjection
::GetFalseNorthing() const
{
	double falseNorthing=m_LambertConformalConicProjection->getFalseNorthing();
	
	return falseNorthing;
}

///M�thode pour r�cup�rer le FalseEasting(pour �viter les coordonn�es n�gatives)
double LambertConformalConicMapProjection
::GetFalseEasting() const
{
	double falseEasting=m_LambertConformalConicProjection->getFalseEasting();
	return falseEasting;
}

} // namespace otb

