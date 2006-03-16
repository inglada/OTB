#ifndef otbGLVectorImageViewBase_h
#define otbGLVectorImageViewBase_h

#include <FL/gl.h>
#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Gl_Window.H>

#include <vector>

#include "itkColorTable.h"
#include "itkImage.h"

#include "otbVectorImageView.h"

namespace otb
{
  
/**
* GLVectorImageViewBase : Derived from abstract class ImageView and Fl_Gl_Window
* See ImageView.h for details...
  **/
template <class TPixel, class OverlayPixelType>
class ITK_EXPORT GLVectorImageViewBase :        public VectorImageView<TPixel>, 
                                                public Fl_Gl_Window
{
public:
    typedef GLVectorImageViewBase               Self;
    typedef VectorImageView<TPixel>             Superclass;
    typedef itk::SmartPointer<Self>             Pointer;
    typedef itk::SmartPointer<const Self>       ConstPointer;

    /** Method for creation through the object factory. */
    itkNewMacro(Self);

    /** Run-time type information (and related methods). */
    itkTypeMacro(GLVectorImageViewBase,VectorImageView);

  
  typedef itk::Image<OverlayPixelType,3>   OverlayType;
  typedef typename OverlayType::Pointer    OverlayPointer;

  typedef typename Superclass::ImageType                ImageType;
  typedef typename Superclass::ImagePointer             ImagePointer;
  typedef typename Superclass::ImageConstPointer        ImageConstPointer;
  typedef typename Superclass::RegionType               RegionType;
  typedef typename Superclass::SizeType                 SizeType;
  typedef typename Superclass::IndexType                IndexType;
  typedef typename Superclass::PixelType                PixelType;

  typedef itk::ColorTable<float>                        ColorTableType;
  typedef typename ColorTableType::Pointer              ColorTablePointer;


  /** Typedef Liste des canaux */
  typedef typename std::vector<int> ChannelsType;
  
  typedef enum { GRAY_LEVEL = 1, RGB_LEVEL = 3 } ModeViewType;

  itkSetMacro(GrayLevelChannel,int);
  itkSetMacro(RedChannel,int);
  itkSetMacro(GreenChannel,int);
  itkSetMacro(BlueChannel,int);

  virtual void SetRGBChannels(int pRedChannel,int pGreenChannel, int pBlueChannel)
  {
        SetRedChannel(pRedChannel);
        SetGreenChannel(pGreenChannel);
        SetBlueChannel(pBlueChannel);
  }

  virtual void ClearSelectChannels(void);
  
protected:
  bool        cValidOverlayData;
  float       cOverlayOpacity;
  
  OverlayPointer cOverlayData;
  void     (* cViewOverlayCallBack)(void);
  
  unsigned char * cWinOverlayData;
  

  ColorTablePointer      cColorTable;

  unsigned int           cOverlayColorIndex;
  
/*! FLTK required constructor - must use imData() to complete 
  definition */
  GLVectorImageViewBase();
  /*! Standard destructor */
  virtual ~GLVectorImageViewBase(void);

  virtual void GenerateChannelsInformations(void);
  /** Liste des canaux qui seront r�ellement trait�s [1...] */
  ChannelsType  m_ChannelsWorks;
  
  /** Nombre de dimensions de l'image : = 1 si GRAY_LEVEL, sinon 3 si RGB_LEVEL */
  int m_NbDim;

  int  m_RedChannel;
  int  m_GreenChannel;
  int  m_BlueChannel;
  int  m_GrayLevelChannel;

  ModeViewType  m_ModeView;

public:

  /*! Display Overlay in Color 'c'. You must ensure that the color-table specified
   * contains color 'c'. For example with the default useDiscrete() color table,
   * SetOverlayColorIndex( 0 ) will display the overlay in red. 
   * SetOverlayColorIndex( 1 ) purple etc.... */
  void SetOverlayColorIndex( unsigned int c)
    {
    cOverlayColorIndex = c;
    }

};


} //namespace

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbGLVectorImageViewBase.txx"
#endif


#endif

