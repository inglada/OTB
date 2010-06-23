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

#include <iostream>
#include <fstream>
#include <string.h>
#include <list>
#include <vector>
#include <math.h>

#include "otbGDALImageIO.h"
#include "otbMacro.h"
#include "otbSystem.h"
#include "otbImage.h"

#include "itkMetaDataObject.h"

#include "itkExceptionObject.h"
#include "itkMacro.h"
#include "itkRGBPixel.h"
#include "itkRGBAPixel.h"

namespace otb
{

GDALImageIO::GDALImageIO()
{
  // By default set number of dimensions to two.
  this->SetNumberOfDimensions(2);

  // By default set pixel type to scalar.
  m_PixelType = SCALAR;

  // By default set component type to unsigned char
  m_ComponentType = UCHAR;
  m_UseCompression = false;
  m_CompressionLevel = 4; // Range 0-9; 0 = no file compression, 9 = maximum file compression

  // Set default spacing to one
  m_Spacing[0] = 1.0;
  m_Spacing[1] = 1.0;
  // Set default origin to zero
  m_Origin[0] = 0.0;
  m_Origin[1] = 0.0;

  m_IsIndexed   = false;
  m_currentfile = NULL;
  m_poBands     = NULL;
  m_hDriver     = NULL;
  m_poDataset   = NULL;

  m_NbBands = 0;
  m_FlagWriteImageInformation = true;

//  GDALAllRegister();
}

GDALImageIO::~GDALImageIO()
{
//THOMAS
//  if( m_hDriver != NULL ) GDALClose( m_hDriver ); //Ne pas le faire  sinon SegFault !!!!
//         if( m_poBands != NULL ) delete [] m_poBands;
//   if( m_poDataset != NULL ) delete m_poDataset;

  if (m_poDataset != NULL)
    {

    GDALClose(m_poDataset);
    m_poDataset = NULL;
    }
  if (m_poBands != NULL)
    {
    delete[] m_poBands;
    }
  GDALDestroyDriverManager();
}

// Tell only if the file can be read with GDAL.
bool GDALImageIO::CanReadFile(const char* file)
{
  // First check the extension
  if (file == NULL)
    {
    itkDebugMacro(<< "No filename specified.");
    return false;
    }

  std::string lFileNameGdal;
  lFileNameGdal = std::string(file);

  // Init GDAL parameters
  GDALAllRegister();

  // Open file with GDAL
  m_poDataset = static_cast<GDALDataset *>(GDALOpen(lFileNameGdal.c_str(), GA_ReadOnly));

  if (m_poDataset == NULL)
    {
    fprintf(stderr,
            "GDALOpen failed - %d\n%s\n",
            CPLGetLastErrorNo(), CPLGetLastErrorMsg());

    GDALDestroyDriverManager();
    CPLDumpSharedList(NULL);
    itkDebugMacro(<< "No dataset ");
    otbMsgDevMacro(<< "Not CanReadFile GDAL");
    return false;
    }
  else
    {
    GDALClose(m_poDataset);
    m_poDataset = NULL;
    GDALDestroyDriverManager();
    CPLDumpSharedList(NULL);

    otbMsgDevMacro(<< "CanReadFile GDAL");
    return true;
    }
}

// Used to print information about this object
void GDALImageIO::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Compression Level : " << m_CompressionLevel << "\n";
}

// Read a 3D image (or event more bands)... not implemented yet
void GDALImageIO::ReadVolume(void*)
{
}

// Read image with GDAL
void GDALImageIO::Read(void* buffer)
{
  std::streamoff  step = static_cast<std::streamoff>(this->GetNumberOfComponents());
  unsigned char * p = static_cast<unsigned char *>(buffer);
  if (p == NULL)
    {
    itkExceptionMacro(<< "Memory allocation error");
    return;
    }

  int lNbLines   = this->GetIORegion().GetSize()[1];
  int lNbColumns = this->GetIORegion().GetSize()[0];
  int lFirstLine   = this->GetIORegion().GetIndex()[1]; // [1... ]
  int lFirstColumn = this->GetIORegion().GetIndex()[0]; // [1... ]

  std::streamoff lNbPixels = (static_cast<std::streamoff>(lNbColumns)) * (static_cast<std::streamoff>(lNbLines));
  std::streamoff lBufferSize = static_cast<std::streamoff>(m_NbOctetPixel) * lNbPixels;

  unsigned char* value = new unsigned char[lBufferSize];
  if (value == NULL)
    {
    itkExceptionMacro(<< "Memory allocation error");
    return;
    }

  CPLErr         lCrGdal;
  std::streamoff cpt(0);

  if (GDALDataTypeIsComplex(m_PxType))
    {
    lCrGdal = m_poBands[0]->RasterIO(GF_Read,
                                     lFirstColumn,
                                     lFirstLine,
                                     lNbColumns,
                                     lNbLines,
                                     value,
                                     lNbColumns,
                                     lNbLines,
                                     m_PxType,
                                     0,
                                     0);

    if (lCrGdal == CE_Failure)
      {
      itkExceptionMacro(<< "Error while reading image (GDAL format) " << m_FileName.c_str() << ".");
      }
    cpt = 0;
    for (std::streamoff i = 0; i < lBufferSize; i = i + static_cast<std::streamoff>(m_NbOctetPixel))
      {
      memcpy((void*) (&(p[cpt])), (const void*) (&(value[i])), (size_t) (m_NbOctetPixel));
      cpt += static_cast<std::streamoff>(m_NbOctetPixel);
      }
    }
  else if (m_IsIndexed)
    {
    step = step * static_cast<std::streamoff>(m_NbOctetPixel);

    lCrGdal = m_poBands[0]->RasterIO(GF_Read,
                                     lFirstColumn,
                                     lFirstLine,
                                     lNbColumns,
                                     lNbLines,
                                     value,
                                     lNbColumns,
                                     lNbLines,
                                     m_PxType,
                                     0,
                                     0);
    if (lCrGdal == CE_Failure)
      {
      itkExceptionMacro(<< "Error while reading image (GDAL format) " << m_FileName.c_str() << ".");
      }
    // Recopie dans le buffer
    cpt = 0;
    for (std::streamoff i = 0; i < lBufferSize; i = i + static_cast<std::streamoff>(m_NbOctetPixel))
      {
      GDALColorEntry color;
      m_poBands[0]->GetColorTable()->GetColorEntryAsRGB(value[i], &color);
      p[cpt] = color.c1;
      p[cpt + 1] = color.c2;
      p[cpt + 2] = color.c3;
      p[cpt + 3] = color.c4;
      cpt += step;
      }

    }
  else
    {
    // Mise a jour du step
    step = step * static_cast<std::streamoff>(m_NbOctetPixel);

    for (unsigned int nbComponents = 0; nbComponents < this->GetNumberOfComponents(); ++nbComponents)
      {
      lCrGdal = m_poBands[nbComponents]->RasterIO(GF_Read,
                                                  lFirstColumn,
                                                  lFirstLine,
                                                  lNbColumns,
                                                  lNbLines,
                                                  value,
                                                  lNbColumns,
                                                  lNbLines,
                                                  m_PxType,
                                                  0,
                                                  0);
      if (lCrGdal == CE_Failure)
        {
        itkExceptionMacro(<< "Error while reading image (GDAL format) " << m_FileName.c_str() << ".");
        }
      // Recopie dans le buffer
      cpt = static_cast<std::streamoff>(nbComponents) * static_cast<std::streamoff>(m_NbOctetPixel);
      for (std::streamoff i = 0; i < lBufferSize; i = i + static_cast<std::streamoff>(m_NbOctetPixel))
        {
        memcpy((void*) (&(p[cpt])), (const void*) (&(value[i])), (size_t) (m_NbOctetPixel));
        cpt += step;
        }
      }
    }

  delete[] value;
  value = NULL;
}

void GDALImageIO::ReadImageInformation()
{
  //std::ifstream file;
  this->InternalReadImageInformation();
}

void GDALImageIO::InternalReadImageInformation()
{
  int i;

  if (m_FileName.empty() == true)
    {
    itkExceptionMacro(<< "GDAl read : empty image file name file.");
    }

  std::string lFileNameGdal = m_FileName;

  // Init GDAL parameters
  GDALAllRegister();

  // Get Dataset
  if (m_poDataset != NULL)
    {
//       delete m_poDataset;
    GDALClose(m_poDataset);
    m_poDataset = NULL;
    }
  m_poDataset = static_cast<GDALDataset *>(GDALOpen(lFileNameGdal.c_str(), GA_ReadOnly));
  otbMsgDevMacro(<< "  GCPCount (original): " << m_poDataset->GetGCPCount());

  if (m_poDataset == NULL)
    {
    itkExceptionMacro(<< "Gdal dataset is null.");
    return;
    }

//  else
//  {
  // Get image dimensions
  m_width = m_poDataset->GetRasterXSize();
  m_height = m_poDataset->GetRasterYSize();

  if ((m_width == 0) || (m_height == 0))
    {
    itkExceptionMacro(<< "Dimension is undefined.");
    }
  else
    {
    // Set image dimensions into IO
    m_Dimensions[0] = m_width;
    m_Dimensions[1] = m_height;
    }

  // Get Number of Bands
  m_NbBands = m_poDataset->GetRasterCount();
  if (m_NbBands == 0)
    {
//FIXME this happen in the case of a hdf file with SUBDATASETS
// in this situation, at least the first dataset should be open (ideally all in an imagelist)
//         char** papszMetadata;
//         papszMetadata = m_poDataset->GetMetadata("SUBDATASETS");
//         if( CSLCount(papszMetadata) > 0 )
//         {
//           std::string key;
//           itk::MetaDataDictionary & dict = this->GetMetaDataDictionary();
//           for( int cpt = 0; papszMetadata[cpt] != NULL; cpt++ )
//           {
//             std::cout << papszMetadata[cpt] << std::endl;
//               ::itk::OStringStream lStream;
//               lStream << MetaDataKey::SubMetadataKey << cpt;
//               key = lStream.str();
//
//               itk::EncapsulateMetaData<std::string>(dict, key,
//                   static_cast<std::string>( papszMetadata[cpt] ) );
//           }
//           std::cout << dict[dict.GetKeys()[0]] << std::endl;
//           std::cout << key << std::endl;
//         }

    itkExceptionMacro(<< "Zero band found in the dataset");
    return;
    }

  this->SetNumberOfComponents(m_NbBands);

  // Set the number of dimensions (verify for the dim )
  this->SetNumberOfDimensions(2);

  otbMsgDevMacro(<< "Nb of Dimensions : " << m_NumberOfDimensions);

  // Automatically set the Type to Binary for GDAL data
  this->SetFileTypeToBinary();

  // Get all the Bands
  m_poBands = new GDALRasterBand *[m_NbBands];
  if (m_poBands == NULL)
    {
    itkExceptionMacro(<< "Memory allocation error for the 'rasterBands'");
    return;
    }
  for (i = 0; i < m_NbBands; ++i)
    m_poBands[i] = m_poDataset->GetRasterBand(i + 1);

  // Get Data Type
  // Consider only the data type given by the first band!!!!!
  // Maybe be could changed (to check)
  m_PxType = m_poBands[0]->GetRasterDataType();

  // Following the data type given by GDAL we set it for ImageIO
  // BE CAREFUL !!!! At this time the complex data type are regarded
  // as integer data type in hope that ITK uses that kind of system
  // (take time to check !!)
  if (m_PxType == GDT_Byte)
    {
    SetComponentType(UCHAR);
    }
  else if (m_PxType == GDT_UInt16)
    {
    SetComponentType(USHORT);
    }
  else if ((m_PxType == GDT_Int16) || (m_PxType == GDT_CInt16))
    {
    SetComponentType(SHORT);
    }
  else if (m_PxType == GDT_UInt32)
    {
    SetComponentType(UINT);
    }
  else if ((m_PxType == GDT_Int32) || (m_PxType == GDT_CInt32))
    {
    SetComponentType(INT);
    }
  else if ((m_PxType == GDT_Float32) || (m_PxType == GDT_CFloat32))
    {
    SetComponentType(FLOAT);
    }
  else if ((m_PxType == GDT_Float64) || (m_PxType == GDT_CFloat64))
    {
    SetComponentType(DOUBLE);
    }
  else
    {
    itkExceptionMacro(<< "Pixel type unknown");
    }

  if (this->GetComponentType() == CHAR)
    {
    m_NbOctetPixel = 1;
    }
  else if (this->GetComponentType() == UCHAR)
    {
    m_NbOctetPixel = 1;
    }
  else if (this->GetComponentType() == USHORT)
    {
    m_NbOctetPixel = 2;
    }
  else if (this->GetComponentType() == SHORT)
    {
    m_NbOctetPixel = 2;
    }
  else if (this->GetComponentType() == INT)
    {
    m_NbOctetPixel = 4;
    }
  else if (this->GetComponentType() == UINT)
    {
    m_NbOctetPixel = 4;
    }
  else if (this->GetComponentType() == FLOAT)
    {
    m_NbOctetPixel = 4;
    }
  else if (this->GetComponentType() == DOUBLE)
    {
    m_NbOctetPixel = 8;
    }
  else
    {
    itkExceptionMacro(<< "Component type unknown");
    }

  /******************************************************************/
  // Pixel Type always set to Scalar for GDAL ? maybe also to vector ?

  // Modif Patrick: LIRE LES IMAGES COMPLEXES
  if (GDALDataTypeIsComplex(m_PxType))
    {
    m_NbOctetPixel = m_NbOctetPixel * 2;
    this->SetNumberOfComponents(2);
    this->SetPixelType(COMPLEX);
    // Is this necessary ?
    //if(m_NbBands !=1) itkExceptionMacro(<<"GDALImageIO::InternalReadImageInformation() Can read only one band image ");
    }
  else
    {
    this->SetNumberOfComponents(m_NbBands);
    if (this->GetNumberOfComponents() == 1)
      {
      this->SetPixelType(SCALAR);
      }
    else
      {
      this->SetPixelType(VECTOR);
      }
    }
  // }

  /*----------------------------------------------------------------------*/
  /*-------------------------- METADATA ----------------------------------*/
  /*----------------------------------------------------------------------*/

  // Now initialize the itk dictionary
  itk::MetaDataDictionary& dict = this->GetMetaDataDictionary();

  /* -------------------------------------------------------------------- */
  /*  Get Spacing                */
  /* -------------------------------------------------------------------- */

  // Default Spacing
  m_Spacing[0] = 1;
  m_Spacing[1] = 1;
  if (m_NumberOfDimensions == 3) m_Spacing[2] = 1;

  char** papszMetadata;
  papszMetadata =  m_poDataset->GetMetadata(NULL);

  /* -------------------------------------------------------------------- */
  /*      Report general info.                                            */
  /* -------------------------------------------------------------------- */
  GDALDriverH hDriver;

  hDriver = m_poDataset->GetDriver();

  std::string driverShortName =  static_cast<std::string>(GDALGetDriverShortName(hDriver));
  std::string driverLongName =  static_cast<std::string>(GDALGetDriverLongName(hDriver));

  itk::EncapsulateMetaData<std::string>(dict, MetaDataKey::DriverShortNameKey, driverShortName);
  itk::EncapsulateMetaData<std::string>(dict, MetaDataKey::DriverLongNameKey, driverLongName);

  /* -------------------------------------------------------------------- */
  /* Get the projection coordinate system of the image : ProjectionRef  */
  /* -------------------------------------------------------------------- */

  if (m_poDataset->GetProjectionRef() != NULL)
    {
    OGRSpatialReference* pSR;
    const char *         pszProjection = NULL;

    pSR = new OGRSpatialReference(pszProjection);

    pszProjection =  m_poDataset->GetProjectionRef();

    if (pSR->importFromWkt((char **) (&pszProjection)) == CE_None)
      {
      char * pszPrettyWkt = NULL;

      pSR->exportToPrettyWkt(&pszPrettyWkt, FALSE);
      itk::EncapsulateMetaData<std::string> (dict, MetaDataKey::ProjectionRefKey,
                                             static_cast<std::string>(pszPrettyWkt));

      CPLFree(pszPrettyWkt);
      }
    else
      itk::EncapsulateMetaData<std::string>(dict, MetaDataKey::ProjectionRefKey,
                                            static_cast<std::string>(m_poDataset->GetProjectionRef()));

    if (pSR != NULL)
      {
      delete pSR;
      pSR = NULL;
      }

    }

  /* -------------------------------------------------------------------- */
  /* Get the GCP projection coordinates of the image : GCPProjection  */
  /* -------------------------------------------------------------------- */

  unsigned int gcpCount = 0;
  gcpCount = m_poDataset->GetGCPCount();
  if (gcpCount > 0)
    {
    std::string gcpProjectionKey = static_cast<std::string>(m_poDataset->GetGCPProjection());
    itk::EncapsulateMetaData<std::string>(dict, MetaDataKey::GCPProjectionKey, gcpProjectionKey);

    if (gcpProjectionKey.empty())
      {
      gcpCount = 0; //fix for uninitialized gcpCount in gdal (when
                    //reading Palsar image)
      }

    std::string key;

    itk::EncapsulateMetaData<unsigned int>(dict, MetaDataKey::GCPCountKey, gcpCount);

    for (unsigned int cpt = 0; cpt < gcpCount; cpt++)
      {

      const GDAL_GCP *psGCP;
      psGCP = m_poDataset->GetGCPs() + cpt;

      OTB_GCP pOtbGCP(psGCP);

      // Complete the key with the GCP number : GCP_i
      ::itk::OStringStream lStream;
      lStream << MetaDataKey::GCPParametersKey << cpt;
      key = lStream.str();

      itk::EncapsulateMetaData<OTB_GCP>(dict, key, pOtbGCP);

      }

    }

  /* -------------------------------------------------------------------- */
  /*  Get the six coefficients of affine geoTtransform      */
  /* -------------------------------------------------------------------- */

  double     adfGeoTransform[6];
  VectorType VadfGeoTransform;

  if (m_poDataset->GetGeoTransform(adfGeoTransform) == CE_None)
    {
    for (int cpt = 0; cpt < 6; cpt++)
      VadfGeoTransform.push_back(adfGeoTransform[cpt]);

    itk::EncapsulateMetaData<VectorType>(dict, MetaDataKey::GeoTransformKey, VadfGeoTransform);

    /// retrieve orgin and spacing from the geo transform
    m_Origin[0] = VadfGeoTransform[0];
    m_Origin[1] = VadfGeoTransform[3];
    m_Spacing[0] = VadfGeoTransform[1];
    m_Spacing[1] = VadfGeoTransform[5];

    //In this case, we are in a geographic projection
    // FIXME is there any way to know if we are in WGS 84 ???
    std::string projRef =
          "GEOGCS[\"GCS_WGS_1984\",DATUM[\"D_WGS_1984\",SPHEROID[\"WGS_1984\",6378137,298.257223563]],PRIMEM[\"Greenwich\",0],UNIT[\"Degree\",0.017453292519943295]]";

    itk::EncapsulateMetaData<std::string>(dict, MetaDataKey::ProjectionRefKey, projRef);
    }

  /* -------------------------------------------------------------------- */
  /*      Report metadata.                                                */
  /* -------------------------------------------------------------------- */

  papszMetadata = m_poDataset->GetMetadata(NULL);
  if (CSLCount(papszMetadata) > 0)
    {
    std::string key;

    for (int cpt = 0; papszMetadata[cpt] != NULL; cpt++)
      {
      ::itk::OStringStream lStream;
      lStream << MetaDataKey::MetadataKey << cpt;
      key = lStream.str();

      itk::EncapsulateMetaData<std::string>(dict, key,
                                            static_cast<std::string>(papszMetadata[cpt]));
      }
    }

  /* -------------------------------------------------------------------- */
  /*      Report subdatasets.                                             */
  /* -------------------------------------------------------------------- */

  papszMetadata = m_poDataset->GetMetadata("SUBDATASETS");
  if (CSLCount(papszMetadata) > 0)
    {
    std::string key;

    for (int cpt = 0; papszMetadata[cpt] != NULL; cpt++)
      {
      ::itk::OStringStream lStream;
      lStream << MetaDataKey::SubMetadataKey << cpt;
      key = lStream.str();

      itk::EncapsulateMetaData<std::string>(dict, key,
                                            static_cast<std::string>(papszMetadata[cpt]));
      }
    }

  /* -------------------------------------------------------------------- */
  /* Report corners              */
  /* -------------------------------------------------------------------- */

  double     GeoX(0), GeoY(0);
  VectorType VGeo;

  GDALInfoReportCorner("Upper Left", 0.0, 0.0, GeoX, GeoY);
  VGeo.push_back(GeoX);
  VGeo.push_back(GeoY);

  itk::EncapsulateMetaData<VectorType>(dict, MetaDataKey::UpperLeftCornerKey, VGeo);

  VGeo.clear();

  GDALInfoReportCorner("Upper Right", m_width, 0.0, GeoX, GeoY);
  VGeo.push_back(GeoX);
  VGeo.push_back(GeoY);

  itk::EncapsulateMetaData<VectorType>(dict, MetaDataKey::UpperRightCornerKey, VGeo);

  VGeo.clear();

  GDALInfoReportCorner("Lower Left", 0.0, m_height, GeoX, GeoY);
  VGeo.push_back(GeoX);
  VGeo.push_back(GeoY);

  itk::EncapsulateMetaData<VectorType>(dict, MetaDataKey::LowerLeftCornerKey, VGeo);

  VGeo.clear();

  GDALInfoReportCorner("Lower Right", m_width, m_height, GeoX, GeoY);
  VGeo.push_back(GeoX);
  VGeo.push_back(GeoY);

  itk::EncapsulateMetaData<VectorType>(dict, MetaDataKey::LowerRightCornerKey, VGeo);

  VGeo.clear();

  /* -------------------------------------------------------------------- */
  /* Color Table                                                          */
  /* -------------------------------------------------------------------- */

  for (int iBand = 0; iBand < m_poDataset->GetRasterCount(); iBand++)
    {
    GDALColorTableH hTable;
    GDALRasterBandH hBand;
    hBand = GDALGetRasterBand(m_poDataset, iBand + 1);
    if ((GDALGetRasterColorInterpretation(hBand) == GCI_PaletteIndex)
        && (hTable = GDALGetRasterColorTable(hBand)) != NULL)
      {
      m_IsIndexed = true;

      unsigned int ColorEntryCount = GDALGetColorEntryCount(hTable);

      itk::EncapsulateMetaData<std::string>(dict, MetaDataKey::ColorTableNameKey,
                                            static_cast<std::string>(GDALGetPaletteInterpretationName(
                                                                       GDALGetPaletteInterpretation(hTable))));

      itk::EncapsulateMetaData<unsigned int>(dict, MetaDataKey::ColorEntryCountKey, ColorEntryCount);

      for (int i = 0; i < GDALGetColorEntryCount(hTable); ++i)
        {
        GDALColorEntry sEntry;
        VectorType     VColorEntry;

        GDALGetColorEntryAsRGB(hTable, i, &sEntry);

        VColorEntry.push_back(sEntry.c1);
        VColorEntry.push_back(sEntry.c2);
        VColorEntry.push_back(sEntry.c3);
        VColorEntry.push_back(sEntry.c4);

        itk::EncapsulateMetaData<VectorType>(dict, MetaDataKey::ColorEntryAsRGBKey, VColorEntry);

        }
      }
    }
  if (m_IsIndexed)
    {
    m_NbBands *= 4;
    this->SetNumberOfComponents(m_NbBands);
    this->SetPixelType(VECTOR);
    }
}

bool GDALImageIO::CanWriteFile(const char* name)
{
  // First check the extension
  if (name == NULL)
    {
    itkDebugMacro(<< "No filename specified.");
    return false;
    }

  // Recuperation du type a partir du nom de fichier
  std::string extGDAL = TypeConversion(name);
  if (extGDAL == "NOT-FOUND")
    {
    return false;
    }

  return true;

}

/** TODO : Methode Write non implementee */
void GDALImageIO::Write(const void* buffer)
{
  if (m_FlagWriteImageInformation == true)
    {
    this->InternalWriteImageInformation();
    m_FlagWriteImageInformation = false;
    }

  const unsigned char *p = static_cast<const unsigned char *>(buffer);
  if (p == NULL)
    {
    itkExceptionMacro(<< "GDAL : Bad alloc");
    return;
    }

  unsigned int lNbLines   = this->GetIORegion().GetSize()[1];
  unsigned int lNbColumns = this->GetIORegion().GetSize()[0];
  int          lFirstLine   = this->GetIORegion().GetIndex()[1]; // [1... ]
  int          lFirstColumn = this->GetIORegion().GetIndex()[0]; // [1... ]

  // Particular case: checking that the writen region is the same size
  // of the entire image
  // starting at offset 0 (when no streaming)
  if ((lNbLines == m_Dimensions[1]) && (lNbColumns == m_Dimensions[0]))
    {
    lFirstLine = 0;
    lFirstColumn = 0;
    }

  std::streamoff lNbPixels = static_cast<std::streamoff>(lNbColumns) * static_cast<std::streamoff>(lNbLines);
  std::streamoff lBufferSize = static_cast<std::streamoff>(m_NbOctetPixel) * lNbPixels;
  otbMsgDevMacro(<< " BufferSize allocated : " << lBufferSize);

  unsigned char* value = new unsigned char[lBufferSize];
  if (value == NULL)
    {
    itkExceptionMacro(<< "Memory allocation error");
    return;
    }

  // Mise a jour du step
  std::streamoff step = static_cast<std::streamoff>(m_NbBands);
  step = step * static_cast<std::streamoff>(m_NbOctetPixel);

  CPLErr lCrGdal;

  std::streamoff cpt(0);
  for (int nbComponents = 0; nbComponents < m_NbBands; ++nbComponents)
    {
    cpt = static_cast<std::streamoff>(nbComponents) * static_cast<std::streamoff>(m_NbOctetPixel);

    for (std::streamoff i = 0; i < lBufferSize; i = i + static_cast<std::streamoff>(m_NbOctetPixel))
      {
      memcpy((void*) (&(value[i])), (const void*) (&(p[cpt])), (size_t) (m_NbOctetPixel));
      cpt += step;
      }
    GDALRasterBand *poBand;
    poBand =  m_poBands[nbComponents]; //m_poDataset->GetRasterBand(nbComponents+1);
//          lCrGdal = poBand->RasterIO(GF_Write,lFirstColumn,lFirstLine,lNbColumns, lNbLines, value , lNbColumns, lNbLines, m_PxType,0, 0 );
    lCrGdal = m_poBands[nbComponents]->RasterIO(GF_Write,
                                                lFirstColumn,
                                                lFirstLine,
                                                lNbColumns,
                                                lNbLines,
                                                value,
                                                lNbColumns,
                                                lNbLines,
                                                m_PxType,
                                                0,
                                                0);
    if (lCrGdal == CE_Failure)
      {
      itkExceptionMacro(<< "Error while writing image (GDAL format) " << m_FileName.c_str() << ".");
      }
    m_poBands[nbComponents]->FlushCache();
    }

  delete[] value;
  value = NULL;

  m_poDataset->FlushCache();
}

/** TODO : Methode WriteImageInformation non implementee */
void GDALImageIO::WriteImageInformation()
{

}

void GDALImageIO::InternalWriteImageInformation()
{

  char **     papszOptions = NULL;
  std::string extGDAL;
  m_NbBands = this->GetNumberOfComponents();

  if ((m_Dimensions[0] == 0) && (m_Dimensions[1] == 0))
    {
    itkExceptionMacro(<< "Dimensions are not defined.");
    }

  if ((this->GetPixelType() == COMPLEX) && (m_NbBands / 2 > 0))
    {
    m_NbBands /= 2;

    if (this->GetComponentType() == SHORT)
      {
      m_NbOctetPixel = 4;
      m_PxType = GDT_CInt16;
      }
    else if (this->GetComponentType() == INT)
      {
      m_NbOctetPixel = 8;
      m_PxType = GDT_CInt32;
      }
    else if (this->GetComponentType() == DOUBLE)
      {
      m_NbOctetPixel = 16;
      m_PxType = GDT_CFloat64;
      }
    else if (this->GetComponentType() == FLOAT)
      {
      m_NbOctetPixel = 8;
      m_PxType = GDT_CFloat32;
      }
    else
      {
      this->SetComponentType(FLOAT);
      m_NbOctetPixel = 8;
      m_PxType = GDT_CFloat32;
      }
    }
  else
    {
    if (this->GetComponentType() == CHAR)
      {
      m_NbOctetPixel = 1;
      m_PxType = GDT_Byte;
      }
    else if (this->GetComponentType() == UCHAR)
      {
      m_NbOctetPixel = 1;
      m_PxType = GDT_Byte;
      }
    else if (this->GetComponentType() == USHORT)
      {
      m_NbOctetPixel = 2;
      m_PxType = GDT_UInt16;
      }
    else if (this->GetComponentType() == SHORT)
      {
      m_NbOctetPixel = 2;
      m_PxType = GDT_Int16;
      }
    else if (this->GetComponentType() == INT)
      {
      m_NbOctetPixel = 4;
      m_PxType = GDT_Int32;
      }
    else if (this->GetComponentType() == UINT)
      {
      m_NbOctetPixel = 4;
      m_PxType = GDT_UInt32;
      }
    else if (this->GetComponentType() == FLOAT)
      {
      m_NbOctetPixel = 4;
      m_PxType = GDT_Float32;
      }
    else if (this->GetComponentType() == DOUBLE)
      {
      m_NbOctetPixel = 8;
      m_PxType = GDT_Float64;
      }
    else
      {
      m_NbOctetPixel = 1;
      m_PxType = GDT_Byte;
      }
    }

  // Automatically set the Type to Binary for GDAL data
  this->SetFileTypeToBinary();

  GDALAllRegister();

  extGDAL = TypeConversion(m_FileName);
  if (extGDAL == "NOT-FOUND")
    {
    itkExceptionMacro(
      << "GDAL Writing failed : the image file name '" << m_FileName.c_str() << "' is not reconized by GDAL.");
    }

  m_hDriver = GetGDALDriverManager()->GetDriverByName(extGDAL.c_str());
  if (m_hDriver == NULL)
    {
    itkExceptionMacro(<< "GDAL Writing failed : Driver not reconized");
    }
  std::string realFileName = GetGdalWriteImageFileName(extGDAL, m_FileName);

  if (m_poDataset != NULL)
    {
    GDALClose(m_poDataset);
    m_poDataset = NULL;
    }
  m_poDataset = m_hDriver->Create(
    realFileName.c_str(), m_Dimensions[0], m_Dimensions[1], m_NbBands, m_PxType, papszOptions);
  if (m_poDataset == NULL)
    {
    itkExceptionMacro(
      << "GDAL Writing failed : Impossible to create the image file name '" << realFileName.c_str() << "'.");
    }

  // Get all the Bands
  m_poBands = new GDALRasterBand *[m_NbBands];

  if (m_poBands == NULL)
    {
    itkExceptionMacro(<< "Memory allocation error for 'rasterBands'");
    }
  for (int i = 0; i < m_NbBands; ++i)
    {
    m_poBands[i] = m_poDataset->GetRasterBand(i + 1);
    }

  // JULIEN: ADDING SUPPORT FOR METADATA WRITING.

  /*----------------------------------------------------------------------*/
  /*-------------------------- METADATA ----------------------------------*/
  /*----------------------------------------------------------------------*/

  // Now initialize the itk dictionary
  itk::MetaDataDictionary& dict = this->GetMetaDataDictionary();
  char**                   papszMetadata;
  papszMetadata =  m_poDataset->GetMetadata(NULL);
  itk::OStringStream oss;

  /* -------------------------------------------------------------------- */
  /* Set the GCPs                                                          */
  /* -------------------------------------------------------------------- */

  unsigned int gcpCount = 0;
  itk::ExposeMetaData<unsigned int>(dict, MetaDataKey::GCPCountKey, gcpCount);

  if (gcpCount > 0)
    {

    GDAL_GCP * gdalGcps = new GDAL_GCP[gcpCount];

    for (unsigned int gcpIndex = 0; gcpIndex < gcpCount; ++gcpIndex)
      {
      //Build the GCP string in the form of GCP_n
      itk::OStringStream lStream;
      lStream << MetaDataKey::GCPParametersKey << gcpIndex;
      std::string key = lStream.str();

      OTB_GCP gcp;
      itk::ExposeMetaData<OTB_GCP>(dict, key, gcp);

      gdalGcps[gcpIndex].pszId = const_cast<char *>(gcp.m_Id.c_str());
      gdalGcps[gcpIndex].pszInfo = const_cast<char *>(gcp.m_Info.c_str());
      gdalGcps[gcpIndex].dfGCPPixel = gcp.m_GCPCol;
      gdalGcps[gcpIndex].dfGCPLine = gcp.m_GCPRow;
      gdalGcps[gcpIndex].dfGCPX = gcp.m_GCPX;
      gdalGcps[gcpIndex].dfGCPY = gcp.m_GCPY;
      gdalGcps[gcpIndex].dfGCPZ = gcp.m_GCPZ;

      }

    std::string gcpProjectionRef;
    itk::ExposeMetaData<std::string>(dict, MetaDataKey::GCPProjectionKey, gcpProjectionRef);
    m_poDataset->SetGCPs(gcpCount, gdalGcps, gcpProjectionRef.c_str());

    delete[] gdalGcps;
    }

  /* -------------------------------------------------------------------- */
  /* Set the projection coordinate system of the image : ProjectionRef  */
  /* -------------------------------------------------------------------- */

  std::string projectionRef;
  itk::ExposeMetaData<std::string>(dict, MetaDataKey::ProjectionRefKey, projectionRef);
  if (!projectionRef.empty())
    {
    m_poDataset->SetProjection(projectionRef.c_str());
    }

  /* -------------------------------------------------------------------- */
  /*  Set the six coefficients of affine geoTtransform      */
  /* -------------------------------------------------------------------- */

  double * geoTransform = new double[6];
  /// Reporting origin and spacing
  geoTransform[0] = m_Origin[0];
  geoTransform[3] = m_Origin[1];
  geoTransform[1] = m_Spacing[0];
  geoTransform[5] = m_Spacing[1];

  // FIXME: Here component 1 and 4 should be replaced by the orientation parameters
  geoTransform[2] = 0.;
  geoTransform[4] = 0.;
  m_poDataset->SetGeoTransform(geoTransform);
  delete[] geoTransform;

  /* -------------------------------------------------------------------- */
  /*      Report metadata.                                                */
  /* -------------------------------------------------------------------- */

  std::string              svalue = "";
  std::vector<std::string> keys = dict.GetKeys();
  MetaDataKey              key;

  for (unsigned int itkey = 0; itkey < keys.size(); ++itkey)
    {
    if (keys[itkey].compare(0, key.MetadataKey.length(), key.MetadataKey) == 0)
      {
      itk::ExposeMetaData<std::string>(dict, keys[itkey], svalue);
      unsigned int equalityPos = svalue.find_first_of('=');
      std::string  tag = svalue.substr(0, equalityPos);
      std::string  value = svalue.substr(equalityPos + 1);
      otbMsgDevMacro(<< "Metadata: " << tag << "=" << value);
      m_poDataset->SetMetadataItem(tag.c_str(), value.c_str(), NULL);
      }
    }
  // END

}

std::string GDALImageIO::TypeConversion(std::string name)
{
  std::string extension;
  std::string extGDAL;

  //Recuperer extension du fichier image
  extension = System::GetExtension(name);

  if ((extension == "tif") || (extension == "tiff") || (extension == "TIF") || (extension == "TIFF")) extGDAL = "GTiff";
  else if ((extension == "hdr") || (extension == "HDR")) extGDAL = "ENVI";
  else if ((extension == "img") || (extension == "IMG")) extGDAL = "HFA";
  else if (extension == "ntf") extGDAL = "NITF";
//Pas PNG car BUG !!
//  else if ((extension=="png")||(extension=="PNG"))
//      extGDAL="PNG";
//Pas JPEG car BUG !!
//  else if ((extension=="jpg")||(extension=="JPG")||(extension=="jpeg")||(extension=="JPEG"))
//      extGDAL="JPEG";
  else extGDAL = "NOT-FOUND";
  return extGDAL;
}

std::string GDALImageIO::GetGdalWriteImageFileName(std::string& extGDAL, std::string filename)
{
  std::string gdalFileName;

  gdalFileName = filename;
  //Suppression de l'extension HDR
  if (extGDAL == "ENVI")
    {
    gdalFileName = System::GetRootName(filename);
    }
  return gdalFileName;
}

bool GDALImageIO::GDALInfoReportCorner(const char * /*corner_name*/, double x, double y, double& GeoX, double& GeoY)
{
  const char *pszProjection;
  double      adfGeoTransform[6];

  bool IsTrue;

  /* -------------------------------------------------------------------- */
  /*      Transform the point into georeferenced coordinates.             */
  /* -------------------------------------------------------------------- */
  if (m_poDataset->GetGeoTransform(adfGeoTransform) == CE_None)
    {
    pszProjection = m_poDataset->GetProjectionRef();

    GeoX = adfGeoTransform[0] + adfGeoTransform[1] * x
           + adfGeoTransform[2] * y;
    GeoY = adfGeoTransform[3] + adfGeoTransform[4] * x
           + adfGeoTransform[5] * y;
    IsTrue = true;
    }

  else
    {
    GeoX = x;
    GeoY = y;
    IsTrue = false;
    }

  return IsTrue;
}

} // end namespace otb
