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
#ifndef __otbTransMercatorMapProjection_h
#define __otbTransMercatorMapProjection_h

#include "projection/ossimMapProjection.h"
#include "projection/ossimTransMercatorProjection.h"
#include "otbMapProjection.h"

namespace otb
{
//Cette classe h�rite de otb::MapProjection
class ITK_EXPORT TransMercatorMapProjection : public MapProjection<ossimTransMercatorProjection> 
{
public :

/******************************************/
/*  D�claration des types utilis�s:       */
/******************************************/
  typedef TransMercatorMapProjection                              Self;
  typedef MapProjection<ossimTransMercatorProjection>             Superclass;
  typedef itk::SmartPointer<Self>                    	      	  Pointer;
  typedef itk::SmartPointer<const Self>              	      	  ConstPointer;

itkTypeMacro( TransMercatorMapProjection, MapProjection );
itkNewMacro( Self );
	  
typedef Superclass::ScalarType  ScalarType;
/*typedef itk::Point<TScalarType,NInputDimensions >   	      InputPointType;
typedef itk::Point<TScalarType,NOutputDimensions >  	      OutputPointType; */   
typedef itk::Point<ScalarType,2>   	      InputPointType;
typedef itk::Point<ScalarType,2>  	      OutputPointType;     

/******************************************/
/*        D�claration des m�thodes:       */
/******************************************/	
void SetEllipsoid();
    
void SetFalseEasting(double falseEasting);

void SetFalseNorthing(double falseNorthing);

void SetScaleFactor(double scaleFactor);

void SetParameters(double falseEasting,double falseNorthing, double scaleFactor);

void SetDefaults();

double GetFalseNorthing() const;

double GetFalseEasting() const;

double GetScaleFactor() const;

protected:
TransMercatorMapProjection(); 
virtual ~TransMercatorMapProjection();

//Variables prot�g�s:
ossimTransMercatorProjection* m_TransMercatorProjection;
};

}//Fin header

/*#ifndef OTB_MANUAL_INSTANTIATION
#include "otbTransMercatorMapProjection.cxx"
#endif*/

#endif
