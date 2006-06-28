#ifndef METAMESH_H
#define METAMESH_H

#include "metaTypes.h"
#include "metaUtils.h"
#include "metaObject.h"

#include <list>


/*!    MetaMesh (.h and .cxx)
 *
 * Description:
 *    Reads and Writes MetaMeshFiles.
 *
 * \author Julien Jomier
 * 
 * \date June, 2004
 * 
 * Depends on:
 *    MetaUtils.h
 */


/** Typedef for the type of cells */
#define MET_NUM_CELL_TYPES 9

enum MET_CellGeometry {MET_VERTEX_CELL=0, MET_LINE_CELL, MET_TRIANGLE_CELL, MET_QUADRILATERAL_CELL, 
        MET_POLYGON_CELL, MET_TETRAHEDRON_CELL, MET_HEXAHEDRON_CELL, 
        MET_QUADRATIC_EDGE_CELL, MET_QUADRATIC_TRIANGLE_CELL};

const unsigned char MET_CellSize[MET_NUM_VALUE_TYPES] = {
  1,2,3,4,5,4,8,3,6};

const char MET_CellTypeName[MET_NUM_VALUE_TYPES][4] = {
   {'V','R','T','\0'},
   {'L','N','E','\0'},
   {'T','R','I','\0'},
   {'Q','A','D','\0'},
   {'P','L','Y','\0'},
   {'T','E','T','\0'},
   {'H','E','X','\0'},   
   {'Q','E','D','\0'}, 
   {'Q','T','R','\0'}};


/** Define a mesh point */
class MeshPoint
{
public:

  MeshPoint(int dim)
  { 
    m_Dim = dim;
    m_X = new float[m_Dim];
    for(unsigned int i=0;i<m_Dim;i++)
    {
      m_X[i] = 0;
    }
  }
  ~MeshPoint()
  { 
    delete []m_X;
  };
  
  unsigned int m_Dim;
  float* m_X;
  int m_Id;
};


/** Define a mesh cell 
 *  a cell contains a list of Ids corresponding to the list 
 *  of points */
class MeshCell
{
public:

  MeshCell(int dim)
  { 
    m_Dim = dim;
    m_Id = -1;
    m_PointsId = new int[m_Dim];
    for(unsigned int i=0;i<m_Dim;i++)
    {
      m_PointsId[i] = -1;
    }

  }
  ~MeshCell()
  { 
    delete []m_PointsId;
  };
  
  int m_Id;
  unsigned int m_Dim;
  int* m_PointsId;
};

/** Define a mesh cell links
 *  a celllink contains a list of Ids corresponding to the list 
 *  of links cells */
class MeshCellLink
{
public:

  MeshCellLink()
    { 
    m_Id = 0;
    }
  ~MeshCellLink()
    { 
    };

  int m_Id; // id of the cell link
  std::list<int> m_Links;
};

/** Define a mesh point data */
class MeshDataBase
{
public:

  MeshDataBase()
    { 
    m_Id = -1;
    }
  virtual ~MeshDataBase()
    { 
    };
  
  virtual void Write( std::ofstream* stream) = 0;
  virtual unsigned int GetSize(void) = 0;
  virtual MET_ValueEnumType GetMetaType() = 0;
  int m_Id;

protected:

  std::ifstream* m_ReadStream;
  std::ofstream* m_WriteStream;

};

/** Mesh point data class for basic types (i.e int, float ... ) */
template<typename TElementType>
class MeshData : public MeshDataBase
{
public:

  MeshData() {m_Id=-1;}
  ~MeshData() {};

  virtual MET_ValueEnumType GetMetaType()
    {
    return MET_GetPixelType(typeid(TElementType));
    }

  virtual void Write( std::ofstream* stream)
    {
    char* id = new char[sizeof(int)];
    MET_DoubleToValue((double)m_Id,MET_INT,id,0);
    stream->write((char *)id,sizeof(int));
    delete [] id;
    char* data = new char[sizeof(m_Data)];
    MET_DoubleToValue((double)m_Data,GetMetaType(),data,0);
    stream->write((char *)data,sizeof(m_Data));
    delete []data;
    }

  virtual unsigned int GetSize(void)
    {
     unsigned int size = sizeof(int);
     size += sizeof(m_Data);
     return size;
    }

  TElementType m_Data;
};


class MetaMesh : public MetaObject
  {

  /////
  //
  // PUBLIC
  //
  ////
  public:

   typedef std::list<MeshPoint*> PointListType;
   typedef std::list<MeshCell*>  CellListType;
   typedef std::list<MeshCellLink*>  CellLinkListType;
   typedef std::list<MeshDataBase*> PointDataListType;
   typedef std::list<MeshDataBase*> CellDataListType;

    ////
    //
    // Constructors & Destructor
    //
    ////
    MetaMesh(void);

    MetaMesh(const char *_headerName);   

    MetaMesh(const MetaMesh *_tube); 
    
    MetaMesh(unsigned int dim);

    ~MetaMesh(void);

    void PrintInfo(void) const;

    void CopyInfo(const MetaMesh * _mesh);

    //    NPoints(...)
    //       Required Field
    //       Number of points wich compose the mesh
    int   NPoints(void) const;

    //    NCells(...)
    //       Required Field
    //       Number of cells wich compose the mesh
    int   NCells(void) const;

    //    NCellLinks(...)
    //       Required Field
    //       Number of cellLinks wich compose the mesh
    int   NCellLinks(void) const;

    //    NCellTypes(...)
    //       Required Field
    //       Number of celles wich compose the mesh
    void  NCellTypes(int ncelltypes);
    int   NCellTypes(void) const;

    /** Clear the metaMesh */
    void  Clear(void);

    PointListType & GetPoints(void) {return m_PointList;}
    const PointListType & GetPoints(void) const  {return m_PointList;}
    
    CellListType & GetCells(MET_CellGeometry geom) {return *(m_CellListArray[geom]);}
    const CellListType & GetCells(MET_CellGeometry geom) const  {return *(m_CellListArray[geom]);}

    CellLinkListType & GetCellLinks(void) {return m_CellLinks;}
    const CellLinkListType & GetCellLinks(void) const  {return m_CellLinks;}

    PointDataListType & GetPointData(void) {return m_PointData;}
    const PointDataListType & GetPointData(void) const  {return m_PointData;}
    
    CellDataListType & GetCellData(void) {return m_CellData;}
    const CellDataListType & GetCellData(void) const  {return m_CellData;}

    MET_ValueEnumType PointDataType(void) const {return m_PointDataType;}
    void  PointDataType(MET_ValueEnumType _elementType){m_PointDataType = _elementType;}

    MET_ValueEnumType CellDataType(void) const {return m_CellDataType;}
    void  CellDataType(MET_ValueEnumType _elementType){m_CellDataType = _elementType;}

  ////
  //
  // PROTECTED
  //
  ////
  protected:

    bool  m_ElementByteOrderMSB;

    void  M_Destroy(void);

    void  M_SetupReadFields(void);

    void  M_SetupWriteFields(void);

    bool  M_Read(void);

    bool  M_Write(void);

    int m_NPoints;   
    int m_NCells; 
    int m_NCellLinks;
    int m_NPointData;
    int m_NCellData;
    char m_PointDim[255]; // "PointDim = "       "x y z r"

    PointListType m_PointList;
   
    // We store the Cell lists in a vector
    CellListType* m_CellListArray[MET_NUM_CELL_TYPES];
    CellLinkListType m_CellLinks;
    PointDataListType m_PointData;
    CellDataListType m_CellData;

    MET_ValueEnumType m_PointType;
    MET_ValueEnumType m_PointDataType;
    MET_ValueEnumType m_CellDataType;

  };


#endif
