/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkStimulateImageIO.cxx,v $
  Language:  C++
  Date:      $Date: 2007/08/27 13:46:29 $
  Version:   $Revision: 1.20 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

  Portions of this code are covered under the VTK copyright.
  See VTKCopyright.txt or http://www.kitware.com/VTKCopyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkStimulateImageIO.h"
#include <stdio.h>
#include <string.h>
#include <itksys/SystemTools.hxx>
#include "itkByteSwapper.h"

namespace itk
{

StimulateImageIO::StimulateImageIO()
{
  this->SetNumberOfDimensions(4);
  m_ByteOrder = BigEndian;
  m_FileType = Binary;
  m_DisplayRange[0] = 0;
  m_DisplayRange[1] = 0;
}

StimulateImageIO::~StimulateImageIO()
{
}

bool StimulateImageIO::OpenStimulateFileForReading(std::ifstream& os, 
                                                   const char* filename)
                                       
{
  // Make sure that we have a file to 
  if ( *filename == 0 )
    {
    itkExceptionMacro(<<"A FileName must be specified.");
    return false;
    }

  // Close file from any previous image
  if ( os.is_open() )
    {
    os.close();
    }
  
  // Open the new file for reading
  itkDebugMacro(<< "Initialize: opening file " << filename);

  // Actually open the file
  os.open( filename, std::ios::in | std::ios::binary );

  if ( os.fail() )
    {
    return false;
    }

  return true;
}


bool StimulateImageIO::OpenStimulateFileForWriting(std::ofstream& os, 
                                                   const char* filename)
                                       
{
  // Make sure that we have a file to 
  if ( *filename == 0 )
    {
    itkExceptionMacro(<<"A FileName must be specified.");
    return false;
    }

  // Close file from any previous image
  if ( os.is_open() )
    {
    os.close();
    }
  
  // Open the new file for writing
  itkDebugMacro(<< "Initialize: opening file " << filename);

#ifdef __sgi
  // Create the file. This is required on some older sgi's
  std::ofstream tFile(filename,std::ios::out);
  tFile.close();
#endif

  // Actually open the file
  os.open( filename, std::ios::out | std::ios::binary );

  if( os.fail() )
    {
    itkExceptionMacro(<< "Could not open file for writing: " << filename);
    return false;
    }

  return true;
}

// This method will only test if the header looks like a
//Stimulate image file.
bool StimulateImageIO::CanReadFile(const char* filename) 
{ 
  std::ifstream file;
  char buffer[256];
  std::string fname(filename);

  if( fname == "" )
    {
    itkDebugMacro(<<"No filename specified.");
    return false;
    }

  bool extensionFound = false;
  std::string::size_type sprPos = fname.rfind(".spr");
  if ((sprPos != std::string::npos)
      && (sprPos == fname.length() - 4))
    {
    extensionFound = true;
    }

  if( !extensionFound )
    {
    itkDebugMacro(<<"The filename extension is not recognized");
    return false;
    }

  if ( ! this->OpenStimulateFileForReading(file, filename))
    {
    return false;
    }

  // Check to see if its a spr file
  file.getline(buffer,255);

  fname = buffer;

  if ( fname.find("numDim:") < fname.length() ||
       fname.find("dim:") < fname.length() ||
       fname.find("dataType:") < fname.length() )
    {
    return true;
    }
  else
    {
    return false;
    }
}
  

void StimulateImageIO::Read(void* buffer)
{
  std::ifstream file;

  //read header information file:
  this->InternalReadImageInformation(file);
  
  //read data file
  std::ifstream file_data;

  if( m_DataFileName == "" )
    {
    //if no data filename was specified try to guess one:   
    //based on filename.spr , options are:
    //1: filename.spr.sdt
    //2: filename.sdt

    //option 1:
    m_DataFileName = m_FileName;
    m_DataFileName.replace(m_DataFileName.length(), 4, ".sdt" );
 
    // Actually open the file
    file_data.open( m_DataFileName.c_str(), std::ios::in | std::ios::binary );
    if ( file_data.fail() )
      {
      //option 2:
      m_DataFileName = m_FileName;
      m_DataFileName.replace(m_DataFileName.length() - 3, 3, "sdt" );
      file_data.open( m_DataFileName.c_str(), std::ios::in | std::ios::binary );
      if ( file_data.fail() )
        {
        itkExceptionMacro(<<"No Data file was specified in header (spr) file and guessing file data name failed." );
        return;
        }
      }
    }//a filename was found for data file

  if( !OpenStimulateFileForReading( file_data, m_DataFileName.c_str()) )
    {
    return;
    }

  if( !this->ReadBufferAsBinary( file_data, buffer, this->GetImageSizeInBytes()) )
    {
    itkExceptionMacro(<<"Read failed: Wanted " 
                      << this->GetImageSizeInBytes()
                      << " bytes, but read " 
                      << file_data.gcount() << " bytes.");
    }

  //byte swapping depending on pixel type:
  switch (this->GetComponentType())
    {
    case CHAR:
      ByteSwapper<char>::SwapRangeFromSystemToBigEndian((char *)buffer, this->GetImageSizeInComponents() );
      break;
    case SHORT:
      ByteSwapper<short>::SwapRangeFromSystemToBigEndian((short *)buffer, this->GetImageSizeInComponents() );
      break;
    case INT:
      ByteSwapper<int>::SwapRangeFromSystemToBigEndian((int *)buffer, this->GetImageSizeInComponents() );
      break;
    case FLOAT:
      ByteSwapper<float>::SwapRangeFromSystemToBigEndian((float *)buffer, this->GetImageSizeInComponents() );
      break;
    case DOUBLE:
      ByteSwapper<double>::SwapRangeFromSystemToBigEndian((double *)buffer, this->GetImageSizeInComponents() );
      break;
    default:
      break;
    }

  //closing files:
  file.close();
  file_data.close();
}

void StimulateImageIO::InternalReadImageInformation(std::ifstream& file)
{
  char line[255];
  std::string text;

  //read .sdt file (header)
  if ( ! this->OpenStimulateFileForReading(file, m_FileName.c_str()) )
    {
    itkExceptionMacro(<< "Cannot read requested file");
    }

  //extract dimensions, spacing, origin
  unsigned int dim;
  unsigned int dims[4];
  float spacing[4];
  float origin[4];
  float fov[4];
  
  // set values in case we don't find them
  this->SetNumberOfDimensions(4);
  m_Spacing[0] = 1.0;
  m_Spacing[1] = 1.0;
  m_Spacing[2] = 1.0;
  m_Spacing[3] = 1.0;

  m_Origin[0] = 0.0;
  m_Origin[1] = 0.0;
  m_Origin[2] = 0.0;
  m_Origin[3] = 0.0;

  char pixelType[256];
  float range[2];
  
  //char fidName[256] = "";   
  //char orient[256] = "";
  char datafilename[256] = "";

  bool fov_specified = false;
  bool origin_specified = false;
  bool spacing_specified = false;
  while((file.getline(line, 255), file.gcount() > 0))
    {
    text = line;
    
    if ( text.find("numDim") < text.length())
      {
      sscanf(line, "%*s %d", &dim);
      this->SetNumberOfDimensions(dim);
      }
    else if ( text.find("dim") < text.length())
      {
      sscanf(line, "%*s %d %d %d %d", dims, dims+1, dims+2 , dims+3);
      if ( m_NumberOfDimensions > 3 && dims[3] <= 1 )
        {
        this->SetNumberOfDimensions(3);
        }
      if ( m_NumberOfDimensions > 2 && dims[2] <= 1 )
        {
        this->SetNumberOfDimensions(2);
        }
      for ( unsigned int i=0; i < m_NumberOfDimensions; i++ )
        {
        m_Dimensions[i] = dims[i];
        }
      }
    else if ( text.find("origin") < text.length())
      {
      //origin
      //Position of the center of the first voxel. One value for each dimension. If
      //the origin is not specified, but the fov is, then the image is assumed
      //to be centered:

      sscanf(line, "%*s %f %f %f %f", origin, origin+1, origin+2 , origin+3);
      for ( unsigned int i=0; i < m_NumberOfDimensions; i++ )
        {
        m_Origin[i] = origin[i];
        }
      origin_specified = true;
      }
    else if ( text.find("extent") < text.length())
      {
      //not documented
      itkDebugMacro(<<"Extent was specified");
      }
    else if ( text.find("fov") < text.length())
      {
      //fov
      //Field of view: The distance between the outside edges of the first and last
      //voxel along each dimension; one value for each dimension. If the fov is not
      //specified it is calculated according to: fov = interval * dim
      
      sscanf(line, "%*s %f %f %f %f", fov, fov+1, fov+2 , fov+3);
      fov_specified = true;
      }
    else if ( text.find("interval") < text.length())
      {
      //interval
      //The center to center distance between adjacent voxels along each dimension;
      //one value for each dimension. If the interval is not specified it is
      //calculated according to: interval = fov / dim

      sscanf(line, "%*s %f %f %f %f", spacing, spacing+1, spacing+2 , spacing+3);
      for ( unsigned int i=0; i < m_NumberOfDimensions; i++ )
        {
        m_Spacing[i] = spacing[i];
        }
      spacing_specified = true;
      }
    else if ( text.find("dataType") < text.length())
      {
      sscanf(line, "%*s %s", pixelType);
      text = pixelType;
      SetPixelType(SCALAR);
      if ( text.find("BYTE") < text.length() )
        {
        SetComponentType(CHAR);
        }
      else if ( text.find("WORD") < text.length() )
        {
        SetComponentType(SHORT);
        }
      else if ( text.find("LWORD") < text.length() )
        {
        SetComponentType(INT);
        }
      else if ( text.find("REAL") < text.length() )
        {
        SetComponentType(FLOAT);
        }
      else if ( text.find("COMPLEX") < text.length() )
        {
        SetPixelType(VECTOR);
        SetComponentType(DOUBLE);
        }
      else
        {
        itkExceptionMacro(<<"Unrecognized type");
        }
      }//found scalars
    else if ( text.find("displayRange") < text.length())
      {
      //displayRange:
      //Two values giving the low_value and high_value. Voxel values below the
      //low_value will be displayed as black and voxels with values above the
      //high_value will be displayed as white. Voxels with values within the display
      //range are displayed with a grey value that is scaled linearly between the
      //low_value and high_value.
      
      sscanf(line, "%*s %f %f", range, range+1);
      m_DisplayRange[0] = range[0];
      m_DisplayRange[1] = range[1];
      }
    else if ( text.find("fidName") < text.length())
      {
      //Not well documented
      //This is a bit tricky to get the value as there is sometime no white space
      //only a ':' separate field from value, we assume there is no other ':'
      char *pch;
      pch = strchr(line,':');
      sscanf(++pch, "%s", m_FidName);  //delete any white space left
      itkDebugMacro(<<"fidName was specified");
      }
    else if ( text.find("sdtOrient") < text.length())
      {
      //Not used now, but later when  ITK Dictionary will be ready
      //don't know for now the format in which to save this.
      //This is a bit tricky to get the value as there is sometime no white space
      //only a ':' separate field from value, we assume there is no other ':'
      char *pch;
      pch = strchr(line,':');
      sscanf(++pch, "%s", m_SdtOrient);  //delete any white space left
      itkDebugMacro(<<"Orientation was specified");
      }
    else if ( text.find("dsplyThres") < text.length())
      {
      //not documented
      itkDebugMacro(<<"Display threshold was specified");
      }
    else if ( text.find("endian") < text.length())
      {
      //BigEndian ieee-be / LittleEndian: ieee-le
      if ( text.find("ieee-le") < text.length())
        {
        itkExceptionMacro(<<"Little Endian Stimulate files are not handled.");
        }
      }
    else if ( text.find("mapParmFileName") < text.length())
      {
      //not documented
      itkDebugMacro(<<"mapParmFileName was specified");
      }
    else if ( text.find("mapTypeName") < text.length())
      {
      //not documented
      itkDebugMacro(<<"mapTypeName was specified");
      }
    else if ( text.find("stimFileName") < text.length())
      {
      //file data name is explicitely specified
      sscanf(line, "%*s %s", datafilename);
      //if the data filename has a directory specified, use it as is,
      //otherwise prepend the path of the .spr file.
      std::string datafilenamePath =
        ::itksys::SystemTools::GetFilenamePath (datafilename);
      if (datafilenamePath == "")
        {
        std::string fileNamePath = ::itksys::SystemTools::GetFilenamePath (m_FileName.c_str());
        m_DataFileName = fileNamePath + "/" + datafilename;
        }
      else
        {
        m_DataFileName = datafilename;
        }
      }
    else if ( text.find("mapConf") < text.length())
      {
      //not documented
      itkDebugMacro(<<"mapConf was specified");
      }
    else if ( text.find("periodStr") < text.length())
      {
      //not documented
      itkDebugMacro(<<"periodStr was specified");
      }
    }


  //compute any missing informations:
  if( !spacing_specified && fov_specified)
    {
    for ( unsigned int i=0; i < m_NumberOfDimensions; i++ )
      {
      m_Spacing[i] = fov[i]/dims[i];
      }
    }
  if( !origin_specified && fov_specified)
    {
    for ( unsigned int i=0; i < m_NumberOfDimensions; i++ )
      {
      m_Origin[i] = (m_Spacing[i] - fov[i])/2.;
      }
    }//otherwise default spacing & origin are used.
}

void StimulateImageIO::ReadImageInformation()
{
  std::ifstream file;
  this->InternalReadImageInformation(file);
}


bool StimulateImageIO::CanWriteFile( const char* name )
{
  std::string filename = name;

  if( filename == "" )
    {
    itkDebugMacro(<<"No filename specified.");
    return false;
    }

  bool extensionFound = false;
  std::string::size_type sprPos = filename.rfind(".spr");
  if ((sprPos != std::string::npos)
      && (sprPos == filename.length() - 4))
    {
    extensionFound = true;
    }

  if( !extensionFound )
    {
    itkDebugMacro(<<"The filename extension is not recognized");
    return false;
    }

  return true;
}

void StimulateImageIO::Write(const void* buffer)
{
  unsigned int i;

  std::ofstream file;
  if ( ! this->OpenStimulateFileForWriting(file, m_FileName.c_str()) )
    {
    return;
    }

  // Check the image region for proper dimensions, etc.
  unsigned int numDims = this->GetNumberOfDimensions();
  if ( numDims < 2 || numDims > 4 )
    {
    itkExceptionMacro(<<"Stimulate Writer can only write 2,3 or 4-dimensional images");
    return;
    }

  // Write the Stimulate header information
  file << "numDim: " <<  this->GetNumberOfDimensions();

  // Write characteristics of the data
  file << "\ndim:";
  for(i=0; i < m_NumberOfDimensions; i++)
    {
    file << " " << m_Dimensions[i];
    }

  file << "\norigin:";
  for(i=0; i < m_NumberOfDimensions; i++)
    {
    file << " " << m_Origin[i];
    }

  file << "\nfov:";
  for(i=0; i < m_NumberOfDimensions; i++)
    {
    file << " " << m_Spacing[i]*m_Dimensions[i]; //fov = interval * dim
    }

  file << "\ninterval:";
  for(i=0; i < m_NumberOfDimensions; i++)
    {
    file << " " << m_Spacing[i];
    }

  //preparation for writing buffer:
  const unsigned long numberOfBytes      = this->GetImageSizeInBytes();
  const unsigned long numberOfComponents = this->GetImageSizeInComponents();

  file << "\ndataType: ";
  {
    char * tempmemory=new char[numberOfBytes];
    memcpy(tempmemory,buffer,numberOfBytes);
    switch(this->GetComponentType())
      {
      case CHAR:
        file << "BYTE";
        ByteSwapper<char>::SwapRangeFromSystemToBigEndian(reinterpret_cast<char *>(tempmemory), numberOfComponents );
        break;
      case SHORT:
        file << "WORD";
        ByteSwapper<short int>::SwapRangeFromSystemToBigEndian(reinterpret_cast<short int *>(tempmemory), numberOfComponents );
        break;
      case INT:
        file << "LWORD";
        ByteSwapper<int>::SwapRangeFromSystemToBigEndian(reinterpret_cast<int *>(tempmemory), numberOfComponents );
        break;
      case FLOAT:
        file << "REAL";
        ByteSwapper<float>::SwapRangeFromSystemToBigEndian(reinterpret_cast<float *>(tempmemory), numberOfComponents );
        break;
      case DOUBLE:
        file << "COMPLEX";
        ByteSwapper<double>::SwapRangeFromSystemToBigEndian(reinterpret_cast<double *>(tempmemory), numberOfComponents );
        break;
      default:
        break;
      }
  
    //add the data filename to the header
    //determine datafile given the spr filename
    m_DataFileName = m_FileName;
    m_DataFileName.replace(m_DataFileName.length() - 3, 3, "sdt" );
    file << "\nstimFileName: " << m_DataFileName.c_str();

    //Last carrier return:
    file << "\n";

    //actually read data file
    std::ofstream file_data;

    if ( ! this->OpenStimulateFileForWriting(file_data, m_DataFileName.c_str()) )
      {
      return;
      }

    // Write the actual pixel data
    file_data.write( static_cast<const char *>(tempmemory) , numberOfBytes );
    delete [] tempmemory;
    file_data.close();
    }
  file.close();
}

void StimulateImageIO::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "\nDisplayRange: " << m_DisplayRange[0] << " " << m_DisplayRange[1];
}

} // end namespace itk
