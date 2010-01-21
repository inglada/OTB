//----------------------------------------------------------------------------
//
// "Copyright Centre National d'Etudes Spatiales"
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
//----------------------------------------------------------------------------
// $Id$

#include <RadarSat/Data/Data.h>
#include <RadarSat/Data/DataFactory.h>
#include <RadarSat/RadarSatRecordHeader.h>
#include <RadarSat/Data/ImageOptionsFileDescriptor.h>

namespace ossimplugins
{

const int Data::ImageOptionsFileDescriptorID = 1;
const int Data::FirstProcessedDataRecordID = 2;
const int Data::LastProcessedDataRecordID = 3;

Data::Data()
{
}

Data::~Data()
{
	ClearRecords();
}

std::ostream& operator<<(std::ostream& os, const Data& data)
{
	std::map<int, RadarSatRecord*>::const_iterator it = data._records.begin();
	while(it != data._records.end())
	{
		(*it).second->Write(os);
		++it;
	}
	return os;

}

std::istream& operator>>(std::istream& is, Data& data)
{
	DataFactory factory;

	data.ClearRecords();

	if (sizeof(int)!=4) std::cout << "RadarSat Data WARNING : (int) not coded over 32 bits, metadata might not be byte swapped correctly"<< std::endl ;
	if (sizeof(float)!=4) std::cout << "RadarSat Data WARNING : (float) not coded over 32 bits, metadata might not be byte swapped correctly"<< std::endl ;
	if (sizeof(double)!=8) std::cout << "RadarSat Data WARNING : (double) not coded over 64 bits, metadata might not be byte swapped correctly"<< std::endl ;

	RadarSatRecordHeader header;
	bool eof = false;

	int nbLin = 0 ; // number of image lines
	int lineLength = 0 ; // size of any ProcessedDataRecord

	while(!eof)
	{
		is>>header;
		if(is.eof())
		{
			eof = true;
		}
		else
		{
			if (header.get_rec_seq() == 1) { // ImageOptionsFileDescriptor
      				RadarSatRecord* record = factory.Instanciate(header.get_rec_seq());
				if (record != NULL)
				{
					record->Read(is);
					data._records[Data::ImageOptionsFileDescriptorID] = record;

					nbLin  = ((ImageOptionsFileDescriptor *) record)->get_nlin() ;
				}
				else
				{
					char* buff = new char[header.get_length()-12];
					is.read(buff, header.get_length()-12);
					delete[] buff;
				}
			}
			else if ((header.get_rec_seq() == 2)) { // First line ProcessedDataRecord

				lineLength = header.get_length() ;
      				RadarSatRecord* record = factory.Instanciate(2);
				if (record != NULL)
				{
					record->Read(is);
					data._records[Data::FirstProcessedDataRecordID] = record;

					char* buff = new char[header.get_length()-192];
					is.read(buff, header.get_length()-192);	// Reads the rest of the line
					delete[] buff;
				}
				else
				{
					char* buff = new char[header.get_length()-12];
					is.read(buff, header.get_length()-12);
					delete[] buff;
				}
			}
			else if ((header.get_rec_seq() == (1+nbLin))) { // Last line ProcessedDataRecord
      				RadarSatRecord* record = factory.Instanciate(2);
				if (record != NULL)
				{
					record->Read(is);
					data._records[Data::LastProcessedDataRecordID] = record;

					char* buff = new char[header.get_length()-192];
					is.read(buff, header.get_length()-192);	// Reads the rest of the line
					delete[] buff;
				}
				else
				{
					char* buff = new char[header.get_length()-12];
					is.read(buff, header.get_length()-12);
					delete[] buff;
				}
			}
			else
			{
				// all lines between the first and last ones are skipped
				if (lineLength != 0)
					is.seekg((nbLin-2)*lineLength-12, std::ios::cur) ;
				else
					is.seekg(0, std::ios::end) ;
			}

		}
/*
	//while(!eof)
	{
		is>>header;
		if(is.eof())
		{
			eof = true;
		}
		else
		{
			RadarSatRecord* record = factory.Instanciate(header.get_rec_seq());
			if (record != NULL)
			{
				record->Read(is);
				data._records[header.get_rec_seq()] = record;
			}
			else
			{
				char* buff = new char[header.get_length()];
				is.read(buff, header.get_length());
				delete buff;
			}
		}
*/

	}
	return is;
}


Data::Data(const Data& rhs)
{
	std::map<int, RadarSatRecord*>::const_iterator it = rhs._records.begin();
	while(it != rhs._records.end())
	{
		_records[(*it).first] = (*it).second->Clone();
		++it;
	}
}

Data& Data::operator=(const Data& rhs)
{
	std::map<int, RadarSatRecord*>::const_iterator it = _records.begin();
	while(it != _records.end())
	{
		delete (*it).second;
		++it;
	}
	_records.clear();

	it = rhs._records.begin();
	while(it != rhs._records.end())
	{
		_records[(*it).first] = (*it).second->Clone();
		++it;
	}

	return *this;
}

void Data::ClearRecords()
{
	std::map<int, RadarSatRecord*>::const_iterator it = _records.begin();
	while(it != _records.end())
	{
		delete (*it).second;
		++it;
	}
	_records.clear();
}

void Data::InsertRecord(int id, RadarSatRecord* record)
{
	_records[id] = record;
}

ImageOptionsFileDescriptor* Data::get_ImageOptionsFileDescriptor()
{
	return (ImageOptionsFileDescriptor*)_records[ImageOptionsFileDescriptorID];
}

ProcessedDataRecord* Data::get_FirstProcessedDataRecord()
{
	return (ProcessedDataRecord*)_records[FirstProcessedDataRecordID];
}

ProcessedDataRecord* Data::get_LastProcessedDataRecord()
{
	return (ProcessedDataRecord*)_records[LastProcessedDataRecordID];
}
}