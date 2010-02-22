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
#include "otbJPEG2000ImageIO.h"

#include "itkExceptionObject.h"
#include "otbMacro.h"

#include "otbSystem.h"

/**
   sample error debug callback expecting no client object
*/
void error_callback(const char *msg, void *client_data)
{
  (void)client_data;
  itkGenericExceptionMacro(<<"OpenJpeg error: "<<msg);
}
/**
   sample warning debug callback expecting no client object
*/
void warning_callback(const char *msg, void *client_data)
{
  (void)client_data;
  otbGenericMsgDebugMacro(<<"OpenJpeg warning: "<<msg);
}
/**
   sample debug callback expecting no client object
*/
void info_callback(const char *msg, void *client_data)
{
  (void)client_data;
  otbMsgDevMacro(<<"OpenJpeg info: "<<msg);
}


namespace otb
{

JPEG2000ImageIO::JPEG2000ImageIO()
{
  // By default set number of dimensions to two.
  this->SetNumberOfDimensions(2);
  m_PixelType = SCALAR;
  m_ComponentType = UCHAR;

  // Set default spacing to one
  m_Spacing[0] = 1.0;
  m_Spacing[1] = 1.0;
  // Set default origin to zero
  m_Origin[0] = 0.0;
  m_Origin[1] = 0.0;

  m_NbOctetPixel = 1;
}

JPEG2000ImageIO::~JPEG2000ImageIO()
{}

bool JPEG2000ImageIO::CanReadFile( const char* filename )
{
  if (filename == NULL)
  {
    return false;
  }

  std::string lFileName(filename);

  // Creating a codec
  opj_codec_t * codec = NULL;

  if (System::SetToLower(System::GetExtension(lFileName)) == "j2k")
  {
    otbMsgDebugMacro(<<"Jpeg2000ImageIO: Creating J2K codec.");
    codec = otb_openjpeg_opj_create_decompress(CODEC_J2K);
  }
  else if (System::SetToLower(System::GetExtension(lFileName)) == "jp2"
           || System::SetToLower(System::GetExtension(lFileName)) == "jpx")
  {
    otbMsgDebugMacro(<<"Jpeg2000ImageIO: Creating JP2 codec.");
    codec = otb_openjpeg_opj_create_decompress(CODEC_JP2);
  }
  else
  {
    otbMsgDebugMacro(<<"Jpeg2000ImageIO: Extension not recognized.");
    return false;
  }

  if (!codec)
  {
    otbMsgDebugMacro(<<"Impossible to create codec.");
    return false;
  }
  otb_openjpeg_opj_set_info_handler(codec, info_callback,00);
  otb_openjpeg_opj_set_warning_handler(codec, warning_callback,00);
  otb_openjpeg_opj_set_error_handler(codec, error_callback,00);

  // Setting default parameters
  opj_dparameters_t parameters;
  otb_openjpeg_opj_set_default_decoder_parameters(&parameters);

  if (! otb_openjpeg_opj_setup_decoder(codec,&parameters))
  {
    otbMsgDebugMacro(<<"Impossible to set parameter.");
    otb_openjpeg_opj_destroy_codec(codec);
    return false;
  }

  // Creating a file
  FILE * file  = fopen(filename,"rb");
  if (!file)
  {
    otbMsgDebugMacro(<<"Impossible to open file.");
    otb_openjpeg_opj_destroy_codec(codec);
    return false;
  }
  // Creating a file stream
  opj_stream_t * stream  = otb_openjpeg_opj_stream_create_default_file_stream(file,true);
  if (!stream)
  {
    otbMsgDebugMacro(<<"Impossible to create stream.");
    otb_openjpeg_opj_destroy_codec(codec);
    fclose(file);
    return false;
  }


  // trying to read image header
  opj_image_t * image;
  OPJ_INT32 tile_x0,tile_y0;
  OPJ_UINT32 tile_width,tile_height,nb_tiles_x,nb_tiles_y;
  bool resp = otb_openjpeg_opj_read_header(codec,
              &image,
              &tile_x0,
              &tile_y0,
              &tile_width,
              &tile_height,
              &nb_tiles_x,
              &nb_tiles_y,
              stream);
  // Destroy the codec
  otb_openjpeg_opj_stream_destroy(stream);
  fclose(file);
  otb_openjpeg_opj_destroy_codec(codec);
  otb_openjpeg_opj_image_destroy(image);
  return resp;
}


// Used to print information about this object
void JPEG2000ImageIO::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

// Read a 3D image (or event more bands)... not implemented yet
void JPEG2000ImageIO::ReadVolume(void*)
{
}

// Read image
void JPEG2000ImageIO::Read(void* buffer)
{
  //  unsigned long step = this->GetNumberOfComponents();
  char * charstarbuffer = static_cast<char *>(buffer);

  int buffer_size_y = this->GetIORegion().GetSize()[1];
  int buffer_size_x = this->GetIORegion().GetSize()[0];
  int buffer_y0     = this->GetIORegion().GetIndex()[1];
  int buffer_x0     = this->GetIORegion().GetIndex()[0];

  otbMsgDevMacro( <<" JPEG2000ImageIO::Read()  ");
  otbMsgDevMacro( <<" ImageDimension   : "<<m_Dimensions[0]<<","<<m_Dimensions[1]);
  otbMsgDevMacro( <<" IORegion         : "<<this->GetIORegion());
  otbMsgDevMacro( <<" Nb Of Components : "<<this->GetNumberOfComponents());

  otbMsgDevMacro(<<"IORegion: "<<this->GetIORegion());
  otbMsgDevMacro(<<"Area to read: "<<buffer_x0<<" "<<buffer_y0  <<" "<< buffer_x0+buffer_size_x-1 <<" "<<buffer_y0+buffer_size_y-1 );
  otbMsgDevMacro(<<"Component type: "<<this->GetComponentTypeAsString(this->GetComponentType()));

  // Creating openjpeg objects
  if (System::SetToLower(System::GetExtension(m_FileName)) == "j2k")
  {
    m_Codec = otb_openjpeg_opj_create_decompress(CODEC_J2K);
  }
  else if (System::SetToLower(System::GetExtension(m_FileName)) == "jp2"
           || System::SetToLower(System::GetExtension(m_FileName)) == "jpx")
  {
    m_Codec = otb_openjpeg_opj_create_decompress(CODEC_JP2);
  }

  otb_openjpeg_opj_set_info_handler(m_Codec, info_callback,00);
  otb_openjpeg_opj_set_warning_handler(m_Codec, warning_callback,00);
  otb_openjpeg_opj_set_error_handler(m_Codec, error_callback,00);

  if (!m_Codec)
  {
    itkExceptionMacro(<<"Failed to create openjpeg codec.");
  }

  // Create default parameters
  otb_openjpeg_opj_set_default_decoder_parameters(&m_Parameters);
  // Set the requested region
  otb_openjpeg_opj_restrict_decoding(&m_Parameters,buffer_x0,buffer_y0,buffer_x0+buffer_size_x-1,buffer_y0+buffer_size_y-1);

  if (!otb_openjpeg_opj_setup_decoder(m_Codec,&m_Parameters))
  {
    itkExceptionMacro(<<"Failed to set up decoder parameters.");
  }

  m_File = fopen(m_FileName.c_str(),"rb");

  if (!m_File)
  {
    itkExceptionMacro(<<"Failed to open file: "<<m_FileName);
  }

  m_OpenJpegStream = otb_openjpeg_opj_stream_create_default_file_stream(m_File,true);

  if (!m_OpenJpegStream)
  {
    itkExceptionMacro(<<"Failed to create file stream.");
  }

  OPJ_INT32 tile_x0,tile_y0;
  OPJ_UINT32 tile_width,tile_height,nb_tiles_x,nb_tiles_y;

  if (!otb_openjpeg_opj_read_header(m_Codec,
                                    &m_OpenJpegImage,
                                    &tile_x0,
                                    &tile_y0,
                                    &tile_width,
                                    &tile_height,
                                    &nb_tiles_x,
                                    &nb_tiles_y,
                                    m_OpenJpegStream))
  {
    itkExceptionMacro(<<"Failed to read image header.");
  }

  OPJ_BYTE * tile_data;
  OPJ_INT32  tile_x1,tile_y1;
  OPJ_UINT32 data_size,nb_comps;
  OPJ_UINT32 tile_index;
  OPJ_BOOL   goesOn = true;

  while (goesOn)
  {
    if (! otb_openjpeg_opj_read_tile_header(m_Codec,
                                            &tile_index,
                                            &data_size,
                                            &tile_x0,
                                            &tile_y0,
                                            &tile_x1,
                                            &tile_y1,
                                            &nb_comps,
                                            &goesOn,
                                            m_OpenJpegStream))
    {
      itkExceptionMacro(<<"Error while reading tile header.");
    }
    if (goesOn)
    {
      otbMsgDebugMacro(<<"==========================");
      otbMsgDebugMacro(<<"Tile index: "<<tile_index);
      otbMsgDebugMacro(<<"Data size: "<<data_size);
      otbMsgDebugMacro(<<"Tile (x0,y0): "<<tile_x0<<" "<<tile_y0);
      otbMsgDebugMacro(<<"Tile (x1,y1): "<<tile_x1<<" "<<tile_y1);
      otbMsgDebugMacro(<<"Tile number of component: "<<nb_comps);
      otbMsgDebugMacro(<<"Goes on: "<<goesOn);
      otbMsgDevMacro(<<"--------------------------");


      tile_data = new OPJ_BYTE[data_size];

      if (! otb_openjpeg_opj_decode_tile_data(m_Codec,tile_index,tile_data,data_size,m_OpenJpegStream))
      {
        itkExceptionMacro(<<"Error while reading tile data.");
      }

      std::streamsize tile_component_size = data_size/nb_comps;
      std::streamoff  buffer_skip         = std::max(0,tile_y0-buffer_y0)*buffer_size_x*nb_comps*m_NbOctetPixel;
      std::streamoff tile_skip            = std::max(0,buffer_y0-tile_y0)*(tile_x1-tile_x0)*m_NbOctetPixel;
      std::streamoff tile_offset_begin    = std::max(0,buffer_x0-tile_x0)*m_NbOctetPixel;
      std::streamoff buffer_offset_begin  = std::max(0,tile_x0-buffer_x0)*nb_comps*m_NbOctetPixel;
      std::streamsize line_size           = (std::min(tile_x1,buffer_x0+buffer_size_x-1)-std::max(tile_x0,buffer_x0)+1);
      std::streamsize nb_lines            = std::min(tile_y1,buffer_y0+buffer_size_y-1)- std::max(tile_y0,buffer_y0)+1;
      std::streamsize buffer_line_size    = buffer_size_x*nb_comps*m_NbOctetPixel;
      std::streamsize tile_line_size      = (tile_x1-tile_x0)*m_NbOctetPixel;
      std::streampos buffer_step          = nb_comps * m_NbOctetPixel;

      otbMsgDevMacro(<<"buffer_skip: "<<buffer_skip);
      otbMsgDevMacro(<<"tile_skip: "<<tile_skip);
      otbMsgDevMacro(<<"buffer_offset_begin: "<<buffer_offset_begin);
      otbMsgDevMacro(<<"tile_offset_begin: "<<tile_offset_begin);
      otbMsgDevMacro(<<"buffer_line_size: "<<buffer_line_size);
      otbMsgDevMacro(<<"tile_line_size: "<<tile_line_size);
      otbMsgDevMacro(<<"line_size: "<<line_size);
      otbMsgDevMacro(<<"nb_lines: "<<nb_lines);
      otbMsgDevMacro(<<"buffer_step: "<<buffer_step);


      std::streamoff buffer_pos,tile_pos;

      for (unsigned int comp = 0;comp<nb_comps;++comp)
      {
        for (int line = 0; line<nb_lines;++line)
        {
          buffer_pos = buffer_skip + comp*m_NbOctetPixel + line*buffer_line_size + buffer_offset_begin;
          tile_pos   = comp *tile_component_size + tile_skip + line*tile_line_size + tile_offset_begin;

          for (int cols = 0;cols<line_size;++cols)
          {
            for (unsigned int octet = 0;octet<m_NbOctetPixel;++octet)
            {
              charstarbuffer[buffer_pos + cols*buffer_step + octet]= tile_data[tile_pos + cols*m_NbOctetPixel + octet];
            }
          }
        }
      }
      delete[] tile_data;
    }
  }
  otbMsgDebugMacro(<<"==========================");


  otb_openjpeg_opj_stream_destroy(m_OpenJpegStream);
  fclose(m_File);
  otb_openjpeg_opj_destroy_codec(m_Codec);
  otb_openjpeg_opj_image_destroy(m_OpenJpegImage);
}


void JPEG2000ImageIO::ReadImageInformation()
{
  if (  m_FileName.empty() == true )
  {
    itkExceptionMacro(<<"JPEG2000ImageIO: empty image filename.");
  }

  // Creating openjpeg objects
  if (System::SetToLower(System::GetExtension(m_FileName)) == "j2k")
  {
    m_Codec = otb_openjpeg_opj_create_decompress(CODEC_J2K);
  }
  else if (System::SetToLower(System::GetExtension(m_FileName)) == "jp2"
           || System::SetToLower(System::GetExtension(m_FileName)) == "jpx")
  {
    m_Codec = otb_openjpeg_opj_create_decompress(CODEC_JP2);
  }

  otb_openjpeg_opj_set_info_handler(m_Codec, info_callback,00);
  otb_openjpeg_opj_set_warning_handler(m_Codec, warning_callback,00);
  otb_openjpeg_opj_set_error_handler(m_Codec, error_callback,00);

  if (!m_Codec)
  {
    itkExceptionMacro(<<"Failed to create openjpeg codec.");
  }

  // Create default parameters
  otb_openjpeg_opj_set_default_decoder_parameters(&m_Parameters);

  if (!otb_openjpeg_opj_setup_decoder(m_Codec,&m_Parameters))
  {
    itkExceptionMacro(<<"Failed to set up decoder parameters.");
  }

  m_File = fopen(m_FileName.c_str(),"rb");

  if (!m_File)
  {
    itkExceptionMacro(<<"Failed to open file: "<<m_FileName);
  }

  m_OpenJpegStream = otb_openjpeg_opj_stream_create_default_file_stream(m_File,true);

  if (!m_OpenJpegStream)
  {
    itkExceptionMacro(<<"Failed to create file stream.");
  }

  OPJ_INT32 tile_x0,tile_y0;
  OPJ_UINT32 tile_width,tile_height,nb_tiles_x,nb_tiles_y;

  if (!otb_openjpeg_opj_read_header(m_Codec,
                                    &m_OpenJpegImage,
                                    &tile_x0,
                                    &tile_y0,
                                    &tile_width,
                                    &tile_height,
                                    &nb_tiles_x,
                                    &nb_tiles_y,
                                    m_OpenJpegStream))
  {
    itkExceptionMacro(<<"Failed to read image header.");
  }

  m_Dimensions[0]=m_OpenJpegImage->x1 - m_OpenJpegImage->x0;
  m_Dimensions[1]=m_OpenJpegImage->y1 - m_OpenJpegImage->y0;

  this->SetNumberOfDimensions(2);

  if (m_Dimensions[0] == 0 || m_Dimensions[1] == 0)
  {
    itkExceptionMacro(<<"Image size is null.");
  }

  this->SetNumberOfComponents(m_OpenJpegImage->numcomps);

  this->SetFileTypeToBinary();

  unsigned int precision = m_OpenJpegImage->comps->prec;
  int isSigned = m_OpenJpegImage->comps->sgnd;

  if (precision<=8)
  {
    m_NbOctetPixel = 1;
    if (isSigned)
    {
      SetComponentType(CHAR);
    }
    else
    {
      SetComponentType(UCHAR);
    }
  }
  else if (precision<=16)
  {
    m_NbOctetPixel = 2;
    if (isSigned)
    {
      SetComponentType(SHORT);
    }
    else
    {
      SetComponentType(USHORT);
    }
  }
  else
  {
    m_NbOctetPixel = 4;
    if (isSigned)
    {
      SetComponentType(INT);
    }
    else
    {
      SetComponentType(UINT);
    }
  }

  if ( this->GetNumberOfComponents() == 1 )
  {
    this->SetPixelType(SCALAR);
  }
  else
  {
    this->SetPixelType(VECTOR);
  }

  otbMsgDebugMacro(<<"==========================");
  otbMsgDebugMacro(<<"ReadImageInformation: ");
  otbMsgDebugMacro(<<"Tile (x0,y0): "<<tile_x0<<" "<<tile_y0);
  otbMsgDebugMacro(<<"Tile size: "<<tile_width<<" x "<<tile_height);
  otbMsgDebugMacro(<<"Number of tiles: "<<nb_tiles_x<<" "<<nb_tiles_y);
  otbMsgDebugMacro(<<"Precision: "<<precision);
  otbMsgDebugMacro(<<"Signed: "<<isSigned);
  otbMsgDebugMacro(<<"Number of octet per value: "<<m_NbOctetPixel);
  otbMsgDebugMacro(<<"==========================");


  otbMsgDebugMacro( <<"Driver to read: JPEG2000");
  otbMsgDebugMacro( <<"         Read  file         : "<< m_FileName);
  otbMsgDebugMacro( <<"         Size               : "<<m_Dimensions[0]<<","<<m_Dimensions[1]);
  otbMsgDebugMacro( <<"         ComponentType      : "<<this->GetComponentType());
  otbMsgDebugMacro( <<"         NumberOfComponents : "<<this->GetNumberOfComponents());
  otbMsgDebugMacro( <<"         ComponentSize      : "<<this->GetComponentSize());
  otbMsgDebugMacro( <<"         GetPixelSize       : "<<this->GetPixelSize());

  otb_openjpeg_opj_stream_destroy(m_OpenJpegStream);
  fclose(m_File);
  otb_openjpeg_opj_destroy_codec(m_Codec);
  otb_openjpeg_opj_image_destroy(m_OpenJpegImage);
}


bool JPEG2000ImageIO::CanWriteFile( const char* /*filename*/ )
{
  return false;
//     std::string lFileName(filename);
//     if( System::IsADirName(lFileName) == true )
//       {
//   return false;
//       }
//     const std::string Extension = System::GetExtension(filename);
//     if( (Extension == "j2k") || (Extension == "J2K") || (Extension == "jp2") || (Extension == "JP2"))
//       {
//   return true;
//       }
//     else
//       {
//   return false;
//       }
}

void JPEG2000ImageIO::Write(const void* /*buffer*/)
{
  // char * charstarbuffer = static_cast<char *>(buffer);

// m_NbBands = this->GetNumberOfComponents();

//       if( (m_Dimensions[0]==0) && (m_Dimensions[1]==0))
//         {
//                 itkExceptionMacro(<<"Dimensions are not defined.");
//         }

//         if ( this->GetComponentType() == CHAR )
//         {
//                 m_NbOctetPixel = 1;
//                 m_PxType = GDT_Byte;
//         }
//         else if ( this->GetComponentType() == UCHAR )
//         {
//                 m_NbOctetPixel = 1;
//                 m_PxType = GDT_Byte;
//         }
//         else if ( this->GetComponentType() == USHORT )
//         {
//                 m_NbOctetPixel = 2;
//                 m_PxType = GDT_UInt16;
//         }
//         else if ( this->GetComponentType() == SHORT )
//         {
//                 m_NbOctetPixel = 2;
//                 m_PxType = GDT_Int16;
//         }
//         else if ( this->GetComponentType() == INT )
//         {
//                 m_NbOctetPixel = 4;
//                 m_PxType = GDT_Int32;
//         }
//         else if ( this->GetComponentType() == UINT )
//         {
//                 m_NbOctetPixel = 4;
//                 m_PxType = GDT_UInt32;
//         }
//         else if ( this->GetComponentType() == FLOAT )
//         {
//                 m_NbOctetPixel = 4;
//                 m_PxType = GDT_Float32;
//         }
//         else if ( this->GetComponentType() == DOUBLE )
//         {
//                 m_NbOctetPixel = 8;
//                 m_PxType = GDT_Float64;
//         }
//         else
//         {
//                 m_NbOctetPixel = 1;
//                 m_PxType = GDT_Byte;
//         }

//         // Automatically set the Type to Binary for GDAL data
//         this->SetFileTypeToBinary();

//     /** you may here add custom encoding parameters */
//     /* rate specifications */
//     /** number of quality layers in the stream */
//     m_Parameters.tcp_numlayers = 1;
//     m_Parameters.cp_fixed_quality = 1;
//     m_Parameters.tcp_distoratio[0] = 20;
//     /* is using others way of calculation */
//     /* m_Parameters.cp_disto_alloc = 1 or m_Parameters.cp_fixed_alloc = 1 */
//     /* m_Parameters.tcp_rates[0] = ... */


//     /* tile definitions parameters */
//     /* position of the tile grid aligned with the image */
//     m_Parameters.cp_tx0 = 0;
//     m_Parameters.cp_ty0 = 0;
//     /* tile size, we are using tile based encoding */
//     m_Parameters.tile_size_on = true;
//     m_Parameters.cp_tdx = 512;
//     m_Parameters.cp_tdy = 512;

//     /* use irreversible encoding ?*/
//     m_Parameters.irreversible = 1;

//     /* do not bother with mct, the rsiz is set when calling opj_set_MCT*/
//     /*m_Parameters.cp_rsiz = STD_RSIZ;*/

//     /* no cinema */
//     /*m_Parameters.cp_cinema = 0;*/

//     /* no not bother using SOP or EPH markers, do not use custom size precinct */
//     /* number of precincts to specify */
//     /* m_Parameters.csty = 0;*/
//     /* m_Parameters.res_spec = ... */
//     /* m_Parameters.prch_init[i] = .. */
//     /* m_Parameters.prcw_init[i] = .. */


//     /* do not use progression order changes */
//     /*m_Parameters.numpocs = 0;*/
//     /* m_Parameters.POC[i].... */

//     /* do not restrain the size for a component.*/
//     /* m_Parameters.max_comp_size = 0; */

//     /** block encoding style for each component, do not use at the moment */
//     /** J2K_CCP_CBLKSTY_TERMALL, J2K_CCP_CBLKSTY_LAZY, J2K_CCP_CBLKSTY_VSC, J2K_CCP_CBLKSTY_SEGSYM, J2K_CCP_CBLKSTY_RESET */
//     /* m_Parameters.mode = 0;*/

//     /** number of resolutions */
//     m_Parameters.numresolution = 6;

//     /** progression order to use*/
//     /** LRCP, RLCP, RPCL, PCRL, CPRL */
//     m_Parameters.prog_order = LRCP;

//     /** no "region" of interest, more precisally component */
//     /* m_Parameters.roi_compno = -1; */
//     /* m_Parameters.roi_shift = 0; */

//     /* we are not using multiple tile parts for a tile. */
//     /* m_Parameters.tp_on = 0; */
//     /* m_Parameters.tp_flag = 0; */

//     /* if we are using mct */
//     /* opj_set_MCT(&m_Parameters,l_mct,l_offsets,NUM_COMPS); */


//     /* image definition */
//     l_current_param_ptr = m_Parameters;
//     for
//       (i=0;i<NUM_COMPS;++i)
//       {
//   /* do not bother bpp useless */
//   /*l_current_param_ptr->bpp = COMP_PREC;*/
//   l_current_param_ptr->dx = 1;
//   l_current_param_ptr->dy = 1;
//   l_current_param_ptr->h = m_Dimensions[1];
//   l_current_param_ptr->sgnd = 0;
//   l_current_param_ptr->prec = 8*m_NbOctetPixel;
//   l_current_param_ptr->w = m_Dimensions[0];
//   l_current_param_ptr->x0 = 0;
//   l_current_param_ptr->y0 = 0;
//   ++l_current_param_ptr;
//       }


//     m_Codec = opj_create_compress(CODEC_JP2);
//     opj_set_info_handler(m_Codec, info_callback,00);
//     opj_set_warning_handler(m_Codec, warning_callback,00);
//     opj_set_error_handler(m_Codec, error_callback,00);

//     if(!m_Codec)
//       {
//   itkExceptionMacro(<<"Failed to create openjpeg codec.");
//       }

//     m_OpenJpegImage = opj_image_tile_create(m_NbBands,m_Parameters,CLRSPC_SRGB);

//     m_OpenJpegImage->x0 = 0;
//     m_OpenJpegImage->y0 = 0;
//     m_OpenJpegImage->x1 = m_Dimensions[0]-1;
//     m_OpenJpegImage->y1 = m_Dimensions[1]-1;
//     m_OpenJpegImage->color_space = CLRSPC_SRGB;

//     if(!m_OpenJpegImage)
//       {
//   opj_destroy_codec(m_Code);
//   itkExceptionMacro(<<"Failed to create openjpeg image.");
//       }

//     // Create default parameters
//     opj_set_default_encoder_parameters(&m_Parameters);
//     // TODO: add custom parameters here

//     if(!opj_setup_encoder(m_Codec,&m_Parameters,m_OpenJpegImage))
//       {
//   itkExceptionMacro(<<"Failed to set up decoder parameters.");
//       }

//     m_File = fopen(m_FileName.c_str(),"wb");

//     if(!m_File)
//       {
//   itkExceptionMacro(<<"Failed to open file: "<<m_FileName);
//       }

//     m_OpenJpegStream = opj_stream_create_default_file_stream(m_File,true);

//     if(!m_OpenJpegStream)
//       {
//   itkExceptionMacro(<<"Failed to create file stream.");
//       }

//     std::streamoff buffer_x0 = this->GetIORegion().GetIndex()[0];
//     std::streamoff buffer_y0 = this->GetIORegion().GetIndex()[1];
//     std::streamsize buffer_size_x = this->GetIORegion().GetSize()[0];
//     std::streamsize buffer_size_y = this->GetIORegion().GetSize()[1];

//     std::streamsize buffer_size = this->GetIORegion().GetNumberOfPixels()*m_NbOctetPixel*m_nbBands;

//     unsigned int nb_tile_x = (unsigned int)vcl_ceil((double)m_Dimensions[0]/(double)m_Parameters.cp_tdx);
//     unsigned int nb_tile_y = (unsigned int)vcl_ceil((double)m_Dimensions[1]/(double)m_Parameters.cp_tdy);

//     unsigned int tile_index = nb_tile_x*buffer_y0/m_Parameters.cp_tdy+buffer_x0/m_Parameters.cp_tdx;


//     OPJ_BYTE * desinterleaved_data = new OPJ_BYTE[buffer_size];

//     std::streamoff step = m_NbBands*m_NbOctetPixel;

//     std::streamsize component_size = new

//     for(comp = 0;comp<m_NbBands;++comp)
//       {


//       }


//     if(! opj_write_tile(m_Codec,tile_index,charstarbuffer,buffer_size,m_OpenJpegStream))
//       {


//       }
}


void JPEG2000ImageIO::WriteImageInformation()
{
  if ( m_FileName == "" )
  {
    itkExceptionMacro(<<"A FileName must be specified.");
  }
  if ( CanWriteFile(m_FileName.c_str()) == false)
  {
    itkExceptionMacro(<< "The file "<<m_FileName.c_str()<<" is not defined as a JPEG2000 file");
  }

  otbMsgDebugMacro( <<"Driver to write: JPEG2000");
  otbMsgDebugMacro( <<"         Write file         : "<< m_FileName);
  otbMsgDebugMacro( <<"         Size               : "<<m_Dimensions[0]<<","<<m_Dimensions[1]);
  otbMsgDebugMacro( <<"         ComponentType      : "<<this->GetComponentType());
  otbMsgDebugMacro( <<"         NumberOfComponents : "<<this->GetNumberOfComponents());
  otbMsgDebugMacro( <<"         ComponentSize      : "<<this->GetComponentSize());
  otbMsgDebugMacro( <<"         GetPixelSize       : "<<this->GetPixelSize());

}


} // end namespace otb

