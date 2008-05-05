/*=========================================================================

Program:   Insight Segmentation & Registration Toolkit
Module:    $RCSfile: itkSimplexMeshToTriangleMeshFilter.txx,v $
Language:  C++
Date:      $Date: 2004-11-08 13:33:19 $
Version:   $Revision: 1.5 $

Copyright (c) Insight Software Consortium. All rights reserved.
See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

This software is distributed WITHOUT ANY WARRANTY; without even 
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __SimplexMeshToTriangleMeshFilter_txx
#define __SimplexMeshToTriangleMeshFilter_txx

#include "itkSimplexMeshToTriangleMeshFilter.h"

namespace itk
{

template<typename TInputMesh, typename TOutputMesh>
SimplexMeshToTriangleMeshFilter<TInputMesh,TOutputMesh>::SimplexMeshToTriangleMeshFilter()
{
}

template <typename TInputMesh, typename TOutputMesh>
SimplexMeshToTriangleMeshFilter<TInputMesh, TOutputMesh>
::~SimplexMeshToTriangleMeshFilter()
{
}


template <typename TInputMesh, typename TOutputMesh>
void SimplexMeshToTriangleMeshFilter<TInputMesh, TOutputMesh>
::GenerateData()
{
  this->Initialize();
  this->CreateTriangles();
}

template <typename TInputMesh, typename TOutputMesh>
void SimplexMeshToTriangleMeshFilter<TInputMesh, TOutputMesh>
::Initialize()
{
  SimplexVisitorInterfacePointer simplexVisitor = SimplexVisitorInterfaceType::New();
  simplexVisitor->SetMesh( this->GetInput(0) );
  CellMultiVisitorPointer mv = CellMultiVisitorType::New();
  mv->AddVisitor(simplexVisitor);
  this->GetInput(0)->Accept(mv);
  this->GetInput(0)->BuildCellLinks();
  m_Centers = simplexVisitor->GetCenterMap();
}

template <typename TInputMesh, typename TOutputMesh>
void SimplexMeshToTriangleMeshFilter<TInputMesh, TOutputMesh>
::CreateTriangles()
{
  typename AutoMeshSourceType::Pointer meshSource = AutoMeshSourceType::New();
  typename AutoMeshSourceType::PointType p1,p2,p3;

  typename TInputMesh::Pointer inputMesh = this->GetInput(0);
  typename InputPointsContainer::Pointer points = inputMesh->GetPoints();
  typename TInputMesh::PointsContainerIterator pointsIt = points->Begin();

  meshSource->Update();

  while ( pointsIt != points->End() )
    {
    typename InputMeshType::IndexArray n = this->GetInput(0)->GetNeighbors( pointsIt.Index() );

    unsigned long newId1 = FindCellId(n[0], pointsIt.Index(), n[1]);
    unsigned long newId2 = FindCellId(n[1], pointsIt.Index(), n[2]);
    unsigned long newId3 = FindCellId(n[2], pointsIt.Index(), n[0]);

    bool b1 = m_Centers->GetElementIfIndexExists(newId1, &p1 );
    bool b2 = m_Centers->GetElementIfIndexExists(newId2, &p2 );
    bool b3 = m_Centers->GetElementIfIndexExists(newId3, &p3 );

    meshSource->AddTriangle(p1,p2,p3);

    if( !(b1 && b2 && b3) )
      {
      itkExceptionMacro(<<"Assertion failed for test of GetElementIfIndexExists()");
      }

    pointsIt++;
    }

  this->ProcessObject::SetNthOutput(0,  meshSource->GetOutput());
}

template <typename TInputMesh, typename TOutputMesh>
unsigned long SimplexMeshToTriangleMeshFilter<TInputMesh, TOutputMesh>
::FindCellId(unsigned long id1, unsigned long id2, unsigned long id3)
{
  std::set<unsigned long> cells1 =  this->GetInput(0)->GetCellLinks()->GetElement(id1);
  std::set<unsigned long> cells2 =  this->GetInput(0)->GetCellLinks()->GetElement(id2);
  std::set<unsigned long> cells3 =  this->GetInput(0)->GetCellLinks()->GetElement(id3);
  std::set<unsigned long>::iterator cellIt = cells1.begin();

  while (cellIt != cells1.end() )
    {
    std::set<unsigned long>::iterator found2 = std::find(cells2.begin(), cells2.end(), *cellIt);
    std::set<unsigned long>::iterator found3 = std::find(cells3.begin(), cells3.end(), *cellIt);

    if ( found2 != cells2.end() && found3 != cells3.end() )
      {
      break;
      }
    cellIt++;
    }

  if (cellIt == cells1.end() )
    {
    itkExceptionMacro(<<"Cell was not found, altough it should be there");
    }

  return *cellIt;
}

/* PrintSelf. */
template <typename TInputMesh, typename TOutputMesh>
void
SimplexMeshToTriangleMeshFilter<TInputMesh, TOutputMesh>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);
  os << indent << "ToDo: implement PrinSelf!!!";    
}


} // end of namspace itk

#endif //__SimplexMeshToTriangleMeshFilter_txx
