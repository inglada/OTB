#ifndef METALANDMARK_H
#define METALANDMARK_H

#include <metaTypes.h>
#include <metaUtils.h>
#include <metaObject.h>

#include <list>


/*!    MetaLandmark (.h and .cxx)
 *
 * Description:
 *    Reads and Writes MetaLandmarkFiles.
 *
 * \author Julien Jomier
 * 
 * \date July 02, 2002
 * 
 * Depends on:
 *    MetaUtils.h
 *    MetaFileLib.h
 */

class LandmarkPnt
{
public:

  LandmarkPnt(int dim)
  { 
    m_Dim = dim;
    m_X = new float[m_Dim];
    for(unsigned int i=0;i<m_Dim;i++)
    {
      m_X[i] = 0;
    }
    
    //Color is red by default
    m_Color[0]=1.0;
    m_Color[1]=0.0;
    m_Color[2]=0.0;
    m_Color[3]=1.0;
  }
  ~LandmarkPnt()
  { 
    delete []m_X;
  };
  
  unsigned int m_Dim;
  float* m_X;
  float  m_Color[4];
};




class MetaLandmark : public MetaObject
  {

  /////
  //
  // PUBLIC
  //
  ////
  public:

   typedef std::list<LandmarkPnt*> PointListType;
    ////
    //
    // Constructors & Destructor
    //
    ////
    MetaLandmark(void);

    MetaLandmark(const char *_headerName);   

    MetaLandmark(const MetaLandmark *_tube); 
    
    MetaLandmark(unsigned int dim);

    ~MetaLandmark(void);

    void PrintInfo(void) const;

    void CopyInfo(const MetaLandmark * _tube);

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


    void  Clear(void);

    PointListType & GetPoints(void) {return m_PointList;}
    const PointListType & GetPoints(void) const  {return m_PointList;}
 
    MET_ValueEnumType ElementType(void) const;
    void  ElementType(MET_ValueEnumType _elementType);

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

    int m_NPoints;      // "NPoints = "         0

    char m_PointDim[255]; // "PointDim = "       "x y z r"

    PointListType m_PointList;

    MET_ValueEnumType m_ElementType;
  };


#endif
