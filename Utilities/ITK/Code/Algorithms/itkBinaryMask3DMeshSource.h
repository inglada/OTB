/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkBinaryMask3DMeshSource.h,v $
  Language:  C++
  Date:      $Date: 2009-04-23 03:53:34 $
  Version:   $Revision: 1.18 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkBinaryMask3DMeshSource_h
#define __itkBinaryMask3DMeshSource_h

#include "vnl/vnl_matrix_fixed.h"
#include "itkMesh.h"
#include "itkImageToMeshFilter.h"
#include "itkVector.h"
#include "itkCellInterface.h"
#include "itkTriangleCell.h"
#include "itkCovariantVector.h"
#include "itkDefaultStaticMeshTraits.h"
#include "itkImageRegionConstIterator.h"

namespace itk
{

/** \class BinaryMask3DMeshSource
 * 
 * 
 * \par
 * This class tries to construct a 3D mesh surface based on a binary mask.
 * It can be used to integrate a region-based segmentation method and a deformable 
 * model into one hybrid framework.
 *
 * \par
 * To construct a mesh, we need to construct elements in a voxel and combine
 * those elements later to form the final mesh. Before go through every voxel
 * in the 3D volume, we first construct 2 look up tables. The index of these 2
 * tables are the on-off combination of the 8 nodes that form the voxel. So
 * both of these tables has the size of \$2^8\$ bytes. According to previous
 * work, all those \$2^8\$ combination of the nodes can be grouped into 16
 * final combinations. In the first table, we record the final combination that
 * can be transformed from the current combination. The entries of the second
 * table are made up of the transforming sequence that is necessary for the
 * current combination transform to one of the final combinations.
 *
 * \par
 * We then go through the 3D volume voxel by voxel, using those two tables we have defined
 * to construct elements within each voxel. We then merge all these mesh elements into 
 * one 3D mesh.
 * 
 * \par PARAMETERS
 * The ObjectValue parameter is used to identify the object. In most applications,
 * pixels in the object region are assigned to "1", so the default value of ObjectValue is
 * set to "1"
 *
 * \par REFERENCE
 * W. Lorensen and H. Cline, "Marching Cubes: A High Resolution 3D Surface Construction Algorithm", 
 * Computer Graphics 21, pp. 163-169, 1987. 
 * 
 * \par INPUT
 * The input should be a 3D binary image. 
 *
 *
 *  */
#ifndef ITK_EXPLICIT_INSTANTIATION
//This is already explicitly defined when ITK_EXPLICIT_INSTANTIATION is used.
template class Image<unsigned short,3>;
#endif

template <class TInputImage, class TOutputMesh>
class ITK_EXPORT BinaryMask3DMeshSource : public ImageToMeshFilter< TInputImage, TOutputMesh >
{
public:
  /** Standard "Self" typedef. */
  typedef BinaryMask3DMeshSource                        Self;
  typedef ImageToMeshFilter< TInputImage, TOutputMesh > Superclass;
  typedef SmartPointer<Self>                            Pointer;
  typedef SmartPointer<const Self>                      ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);  

  /** Run-time type information (and related methods). */
  itkTypeMacro(BinaryMask3DMeshSource, ImageToMeshFilter);

  /** Hold on to the type information specified by the template parameters. */
  typedef TOutputMesh                           OutputMeshType;
  typedef typename OutputMeshType::MeshTraits   OMeshTraits;
  typedef typename OutputMeshType::PointType    OPointType;
  typedef typename OMeshTraits::PixelType       OPixelType;  

  /** Some convenient typedefs. */
  typedef typename OutputMeshType::Pointer                OutputMeshPointer;
  typedef typename OutputMeshType::CellTraits             CellTraits;
  typedef typename OutputMeshType::PointsContainerPointer PointsContainerPointer;
  typedef typename OutputMeshType::PointsContainer        PointsContainer;
  typedef typename OutputMeshType::CellsContainerPointer  CellsContainerPointer;
  typedef typename OutputMeshType::CellsContainer         CellsContainer;
  typedef CovariantVector<double, 2>                      doubleVector;
  typedef CovariantVector<int, 2>                         intVector;
  
  /** Define the triangular cell types which forms the surface of the model
   * and will be used in FEM application. */
  typedef CellInterface<OPixelType, CellTraits>  TCellInterface;
  typedef TriangleCell<TCellInterface>           TriCell;
  typedef typename TriCell::SelfAutoPointer      TriCellAutoPointer;

  /** Input Image Type Definition. */
  typedef TInputImage                              InputImageType;
  typedef typename InputImageType::Pointer         InputImagePointer;
  typedef typename InputImageType::ConstPointer    InputImageConstPointer;
  typedef typename InputImageType::PixelType       InputPixelType;
  typedef typename InputImageType::SpacingType     SpacingType;
  typedef typename InputImageType::PointType       OriginType;
     
  /** Type definition for the classified image index type. */
  typedef typename InputImageType::IndexType       InputImageIndexType;

  typedef ImageRegionConstIterator< InputImageType > InputImageIterator;
  
  itkSetMacro(ObjectValue, InputPixelType);

  itkGetConstMacro(NumberOfNodes, unsigned long);
  itkGetConstMacro(NumberOfCells, unsigned long);

  /** accept the input image */
  virtual void SetInput( const InputImageType * inputImage );


protected:
  BinaryMask3DMeshSource();
  ~BinaryMask3DMeshSource();
  void PrintSelf(std::ostream& os, Indent indent) const;

  void GenerateData();
  virtual void GenerateOutputInformation(){}; // do nothing
  
private:
  BinaryMask3DMeshSource(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  typedef typename InputImageType::SizeType InputImageSizeType;

  void CreateMesh();
  void XFlip ( unsigned char *tp );  // 7 kinds of transformation
  void YFlip ( unsigned char *tp );
  void ZFlip ( unsigned char *tp );
  void XRotation ( unsigned char *tp );
  void YRotation ( unsigned char *tp );
  void ZRotation ( unsigned char *tp );
  void inverse ( unsigned char *tp );
  void InitializeLUT(); // initialize the look up table before the mesh construction
  void AddCells( unsigned char celltype, unsigned char celltran, int index );
  void AddNodes( int index, unsigned char *nodesid, unsigned long *globalnodesid, unsigned long **currentrowtmp, unsigned long **currentframetmp );
  void CellTransfer( unsigned char *nodesid, unsigned char celltran );
  unsigned long SearchThroughLastRow( int index, int start, int end );
  unsigned long SearchThroughLastFrame( int index, int start, int end );

  unsigned char m_LUT[256][2]; // the two lookup tables

  unsigned long m_LastVoxel[14];
  unsigned long m_CurrentVoxel[14];

  unsigned long **m_LastRow;
  unsigned long **m_LastFrame;
  unsigned long **m_CurrentRow;
  unsigned long **m_CurrentFrame;

  unsigned short m_CurrentRowIndex;
  unsigned short m_CurrentFrameIndex;
  unsigned short m_LastRowNum;
  unsigned short m_LastFrameNum;
  unsigned short m_CurrentRowNum;
  unsigned short m_CurrentFrameNum;
  unsigned char  m_AvailableNodes[14];

  double m_LocationOffset[14][3];

  unsigned long m_NumberOfNodes;
  unsigned long m_NumberOfCells;

  int m_NodeLimit;
  int m_CellLimit;
  int m_ImageWidth;
  int m_ImageHeight;
  int m_ImageDepth;
  int m_ColFlag;
  int m_RowFlag;
  int m_FrameFlag;
  int m_LastRowIndex;
  int m_LastVoxelIndex;
  int m_LastFrameIndex;

  unsigned char  m_PointFound;
  InputPixelType m_ObjectValue;
};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkBinaryMask3DMeshSource.txx"
#endif

#endif
