/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkVTKPolyDataWriter.h,v $
  Language:  C++
  Date:      $Date: 2007/02/01 14:57:50 $
  Version:   $Revision: 1.1 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkVTKPolyDataWriter_h
#define __itkVTKPolyDataWriter_h

#include "itkMesh.h"
#include "itkTriangleCell.h"

namespace itk
{
/** \class VTKPolyDataWriter
 * \brief
 * Writes an itkMesh to a file in VTK file format.
 *
 * Caveat: The input to itkVTKPolyDataWriter must be a triangle mesh.
 *         Use vtkTriangleFilter to convert your mesh to a triangle mesh.
 */
template <class TInputMesh>
class VTKPolyDataWriter : public Object
{
public:
  /** Standard "Self" typedef. */
  typedef VTKPolyDataWriter         Self;
  typedef Object                    Superclass;
  typedef SmartPointer<Self>        Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Method for creation through the object factory */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(VTKPolyDataWriter, Object);

  /** Write the Input mesh to the Output file.
   * Use either Update() or Write(). */
  void Update(void);
  void Write(void);

  /** Hold on to the type information specified by the template parameters.
   */
  typedef TInputMesh                          InputMeshType;
  typedef typename InputMeshType::PixelType   PixelType;
  typedef typename InputMeshType::PointType   PointType;
  typedef typename InputMeshType::CellType    CellType;

  /** Some convenient typedefs. */
  typedef typename InputMeshType::Pointer     InputMeshPointer;
  typedef typename InputMeshType::CellTraits  CellTraits;

  /** Define the triangular cell types which form the surface  */
  typedef CellInterface<PixelType, CellTraits> CellInterfaceType;
  typedef TriangleCell<CellInterfaceType>      TriangleCellType;

  typedef typename InputMeshType::PointsContainer    PointsContainer;
  typedef typename InputMeshType::CellsContainer     CellsContainer;

  typedef typename PointsContainer::ConstIterator    PointIterator;
  typedef typename CellsContainer::ConstIterator     CellIterator;

  typedef typename CellType::PointIdIterator         PointIdIterator;

  /** Set the Input */
  void SetInput(InputMeshType * input);

  /** Set/Get the name of the file where data are written. */
  itkSetStringMacro(FileName);
  itkGetStringMacro(FileName);

protected:
  VTKPolyDataWriter();
  virtual ~VTKPolyDataWriter();

  void GenerateData();

  std::string m_FileName;

  void PrintSelf(std::ostream& os, Indent indent) const;

  VTKPolyDataWriter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

private:
  InputMeshPointer m_Input;
};

} // namespace itk


#ifndef ITK_MANUAL_INSTANTIATION
#include "itkVTKPolyDataWriter.txx"
#endif

#endif
