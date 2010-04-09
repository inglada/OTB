/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbImageViewerZoomWidget_h
#define __otbImageViewerZoomWidget_h

#include "otbZoomableImageWidget.h"
#include "otbImageWidgetRectangleForm.h"
#include "otbImageWidgetBoxForm.h"
#include "otbImageWidgetPointForm.h"
#include <FL/fl_draw.H>

namespace otb
{
template <class TPixel, class TLabel> class ImageViewerBase;
/**
 * \class ImageViewerZoomWidget
 *
 * \brief <b>DEPRECATED</b>: Custom scroll image widget.
 *
 * This class derives from otb::ZoomableImageWidget, and implements behaviours specific to the image viewer.
 *
 *  \deprecated use the new Visualization framework instead.
 * \sa StandardImageViewer
 *
 * \sa ImageViewer, ZoomableImageWidget
 *
 */
template <class TPixel, class TLabel>
class ITK_EXPORT ImageViewerZoomWidget
  : public ZoomableImageWidget<TPixel>
{
public:
  /** Standard class typedefs */
  typedef ImageViewerZoomWidget         Self;
  typedef ZoomableImageWidget<TPixel>   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(ImageViewerZoomWidget, FullResolutionImageWidget);

  typedef TPixel                         PixelType;
  typedef TLabel                         LabelType;
  typedef typename Superclass::IndexType IndexType;
  typedef typename Superclass::SizeType  SizeType;

  typedef ImageViewerBase<PixelType, LabelType> ParentType;
  typedef ParentType*                           ParentPointerType;

  itkSetMacro(Parent, ParentPointerType);
  itkGetMacro(Parent, ParentPointerType);
  /** Handle method */
  virtual int  handle(int event)
  {
    switch (event)
      {
    case FL_ENTER:
      {
      m_MouseIn = true;
      return 1;
      }
    case FL_LEAVE:
      {
      m_MouseIn = false;
      m_Parent->UpdateZoomWidget();
      m_Parent->ClearPixLocVal();
      return 1;
      }
    case FL_MOVE:
      {
      m_MouseIn = true;
      if (m_MouseMoveCount % m_ValueUpdateFrequency == 0)
        {
        m_MousePos[0] = Fl::event_x();
        m_MousePos[1] = Fl::event_y();
        IndexType newIndex = this->WindowToImageCoordinates(m_MousePos);
        m_Parent->ReportPixel(newIndex);
        m_MouseMoveCount = 0;
        }
      m_MouseMoveCount++;
      return 1;
      }
    case FL_MOUSEWHEEL:
      {
      int dy = Fl::event_dy();
      if (dy < 0)
        {
        this->SetZoomFactor(this->GetOpenGlIsotropicZoom() + m_ZoomStep);
        }
      else
        {
        if (this->GetOpenGlIsotropicZoom() - m_ZoomStep >= 1)
          {
          this->SetZoomFactor(this->GetOpenGlIsotropicZoom() - m_ZoomStep);
          }
        else
          {
          this->SetZoomFactor(1.0);
          }
        }
      m_Parent->Update();
      return 1;
      }
    case FL_FOCUS:
      {
      return 1;
      }
    case FL_UNFOCUS:
      {
      return 1;
      }
    case FL_KEYDOWN:
      {
      IndexType newIndex = this->GetViewedRegion().GetIndex();
      SizeType  newSize  = this->GetViewedRegion().GetSize();
      newIndex[0] = newIndex[0] + newSize[0] / 2;
      newIndex[1] = newIndex[1] + newSize[1] / 2;
      switch (Fl::event_key())
        {
      case FL_Down:
        {
        newIndex[1] = newIndex[1] + newSize[1] / 8;
        break;
        }
      case FL_Up:
        {
        newIndex[1] = newIndex[1] - newSize[1] / 8;
        break;
        }
      case FL_Left:
        {
        newIndex[0] = newIndex[0] - newSize[0] / 8;
        break;
        }
      case FL_Right:
        {
        newIndex[0] = newIndex[0] + newSize[0] / 8;
        break;
        }
        }
      m_Parent->ChangeZoomViewedRegion(newIndex);
      return 1;
      }
      }
    return 0;
  }

  virtual void resize(int x, int y, int w, int h)
  {
    Superclass::resize(x, y, w, h);
    if (m_Parent->GetBuilt()) m_Parent->Update();
  }

protected:
  /**
   * Constructor.
   */
  ImageViewerZoomWidget()
    {
    m_ZoomStep = 0.2;
    m_MouseIn = false;
    m_MousePos.Fill(0);
    m_MouseMoveCount = 0;
    m_ValueUpdateFrequency = 5;
    };
  /**
   * Destructor.
   */
  virtual ~ImageViewerZoomWidget()
    {
    m_Parent = NULL;
    }

private:
  ParentPointerType m_Parent;
  double            m_ZoomStep;
  IndexType         m_MousePos;
  bool              m_MouseIn;
  unsigned int      m_MouseMoveCount;
  unsigned int      m_ValueUpdateFrequency;

};

} // end namespace otb
#endif
