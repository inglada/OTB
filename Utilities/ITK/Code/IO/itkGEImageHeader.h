/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkGEImageHeader.h,v $
  Language:  C++
  Date:      $Date: 2006/02/05 20:57:46 $
  Version:   $Revision: 1.10 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

  Portions of this code are covered under the VTK copyright.
  See VTKCopyright.txt or http://www.kitware.com/VTKCopyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itkGEImageHeader_h
#define _itkGEImageHeader_h

#include "itkIOCommon.h"

//#include "idbm_hdr_def.h"
enum GE_PANE_STRUCT {
  GE_AXIAL   =2,
  GE_SAGITTAL=4,
  GE_CORONAL =8
};

struct GEImageHeader
{
  short int seriesNumber;
  short int numberOfEchoes;
  short int echoNumber;
  short int imageNumber;
  float sliceLocation;
  float sliceThickness;
  float sliceGap;
  float TI;
  float TE;
  float TE2;
  float TR;
  short int flipAngle;
  int NEX;
  float xFOV;
  float yFOV;
  short int acqXsize;
  short int acqYsize;
  short int frequencyDir;
  char scanner[16];
  char pulseSequence[128]; //Needs to be at least 65 for seimens vision
  char patientId[32];
  char scanId[32];
  char name[64];
  char date[32];
  short int imageXsize;
  short int imageYsize;
  float imageXres;
  float imageYres;
  //itk::IOCommon::ValidAnalyzeOrientationFlags imagePlane;
  //itk::IOCommon::ValidOriginFlags origin;
  itk::SpatialOrientation::ValidCoordinateOrientationFlags coordinateOrientation;
  short int numberOfSlices;
  short int offset;
  char filename[itk::IOCommon::ITK_MAXPATHLEN+1];
  char hospital[35];
  short int imagesPerSlice;
  short int turboFactor; //This is only relevant for the geADW image format, but is put here for convenience
};
#endif
