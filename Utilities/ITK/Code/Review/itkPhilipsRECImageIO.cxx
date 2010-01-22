/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkPhilipsRECImageIO.cxx,v $
  Language:  C++
  Date:      $Date: 2009-04-29 12:20:40 $
  Version:   $Revision: 1.13 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
 
=========================================================================*/
/** 
 * \author Don C. Bigler
 *         The Pennsylvania State University 2005
 *
 * This implementation was contributed as a paper to the Insight Journal
 * http://insight-journal.org/midas/handle.php?handle=1926/1381
 *
 */
 
#include "itkPhilipsRECImageIO.h"
#include "itkPhilipsPAR.h"
#include "itkIOCommon.h"
#include "itkExceptionObject.h"
#include "itkByteSwapper.h"
#include "itkMetaDataObject.h"
#include "itkSpatialOrientationAdapter.h"
#include <itksys/SystemTools.hxx>
#include "itk_zlib.h"
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

namespace itk
{

const char *const PAR_Version = "PAR_Version";
const char *const PAR_SliceOrientation = "PAR_SliceOrientation";
const char *const PAR_ExaminationName = "PAR_ExaminationName";
const char *const PAR_ProtocolName = "PAR_ProtocolName";
const char *const PAR_SeriesType = "PAR_SeriesType";
const char *const PAR_AcquisitionNr = "PAR_AcquisitionNr";
const char *const PAR_ReconstructionNr = "PAR_ReconstructionNr";
const char *const PAR_ScanDuration = "PAR_ScanDuration";
const char *const PAR_MaxNumberOfCardiacPhases = "PAR_MaxNumberOfCardiacPhases";
const char *const PAR_TriggerTimes = "PAR_TriggerTimes";
const char *const PAR_MaxNumberOfEchoes = "PAR_MaxNumberOfEchoes";
const char *const PAR_EchoTimes = "PAR_EchoTimes";
const char *const PAR_MaxNumberOfDynamics = "PAR_MaxNumberOfDynamics";
const char *const PAR_MaxNumberOfMixes = "PAR_MaxNumberOfMixes";
const char *const PAR_PatientPosition = "PAR_PatientPosition";
const char *const PAR_PreparationDirection = "PAR_PreparationDirection";
const char *const PAR_Technique = "PAR_Technique";
const char *const PAR_ScanMode = "PAR_ScanMode";
const char *const PAR_NumberOfAverages = "PAR_NumberOfAverages";
const char *const PAR_ScanResolution = "PAR_ScanResolution";
const char *const PAR_RepetitionTimes = "PAR_RepetitionTimes";
const char *const PAR_ScanPercentage = "PAR_ScanPercentage";
const char *const PAR_FOV = "PAR_FOV";
const char *const PAR_WaterFatShiftPixels = "PAR_WaterFatShiftPixels";
const char *const PAR_AngulationMidSlice = "PAR_AngulationMidSlice";
const char *const PAR_OffCentreMidSlice = "PAR_OffCentreMidSlice";
const char *const PAR_FlowCompensation = "PAR_FlowCompensation";
const char *const PAR_Presaturation = "PAR_Presaturation";
const char *const PAR_CardiacFrequency = "PAR_CardiacFrequency";
const char *const PAR_MinRRInterval = "PAR_MinRRInterval";
const char *const PAR_MaxRRInterval = "PAR_MaxRRInterval";
const char *const PAR_PhaseEncodingVelocity = "PAR_PhaseEncodingVelocity";
const char *const PAR_MTC = "PAR_MTC";
const char *const PAR_SPIR = "PAR_SPIR";
const char *const PAR_EPIFactor = "PAR_EPIFactor";
const char *const PAR_TurboFactor = "PAR_TurboFactor";
const char *const PAR_DynamicScan = "PAR_DynamicScan";
const char *const PAR_Diffusion = "PAR_Diffusion";
const char *const PAR_DiffusionEchoTime = "PAR_DiffusionEchoTime";
const char *const PAR_MaxNumberOfDiffusionValues = 
  "PAR_MaxNumberOfDiffusionValues";
const char *const PAR_GradientBValues = "PAR_GradientBValues";
const char *const PAR_MaxNumberOfGradientOrients = 
  "PAR_MaxNumberOfGradientOrients";
const char *const PAR_GradientDirectionValues = "PAR_GradientDirectionValues";
const char *const PAR_InversionDelay = "PAR_InversionDelay";
const char *const PAR_NumberOfImageTypes = "PAR_NumberOfImageTypes";
const char *const PAR_ImageTypes = "PAR_ImageTypes";
const char *const PAR_NumberOfScanningSequences = 
  "PAR_NumberOfScanningSequences";
const char *const PAR_ScanningSequences = "PAR_ScanningSequences";
const char *const PAR_ScanningSequenceImageTypeRescaleValues = 
  "PAR_ScanningSequenceImageTypeRescaleValues";
const char *const PAR_NumberOfASLLabelTypes = "PAR_NumberOfASLLabelTypes";
const char *const PAR_ASLLabelTypes = "PAR_ASLLabelTypes";

static std::string
GetExtension( const std::string& filename )
{

  std::string fileExt(itksys::SystemTools::GetFilenameLastExtension(filename));
  // If the last extension is .gz, then need to pull off 2 extensions.
  //.gz is the only valid compression extension.
  if(fileExt == std::string(".gz"))
    {
    fileExt = itksys::SystemTools::GetFilenameLastExtension(
      itksys::SystemTools::GetFilenameWithoutLastExtension(filename));
    fileExt += ".gz";
    }
  // Check that a valid extension was found
  // Will check for either all caps or all lower-case.
  // By default the Philips Pride Workstation outputs
  // the filenames as all caps, but a user may change the
  // filenames to lowercase.  This will allow one or the
  // other.  Mixed caps/lower-case will always (with the 
  // exception of the lower-case gz on the end which is 
  // always assumed to be lower-case) fail on an OS with
  // a case sensitive file system.
  if(fileExt != ".REC.gz" 
    && fileExt != ".REC" 
    && fileExt != ".PAR"
    && fileExt != ".rec.gz" 
    && fileExt != ".rec" 
    && fileExt != ".par")
    {
    return ( "" );
    }

  return( fileExt );
}

static std::string
GetRootName( const std::string& filename )
{
  const std::string fileExt = GetExtension(filename);

  // Create a base filename
  // i.e Image.PAR --> Image
  if( fileExt.length() > 0 
      && filename.length() > fileExt.length() )
    {
    const std::string::size_type it = filename.find_last_of( fileExt );
    std::string baseName( filename, 0, it-(fileExt.length()-1) );
    return( baseName );
    }
  //Default to return same as input when the extension is nothing.
  return( filename );
}


static std::string
GetHeaderFileName( const std::string & filename )
{
  std::string ImageFileName(filename);
  const std::string fileExt = GetExtension(filename);
  // Accomodate either all caps or all lower-case filenames.
  if( (fileExt == ".REC") || (fileExt == ".REC.gz") ) 
    {
    ImageFileName = GetRootName(filename);
    ImageFileName += ".PAR";
    }
  else if( (fileExt == ".rec") || (fileExt == ".rec.gz") )
    {
    ImageFileName = GetRootName(filename);
    ImageFileName += ".par";
    }
  return( ImageFileName );
}

//Returns the base image filename.
static std::string GetImageFileName( const std::string& filename )
{
  std::string ImageFileName(filename);
  const std::string fileExt = GetExtension(filename);
  // Default to uncompressed .REC if .PAR is given as file name.
  if(fileExt == ".PAR") 
    {
    ImageFileName = GetRootName(filename);
    ImageFileName += ".REC";
    }
  else if(fileExt == ".par")
    {
    ImageFileName = GetRootName(filename);
    ImageFileName += ".rec";
    }
  return( ImageFileName );
}

//----------------------------------------------------------------------------
// This generates the correct offset to the desired image type and
// scanning sequence (randomly ordered in the REC).
int PhilipsRECImageIOGetImageTypeOffset(int imageType, int scanSequence, 
  int volumeIndex, int slice, int numSlices, struct par_parameter parParam, 
  PhilipsPAR::PARSliceIndexImageTypeVector sliceImageTypesIndex,
  PhilipsPAR::PARSliceIndexScanSequenceVector sliceScanSequenceIndex)
{
  int index = volumeIndex*parParam.num_slice_repetitions*numSlices + 
    slice*parParam.num_slice_repetitions;
  int i;
  for(i=0; i<parParam.num_slice_repetitions; i++)
    {
    if( (sliceImageTypesIndex[index+i].second == imageType) &&
        (sliceScanSequenceIndex[index+i].second == scanSequence) )
      {
      break;
      }
    }
  return i;
}

//----------------------------------------------------------------------------
// This creates the desired slice order index (slice or image block).
void PhilipsRECImageIOSetupSliceIndex(
  PhilipsRECImageIO::SliceIndexType *indexMatrix, int sortBlock,
  struct par_parameter parParam, 
  PhilipsPAR::PARImageTypeScanSequenceVector imageTypesScanSequenceIndex,
  PhilipsPAR::PARSliceIndexImageTypeVector sliceImageTypesIndex,
  PhilipsPAR::PARSliceIndexScanSequenceVector sliceScanSequenceIndex)
{
  int index = 0;
  int actualSlices = parParam.slice;
  int remainingVolumes = parParam.image_blocks/parParam.num_slice_repetitions;
  
  if(indexMatrix->size() != 
    (PhilipsRECImageIO::SliceIndexType::size_type)parParam.dim[2])
    {
    OStringStream message;
    message << "indexMatrix->size(): "
            << indexMatrix->size()
            << " != parParam.dim[2]: "
            << parParam.dim[2];
    ExceptionObject exception(__FILE__, __LINE__,
                              message.str(),
                              ITK_LOCATION);
    throw exception;
    }
  if(parParam.dim[2] != (parParam.slice*parParam.image_blocks))
    {
    OStringStream message;
    message << "parParam.dim[2]: " 
            << parParam.dim[2] 
            << " != (parParam.slice*parParam.image_blocks): "
            << parParam.slice * parParam.image_blocks;
    ExceptionObject exception(__FILE__, __LINE__,
                              message.str(),
                              ITK_LOCATION);
    throw exception;
    }
  if(imageTypesScanSequenceIndex.size() != 
    (PhilipsRECImageIO::SliceIndexType::size_type)parParam.num_slice_repetitions)
    {
    OStringStream message;
    message << "imageTypesScanSequenceIndex.size(): "
            << imageTypesScanSequenceIndex.size()
            << " != parParam.num_slice_repetitions "
            << parParam.num_slice_repetitions;
    ExceptionObject exception(__FILE__, __LINE__,
                              message.str(),
                              ITK_LOCATION);
    throw exception;
    }
  
  // Different index depending on the desired slice sort and the REC
  // slice order.
  if( (sortBlock && parParam.slicessorted) ||
      (!sortBlock && !parParam.slicessorted) )
    {
    // No sorting nessecary for these cases.
    for(int i=0; i<parParam.dim[2]; i++)
      {
      (*indexMatrix)[i] = i;
      }
    }
  // This case is the real problematic one.
  else if( sortBlock && !parParam.slicessorted && 
    (parParam.num_slice_repetitions > 1) )
    {
    // Ok, need to figure out where all of the images are located
    // using sliceImageTypesIndex and sliceScanSequenceIndex.
    for(int i=0; i<parParam.num_slice_repetitions; i++)
      {
      for(int j=0; j<remainingVolumes; j++)
        {
        for(int k=0; k<actualSlices; k++)
          {
          (*indexMatrix)[index] = j*parParam.num_slice_repetitions*actualSlices 
            + k*parParam.num_slice_repetitions
            + PhilipsRECImageIOGetImageTypeOffset(
                imageTypesScanSequenceIndex[i].first,
                imageTypesScanSequenceIndex[i].second,j,k,actualSlices,parParam,
                sliceImageTypesIndex,sliceScanSequenceIndex);
          index++;
          }
        }
      }
    }
  else
    {
    // Unsort image block or sort by image block.
    for(int i=0; i<parParam.image_blocks; i++)
      {
      for(int j=0; j<actualSlices; j++)
        {
        (*indexMatrix)[index] = j*parParam.image_blocks+i;
        index++;
        }
      }
    }
}

void
PhilipsRECImageIO::SwapBytesIfNecessary( void* buffer, 
  unsigned long numberOfPixels )
{
  if ( m_ByteOrder == LittleEndian )
    {
    switch(this->m_ComponentType)
      {
      case CHAR:
        ByteSwapper<char>::SwapRangeFromSystemToLittleEndian
          ((char*)buffer, numberOfPixels );
        break;
      case UCHAR:
        ByteSwapper<unsigned char>::SwapRangeFromSystemToLittleEndian
          ((unsigned char*)buffer, numberOfPixels );
        break;
      case SHORT:
        ByteSwapper<short>::SwapRangeFromSystemToLittleEndian
          ((short*)buffer, numberOfPixels );
        break;
      case USHORT:
        ByteSwapper<unsigned short>::SwapRangeFromSystemToLittleEndian
          ((unsigned short*)buffer, numberOfPixels );
        break;
      case INT:
        ByteSwapper<int>::SwapRangeFromSystemToLittleEndian
          ((int*)buffer, numberOfPixels );
        break;
      case UINT:
        ByteSwapper<unsigned int>::SwapRangeFromSystemToLittleEndian
          ((unsigned int*)buffer, numberOfPixels );
        break;
      case LONG:
        ByteSwapper<long>::SwapRangeFromSystemToLittleEndian
          ((long*)buffer, numberOfPixels );
        break;
      case ULONG:
        ByteSwapper<unsigned long>::SwapRangeFromSystemToLittleEndian
          ((unsigned long*)buffer, numberOfPixels );
        break;
      case FLOAT:
        ByteSwapper<float>::SwapRangeFromSystemToLittleEndian
          ((float*)buffer, numberOfPixels );
        break;
      case DOUBLE:
        ByteSwapper<double>::SwapRangeFromSystemToLittleEndian
          ((double*)buffer, numberOfPixels );
        break;
      default:
        ExceptionObject exception(__FILE__, __LINE__,
                                  "Component Type Unknown",
                                  ITK_LOCATION);
        throw exception;
      }
    }
  else
    {
    switch(this->m_ComponentType)
      {
      case CHAR:
        ByteSwapper<char>::SwapRangeFromSystemToBigEndian
          ((char *)buffer, numberOfPixels );
        break;
      case UCHAR:
        ByteSwapper<unsigned char>::SwapRangeFromSystemToBigEndian
          ((unsigned char *)buffer, numberOfPixels );
        break;
      case SHORT:
        ByteSwapper<short>::SwapRangeFromSystemToBigEndian
          ((short *)buffer, numberOfPixels );
        break;
      case USHORT:
        ByteSwapper<unsigned short>::SwapRangeFromSystemToBigEndian
          ((unsigned short *)buffer, numberOfPixels );
        break;
      case INT:
        ByteSwapper<int>::SwapRangeFromSystemToBigEndian
          ((int *)buffer, numberOfPixels );
        break;
      case UINT:
        ByteSwapper<unsigned int>::SwapRangeFromSystemToBigEndian
          ((unsigned int *)buffer, numberOfPixels );
        break;
      case LONG:
        ByteSwapper<long>::SwapRangeFromSystemToBigEndian
          ((long *)buffer, numberOfPixels );
        break;
      case ULONG:
        ByteSwapper<unsigned long>::SwapRangeFromSystemToBigEndian
          ((unsigned long *)buffer, numberOfPixels );
        break;
      case FLOAT:
        ByteSwapper<float>::SwapRangeFromSystemToBigEndian
          ((float *)buffer, numberOfPixels );
        break;
      case DOUBLE:
        ByteSwapper<double>::SwapRangeFromSystemToBigEndian
          ((double *)buffer, numberOfPixels );
        break;
      default:
        ExceptionObject exception(__FILE__, __LINE__,
                                  "Component Type Unknown",
                                  ITK_LOCATION);
        throw exception;
      }
    }
}

PhilipsRECImageIO::PhilipsRECImageIO()
{
  //by default, have 4 dimensions
  this->SetNumberOfDimensions(4);
  this->m_PixelType         = SCALAR;
  this->m_ComponentType     = CHAR;

  // Set m_MachineByteOrder to the ByteOrder of the machine
  // Start out with file byte order == system byte order
  // this will be changed if we're reading a file to whatever
  // the file actually contains.
  if(ByteSwapper<int>::SystemIsBigEndian())
    {
    this->m_MachineByteOrder = this->m_ByteOrder = BigEndian;
    }
  else
    {
    this->m_MachineByteOrder = this->m_ByteOrder = LittleEndian;
    }
  this->m_SliceIndex = new SliceIndexType();
}

PhilipsRECImageIO::~PhilipsRECImageIO()
{
  delete this->m_SliceIndex;
}

void PhilipsRECImageIO::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

int PhilipsRECImageIO::GetSliceIndex(int index)
{
  if( (index < 0) || (index > (int)(this->m_SliceIndex->size()-1)) )
    {
    return -1;
    }
  return (*this->m_SliceIndex)[index];
}

void PhilipsRECImageIO::Read(void* buffer)
{
  unsigned int dim;
  const unsigned int dimensions = this->GetNumberOfDimensions();
  unsigned int numberOfPixels = 1;
  for(dim=0; dim< dimensions; dim++ )
    {
    numberOfPixels *= this->m_Dimensions[ dim ];
    }

  char * const p = static_cast<char *>(buffer);
  //6 cases to handle
  //1: given .PAR and image is .REC
  //2: given .REC
  //3: given .REC.gz
  //4: given .par and image is .rec
  //5: given .rec
  //6: given .rec.gz

  /* Returns proper name for cases 1,2,3,4,5,6 */
  std::string ImageFileName = GetImageFileName( this->m_FileName );
  //NOTE: gzFile operations act just like FILE * operations when the files
  // are not in gzip fromat.
  // This greatly simplifies the following code, and gzFile types are used
  // everywhere.
  // In addition, it has the added benifit of reading gzip compressed image
  // files that do not have a .gz ending.
  gzFile file_p = ::gzopen( ImageFileName.c_str(), "rb" );
  if( file_p == NULL )
    {
    OStringStream message;
    message << "Philips REC Data File can not be opened. "
            << "The following files were attempted:" << std::endl
            << GetImageFileName( this->m_FileName ) << std::endl
            << ImageFileName;
    ExceptionObject exception(__FILE__, __LINE__,
                              message.str(),
                              ITK_LOCATION);
    throw exception;
    }
  
  // read image a slice at a time (sorted).
  unsigned int imageSliceSizeInBytes = this->GetImageSizeInBytes()
    /(this->m_Dimensions[2]*this->m_Dimensions[3]);
  for(unsigned int slice=0; slice<this->m_Dimensions[2]*this->m_Dimensions[3]; 
    slice++)
    {
    int realIndex = this->GetSliceIndex((int)slice);
    if( realIndex < 0 )
      {
      OStringStream message;
      message << "Philips REC Data File can not be read. "
              << "The following files were attempted:" << std::endl
              << GetImageFileName( this->m_FileName ) << std::endl
              << ImageFileName;
      ExceptionObject exception(__FILE__, __LINE__,
                                message.str(),
                                ITK_LOCATION);
      throw exception;
      }
    ::gzseek( file_p, (unsigned int)realIndex*imageSliceSizeInBytes, SEEK_SET );
    ::gzread( file_p, p+(slice*imageSliceSizeInBytes), imageSliceSizeInBytes);
    }
  gzclose( file_p );
  SwapBytesIfNecessary( buffer, numberOfPixels );
}

bool PhilipsRECImageIO::CanReadFile( const char* FileNameToRead )
{
  std::string filename(FileNameToRead);

  // we check that the correct extension is given by the user
  std::string filenameext = GetExtension(filename);
  if(  filenameext != std::string(".PAR")
    && filenameext != std::string(".REC")
    && filenameext != std::string(".REC.gz") 
    && filenameext != std::string(".par")
    && filenameext != std::string(".rec")
    && filenameext != std::string(".rec.gz"))
    {
    return false;
    }
  
  const std::string HeaderFileName = GetHeaderFileName(filename);

  // Try to read the par file.
  struct par_parameter par;
  // Zero out par_parameter.
  memset(&par,0, sizeof(struct par_parameter));

  PhilipsPAR::Pointer philipsPAR = PhilipsPAR::New();
  try
    {
    philipsPAR->ReadPAR(HeaderFileName, &par);
    // Check to see if there were any problems reading
    // the par file.
    if( par.problemreading )
      {
      return false;
      }
    }
  catch(ExceptionObject &)
    {
    return false;
    }

  return true;
}

void PhilipsRECImageIO::ReadImageInformation()
{
  const std::string HeaderFileName = GetHeaderFileName( this->m_FileName );
  struct par_parameter par;

  // Zero out par_parameter.
  memset(&par,0, sizeof(struct par_parameter));

  // Read PAR file.
  PhilipsPAR::Pointer philipsPAR = PhilipsPAR::New();
  try
    {
   philipsPAR->ReadPAR( HeaderFileName, &par);
    }
  catch(itk::ExceptionObject &err)
    {
    throw err;
    }
  if( par.problemreading )
    {
    ExceptionObject exception(__FILE__, __LINE__,
                              "Problem reading PAR file",
                              ITK_LOCATION);
    throw exception;
    }
  
  // Get all the diffusion info, rescale, etc.
  GradientBvalueContainerType::Pointer diffusionBvalueVector 
    = GradientBvalueContainerType::New();
  GradientDirectionContainerType::Pointer diffusionGradientOrientationVector 
    = GradientDirectionContainerType::New();
  if( !philipsPAR->GetDiffusionGradientOrientationAndBValues(HeaderFileName,
    diffusionGradientOrientationVector, diffusionBvalueVector) )
    {
    ExceptionObject exception(__FILE__, __LINE__,
      "Problem reading diffusion gradients and b values from PAR file",
      ITK_LOCATION);
    throw exception;
    }
    
  // Get ASL label types.
  LabelTypesASLContainerType::Pointer labelTypesASLVector = 
    LabelTypesASLContainerType::New();
  if( !philipsPAR->GetLabelTypesASL(HeaderFileName, labelTypesASLVector) )
    {
    ExceptionObject exception(__FILE__, __LINE__,
      "Problem reading ASL label types from PAR file",
      ITK_LOCATION);
    throw exception;
    }
    
  // Get rescale values associated with each scanning sequence.
  ScanningSequenceImageTypeRescaleValuesContainerType::Pointer 
    scanningSequenceImageTypeRescaleVector = 
    ScanningSequenceImageTypeRescaleValuesContainerType::New();
  scanningSequenceImageTypeRescaleVector->clear();
  // Must match number of scanning sequences.
  scanningSequenceImageTypeRescaleVector->resize(par.num_scanning_sequences); 
  for(int scanIndex=0; scanIndex<par.num_scanning_sequences; scanIndex++)
    {
    ImageTypeRescaleValuesContainerType::Pointer imageTypeRescaleValuesVector = 
      ImageTypeRescaleValuesContainerType::New();
    if( !philipsPAR->GetRECRescaleValues(HeaderFileName,imageTypeRescaleValuesVector,
      par.scanning_sequences[scanIndex]) )
      {
      ExceptionObject exception(__FILE__, __LINE__,
        "Problem reading recale values for each scanning sequence from PAR file",
        ITK_LOCATION);
      throw exception;
      }
    (*scanningSequenceImageTypeRescaleVector)[scanIndex] = 
      imageTypeRescaleValuesVector;
    }
  
  // Setup the slice index matrix.
  this->m_SliceIndex->clear();
  this->m_SliceIndex->resize(par.dim[2]);
  PhilipsPAR::PARSliceIndexImageTypeVector sliceImageTypesIndexes = 
    philipsPAR->GetRECSliceIndexImageTypes(HeaderFileName);
  PhilipsPAR::PARSliceIndexScanSequenceVector sliceScanSequencesIndexes = 
    philipsPAR->GetRECSliceIndexScanningSequence(HeaderFileName);
  PhilipsPAR::PARImageTypeScanSequenceVector imageTypesScanSequencesIndexes = 
    philipsPAR->GetImageTypesScanningSequence(HeaderFileName);
  PhilipsRECImageIOSetupSliceIndex(this->m_SliceIndex,1,par,
    imageTypesScanSequencesIndexes,sliceImageTypesIndexes,
    sliceScanSequencesIndexes);

  // As far as I know all Philips REC files are littleEndian.
  this->m_ByteOrder=LittleEndian;

  // Set dimensions.
  unsigned int numberOfDimensions = 4;
  // In reality PAR/REC files can have more dimensions
  // but it is very difficult to sort out all of the
  // possibilities.  The reader will sort the images
  // by block and the different types of images
  // stored in the blocks may be determined using the 
  // MetaDataDictionary.
  this->SetNumberOfDimensions(numberOfDimensions);
  
  // As far as I know, Philips REC files are only
  // 8-bit or 16-bit signed integers.
  switch( par.bit )
    {
    case 8:
      m_ComponentType = CHAR;
      m_PixelType = SCALAR;
      break;
    case 16:
      m_ComponentType = SHORT;
      m_PixelType = SCALAR;
      break;
    default:
      OStringStream message;
      message << "Unknown data type. par.bit must be 8 or 16. " 
              << "par.bit is "
              << par.bit;
      ExceptionObject exception(__FILE__, __LINE__,
                                message.str(),
                                ITK_LOCATION);
      throw exception;
    }
  //
  // set up the dimension stuff
  this->SetDimensions(0,par.dim[0]);
  this->SetDimensions(1,par.dim[1]);
  this->SetDimensions(2,par.slice);
  this->SetDimensions(3,par.image_blocks);
  this->SetSpacing(0,par.vox[0]);
  this->SetSpacing(1,par.vox[1]);
  this->SetSpacing(2,par.vox[2]);
  // Just 1 for the fourth dimension.
  this->SetSpacing(3,1.0f);

  //
  // figure out re-orientation required if not in Coronal
  this->ComputeStrides();


  //Get Dictionary Information
  //Important hk fields.
  MetaDataDictionary &thisDic=this->GetMetaDataDictionary();
  std::string classname(this->GetNameOfClass());
  EncapsulateMetaData<std::string>(thisDic,ITK_InputFilterName, classname);

  EncapsulateMetaData<std::string>(thisDic,ITK_ImageFileBaseName,
    GetRootName( this->m_FileName ));

  //Important dime fields
  EncapsulateMetaData<std::string>(thisDic,ITK_VoxelUnits,std::string("mm",4));
  EncapsulateMetaData<short int>(thisDic,ITK_OnDiskBitPerPixel,par.bit);
  EncapsulateMetaData<int>(thisDic,ITK_NumberOfDimensions,numberOfDimensions);

  switch( par.bit )
    {
    case 8:
      EncapsulateMetaData<std::string>(thisDic,ITK_OnDiskStorageTypeName,
        std::string(typeid(char).name()));
      break;
    case 16:
      EncapsulateMetaData<std::string>(thisDic,ITK_OnDiskStorageTypeName,
        std::string(typeid(short).name()));
      break;
    default:
      break;
    }

  //Important hist fields
  EncapsulateMetaData<std::string>(thisDic,ITK_FileNotes,
    std::string(par.series_type,32));

  SpatialOrientation::ValidCoordinateOrientationFlags coord_orient;

  switch (par.sliceorient)
    {
    case PAR_SLICE_ORIENTATION_TRANSVERSAL: 
      // Transverse - the REC data appears to be stored as right-left, 
      // anterior-posterior, and inferior-superior.
      // Verified using a marker on right side of brain.
      coord_orient = SpatialOrientation::ITK_COORDINATE_ORIENTATION_RAI;
      break;
    case PAR_SLICE_ORIENTATION_SAGITTAL: 
      // Sagittal - the REC data appears to be stored as anterior-posterior, 
      // superior-inferior, and right-left.
      // Verified using marker on right side of brain.
      coord_orient = SpatialOrientation::ITK_COORDINATE_ORIENTATION_ASL;
      break;
    case PAR_SLICE_ORIENTATION_CORONAL: 
      // Coronal - the REC data appears to be stored as right-left, 
      // superior-inferior, and anterior-posterior.
      // Verified using marker on right side of brain.
      // fall thru
    default:
      coord_orient = SpatialOrientation::ITK_COORDINATE_ORIENTATION_RSA;
    }

  typedef SpatialOrientationAdapter OrientAdapterType;
  SpatialOrientationAdapter::DirectionType dir =  
    OrientAdapterType().ToDirectionCosines(coord_orient);

  std::vector<double> dirx(numberOfDimensions,0),
    diry(numberOfDimensions,0),dirz(numberOfDimensions,0),
    dirBlock(numberOfDimensions,0);
  dirBlock[numberOfDimensions-1] = 1;
  dirx[0] = dir[0][0];
  dirx[1] = dir[1][0];
  dirx[2] = dir[2][0];
  diry[0] = dir[0][1];
  diry[1] = dir[1][1];
  diry[2] = dir[2][1];
  dirz[0] = dir[0][2];
  dirz[1] = dir[1][2];
  dirz[2] = dir[2][2];

  this->SetDirection(0,dirx);
  this->SetDirection(1,diry);
  this->SetDirection(2,dirz);
  this->SetDirection(3,dirBlock);

#if defined(ITKIO_DEPRECATED_METADATA_ORIENTATION)
  EncapsulateMetaData<SpatialOrientation::ValidCoordinateOrientationFlags>(
    thisDic,ITK_CoordinateOrientation, coord_orient);
#endif

  EncapsulateMetaData<std::string>(thisDic,ITK_PatientID,
    std::string(par.patient_name,32));
  EncapsulateMetaData<std::string>(thisDic,ITK_ExperimentDate,
    std::string(par.exam_date,32));
  EncapsulateMetaData<std::string>(thisDic,ITK_ExperimentTime,
    std::string(par.exam_time,32));
  
  // Encapsulate remaining PAR parameters
  EncapsulateMetaData<int>(thisDic,PAR_SliceOrientation,par.sliceorient);
  switch(par.ResToolsVersion)
    {
    case RESEARCH_IMAGE_EXPORT_TOOL_V3:
      EncapsulateMetaData<std::string>(thisDic,PAR_Version,std::string("V3",4));
      break;
    case RESEARCH_IMAGE_EXPORT_TOOL_V4:
      EncapsulateMetaData<std::string>(thisDic,PAR_Version,std::string("V4",4));
      break;
    case RESEARCH_IMAGE_EXPORT_TOOL_V4_1:
      EncapsulateMetaData<std::string>(thisDic,PAR_Version,
        std::string("V4.1",6));
      break;
    case RESEARCH_IMAGE_EXPORT_TOOL_V4_2:
      EncapsulateMetaData<std::string>(thisDic,PAR_Version,
        std::string("V4.2",6));
      break;
    }

  EncapsulateMetaData<std::string>(thisDic,PAR_ExaminationName,
    std::string(par.exam_name,32));
  EncapsulateMetaData<std::string>(thisDic,PAR_ProtocolName,
    std::string(par.protocol_name,32));
  EncapsulateMetaData<std::string>(thisDic,PAR_SeriesType,
    std::string(par.series_type,32));
  EncapsulateMetaData<int>(thisDic,PAR_AcquisitionNr,par.scno);
  EncapsulateMetaData<int>(thisDic,PAR_ReconstructionNr,par.recno);
  EncapsulateMetaData<int>(thisDic,PAR_ScanDuration,par.scan_duration);
  EncapsulateMetaData<int>(thisDic,PAR_MaxNumberOfCardiacPhases,
    par.cardiac_phases);
  TriggerTimesContainerType::Pointer triggerTimes = 
    TriggerTimesContainerType::New();
  triggerTimes->resize(par.cardiac_phases);

  for(unsigned int ttime_index=0; ttime_index<(unsigned int)par.cardiac_phases; 
    ttime_index++)
    {
    triggerTimes->SetElement(ttime_index,(double)par.trigger_times[ttime_index]);
    }

  EncapsulateMetaData<TriggerTimesContainerType::Pointer>(thisDic,
    PAR_TriggerTimes,triggerTimes);
  EncapsulateMetaData<int>(thisDic,PAR_MaxNumberOfEchoes,par.echoes);
  EchoTimesContainerType::Pointer echoTimes = EchoTimesContainerType::New();
  echoTimes->resize(par.echoes);

  for(unsigned int echo_index=0; echo_index<(unsigned int)par.echoes; 
    echo_index++)
    {
    echoTimes->SetElement(echo_index,(double)par.echo_times[echo_index]);
    }

  EncapsulateMetaData<EchoTimesContainerType::Pointer>(thisDic,PAR_EchoTimes,
    echoTimes);
  EncapsulateMetaData<int>(thisDic,PAR_MaxNumberOfDynamics,par.dyn);
  EncapsulateMetaData<int>(thisDic,PAR_MaxNumberOfMixes,par.mixes);
  EncapsulateMetaData<std::string>(thisDic,PAR_PatientPosition,
    std::string(par.patient_position,32));
  EncapsulateMetaData<std::string>(thisDic,PAR_PreparationDirection,
    std::string(par.prep_direction,32));
  EncapsulateMetaData<std::string>(thisDic,PAR_Technique,
    std::string(par.technique,32));
  EncapsulateMetaData<std::string>(thisDic,PAR_ScanMode,
    std::string(par.scan_mode,32));
  EncapsulateMetaData<int>(thisDic,PAR_NumberOfAverages,par.num_averages);
  EncapsulateMetaData<ScanResolutionType>(thisDic,PAR_ScanResolution,
    ScanResolutionType(par.scan_resolution));
  RepetitionTimesContainerType::Pointer repTimes = 
   RepetitionTimesContainerType::New();
  repTimes->resize(par.mixes); // This has only been verified using a 
                               // Look-Locker sequence and may not be valid. 

  for(unsigned int rep_index=0; rep_index<(unsigned int)par.mixes; rep_index++)
    {
    repTimes->SetElement(rep_index,(double)par.repetition_time[rep_index]);
    }

  EncapsulateMetaData<RepetitionTimesContainerType::Pointer>(thisDic,
   PAR_RepetitionTimes,repTimes);
  EncapsulateMetaData<int>(thisDic,PAR_ScanPercentage,par.scan_percent);
  EncapsulateMetaData<FOVType>(thisDic,PAR_FOV,FOVType(par.fov));
  EncapsulateMetaData<float>(thisDic,PAR_WaterFatShiftPixels,
    par.water_fat_shift);
  AngulationMidSliceType tempAngulation;
  tempAngulation[0] = (double)par.angAP;
  tempAngulation[1] = (double)par.angFH;
  tempAngulation[2] = (double)par.angRL;
  EncapsulateMetaData<AngulationMidSliceType>(thisDic,PAR_AngulationMidSlice,
    tempAngulation);
  OffCentreMidSliceType tempOffcentre;
  tempOffcentre[0] = (double)par.offAP;
  tempOffcentre[1] = (double)par.offFH;
  tempOffcentre[2] = (double)par.offRL;
  EncapsulateMetaData<OffCentreMidSliceType>(thisDic,PAR_OffCentreMidSlice,
    tempOffcentre);
  EncapsulateMetaData<int>(thisDic,PAR_FlowCompensation,par.flow_comp);
  EncapsulateMetaData<int>(thisDic,PAR_Presaturation,par.presaturation);
  EncapsulateMetaData<int>(thisDic,PAR_CardiacFrequency,par.cardiac_freq);
  EncapsulateMetaData<int>(thisDic,PAR_MinRRInterval,par.min_rr_int);
  EncapsulateMetaData<int>(thisDic,PAR_MaxRRInterval,par.max_rr_int);
  EncapsulateMetaData<PhaseEncodingVelocityType>(thisDic,
    PAR_PhaseEncodingVelocity, PhaseEncodingVelocityType(par.phase_encode_vel));
  EncapsulateMetaData<int>(thisDic,PAR_MTC,par.mtc);
  EncapsulateMetaData<int>(thisDic,PAR_SPIR,par.spir);
  EncapsulateMetaData<int>(thisDic,PAR_EPIFactor,par.epi);
  EncapsulateMetaData<int>(thisDic,PAR_TurboFactor,par.turbo);
  EncapsulateMetaData<int>(thisDic,PAR_DynamicScan,par.dynamic_scan);
  EncapsulateMetaData<int>(thisDic,PAR_Diffusion,par.diffusion);
  EncapsulateMetaData<float>(thisDic,PAR_DiffusionEchoTime,par.diff_echo);
  EncapsulateMetaData<int>(thisDic,PAR_MaxNumberOfDiffusionValues,
    par.max_num_diff_vals);
  EncapsulateMetaData<GradientBvalueContainerType::Pointer>(thisDic,
    PAR_GradientBValues, diffusionBvalueVector);
  EncapsulateMetaData<int>(thisDic,PAR_MaxNumberOfGradientOrients,
    par.max_num_grad_orient);
  EncapsulateMetaData<GradientDirectionContainerType::Pointer>(thisDic,
    PAR_GradientDirectionValues, diffusionGradientOrientationVector);
  EncapsulateMetaData<float>(thisDic,PAR_InversionDelay,par.inversion_delay);
  EncapsulateMetaData<int>(thisDic,PAR_NumberOfImageTypes,par.num_image_types);
  EncapsulateMetaData<ImageTypesType>(thisDic,PAR_ImageTypes,
    ImageTypesType(par.image_types));
  EncapsulateMetaData<int>(thisDic,PAR_NumberOfScanningSequences,
    par.num_scanning_sequences);
  EncapsulateMetaData<ScanningSequencesType>(thisDic,PAR_ScanningSequences,
    ScanningSequencesType(par.scanning_sequences));
  typedef ScanningSequenceImageTypeRescaleValuesContainerType::Pointer
    ScanningSequenceImageTypeRescaleValuesContainerTypePtr;
  EncapsulateMetaData<ScanningSequenceImageTypeRescaleValuesContainerTypePtr>(
    thisDic,PAR_ScanningSequenceImageTypeRescaleValues,
    scanningSequenceImageTypeRescaleVector);
  EncapsulateMetaData<int>(thisDic,PAR_NumberOfASLLabelTypes,
    par.num_label_types);
  EncapsulateMetaData<LabelTypesASLContainerType::Pointer>(thisDic,
    PAR_ASLLabelTypes, labelTypesASLVector);
  
  return;
}  

} // end namespace itk
