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
#include "otbRADImageIO.h"
#include "itkByteSwapper.h"
#include <itksys/SystemTools.hxx>
#include <string.h>  // for strncpy

#include "itkExceptionObject.h"
#include "otbMacro.h"
#include "itkMacro.h"

#include "otbSystem.h"

#include <fstream>
#include <iostream>


namespace otb
{

RADImageIO::RADImageIO()
{
  // By default set number of dimensions to two.
  this->SetNumberOfDimensions(2);

  // By default the type to CI2
  m_TypeRAD= "CI2";  
  m_NbOctetPixel=4;
  m_NbOfChannels=1;
  this->SetNumberOfComponents(2);
  m_PixelType = COMPLEX;
  m_ComponentType = SHORT;

  
  if ( itk::ByteSwapper<char>::SystemIsLittleEndian() == true)
  {
        m_ByteOrder = LittleEndian;
  }
  else
  {
        m_ByteOrder = BigEndian;
  }

  m_FileByteOrder = m_ByteOrder;

  // Set default spacing to one
  m_Spacing[0] = 1.0;
  m_Spacing[1] = 1.0;
  // Set default origin to zero
  m_Origin[0] = 0.0;
  m_Origin[1] = 0.0;
  m_ChannelsFile = NULL;
  m_FlagWriteImageInformation = true;
  
  std::cout<< " Fin constructeur" <<std::endl;
}

RADImageIO::~RADImageIO()
{

        if( m_HeaderFile.is_open() )
        {
                m_HeaderFile.close();
        }
        if( m_ChannelsFile !=  NULL)
        {
                for(unsigned int numChannel = 0 ; numChannel< m_NbOfChannels ; numChannel++)
                {
                        if( m_ChannelsFile[numChannel].is_open() )
                        {
                                m_ChannelsFile[numChannel].close();
                        }
                }
                delete [] m_ChannelsFile;
        }
}

bool RADImageIO::CanReadFile( const char* filename )
{
        std::fstream header_file;
        std::string lFileName(filename);
        if( System::IsADirName(lFileName) == true )
        {
                return false;
        }
        if( System::SetToLower(System::GetExtension(lFileName)) != "rad" )
        {
                return false;
        }

        header_file.open( lFileName.c_str(),  std::ios::in );
        if( header_file.fail() )
        {
                otbMsgDevMacro(<<"RADImageIO::CanReadFile() failed header open ! " );
                return false;
        }
        
        //Read header informations
        bool lResult = InternalReadHeaderInformation(lFileName, header_file,false);
        header_file.close();
        return (lResult);
}


// Used to print information about this object
void RADImageIO::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

// Read a 3D image (or event more bands)... not implemented yet
void RADImageIO::ReadVolume(void*)
{
}

// Read image 
void RADImageIO::Read(void* buffer)
{
        unsigned long step = this->GetNumberOfComponents();
        char * p = static_cast<char *>(buffer);
 // unsigned char  
 
        int lNbLignes   = this->GetIORegion().GetSize()[1];
        int lNbColonnes = this->GetIORegion().GetSize()[0];
        int lPremiereLigne   = this->GetIORegion().GetIndex()[1] ; // [1... ]
        int lPremiereColonne = this->GetIORegion().GetIndex()[0] ; // [1... ]

otbMsgDevMacro( <<" RADImageIO::Read()  ");
otbMsgDevMacro( <<" Dimensions de l'image  : "<<m_Dimensions[0]<<","<<m_Dimensions[1]);
otbMsgDevMacro( <<" Region lue (IORegion)  : "<<this->GetIORegion());
otbMsgDevMacro( <<" Nb Of Components       : "<<this->GetNumberOfComponents());
otbMsgDevMacro( <<" Size Of Components     : "<<this->GetComponentSize());
otbMsgDevMacro( <<" Nb Of Channels         : "<<m_NbOfChannels);

std::cout <<" RADImageIO::Read()  "<<std::endl;
std::cout <<" Dimensions de l'image  : "<<m_Dimensions[0]<<","<<m_Dimensions[1]<<std::endl;
std::cout <<" Region lue (IORegion)  : "<<this->GetIORegion()<<std::endl;
std::cout <<" Nb Of Components       : "<<this->GetNumberOfComponents()<<std::endl;
std::cout <<" Size Of Components     : "<<this->GetComponentSize()<<std::endl;
std::cout <<" Nb Of Channels         : "<<m_NbOfChannels<<std::endl;


        std::streamoff  headerLength(0);
        std::streamoff  offset;        
        std::streamoff  numberOfBytesPerLines = static_cast<std::streamoff>( m_NbOctetPixel * m_Dimensions[0]);
        std::streamsize numberOfBytesToBeRead = m_NbOctetPixel * lNbColonnes;       
        std::streamsize numberOfBytesRead;        
        unsigned long cpt = 0;

        // Update the step variable
        step = step * (unsigned long)(this->GetComponentSize());
        
        char * value = new char[numberOfBytesToBeRead];
        if(value==NULL)
        {
                itkExceptionMacro(<<"RADImageIO::Read(): Bad alloc");
                return;
        }

otbMsgDevMacro( <<" sizeof(streamsize)    : "<<sizeof(std::streamsize));
otbMsgDevMacro( <<" sizeof(streampos)     : "<<sizeof(std::streampos));
otbMsgDevMacro( <<" sizeof(streamoff)     : "<<sizeof(std::streamoff));
otbMsgDevMacro( <<" sizeof(std::ios::beg) : "<<sizeof(std::ios::beg));
otbMsgDevMacro( <<" sizeof(size_t)        : "<<sizeof(size_t));
otbMsgDevMacro( <<" sizeof(unsigned long) : "<<sizeof(unsigned long));

        for (unsigned int numChannel = 0 ; numChannel < m_NbOfChannels ; numChannel++)
        {
                cpt = (unsigned long )(numChannel)* (unsigned long)(m_NbOctetPixel);
                //Read region of the channel
                for(int LineNo = lPremiereLigne;LineNo <lPremiereLigne + lNbLignes; LineNo++ )
                {
	                offset  =  headerLength + numberOfBytesPerLines * static_cast<std::streamoff>(LineNo);
        	        offset +=  static_cast<std::streamoff>( m_NbOctetPixel * lPremiereColonne);
  	                m_ChannelsFile[numChannel].seekg(offset, std::ios::beg);
                        //Read a line
                        m_ChannelsFile[numChannel].read( static_cast<char *>( value ), numberOfBytesToBeRead );                        

            	        numberOfBytesRead = m_ChannelsFile[numChannel].gcount();
#ifdef __APPLE_CC__
                        // fail() is broken in the Mac. It returns true when reaches eof().
                        if ( numberOfBytesRead != numberOfBytesToBeRead )
#else
                        if ( ( numberOfBytesRead != numberOfBytesToBeRead )  || m_ChannelsFile[numChannel].fail() )
#endif
                        {
      		                itkExceptionMacro(<<"RADImageIO::Read() Can Read the specified Region"); // read failed
                        }
                        for ( std::streamsize  i=0 ; i < numberOfBytesToBeRead ; i = i+static_cast<std::streamsize>(m_NbOctetPixel) )
                        {
                                memcpy((void*)(&(p[cpt])),(const void*)(&(value[i])),(size_t)(m_NbOctetPixel));
                                cpt += step;
                        }
                }
        }
        unsigned long numberOfPixelsOfRegion = lNbLignes * lNbColonnes * this->GetNumberOfComponents();


        // Swap bytes if necessary
        if ( 0 ) {}
        otbSwappFileToSystemMacro( unsigned short, USHORT, buffer, numberOfPixelsOfRegion )
        otbSwappFileToSystemMacro( short, SHORT, buffer, numberOfPixelsOfRegion )
        otbSwappFileToSystemMacro( char, CHAR, buffer, numberOfPixelsOfRegion )
        otbSwappFileToSystemMacro( unsigned char, UCHAR, buffer, numberOfPixelsOfRegion )
        otbSwappFileToSystemMacro( unsigned int, UINT, buffer, numberOfPixelsOfRegion )
        otbSwappFileToSystemMacro( int, INT, buffer, numberOfPixelsOfRegion )
        otbSwappFileToSystemMacro( long, LONG, buffer, numberOfPixelsOfRegion )
        otbSwappFileToSystemMacro( unsigned long, ULONG, buffer, numberOfPixelsOfRegion )
        otbSwappFileToSystemMacro( float, FLOAT, buffer, numberOfPixelsOfRegion )
        otbSwappFileToSystemMacro( double, DOUBLE, buffer, numberOfPixelsOfRegion )
        else
        {
                itkExceptionMacro(<<"RADImageIO::Read() undefined component type! " );
        }
       
       
  delete [] value;
  value = NULL;
        
}


void RADImageIO::ReadImageInformation()
{
        if( m_HeaderFile.is_open() )
        {
                m_HeaderFile.close();
        }
        m_HeaderFile.open( m_FileName.c_str(),  std::ios::in );
        if( m_HeaderFile.fail() )
        {
                itkExceptionMacro(<<"RADImageIO::ReadImageInformation() failed header open ! " );
        }

        //Read header informations
        InternalReadHeaderInformation(m_FileName, m_HeaderFile,true);

otbMsgDebugMacro( <<"Driver to read: RAD");
otbMsgDebugMacro( <<"         Read  file         : "<< m_FileName);
otbMsgDebugMacro( <<"         Size               : "<<m_Dimensions[0]<<","<<m_Dimensions[1]);
otbMsgDebugMacro( <<"         ComponentType      : "<<this->GetComponentType() );
otbMsgDebugMacro( <<"         NumberOfComponents : "<<this->GetNumberOfComponents());
otbMsgDebugMacro( <<"         ComponentSize      : "<<this->GetComponentSize());
otbMsgDebugMacro( <<"         GetPixelSize       : "<<this->GetPixelSize());

}


bool RADImageIO::InternalReadHeaderInformation(const std::string & file_name, std::fstream & file, const bool reportError)
{

        std::string lString;
        std::string lStrCodePix;
        int lNbOfChannels;
        
        // Read NBCOLONNES information
        file >> lString;
        lString = System::SetToUpper(lString);
        if( (lString != "NBCOLONNES") && (lString != "NBCOLUMNS"))
        {
                if( reportError == true )
                {
                        itkExceptionMacro(<< "RAD : 'NBCOLONNES' keyword is not find in the header file.");  
                }
                else
                {
                        return false;
                }
        }
        file >> m_Dimensions[0];
        
        // Read NBLIGNES information
        file >> lString;
        lString = System::SetToUpper(lString);
        if( (lString != "NBLIGNES") && (lString != "NBLINES") )
        {
                if( reportError == true )
                {
                        itkExceptionMacro(<< "RAD : 'NBLIGNES' keyword is not find in the header file.");  
                }
                else
                {
                        return false;
                }
        }
        file >> m_Dimensions[1];
       
        // Read NBPLANS information
        file >> lString;
        lString = System::SetToUpper(lString);        
        if( (lString != "NBPLANS") && (lString != "NBBANDS") )
        {
                if( reportError == true )
                {
                        itkExceptionMacro(<< "RAD : 'NBPLANS' keyword is not find in the header file.");  
                }
                else
                {
                        return false;
                }
        }
        file >> m_NbOfChannels;
        // Because we read complex : *2
        this->SetNumberOfComponents(2*m_NbOfChannels);      
        
        // Read TYPECODAGE information
        file >> lString;
        lString = System::SetToUpper(lString);
        if( (lString != "TYPECODAGE") && (lString != "TYPE") )
        {
                if( reportError == true )
                {
                        itkExceptionMacro(<< "RAD : the first line of the header file must be contains 'TYPECODAGE' caracters.");  
                }
                else
                {
                        return false;
                }
        }
        file >> lStrCodePix;


        lStrCodePix = System::SetToUpper(lStrCodePix);
        if(lStrCodePix == "OCT")
        {
                m_PixelType = SCALAR;
                SetComponentType(UCHAR);
                m_NbOctetPixel=1;       //ok
        }
        if(lStrCodePix == "PHA")
        {
                m_PixelType = SCALAR;        
                // 0 .. 360 degree
                SetComponentType(USHORT);
                m_NbOctetPixel=1;       //cmt tu codes 360 avec 8 bits stp ?
        }
        if(lStrCodePix == "I2")
        {
                m_PixelType = SCALAR;        
                SetComponentType(SHORT);
                m_NbOctetPixel=2;
        }
        if(lStrCodePix == "I4")
        {
                m_PixelType = SCALAR;        
                SetComponentType(LONG);
                m_NbOctetPixel=4;
        }
        if(lStrCodePix == "R4")
        {
                m_PixelType = SCALAR;
                SetComponentType(FLOAT);
                m_NbOctetPixel=4;
        }
        else if(lStrCodePix == "CI2") 
        {
                m_PixelType = COMPLEX;
                SetComponentType(SHORT);
                m_NbOctetPixel=4;
        }
        else if(lStrCodePix == "CR4")
        {
                m_PixelType = COMPLEX;
                SetComponentType(FLOAT);
                m_NbOctetPixel=8;
        }
        else if(lStrCodePix == "COCT")
        {
                m_PixelType = COMPLEX;
                SetComponentType(FLOAT);
                m_NbOctetPixel=2;
        }
        else if(lStrCodePix == "C3B")
        {
                m_PixelType = COMPLEX;
                SetComponentType(FLOAT);
                m_NbOctetPixel=2;
        }
        else if(lStrCodePix == "C5B")
        {
                m_PixelType = COMPLEX;
                SetComponentType(FLOAT);
                m_NbOctetPixel=2;
        }
        else if(lStrCodePix == "C7B")
        {
                m_PixelType = COMPLEX;
                SetComponentType(FLOAT);
                m_NbOctetPixel=2;
        }
        else if(lStrCodePix == "CI4")
        {
                m_PixelType = COMPLEX;
                SetComponentType(LONG);
                m_NbOctetPixel=8;
        }
        else if(lStrCodePix == "CR8")
        {
                m_PixelType = COMPLEX;
                SetComponentType(DOUBLE);
                m_NbOctetPixel=16;
        }
        else
        {
                if( reportError == true )
                {
                        itkExceptionMacro(<< "RAD : the value type '"<<lStrCodePix<<"' (second line) set in the header file is not reconized as correct value.");  
                }
                else
                {
                        return false;
                }
        }

        // Read "SENSCODAGE" informations (optionnal)
        file >> lString;
        if( lString.empty() == false)
        {
                lString = System::SetToUpper(lString);
                if( (lString == "SENSCODAGE") || (lString == "ORDER") )
                {
                        file >> lString;
                        lString = System::SetToUpper(lString);
                        if( lString == "INTEL" )
                        {
                                m_FileByteOrder = LittleEndian;
                        }                        
                        else if( lString == "IEEE" )
                        {
                                m_FileByteOrder = BigEndian;
                        }
                        else
                        {
                                if( reportError == true )
                                {
                                        itkExceptionMacro(<< "RAD : the value SENSCODAGE '"<<lString<<"' set in the header file is not reconized as correct value. Possible values are INTEL or IEEE");  
                                }
                                else
                                {
                                        return false;
                                }
                        }
                }
        }

        
        // Read FileName information
        std::string lPathName = System::GetPathName( file_name );
        unsigned int i=0;
        m_ChannelsFileName.clear();        
        for(unsigned int i=0 ; i<m_NbOfChannels ; i++)
        {
                file >> lString;
                ::itk::OStringStream lStream;
                lStream << lPathName <<"/"<< lString ;                
                m_ChannelsFileName.push_back(lStream.str());
   
        }
        file.close();

        m_ChannelsFile = new std::fstream[m_NbOfChannels];
       
        // Try to open channels file
        for (unsigned int channels = 0 ; channels<m_ChannelsFileName.size() ; channels++)
        {

                m_ChannelsFile[channels].open( m_ChannelsFileName[channels].c_str(), std::ios::in | std::ios::binary );
                if( m_ChannelsFile[channels].fail() )
                {
                        if( reportError == true )
                        {
                                itkExceptionMacro(<< "RAD : impossible to find the file <"<<m_ChannelsFileName[channels]<<">.");  
                        }
                        else
                        {
                                return false;
                        }
                }
        }
      	this->SetFileTypeToBinary();
        this->SetNumberOfDimensions(2);

        return(true);
}



bool RADImageIO::CanWriteFile( const char* filename )
{
        std::string lFileName(filename);
        if( System::SetToLower(System::GetExtension(lFileName)) != "rad" )
	  {
	    return false;
	  }
        if( System::IsADirName(lFileName) == true )
        {
                return false;
        }
}

void RADImageIO::Write(const void* buffer)
{

std::cout<< " Write !!!!!!!!! " <<std::endl;


        if( m_FlagWriteImageInformation == true )
        {
    	        this->WriteImageInformation();
  	        m_FlagWriteImageInformation = false;
        }

        unsigned long step = this->GetNumberOfComponents();
        unsigned int lNbLignes   = this->GetIORegion().GetSize()[1];
        unsigned int lNbColonnes = this->GetIORegion().GetSize()[0];
        int lPremiereLigne   = this->GetIORegion().GetIndex()[1] ; // [1... ]
        int lPremiereColonne = this->GetIORegion().GetIndex()[0] ; // [1... ]


std::cout<< " Write2 !!!!!!!!! " <<std::endl;
	// Cas particuliers : on controle que si la r�gion � �crire est de la m�me dimension que l'image enti�re,
	// on commence l'offset � 0 (lorsque que l'on est pas en "Streaming")
	if( (lNbLignes == m_Dimensions[1]) && (lNbColonnes == m_Dimensions[0]))
	{
                otbMsgDevMacro(<<"Force the IORegion offset to 0");
		lPremiereLigne = 0;
		lPremiereColonne = 0;
	}

otbMsgDevMacro( <<" RADImageIO::Write()  ");
otbMsgDevMacro( <<" Dimensions de l'image  : "<<m_Dimensions[0]<<","<<m_Dimensions[1]);
otbMsgDevMacro( <<" Region lue (IORegion)  : "<<this->GetIORegion());
otbMsgDevMacro( <<" Nb Of Components       : "<<this->GetNumberOfComponents());
otbMsgDevMacro( <<" GetComponentSize       : "<<this->GetComponentSize());

        std::streamoff headerLength(0);
        std::streamoff  numberOfBytesPerLines = static_cast<std::streamoff>(m_NbOctetPixel * m_Dimensions[0]);
        std::streamsize numberOfBytesToBeWrite = static_cast<std::streamsize>(m_NbOctetPixel * lNbColonnes);
        
        std::streamoff offset = 0;
        unsigned long cpt = 0;

        // Update the step variable
      step = m_NbOctetPixel;  //(unsigned long)(this->GetComponentSize()); //step * 
        
        const char * p = static_cast<const char *>(buffer);

std::cout<< " step : " <<step <<std::endl;
std::cout<< " numberOfBytesPerLines  : " <<numberOfBytesPerLines <<std::endl;
std::cout<< " numberOfBytesToBeWrite : " <<numberOfBytesToBeWrite <<std::endl;
  

 
        char* value = new char[numberOfBytesToBeWrite];
        if(value==NULL)
        {
                itkExceptionMacro(<<"RADImageIO::Write : Bad Alloc");
                return;
        }

        for (unsigned int numChannel = 0 ; numChannel < m_NbOfChannels ; numChannel++)
        {
                cpt = (unsigned long )(numChannel)* (unsigned long)(m_NbOctetPixel);
                //Read region of the channel
                for(unsigned int LineNo = lPremiereLigne;LineNo <lPremiereLigne + lNbLignes; LineNo++ )
                {
                        for ( std::streamsize  i=0 ; i < numberOfBytesToBeWrite ; i = i+static_cast<std::streamsize>(m_NbOctetPixel) )
                        {
                                memcpy((void*)(&(value[i])),(const void*)(&(p[cpt])),(size_t)(m_NbOctetPixel));
                                cpt += step;
                        }

	                offset  =  headerLength + numberOfBytesPerLines * static_cast<std::streamoff>(LineNo);
        	        offset +=  static_cast<std::streamoff>(m_NbOctetPixel * lPremiereColonne);
  	                m_ChannelsFile[numChannel].seekp(offset, std::ios::beg);
                        //Write a line
                        m_ChannelsFile[numChannel].write( static_cast<char *>( value ), numberOfBytesToBeWrite );
                }
        }

        delete [] value;
}


void RADImageIO::WriteImageInformation()
{
std::cout<< " WriteImageInformation !!!!!!!!! " <<std::endl;
        if ( m_FileName == "" )
        {
                itkExceptionMacro(<<"A FileName must be specified.");
        }
        if( CanWriteFile(m_FileName.c_str()) == false)
        {
                itkExceptionMacro(<< "The file "<<m_FileName.c_str()<<" is not defined as a RAD file");
        }

std::cout<< " Avant open !!!!!!!!! " <<std::endl;


        // Close file from any previous image
        if ( m_HeaderFile.is_open() )
        {
                m_HeaderFile.close();
        }
  
        // Open the new file for writing
        // Actually open the file
        m_HeaderFile.open( m_FileName.c_str(),  std::ios::out | std::ios::trunc );
        if( m_HeaderFile.fail() )
        {
                itkExceptionMacro(<< "Cannot write requested file "<<m_FileName.c_str()<<".");
        } 

      /*  if( 0 ) {}
        otbSetTypeRadMacro( CHAR,   "OCT")
        otbSetTypeRadMacro( UCHAR,  "UOCT")
        otbSetTypeRadMacro( SHORT,  "I2")
        otbSetTypeRadMacro( USHORT, "UI2")
        otbSetTypeRadMacro( INT,    "I4")
        otbSetTypeRadMacro( UINT,   "UI4")
        otbSetTypeRadMacro( FLOAT,  "R4")
        otbSetTypeRadMacro( DOUBLE, "R8")
        else
        {
                itkExceptionMacro(<< "RAD format doesn't reconized (TYPE).");
        }


*/        //Write COLUMNS information
        m_HeaderFile <<  "NBCOLUMNS "; //  << std::endl;
        m_HeaderFile << m_Dimensions[0] << std::endl;

        //Write LINES information
        m_HeaderFile <<  "NBLINES "; //  << std::endl;
        m_HeaderFile << m_Dimensions[1] << std::endl;
        
        //Write CHANNELS information
        m_HeaderFile << "NBBANDS "; //  << std::endl;
        m_HeaderFile << m_NbOfChannels << std::endl;        
        
        std::string lString;
        //Write TYPE information
        m_HeaderFile << "TYPECODAGE "; //  << std::endl;
        m_HeaderFile << m_TypeRAD << std::endl;

        //Write "SENSCODAGE" informations
        m_HeaderFile <<  "SENSCODAGE "; // << std::endl;
        if( m_ByteOrder == LittleEndian )
        {
                m_HeaderFile <<  "INTEL" << std::endl;
        }
        else
        {
                m_HeaderFile <<  "IEEE" << std::endl;
        }

        //Define channels file name
        std::string lRootName = System::GetRootName( m_FileName );
        m_ChannelsFileName.clear();
        for(unsigned int i=0 ; i<m_NbOfChannels ; i++)
        {
                ::itk::OStringStream lStream;

//comment on sait HH/HV/VH/VV ??

                lStream << lRootName <<"_"<< i+1 << ".ci2";
                m_ChannelsFileName.push_back(lStream.str());
        }

        for(unsigned int i=0;i<m_NbOfChannels;i++)
        {
                m_HeaderFile << System::GetShortFileName(this->m_ChannelsFileName[i].c_str()) << std::endl;
        }
        m_HeaderFile.close();

   


        //Allocate  buffer of stream file
        m_ChannelsFile = new std::fstream[m_NbOfChannels];

        //Try to open channels file
        for (unsigned int channels = 0 ; channels<m_ChannelsFileName.size() ; channels++)
        {
                m_ChannelsFile[channels].open( m_ChannelsFileName[channels].c_str(),  std::ios::out | std::ios::trunc | std::ios::binary );
                if( m_ChannelsFile[channels].fail() )
                {
                        itkExceptionMacro(<< "RAD : impossible to find the file <"<<m_ChannelsFileName[channels]<<">.");  
                }
        }
      	this->SetFileTypeToBinary();
        this->SetNumberOfDimensions(2);
        this->SetNumberOfComponents(2*m_NbOfChannels);

        unsigned long numberOfBytesPerLines = m_NbOctetPixel * m_Dimensions[0];
        char* value = new char[numberOfBytesPerLines];

        for (unsigned int channels = 0 ; channels<m_ChannelsFileName.size() ; channels++)
        {
                m_ChannelsFile[channels].seekp(0, std::ios::beg );
                //Write Header line and all file (whitout information)
                for(unsigned int numLigne=0 ; numLigne<(m_Dimensions[1]) ; numLigne++)
                {
                        m_ChannelsFile[channels].write(value,numberOfBytesPerLines);
                }
        }

        delete [] value;

otbMsgDebugMacro( <<"Driver to write: RAD");
otbMsgDebugMacro( <<"         Write file         : "<< m_FileName);
otbMsgDebugMacro( <<"         Size               : "<<m_Dimensions[0]<<","<<m_Dimensions[1]);
otbMsgDebugMacro( <<"         Type Rad           : "<<m_TypeRAD);
otbMsgDebugMacro( <<"         ComponentType      : "<<this->GetComponentType() );
otbMsgDebugMacro( <<"         NumberOfComponents : "<<this->GetNumberOfComponents());
otbMsgDebugMacro( <<"         ComponentSize      : "<<this->GetComponentSize());
otbMsgDebugMacro( <<"         GetPixelSize       : "<<this->GetPixelSize());


std::cout <<"Driver to write: RAD"<<std::endl;;
std::cout <<"         Write file         : "<< m_FileName<<std::endl;
std::cout <<"         Size               : "<<m_Dimensions[0]<<","<<m_Dimensions[1]<<std::endl;;
std::cout <<"         Type Rad           : "<<m_TypeRAD<<std::endl;;
std::cout <<"         ComponentType      : "<<this->GetComponentType() <<std::endl;;
std::cout <<"         NumberOfComponents : "<<this->GetNumberOfComponents()<<std::endl;;
std::cout <<"         ComponentSize      : "<<this->GetComponentSize()<<std::endl;;
std::cout <<"         GetPixelSize       : "<<this->GetPixelSize()<<std::endl;

}

  
} // end namespace otb


