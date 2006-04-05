/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - T.Feuvrier
  Language  :   C++
  Date      :   4 avril 2005
  Version   :   
  Role      :   Classe permettant de g�rer l'affichage d'une image dans un fenetre
  $Id$

=========================================================================*/
#ifndef otbGLVectorImageView_h
#define otbGLVectorImageView_h

#include <FL/gl.h>
#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Gl_Window.H>
#include <FL/Fl_Double_Window.H>

#include "itkColorTable.h"
#include "itkVectorImage.h"

#include "otbGLVectorImageViewBase.h"

namespace otb
{
  
/**
* GLImageView : Derived from abstract class ImageView and Fl_Gl_Window
* See ImageView.h for details...
  **/
template <class TPixel, class OverlayPixelType>
class ITK_EXPORT GLVectorImageView :  public GLVectorImageViewBase<TPixel,OverlayPixelType>
                                
{
public:
    typedef GLVectorImageView                                   Self;
    typedef GLVectorImageViewBase<TPixel,OverlayPixelType>      Superclass;
    typedef itk::SmartPointer<Self>                             Pointer;
    typedef itk::SmartPointer<const Self>                       ConstPointer;

    /** Method for creation through the object factory. */
    itkNewMacro(Self);

    /** Run-time type information (and related methods). */
    itkTypeMacro(GLVectorImageView,GLVectorImageViewBase);

  
  typedef typename Superclass::ImageType                ImageType;
  typedef typename Superclass::ImagePointer             ImagePointer;
  typedef typename Superclass::ImageConstPointer        ImageConstPointer;
  typedef typename Superclass::RegionType               RegionType;
  typedef typename Superclass::SizeType                 SizeType;
  typedef typename Superclass::IndexType                IndexType;
  typedef typename Superclass::PixelType                PixelType;

  itkSetMacro(CenterPointImage,IndexType);
  itkGetConstReferenceMacro(CenterPointImage,IndexType);
  itkSetMacro(DrawViewRectangle,RegionType);
  itkGetConstReferenceMacro(DrawViewRectangle,RegionType);

protected:
  
/*! FLTK required constructor - must use imData() to complete 
  definition */
  GLVectorImageView();
  /*! Standard destructor */
  virtual ~GLVectorImageView(void);

  IndexType m_CenterPointImage;
  
public:

  virtual IndexType     GetCenterRegion( const RegionType & zone );
  static  RegionType    ShrinkRegion( const RegionType & zone, const float shrinkfactor );
  virtual RegionType    GetViewRegion( const IndexType & centerPointImage );
  virtual RegionType    GetViewRegion( const RegionType & region, const IndexType & centerPointImage );
  virtual RegionType    TranslateImageRegionToWindowRegion( const RegionType & imageRegion, const  RegionType & sousimageRegion, const int windowWidth, const int windowHeight);

  /** Update the view region, with the new zoom and new Window sizes */
  virtual void          MajViewRegion(void);
  virtual void          PrintInfos(void);
  virtual void          BuildWithImageRegion();
  virtual void          BuildWithWindowRegion(const int zoom);
  
  virtual IndexType  WindowCoord2ImageCoord( const IndexType & index )const;
  virtual SizeType   WindowSize2ImageSize( const SizeType & size )const;
  virtual RegionType WindowZone2ImageRegion( const RegionType & zone )const;

  virtual void Init(int x, int y, int w, int h, const char *l);

  virtual void FinalizeInitialisation(void);
  
  virtual void size(int w, int h);
  virtual void resize(int x, int y, int w, int h);
  
  virtual void update();
  virtual void draw();
//  virtual int  handle(int event);
  virtual void SetDoubleWindow( Fl_Double_Window * pflDoubleWindow)
  {
        m_flDoubleWindow = pflDoubleWindow;
  }
  protected : 
  bool                  m_ViewRectangle;
  RegionType            m_DrawViewRectangle;
  Fl_Double_Window *    m_flDoubleWindow;
};

} //namespace

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbGLVectorImageView.txx"
#endif


#endif

