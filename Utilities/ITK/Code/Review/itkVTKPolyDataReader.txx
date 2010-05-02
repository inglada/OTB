/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkVTKPolyDataReader.txx,v $
  Language:  C++
  Date:      $Date: 2009-12-02 18:13:18 $
  Version:   $Revision: 1.19 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkVTKPolyDataReader_txx
#define __itkVTKPolyDataReader_txx

#include "itkVTKPolyDataReader.h"
#include <fstream>
#include <stdio.h>
#include <string.h>

namespace itk
{

// 
// Constructor
// 
template<class TOutputMesh>
VTKPolyDataReader<TOutputMesh>
::VTKPolyDataReader()
{
  //
  // Create the output
  //
  typename TOutputMesh::Pointer output = TOutputMesh::New();
  this->ProcessObject::SetNumberOfRequiredOutputs(1);
  this->ProcessObject::SetNthOutput(0, output.GetPointer());
}

template<class TOutputMesh>
void
VTKPolyDataReader<TOutputMesh>
::GenerateData()
{
  typename OutputMeshType::Pointer outputMesh = this->GetOutput();

  outputMesh->SetCellsAllocationMethod(
      OutputMeshType::CellsAllocatedDynamicallyCellByCell );

  if( m_FileName == "" )
    {
    itkExceptionMacro(<< "No input FileName");
    }

  //
  // Read input file
  //
  std::ifstream inputFile( m_FileName.c_str() );

  if( !inputFile.is_open() )
    {
    itkExceptionMacro(<< "Unable to open file\n"
                      << "inputFilename= " << m_FileName );
    }

  inputFile.imbue(std::locale::classic());
  std::string line;

  // The first line must be "# vtk DataFile Version x.x" where x.x can
  // vary
  std::getline( inputFile, m_Version, '\n' );
  if (inputFile.fail())
    {
    itkExceptionMacro(<< "Error reading file: " << m_FileName
                      << "\nUnexpected end-of-file trying to read first line.");
    }
  if (m_Version.find("# vtk DataFile Version ") == std::string::npos)
    {
    itkExceptionMacro(<< "Error reading file: " << m_FileName
                      << "\nOnly vtk legacy format files can be read."
                      << "\nThis file does not start with the line: # vtk DataFile Version x.x where x.x is the version.");
    }

  // Next is a one line description
  std::getline( inputFile, m_Header, '\n' );
  if (inputFile.eof())
    {
    itkExceptionMacro(<< "Error reading file: " << m_FileName
                      << "\nUnexpected end-of-file trying to read header.");
    }
  
  // Next is the file format
  std::getline( inputFile, line, '\n' );
  if (inputFile.eof())
    {
    itkExceptionMacro(<< "Error reading file: " << m_FileName
                      << "\nUnexpected end-of-file trying to file format.");
    }
  if (line.find("ASCII") == std::string::npos)
    {
    itkExceptionMacro(<< "Error reading file: " << m_FileName
                      << "\nFile format is " << line
                      << " but only ASCII files can be read.");
    }
  
  bool foundPoints = false;
  while( !inputFile.eof() )
    {
    std::getline( inputFile, line, '\n' );

    if( line.find("POINTS") != std::string::npos )
      {
      foundPoints = true;
      break;
      }
    }

  if (!foundPoints)
    {
    itkExceptionMacro(<< "Error reading file: " << m_FileName
                      << "\nUnexpected end-of-file before finding POINTS.");
    }
  itkDebugMacro("POINTS line" << line );

  std::string pointLine( line, strlen("POINTS "), line.length() );
  itkDebugMacro("pointLine " << pointLine );

  int numberOfPoints = -1;

  if( sscanf(pointLine.c_str(),"%d",&numberOfPoints) != 1 )
    {
    itkExceptionMacro(<< "Error reading file: " << m_FileName
                      << "\nFailed to read numberOfPoints.\n"
                      << "       pointLine= " << pointLine );
    }

  itkDebugMacro("numberOfPoints= " << numberOfPoints );

  if( numberOfPoints < 1 )
    {
    itkExceptionMacro(<< "Error reading file: " << m_FileName
                      << "numberOfPoints < 1"
                      << "       numberOfPoints line= " << numberOfPoints );
    }

  outputMesh->GetPoints()->Reserve( numberOfPoints );

  //
  // Load the point coordinates into the itk::Mesh
  //

  PointType point;

  for( int i=0; i < numberOfPoints; i++ )
    {
    inputFile >> point;
    if (inputFile.eof())
      {
      itkExceptionMacro(<< "Error while reading file: " << m_FileName
                        << "\nUnexpected end-of-file trying to read points.");
      }
    if (inputFile.fail())
      {
      itkExceptionMacro(<< "Error reading file: " << m_FileName
                        << "\nInput could not be interpreted as a point.");
      }
    outputMesh->SetPoint( i, point );
    }

  // Continue searching for the POLYGONS line
  bool foundPolygons = false;
  while( !inputFile.eof() )
    {
    std::getline( inputFile, line, '\n' );
    if (line.find("POLYGONS") != std::string::npos )
      {
      foundPolygons = true;
      break;
      }
    }

  if (!foundPolygons)
    {
    itkExceptionMacro(<< "Error reading file: " << m_FileName
                      << "\nUnexpected end-of-file before finding POLYGONS.");
    }

  itkDebugMacro( "POLYGONS line" << line );

  std::string polygonLine( line, strlen("POLYGONS "), line.length() );
  itkDebugMacro( "polygonLine " << polygonLine );

  //
  // Read the number of polygons
  //

  CellIdentifier numberOfPolygons = 0;
  CellIdentifier numberOfIndices = 0;

  if( sscanf( polygonLine.c_str(), "%ld %ld", &numberOfPolygons,
        &numberOfIndices ) != 2 )
    {
    itkExceptionMacro(<< "Error reading file: " << m_FileName
                      << "\nFailed to read numberOfPolygons from subline2"
                      << "\npolygonLine = " << polygonLine );
    }

  itkDebugMacro("numberOfPolygons " << numberOfPolygons );
  itkDebugMacro("numberOfIndices " << numberOfIndices );

  if( numberOfPolygons < 1 )
    {
    itkExceptionMacro(<< "Error reading file: " << m_FileName
                      << "\nnumberOfPolygons < 1\nnumberOfPolygons= "
                      << numberOfPolygons );
    }

  if( numberOfIndices < numberOfPolygons )
    {
    itkExceptionMacro(<< "Error reading file: " << m_FileName
                      << "\nnumberOfIndices < numberOfPolygons\n"
                      << "numberOfIndices= " << numberOfIndices << "\n"
                      << "numberOfPolygons= " << numberOfPolygons );
    }

  //
  // Load the polygons into the itk::Mesh
  //

  PointIdentifier numberOfCellPoints;
  long ids[3];

  for(CellIdentifier i=0; i<numberOfPolygons; i++)
    {
    std::getline( inputFile, line, '\n' );
    if( inputFile.eof() )
      {
      itkExceptionMacro(<< "Error reading file: " << m_FileName
                        << "\nFailed to read " << numberOfPolygons
                        << " polygons before the end of file."
                        << " Only read " << i+1);
      }

    if( line.find("DATA") != std::string::npos )
      {
      itkExceptionMacro(<< "Error reading file: " << m_FileName
                        << "\nRead keyword DATA");
      }

    int got;
    if( (got = sscanf( line.c_str(), "%ld %ld %ld %ld", &numberOfCellPoints,
                       &ids[0], &ids[1], &ids[2] )) != 4 )
      {
      itkExceptionMacro(<< "Error reading file: " << m_FileName
                        << "\nError parsing POLYGON cell. Expected 4 items but got "
                        << got << std::endl
                        << "Line is: " << line);
      }

    if( numberOfCellPoints != 3 )
      {
      itkExceptionMacro(<< "Error reading file: " << m_FileName
                        << "\nnumberOfCellPoints != 3\n"
                        << "numberOfCellPoints= " << numberOfCellPoints
                        << ". VTKPolyDataReader can only read triangles");
      }

    if( static_cast<long>(ids[0]) < 0 ||
        static_cast<long>(ids[1]) < 0 ||
        static_cast<long>(ids[2]) < 0 )
      {
      itkExceptionMacro(<< "Error reading file: " << m_FileName
                        << "point ids must be >= 0.\n"
                        "ids=" << ids[0] << " " << ids[1] << " " << ids[2]);
      }

    if( static_cast<long>(ids[0]) >= numberOfPoints ||
        static_cast<long>(ids[1]) >= numberOfPoints ||
        static_cast<long>(ids[2]) >= numberOfPoints )
      {
      itkExceptionMacro(<< "Error reading file: " << m_FileName
                        << "Point ids must be < number of points: "
                        << numberOfPoints
                        << "\nids= " << ids[0] << " " << ids[1] << " " << ids[2]);
      }

    CellAutoPointer cell;
    TriangleCellType * triangleCell = new TriangleCellType;
    for( PointIdentifier k = 0; k < numberOfCellPoints; k++ )
      {
      triangleCell->SetPointId( k, ids[k] );
      }

    cell.TakeOwnership( triangleCell );
    outputMesh->SetCell( i, cell );
    }

  bool foundPointData = false;

  while( !inputFile.eof() )
    {
    std::getline( inputFile, line, '\n' );

    if( line.find("POINT_DATA") != std::string::npos )
      {
      foundPointData = true;
      break;
      }
    }

  if( foundPointData )
    {
    typedef typename OutputMeshType::PointDataContainer PointDataContainer;

    outputMesh->SetPointData( PointDataContainer::New() );
    outputMesh->GetPointData()->Reserve( numberOfPoints );

    itkDebugMacro("POINT_DATA line" << line );

    // Skip two lines
    if (!inputFile.eof())
      {
      std::getline( inputFile, line, '\n' );
      }
    else
      {
      itkExceptionMacro(<< "Error reading file: " << m_FileName
                        << "\nUnexpected end-of-file while trying to read POINT_DATA.");
      }
    if (!inputFile.eof())
      {
      std::getline( inputFile, line, '\n' );
      }
    else
      {
      itkExceptionMacro(<< "Error reading file: " << m_FileName
                        << "\nUnexpected end-of-file while trying to read POINT_DATA.");
      }

    double pointData;

    for( int pid=0; pid < numberOfPoints; pid++ )
      {
      if (inputFile.eof())
        {
        itkExceptionMacro(<< "Error reading file: " << m_FileName
                          << "\nUnexpected end-of-file while trying to read POINT_DATA."
                          << "Failed while trying to reading point data for id: " << pid);
        }
      inputFile >> pointData;
      outputMesh->SetPointData( pid, pointData );
      }
    }
  inputFile.close();
}

template<class TOutputMesh>
void
VTKPolyDataReader<TOutputMesh>
::PrintSelf( std::ostream& os, Indent indent ) const
{
  Superclass::PrintSelf(os,indent);

  os << indent << "FileName: " << m_FileName << std::endl;
  os << indent << "Version: " << m_Version << std::endl;
  os << indent << "Header: " << m_Header << std::endl;
}

} //end of namespace itk


#endif
