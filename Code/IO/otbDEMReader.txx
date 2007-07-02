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
#ifndef __otbDEMReader_txx
#define __otbDEMReader_txx

#include "otbDEMReader.h"

namespace otb
{
/**************************************/
/*    CONSTRUCTEURS & DESTRUCTEURS    */
/**************************************/ 

// Constructeur par d�fault
  template<class TDEMImage> DEMReader<TDEMImage>::DEMReader()
	{
  	m_ElevManager=ossimElevManager::instance();
	//m_DEMImage    = DEMImageType::New();
 	m_Spacing[0]=0;
    m_Spacing[1]=0;	
	m_Ul[0]=0;
    m_Ul[1]=0;
	m_Lr[0]=0;
    m_Lr[1]=0;	
	}
  
// Destructeur 
template<class TDEMImage>
 DEMReader<TDEMImage>::~DEMReader()
{
delete m_ElevManager;
}

/**************************************/
/*            METHODES                */
/**************************************/ 
///M�thode pour sp�cifier un dossier contenant des DEM 
template<class TDEMImage> bool DEMReader<TDEMImage>::OpenDEMDirectory(char* &DEMDirectory)
{
ossimFilename ossimDEMDir;
ossimDEMDir=ossimFilename(DEMDirectory);
bool result= false;
if (m_ElevManager->openDirectory(ossimDEMDir))
{
result= true;
}
return result;
}

///M�thode pour calculer l'altitude d'un point g�ographique
template<class TDEMImage> double DEMReader<TDEMImage>::GetHeightAboveMSL(const PointType& worldPoint)
{
float height;
ossimGpt ossimWorldPoint;
ossimWorldPoint.lat=worldPoint[0];
ossimWorldPoint.lon=worldPoint[1];
height=m_ElevManager->getHeightAboveMSL(ossimWorldPoint);
return height;
}

///M�thode SetSpacing
template<class TDEMImage> void DEMReader<TDEMImage>::SetSpacing( const double* spacing)
{ 
SpacingType s(spacing);
this->SetSpacing(s);
}

///M�thode GenerateOutputInformation
template <class TDEMImage> void DEMReader<TDEMImage>::GenerateOutputInformation()
{

   DEMImageType *output;
   IndexType 			start;
	start[0]=0;     
	start[1]=0; 
	
   PointType			origin;
	origin[0]=m_Ul[0];          //latitude de l'origine.
	origin[1]=m_Ul[1];          //longitude de l'origine.
   output = this->GetOutput(0);
 
//Calcul de la taille de l'image:
   SizeType			size;
	size[0]=int (abs(((m_Lr[0]-m_Ul[0])/m_Spacing[0]))+1.5);
	size[1]=int (abs(((m_Lr[1]-m_Ul[1])/m_Spacing[1]))+1.5);

//On sp�cifie les param�tres de la r�gion
   OutputImageRegionType largestPossibleRegion;
   largestPossibleRegion.SetSize( size );
   largestPossibleRegion.SetIndex( start );
   output->SetLargestPossibleRegion( largestPossibleRegion );
 
   output->SetSpacing(m_Spacing);
   output->SetOrigin(m_Ul);
}//Fin GenerateOutputInformation

///M�thode GenerateData
template <class TDEMImage> void  DEMReader<TDEMImage>::GenerateData()
{
DEMImagePointerType  m_DEMImage = this->GetOutput();

// allocate the output buffer
m_DEMImage->SetBufferedRegion( m_DEMImage->GetRequestedRegion() );
m_DEMImage->Allocate();

// Create an iterator that will walk the output region
ImageIteratorType outIt = ImageIteratorType(m_DEMImage,m_DEMImage->GetRequestedRegion());

// Walk the output image, evaluating the height at each pixel
IndexType 			currentindex;
PointType 			phyPoint;
float 				height;
for (outIt.GoToBegin(); !outIt.IsAtEnd(); ++outIt)
{
 currentindex=outIt.GetIndex();
 m_DEMImage->TransformIndexToPhysicalPoint(currentindex, phyPoint);
 ossimGpt ossimWorldPoint;
 ossimWorldPoint.lat=phyPoint[0];
 ossimWorldPoint.lon=phyPoint[1];
 height=m_ElevManager->getHeightAboveMSL(ossimWorldPoint); //Calcul de l'altitude
 std::cout <<height<<endl;
 if (height>-32768) //On teste si les fichiers MNT recouvre la zone g�ographique demand�e (-32768 = theNullHeightValue)
 {m_DEMImage->SetPixel(currentindex, height);} //On remplit l'image
 else {m_DEMImage->SetPixel(currentindex, 0);}
}
}//fin GenerateData

template <class TDEMImage> void DEMReader<TDEMImage>::PrintSelf(std::ostream& os, Indent indent) const
 {
   Superclass::PrintSelf(os,indent);
 
  os << indent << "Spacing:"<< m_Spacing[0] << ","<< m_Spacing[1] << std::endl;
  os << indent << "Lr:"<< m_Lr[0] << ","<< m_Lr[1] << std::endl;
  os << indent << "Ul:"<< m_Ul[0] << ","<< m_Ul[1] << std::endl;
}
}//fin namespace

#endif
