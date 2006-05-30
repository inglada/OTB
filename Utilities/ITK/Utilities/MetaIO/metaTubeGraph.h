#ifndef METATUBEGRAPH_H
#define METATUBEGRAPH_H

#include <metaTypes.h>
#include <metaUtils.h>
#include <metaObject.h>

#include <vector>


/*!    MetaTubeGraph (.h and .cpp)
 *
 * Description:
 *    Reads and Writes MetaTubeGraph Files.
 *
 * \author Julien Jomier
 * 
 * \date May 22, 2002
 */

class TubeGraphPnt
{
public:

  TubeGraphPnt(int dim)
  { 
    m_Dim = dim;
    m_GraphNode = -1;
    m_R = 0;
    m_P = 0;
    m_T = new float[m_Dim*m_Dim];
  }

  ~TubeGraphPnt()
  {
    delete m_T;
  };
  
  unsigned int m_Dim;
  int    m_GraphNode;
  float  m_R;
  float  m_P;
  float* m_T;
};




class MetaTubeGraph : public MetaObject
  {

  /////
  //
  // PUBLIC
  //
  ////
  public:

   typedef std::vector<TubeGraphPnt*> PointListType;
    ////
    //
    // Constructors & Destructor
    //
    ////
    MetaTubeGraph(void);

    MetaTubeGraph(const char *_headerName);   

    MetaTubeGraph(const MetaTubeGraph *_tube); 
    
    MetaTubeGraph(unsigned int dim);

    ~MetaTubeGraph(void);

    void PrintInfo(void) const;

    void CopyInfo(const MetaTubeGraph * _tube);

    //    NPoints(...)
    //       Required Field
    //       Number of points wich compose the tube
    void  NPoints(int npnt);
    int   NPoints(void) const;

    //    PointDim(...)
    //       Required Field
    //       Definition of points
    void        PointDim(const char* pointDim);
    const char* PointDim(void) const;

    //    Root(...)
    //       Optional Field
    //       Set if this tube is a root
    void  Root(int root);
    int   Root(void) const;


    void  Clear(void);

    PointListType &  GetPoints(void) {return m_PointList;}
    const PointListType &  GetPoints(void) const {return m_PointList;}
    
    MET_ValueEnumType ElementType(void) const;
    void  ElementType(MET_ValueEnumType _elementType);

  ////
  //
  // PROTECTED
  //
  ////
  protected:

    void  M_Destroy(void);

    void  M_SetupReadFields(void);

    void  M_SetupWriteFields(void);

    bool  M_Read(void);

    bool  M_Write(void);

    int m_Root;         // "Root = "            0

    int m_NPoints;      // "NPoints = "         0

    char m_PointDim[255]; // "PointDim = "       "x y z r"

    PointListType m_PointList;

    MET_ValueEnumType m_ElementType;
  };


#endif
