#ifndef METAVesselTube_H
#define METAVesselTube_H

#include <metaTypes.h>
#include <metaUtils.h>
#include <metaObject.h>

#include <list>


/*!    MetaVesselTube (.h and .cpp)
 *
 * Description:
 *    Reads and Writes MetaVesselTubeFiles.
 *
 * \author Julien Jomier
 * 
 * \date May 22, 2002
 */

class VesselTubePnt
{
public:

  VesselTubePnt(int dim)
  { 
    m_Dim = dim;
    m_X = new float[m_Dim];
    m_T = new float[m_Dim];
    m_V1= new float[m_Dim];
    m_V2= new float[m_Dim];
    for(unsigned int i=0;i<m_Dim;i++)
    {
      m_X[i] = 0;
      m_V1[i]= 0;
      m_V2[i]= 0;
      m_T[i]= 0;
    }
    m_Alpha1=0;
    m_Alpha2=0;
    m_Alpha3=0;
    m_R=0;
    m_Medialness=0;
    m_Ridgeness=0;
    m_Branchness=0;
    m_Mark=false;
    
    //Color is red by default
    m_Color[0]=1.0;
    m_Color[1]=0.0;
    m_Color[2]=0.0;
    m_Color[3]=1.0;
    m_ID = -1;
  }

  ~VesselTubePnt()
  {
    delete []m_X;
    delete []m_V1;
    delete []m_V2;
    delete []m_T;
  };
  
  unsigned int m_Dim;
  float* m_V1;
  float* m_V2;
  float* m_X;
  float* m_T;
  float m_Alpha1;
  float m_Alpha2;
  float m_Alpha3;
  float m_R;
  float m_Medialness;
  float m_Ridgeness;
  float m_Branchness;
  bool  m_Mark;
  float m_Color[4];
  int   m_ID;
};




class MetaVesselTube : public MetaObject
  {

  /////
  //
  // PUBLIC
  //
  ////
  public:

   typedef std::list<VesselTubePnt*> PointListType;
    ////
    //
    // Constructors & Destructor
    //
    ////
    MetaVesselTube(void);

    MetaVesselTube(const char *_headerName);   

    MetaVesselTube(const MetaVesselTube *_VesselTube); 
    
    MetaVesselTube(unsigned int dim);

    ~MetaVesselTube(void);

    void PrintInfo(void) const;

    void CopyInfo(const MetaVesselTube * _VesselTube);

    //    NPoints(...)
    //       Required Field
    //       Number of points wich compose the VesselTube
    void  NPoints(int npnt);
    int   NPoints(void) const;

    //    PointDim(...)
    //       Required Field
    //       Definition of points
    void        PointDim(const char* pointDim);
    const char* PointDim(void) const;

    //    Root(...)
    //       Optional Field
    //       Set if this VesselTube is a root
    void  Root(bool root);
    bool  Root(void) const;

    //    Artery(...)
    //       Optional Field
    //       Set if this VesselTube is a root
    void  Artery(bool artery);
    bool  Artery(void) const;


    //    ParentPoint(...)
    //       Optional Field
    //       Set the point number of the parent VesselTube where the branch occurs
    void  ParentPoint(int parentpoint);
    int   ParentPoint(void) const;

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

    bool  m_ElementByteOrderMSB;

    void  M_Destroy(void);

    void  M_SetupReadFields(void);

    void  M_SetupWriteFields(void);

    bool  M_Read(void);

    bool  M_Write(void);

    int   m_ParentPoint;  // "ParentPoint = "     -1

    bool  m_Root;         // "Root = "            false

    bool  m_Artery;         // "Artery = "            true

    int m_NPoints;      // "NPoints = "         0

    char m_PointDim[255]; // "PointDim = "       "x y z r"

    PointListType m_PointList;
    MET_ValueEnumType m_ElementType;
  };


#endif
