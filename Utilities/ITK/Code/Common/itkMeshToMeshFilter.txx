/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkMeshToMeshFilter.txx,v $
  Language:  C++
  Date:      $Date: 2009-09-17 11:14:57 $
  Version:   $Revision: 1.21 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

  Portions of this code are covered under the VTK copyright.
  See VTKCopyright.txt or http://www.kitware.com/VTKCopyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkMeshToMeshFilter_txx
#define __itkMeshToMeshFilter_txx

#include "itkMeshToMeshFilter.h"


namespace itk
{
  
/**
 *
 */
template <class TInputMesh, class TOutputMesh>
MeshToMeshFilter<TInputMesh,TOutputMesh>
::MeshToMeshFilter()
{
  // Modify superclass default values, can be overridden by subclasses
  this->SetNumberOfRequiredInputs(1);

}


/**
 *
 */
template <class TInputMesh, class TOutputMesh>
void 
MeshToMeshFilter<TInputMesh,TOutputMesh>
::SetInput(const TInputMesh *input)
{
  // Process object is not const-correct so the const_cast is required here
  this->ProcessObject::SetNthInput(0,
                                   const_cast< TInputMesh * >( input ) );
}


/**
 *
 */
template <class TInputMesh, class TOutputMesh>
const typename MeshToMeshFilter<TInputMesh,TOutputMesh>::InputMeshType *
MeshToMeshFilter<TInputMesh,TOutputMesh>
::GetInput() const
{
  if (this->GetNumberOfInputs() < 1)
    {
    return 0;
    }
  
  return static_cast< const TInputMesh * >
    (this->ProcessObject::GetInput(0));
}

  
/**
 *
 */
template <class TInputMesh, class TOutputMesh>
const typename MeshToMeshFilter<TInputMesh,TOutputMesh>::InputMeshType *
MeshToMeshFilter<TInputMesh,TOutputMesh>
::GetInput(unsigned int idx) const
{
  return dynamic_cast<const TInputMesh*>
    (this->ProcessObject::GetInput(idx));
}

 
template <class TInputMesh, class TOutputMesh>
void 
MeshToMeshFilter<TInputMesh,TOutputMesh>
::CopyInputMeshToOutputMeshPoints(void) 
{
  const InputMeshType * inputMesh   =  this->GetInput();
  OutputMeshPointer    outputMesh   =  this->GetOutput();

  typedef typename TOutputMesh::PointsContainer OutputPointsContainer;
  typedef typename TInputMesh::PointsContainer  InputPointsContainer;

  typename OutputPointsContainer::Pointer outputPoints = OutputPointsContainer::New();
  const InputPointsContainer * inputPoints = inputMesh->GetPoints();

  if( inputPoints )
    {
    outputPoints->Reserve( inputPoints->Size() );

    typename InputPointsContainer::ConstIterator inputItr = inputPoints->Begin();
    typename InputPointsContainer::ConstIterator inputEnd = inputPoints->End();

    typename OutputPointsContainer::Iterator outputItr = outputPoints->Begin();

    while( inputItr != inputEnd )
      {
      outputItr.Value() = inputItr.Value();
      ++inputItr;
      ++outputItr;
      }

    outputMesh->SetPoints( outputPoints );
    }
}


template <class TInputMesh, class TOutputMesh>
void 
MeshToMeshFilter<TInputMesh,TOutputMesh>
::CopyInputMeshToOutputMeshPointData(void) 
{
  const InputMeshType * inputMesh   =  this->GetInput();
  OutputMeshPointer    outputMesh   =  this->GetOutput();

  typedef typename TOutputMesh::PointDataContainer OutputPointDataContainer;
  typedef typename TInputMesh::PointDataContainer  InputPointDataContainer;

  typename OutputPointDataContainer::Pointer outputPointData = OutputPointDataContainer::New();
  const InputPointDataContainer * inputPointData = inputMesh->GetPointData();

  if( inputPointData )
    {
    outputPointData->Reserve( inputPointData->Size() );

    typename InputPointDataContainer::ConstIterator inputItr = inputPointData->Begin();
    typename InputPointDataContainer::ConstIterator inputEnd = inputPointData->End();

    typename OutputPointDataContainer::Iterator outputItr = outputPointData->Begin();

    while( inputItr != inputEnd )
      {
      outputItr.Value() = inputItr.Value();
      ++inputItr;
      ++outputItr;
      }

    outputMesh->SetPointData( outputPointData );
    }
}

 
template <class TInputMesh, class TOutputMesh>
void 
MeshToMeshFilter<TInputMesh,TOutputMesh>
::CopyInputMeshToOutputMeshCellLinks(void) 
{
  const InputMeshType * inputMesh   =  this->GetInput();
  OutputMeshPointer    outputMesh   =  this->GetOutput();

  typedef typename TOutputMesh::CellLinksContainer OutputCellLinksContainer;
  typedef typename TInputMesh::CellLinksContainer  InputCellLinksContainer;

  typename OutputCellLinksContainer::Pointer outputCellLinks = OutputCellLinksContainer::New();
  const InputCellLinksContainer * inputCellLinks = inputMesh->GetCellLinks();

  if( inputCellLinks )
    {
    outputCellLinks->Reserve( inputCellLinks->Size() );

    typename InputCellLinksContainer::ConstIterator inputItr = inputCellLinks->Begin();
    typename InputCellLinksContainer::ConstIterator inputEnd = inputCellLinks->End();

    typename OutputCellLinksContainer::Iterator outputItr = outputCellLinks->Begin();

    while( inputItr != inputEnd )
      {
      outputItr.Value() = inputItr.Value();
      ++inputItr;
      ++outputItr;
      }

    outputMesh->SetCellLinks( outputCellLinks );
    }
}

 
template <class TInputMesh, class TOutputMesh>
void 
MeshToMeshFilter<TInputMesh,TOutputMesh>
::CopyInputMeshToOutputMeshCells(void) 
{
  const InputMeshType * inputMesh   =  this->GetInput();
  OutputMeshPointer    outputMesh   =  this->GetOutput();

  typedef typename TOutputMesh::CellsContainer   OutputCellsContainer;
  typedef typename TInputMesh::CellsContainer    InputCellsContainer;
  typedef typename TOutputMesh::CellAutoPointer  CellAutoPointer;

  outputMesh->SetCellsAllocationMethod( OutputMeshType::CellsAllocatedDynamicallyCellByCell );

  typename OutputCellsContainer::Pointer outputCells = OutputCellsContainer::New();
  const InputCellsContainer * inputCells = inputMesh->GetCells();

  if( inputCells )
    {
    outputCells->Reserve( inputCells->Size() );

    typename InputCellsContainer::ConstIterator inputItr = inputCells->Begin();
    typename InputCellsContainer::ConstIterator inputEnd = inputCells->End();

    typename OutputCellsContainer::Iterator outputItr = outputCells->Begin();

    CellAutoPointer clone;

    while( inputItr != inputEnd )
      {
//      outputItr.Value() = inputItr.Value();
      // BUG: FIXME: Here we are copying a pointer, which is a mistake. What we should do is to clone the cell.
      inputItr.Value()->MakeCopy( clone );
      outputItr.Value() = clone.ReleaseOwnership();

      ++inputItr;
      ++outputItr;
      }

    outputMesh->SetCells( outputCells );
    }
}

 
template <class TInputMesh, class TOutputMesh>
void 
MeshToMeshFilter<TInputMesh,TOutputMesh>
::CopyInputMeshToOutputMeshCellData(void) 
{
  const InputMeshType * inputMesh   =  this->GetInput();
  OutputMeshPointer    outputMesh   =  this->GetOutput();

  typedef typename TOutputMesh::CellDataContainer OutputCellDataContainer;
  typedef typename TInputMesh::CellDataContainer  InputCellDataContainer;

  typename OutputCellDataContainer::Pointer outputCellData = OutputCellDataContainer::New();
  const InputCellDataContainer * inputCellData = inputMesh->GetCellData();

  if( inputCellData )
    {
    outputCellData->Reserve( inputCellData->Size() );

    typename InputCellDataContainer::ConstIterator inputItr = inputCellData->Begin();
    typename InputCellDataContainer::ConstIterator inputEnd = inputCellData->End();

    typename OutputCellDataContainer::Iterator outputItr = outputCellData->Begin();

    while( inputItr != inputEnd )
      {
      outputItr.Value() = inputItr.Value();
      ++inputItr;
      ++outputItr;
      }

    outputMesh->SetCellData( outputCellData );
    }
}


} // end namespace itk

#endif
