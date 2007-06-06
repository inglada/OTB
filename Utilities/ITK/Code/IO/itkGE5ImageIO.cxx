/*=========================================================================
  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkGE5ImageIO.cxx,v $
  Language:  C++
  Date:      $Date: 2006/10/23 02:15:18 $
  Version:   $Revision: 1.21 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information.

  =========================================================================*/
#include "itkGE5ImageIO.h"
#include "itkExceptionObject.h"
#include "itkByteSwapper.h"
#include "itkDirectory.h"
#include <itksys/SystemTools.hxx>
#include <iostream>
#include <fstream>
#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <string>

#include "itkGEImageHeader.h"
#include "itkIOCommon.h"

//From uiig library "The University of Iowa Imaging Group-UIIG"

namespace itk 
{
static const char GE_PROD_STR[]="SIGNA";
// Default constructor
GE5ImageIO::GE5ImageIO()
{
}

GE5ImageIO::~GE5ImageIO()
{
  //Purposefully left blank
    
}

int GE5ImageIO
::checkGe5xImages (char const * const imageFileTemplate)
{
  //
  // Does it exist?
  if(!itksys::SystemTools::FileExists(imageFileTemplate))
    {
    return -1;
    }
  //
  // is it at least 5000 bytes?
  if(itksys::SystemTools::FileLength(imageFileTemplate) < 5000)
    {
    return -1;
    }


  std::ifstream f(imageFileTemplate,std::ios::binary | std::ios::in);
  if(!f.is_open())
    return -1;

  Ge5xPixelHeader imageHdr;        /* Header Structure for GE 5x images */
  char hdr[GENESIS_SU_HDR_LEN]; /* Header to hold GE Suite header */
  char prod[16];                   /* Product name from Suite Header */

  // First pass see if image is a raw MR extracted via ximg
  if( !this->ReadBufferAsBinary( f, (void *)&imageHdr,sizeof(imageHdr) ) )
    {
    return -1;
    }
  ByteSwapper<int>::SwapFromSystemToBigEndian(&imageHdr.GENESIS_IH_img_magic);
  if (imageHdr.GENESIS_IH_img_magic == GE_5X_MAGIC_NUMBER)
    {
    return 0;
    }
  f.seekg(0,std::ios::beg);

  //
  // Second pass see if image was extracted via tape by Gene's tape
  // reading software.
  //
  if( !this->ReadBufferAsBinary( f, (void *)hdr, GENESIS_SU_HDR_LEN ) )
    {
    return -1;
    }
  strncpy (prod, hdr+GENESIS_SU_PRODID, 13);
  prod[13] = '\0';
  if (strcmp (prod, GE_PROD_STR) == 0)
    {
    return 0;
    }

  return -1;
}

bool GE5ImageIO::CanReadFile( const char* FileNameToRead )
{
  //this->SetFileName(FileNameToRead);
  return checkGe5xImages(FileNameToRead) == 0 ? true : false;
}

void
GE5ImageIO::swapPixHdr (Ge5xPixelHeader * hdr)
{
  ByteSwapper<int>::SwapFromSystemToBigEndian (&(hdr->GENESIS_IH_img_magic));
  ByteSwapper<int>::SwapFromSystemToBigEndian (&(hdr->GENESIS_IH_img_hdr_length));
  ByteSwapper<int>::SwapFromSystemToBigEndian (&(hdr->GENESIS_IH_img_width));
  ByteSwapper<int>::SwapFromSystemToBigEndian (&(hdr->GENESIS_IH_img_height));
  ByteSwapper<int>::SwapFromSystemToBigEndian (&(hdr->GENESIS_IH_img_depth));
  ByteSwapper<int>::SwapFromSystemToBigEndian (&(hdr->GENESIS_IH_img_compress));
  ByteSwapper<int>::SwapFromSystemToBigEndian (&(hdr->GENESIS_IH_img_dwindow));
  ByteSwapper<int>::SwapFromSystemToBigEndian (&(hdr->GENESIS_IH_img_dlevel));
  ByteSwapper<int>::SwapFromSystemToBigEndian (&(hdr->GENESIS_IH_img_bgshade));
  ByteSwapper<int>::SwapFromSystemToBigEndian (&(hdr->GENESIS_IH_img_ovrflow));
  ByteSwapper<int>::SwapFromSystemToBigEndian (&(hdr->GENESIS_IH_img_undflow));
  ByteSwapper<int>::SwapFromSystemToBigEndian (&(hdr->GENESIS_IH_img_top_offset));
  ByteSwapper<int>::SwapFromSystemToBigEndian (&(hdr->GENESIS_IH_img_bot_offset));
  ByteSwapper<short>::SwapFromSystemToBigEndian (&(hdr->GENESIS_IH_img_version));
  ByteSwapper<unsigned short >::SwapFromSystemToBigEndian (&(hdr->GENESIS_IH_img_checksum));
  ByteSwapper<int>::SwapFromSystemToBigEndian (&(hdr->GENESIS_IH_img_p_id));
  ByteSwapper<int>::SwapFromSystemToBigEndian (&(hdr->GENESIS_IH_img_l_id));
  ByteSwapper<int>::SwapFromSystemToBigEndian (&(hdr->GENESIS_IH_img_p_unpack));
  ByteSwapper<int>::SwapFromSystemToBigEndian (&(hdr->GENESIS_IH_img_l_unpack));
  ByteSwapper<int>::SwapFromSystemToBigEndian (&(hdr->GENESIS_IH_img_p_compress));
  ByteSwapper<int>::SwapFromSystemToBigEndian (&(hdr->GENESIS_IH_img_l_compress));
  ByteSwapper<int>::SwapFromSystemToBigEndian (&(hdr->GENESIS_IH_img_p_histo));
  ByteSwapper<int>::SwapFromSystemToBigEndian (&(hdr->GENESIS_IH_img_l_histo));
  ByteSwapper<int>::SwapFromSystemToBigEndian (&(hdr->GENESIS_IH_img_p_text));
  ByteSwapper<int>::SwapFromSystemToBigEndian (&(hdr->GENESIS_IH_img_l_text));
  ByteSwapper<int>::SwapFromSystemToBigEndian (&(hdr->GENESIS_IH_img_p_graphics));
  ByteSwapper<int>::SwapFromSystemToBigEndian (&(hdr->GENESIS_IH_img_l_graphics));
  ByteSwapper<int>::SwapFromSystemToBigEndian (&(hdr->GENESIS_IH_img_p_dbHdr));
  ByteSwapper<int>::SwapFromSystemToBigEndian (&(hdr->GENESIS_IH_img_l_dbHdr));
  ByteSwapper<int>::SwapFromSystemToBigEndian (&(hdr->GENESIS_IH_img_levelOffset));
  ByteSwapper<int>::SwapFromSystemToBigEndian (&(hdr->GENESIS_IH_img_p_user));
  ByteSwapper<int>::SwapFromSystemToBigEndian (&(hdr->GENESIS_IH_img_l_user));
  ByteSwapper<int>::SwapFromSystemToBigEndian (&(hdr->GENESIS_IH_img_p_suite));
  ByteSwapper<int>::SwapFromSystemToBigEndian (&(hdr->GENESIS_IH_img_l_suite));
  ByteSwapper<int>::SwapFromSystemToBigEndian (&(hdr->GENESIS_IH_img_p_exam));
  ByteSwapper<int>::SwapFromSystemToBigEndian (&(hdr->GENESIS_IH_img_l_exam));
  ByteSwapper<int>::SwapFromSystemToBigEndian (&(hdr->GENESIS_IH_img_p_series));
  ByteSwapper<int>::SwapFromSystemToBigEndian (&(hdr->GENESIS_IH_img_l_series));
  ByteSwapper<int>::SwapFromSystemToBigEndian (&(hdr->GENESIS_IH_img_p_image));
  ByteSwapper<int>::SwapFromSystemToBigEndian (&(hdr->GENESIS_IH_img_l_image));

  
  return;
}


struct GEImageHeader *
GE5ImageIO::ReadHeader (const char  *FileNameToRead)
{
  //#define VERBOSE_DEBUGGING
#if defined(VERBOSE_DEBUGGING)
#define RGEDEBUG(x) x
#else
#define RGEDEBUG(x)
#endif

  Ge5xPixelHeader imageHdr;              /* Header Structure for GE 5x images */
  char hdr[GENESIS_IM_HDR_START + GENESIS_MR_HDR_LEN];  /* Header to hold GE header */
  struct GEImageHeader *curImage;
  bool pixelHdrFlag;
  int timeStamp;
  char tmpId[64];
  char *ptr;
  if(checkGe5xImages(FileNameToRead) != 0)
    RAISE_EXCEPTION();

  curImage = new GEImageHeader;
  if (curImage == NULL)
    RAISE_EXCEPTION();
  memset(curImage,0,sizeof(struct GEImageHeader));
  pixelHdrFlag = false;

  
  std::ifstream f(FileNameToRead,std::ios::binary | std::ios::in);
  if(!f.is_open())
    RAISE_EXCEPTION();
  f.read((char *)&imageHdr,sizeof(imageHdr));
  IOCHECK();

  this->swapPixHdr(&imageHdr);

  if (imageHdr.GENESIS_IH_img_magic == GE_5X_MAGIC_NUMBER)
    {
    f.seekg(imageHdr.GENESIS_IH_img_p_suite,std::ios::beg);
    IOCHECK();
    pixelHdrFlag = true;
    }
  else
    {
    f.seekg(0,std::ios::beg);
    }
  f.read(hdr,GENESIS_IM_HDR_START + GENESIS_MR_HDR_LEN);
  IOCHECK();

  /* Set Patient-Name */
  strncpy (curImage->name, &hdr[GENESIS_EX_HDR_START + GENESIS_EX_PATNAME], 
           GENESIS_EX_PATAGE - GENESIS_EX_PATNAME + 1);
  sprintf (curImage->scanner, "GE-5X");

  /* Set Hospital-Name */
  strncpy (curImage->hospital, &hdr[GENESIS_EX_HDR_START + GENESIS_EX_HOSPNAME], 
           GENESIS_EX_DETECT - GENESIS_EX_HOSPNAME + 1);

  /* Set Images-Per-Slice */
  curImage->imagesPerSlice= hdr2Short(&hdr[GENESIS_IM_HDR_START + GENESIS_MR_CPHASE]);

  /* Set Date */
  timeStamp = hdr2Int (&hdr[GENESIS_SE_HDR_START + GENESIS_SE_ACTUAL_DT]);
  statTimeToAscii (&timeStamp, curImage->date);

  /* Set Patient-Id */
  strncpy (tmpId, &hdr[GENESIS_EX_HDR_START + GENESIS_EX_PATID], 
           GENESIS_EX_PATNAME - GENESIS_EX_PATID + 1);
  tmpId[GENESIS_EX_PATNAME - GENESIS_EX_PATID + 1] = '\0';
  curImage->patientId[0] = '\0';

  ptr = strtok (tmpId, "-");

  while (ptr != NULL)
    {
    strcat (curImage->patientId, ptr);
    ptr = strtok (NULL, "-");
    }

  RGEDEBUG(fprintf (stderr, "Id <%s>\n", curImage->patientId);)

    curImage->seriesNumber = hdr2Short (&hdr[GENESIS_SE_HDR_START + GENESIS_SE_NO]);
  RGEDEBUG(fprintf (stderr, "Series Number %d\n", curImage->seriesNumber);)

    curImage->imageNumber = hdr2Short (&hdr[GENESIS_IM_HDR_START + GENESIS_MR_IM_NO]);
  RGEDEBUG(fprintf (stderr, "Image Number %d\n", curImage->imageNumber);)

    curImage->sliceThickness = hdr2Float (&hdr[GENESIS_IM_HDR_START + GENESIS_MR_SLTHICK]);
  RGEDEBUG(fprintf (stderr, "Thickness %f\n", curImage->sliceThickness);)

    curImage->imageXsize = hdr2Short (&hdr[GENESIS_IM_HDR_START + GENESIS_MR_IMATRIX_X]);
  curImage->imageYsize = hdr2Short (&hdr[GENESIS_IM_HDR_START + GENESIS_MR_IMATRIX_Y]);
  RGEDEBUG(fprintf (stderr, "Acq Size %dx%d\n", curImage->acqXsize, curImage->acqYsize);)

    curImage->xFOV = hdr2Float (&hdr[GENESIS_IM_HDR_START + GENESIS_MR_DFOV]);
  curImage->yFOV = hdr2Float (&hdr[GENESIS_IM_HDR_START + GENESIS_MR_DFOV]);
  if (curImage->yFOV == 0.0)
    {
    curImage->yFOV = curImage->xFOV;
    }
  RGEDEBUG(fprintf (stderr, "FOV %fx%f\n", curImage->xFOV, curImage->yFOV);)

    curImage->acqXsize = (int) hdr2Float (&hdr[GENESIS_IM_HDR_START + GENESIS_MR_DIM_X]);
  curImage->acqYsize = (int) hdr2Float (&hdr[GENESIS_IM_HDR_START + GENESIS_MR_DIM_Y]);
  RGEDEBUG(fprintf (stderr, "Image Size %dx%d\n", curImage->imageXsize, curImage->imageYsize);)

    curImage->imageXres = hdr2Float (&hdr[GENESIS_IM_HDR_START + GENESIS_MR_PIXSIZE_X]);
  curImage->imageYres = hdr2Float (&hdr[GENESIS_IM_HDR_START + GENESIS_MR_PIXSIZE_Y]);
  RGEDEBUG(fprintf (stderr, "Image Res %fx%f\n", curImage->imageXres, curImage->imageYres);)
    short int GE_Plane = hdr2Short (&hdr[GENESIS_IM_HDR_START + GENESIS_MR_PLANE]);
  //RGEDEBUG(fprintf (stderr, "Plane %d\n", curImage->imagePlane);)

    //RECODE image plane to be brains2 compliant.!!
  switch (GE_Plane)
      {
  case GE_CORONAL:
      //curImage->imagePlane = itk::IOCommon::ITK_ANALYZE_ORIENTATION_IRP_CORONAL;
      //curImage->origin = itk::IOCommon::ITK_ORIGIN_SLA;
      curImage->coordinateOrientation = itk::SpatialOrientation::ITK_COORDINATE_ORIENTATION_RSP;
      break;
  case GE_SAGITTAL:
      //curImage->imagePlane = itk::SpatialOrientation::ITK_ANALYZE_ORIENTATION_IRP_SAGITTAL;
      //curImage->origin = itk::SpatialOrientation::ITK_ORIGIN_SLP;
      curImage->coordinateOrientation = itk::SpatialOrientation::ITK_COORDINATE_ORIENTATION_AIR;
      break;
  case GE_AXIAL:
      //curImage->imagePlane = itk::SpatialOrientation::ITK_ANALYZE_ORIENTATION_IRP_TRANSVERSE;
      //curImage->origin = itk::SpatialOrientation::ITK_ORIGIN_ILP;
      curImage->coordinateOrientation = itk::SpatialOrientation::ITK_COORDINATE_ORIENTATION_RAI;
      break;
  default:
      //curImage->imagePlane = itk::SpatialOrientation::ITK_ANALYZE_ORIENTATION_IRP_CORONAL;
      //curImage->origin = itk::SpatialOrientation::ITK_ORIGIN_SLA;
      curImage->coordinateOrientation = itk::SpatialOrientation::ITK_COORDINATE_ORIENTATION_RSP;
      break;
      }
  curImage->sliceLocation = hdr2Float (&hdr[GENESIS_IM_HDR_START + GENESIS_MR_LOC]);
  RGEDEBUG(fprintf (stderr, "Location %f %c %c\n", curImage->sliceLocation, hdr[GENESIS_IM_HDR_START + GENESIS_MR_LOC_RAS], hdr[GENESIS_IM_HDR_START + GENESIS_MR_LOC_RAS+1]);)

    curImage->TR = hdr2Int (&hdr[GENESIS_IM_HDR_START + GENESIS_MR_TR]) / 1000.0;
  curImage->TI = hdr2Int (&hdr[GENESIS_IM_HDR_START + GENESIS_MR_TI]) / 1000.0;
  curImage->TE = hdr2Int (&hdr[GENESIS_IM_HDR_START + GENESIS_MR_TE]) / 1000.0;
  curImage->TE2 = hdr2Int (&hdr[GENESIS_IM_HDR_START + GENESIS_MR_TE2]) / 1000.0;
  RGEDEBUG(fprintf (stderr, "TR %f, TI %f, TE %f, TE2 %f\n", curImage->TR, curImage->TI, curImage->TE, curImage->TE2);)

    curImage->numberOfEchoes = hdr2Short (&hdr[GENESIS_IM_HDR_START + GENESIS_MR_NUMECHO]);
  curImage->echoNumber = hdr2Short (&hdr[GENESIS_IM_HDR_START + GENESIS_MR_ECHONUM]);

  if (curImage->numberOfEchoes == 0)
    curImage->numberOfEchoes = 1;
  RGEDEBUG(fprintf (stderr, "Echos %d,  Number %d\n", curImage->numberOfEchoes, curImage->echoNumber);)

    curImage->NEX = (int) hdr2Float (&hdr[GENESIS_IM_HDR_START + GENESIS_MR_NEX]);
  RGEDEBUG(fprintf (stderr, "NEX %d\n", curImage->NEX);)

    curImage->flipAngle = hdr2Short (&hdr[GENESIS_IM_HDR_START + GENESIS_MR_FLIP]);
  RGEDEBUG(fprintf (stderr, "Flip Angle %d\n", curImage->flipAngle);)

    strncpy (curImage->pulseSequence, &hdr[GENESIS_IM_HDR_START + GENESIS_MR_PSDNAME], 31);
  curImage->pulseSequence[31] = '\0';
  RGEDEBUG(fprintf (stderr, "Sequence %s\n", curImage->pulseSequence);)

    curImage->numberOfSlices = hdr2Short (&hdr[GENESIS_IM_HDR_START + GENESIS_MR_SLQUANT]);
  RGEDEBUG(fprintf (stderr, "Number Of Slices %d\n", curImage->numberOfSlices);)

    if (pixelHdrFlag)
      {
      curImage->offset = imageHdr.GENESIS_IH_img_hdr_length;
      }
    else
      {
      curImage->offset = itksys::SystemTools::FileLength(FileNameToRead) - (curImage->imageXsize * curImage->imageYsize * 2);
      }

  strncpy (curImage->filename,FileNameToRead, IOCommon::ITK_MAXPATHLEN+1);

  return (curImage);
}

} // end namespace itk
