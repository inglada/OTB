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
#ifndef __otbInverseSensorModel_h
#define __otbInverseSensorModel_h

#include <sstream>
#include <stdio.h>
#include <iostream>
#include "itkExceptionObject.h"
#include "itkTransform.h"
#include "itkExceptionObject.h"
#include "itkMacro.h"
#include "projection/ossimSensorModelFactory.h"
#include "projection/ossimProjection.h"
#include "projection/ossimProjectionFactoryRegistry.h"
#include "base/ossimKeywordlist.h"
#include "imaging/ossimImageHandlerRegistry.h"
#include "imaging/ossimImageHandler.h"
#include "itkSmartPointer.h"
#include "itkObject.h"


namespace otb
{
//On d�finit le template avec un type g�n�rique.
//Cette classe h�rite de itk::Transform
template <class TScalarType,
          unsigned int NInputDimensions=2,
          unsigned int NOutputDimensions=2,
          unsigned int NParametersDimensions=3>
class ITK_EXPORT InverseSensorModel : public itk::Transform<TScalarType,          
			                                    NInputDimensions,  
			                                    NOutputDimensions> 
{
public :

/******************************************/
/*  D�claration des types utilis�s:       */
/******************************************/
  typedef InverseSensorModel                                  Self;
  typedef itk::Transform< TScalarType,
                   NInputDimensions,
                   NOutputDimensions >                        Superclass;
  typedef itk::SmartPointer<Self>                    	      Pointer;
  typedef itk::SmartPointer<const Self>              	      ConstPointer;

itkTypeMacro( InverseSensorModel, Transform );
itkNewMacro( Self );

  typedef itk::Point<TScalarType, NInputDimensions >          InputPointType;
  typedef itk::Point<TScalarType, NOutputDimensions >         OutputPointType;      
	       
  itkStaticConstMacro(InputSpaceDimension, unsigned int, NInputDimensions);
  itkStaticConstMacro(OutputSpaceDimension, unsigned int, NOutputDimensions);
  itkStaticConstMacro(ParametersDimension, unsigned int, NParametersDimensions); //A voir!!
	         
/******************************************/
/*        D�claration des m�thodes:       */
/******************************************/	    
//Pour lire les kwl.                                  
ossimKeywordlist    GetImageGeometryKeywordlist(char *src);	  
//Pour construire le mod�le.
void                SetImageGeometry(ossimKeywordlist &geom_kwl);
// Pour projeter un point g�o sur l'image en g�om�trie capteur.
OutputPointType     InverseTransformPoint(const InputPointType &point) const;
// Pour projeter un point g�o connaissant son altitude.
OutputPointType     InverseTransformPoint(const InputPointType &point, double height) const;

protected:
InverseSensorModel(); 
virtual ~InverseSensorModel();

//Variables prot�g�s:
ossimKeywordlist m_geom_kwl;
ossimProjection* m_Model;
};

}//Fin header

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbInverseSensorModel.txx"
#endif


#endif
