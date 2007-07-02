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
#ifndef __otbComposite_h
#define __otbComposite_h

#include <iostream>
#include <sstream>
#include <stdio.h>
#include "itkTransform.h"
#include "itkMacro.h"
#include "otbMapProjection.h"

namespace otb
{

//On d�finit le template avec un type g�n�rique.
const unsigned int NInputDimensions = 2;
const unsigned int NOutputDimensions = 2;
typedef double TScalarType;

/**Template otbComposite.txx
* Cette classe permet de passer d'une MapProjection � une autre en passant par un point g�ographique. 
* Ceci pour �viter TOUTES les combinaisons possibles de passage d'une proj. carto � une autre.
* Template: Pour v�rifier les types.
**/
template <
class TInputMapProjection, 
class TOutputMapProjection,
class TScalarType=double, 
unsigned int NInputDimensions=2,
unsigned int NOutputDimensions=2>
class ITK_EXPORT Composite: public itk::Transform<
TScalarType,         // Data type for scalars 
NInputDimensions,  // Number of dimensions in the input space
NOutputDimensions> // Number of dimensions in the output space
{
public :
  
//D�claration des types utilis�s:

  typedef itk::Transform< TScalarType,
                   NInputDimensions,
                   NOutputDimensions >                        Superclass;
  typedef Composite                                	          Self;
  typedef itk::SmartPointer<Self>                    	      Pointer;
  typedef itk::SmartPointer<const Self>              	      ConstPointer;
  typedef TInputMapProjection                                 InputMapProjectionType;
  typedef TOutputMapProjection                                OutputMapProjectionType;
  
itkTypeMacro( Composite, itk::Transform );
itkNewMacro( Self );

   itkStaticConstMacro(InputSpaceDimension, unsigned int, NInputDimensions);
   itkStaticConstMacro(OutputSpaceDimension, unsigned int, NOutputDimensions);
   itkStaticConstMacro(SpaceDimension, unsigned int, NInputDimensions);
   itkStaticConstMacro(ParametersDimension, unsigned int,NInputDimensions*(NInputDimensions+1));

  typedef itk::Point<TScalarType,NInputDimensions >   	      InputPointType;
  typedef itk::Point<TScalarType,NOutputDimensions >  	      OutputPointType;        
	                                      
//D�claration des m�thodes:
itkSetObjectMacro(InputMapProjection,InputMapProjectionType); // SetMacro pour la 1� MapProjection

itkSetObjectMacro(OutputMapProjection,OutputMapProjectionType); // SetMacro pour la 2� MapProjection

// itkGetObjectMacro(InputMapProjection,InputMapProjectionType);
//  
// itkGetObjectMacro(OutputMapProjection,OutputMapProjectionType);

OutputPointType ComputeProjection1ToProjection2(const InputPointType &point1);

InputPointType ComputeProjection2ToProjection1(const OutputPointType &point2);

protected:
Composite();
~Composite();

TInputMapProjection* m_InputMapProjection;
TOutputMapProjection* m_OutputMapProjection;  
};
}//Fin header

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbComposite.txx"
#endif

#endif
