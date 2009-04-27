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
#ifndef __otbPackedWidgetManager_cxx
#define __otbPackedWidgetManager_cxx

#include "otbPackedWidgetManager.h"



namespace otb
{

PackedWidgetManager
::PackedWidgetManager() 
{
  m_PackedWidget = new ImageWidgetPackedManagerType();
}

PackedWidgetManager
::~PackedWidgetManager() 
{
  delete m_PackedWidget;
}


void 
PackedWidgetManager
::RegisterFullWidget(WidgetType* fullWidget)
{
  m_InternalFullWidget = fullWidget;
  m_PackedWidget->m_FullGroup->add(fullWidget);
  m_PackedWidget->m_FullGroup->resizable(fullWidget);
  fullWidget->resize(m_PackedWidget->m_FullGroup->x(),m_PackedWidget->m_FullGroup->y(),
                     m_PackedWidget->m_FullGroup->w(),m_PackedWidget->m_FullGroup->h());
}


void 
PackedWidgetManager
::RegisterScrollWidget(WidgetType* scrollWidget) 
{
  m_InternalScrollWidget = scrollWidget;
  m_PackedWidget->m_QuicklookGroup->add(scrollWidget);
  m_PackedWidget->m_QuicklookGroup->resizable(scrollWidget);
  scrollWidget->resize(m_PackedWidget->m_QuicklookGroup->x(),m_PackedWidget->m_QuicklookGroup->y(),
                       m_PackedWidget->m_QuicklookGroup->w(),m_PackedWidget->m_QuicklookGroup->h());

}

void 
PackedWidgetManager
::RegisterZoomWidget(WidgetType* zoomWidget)
{
  m_InternalZoomWidget = zoomWidget;
  m_PackedWidget->m_ZoomGroup->add(zoomWidget);
  m_PackedWidget->m_ZoomGroup->resizable(zoomWidget);
  zoomWidget->resize(m_PackedWidget->m_ZoomGroup->x(),m_PackedWidget->m_ZoomGroup->y(),
                     m_PackedWidget->m_ZoomGroup->w(),m_PackedWidget->m_ZoomGroup->h());
}
void 
PackedWidgetManager
::RegisterPixelInformationWidget(Fl_Output * pixelWidget)
{
  m_IntenalPixelWidget = pixelWidget;
  m_PackedWidget->m_PixelInformationGroup->add(pixelWidget);
  m_PackedWidget->m_PixelInformationGroup->resizable(pixelWidget);
  pixelWidget->resize(m_PackedWidget->m_PixelInformationGroup->x(),m_PackedWidget->m_PixelInformationGroup->y(),
                      m_PackedWidget->m_PixelInformationGroup->w(),m_PackedWidget->m_PixelInformationGroup->h());

}

void 
PackedWidgetManager
::RegisterHistogramWidget(WidgetType* curveWidget)
{
  m_InternalCurveWidget = curveWidget;
  m_PackedWidget->m_HistogramsGroup->add(curveWidget);
  m_PackedWidget->m_HistogramsGroup->resizable(curveWidget);
 curveWidget->resize(m_PackedWidget->m_HistogramsGroup->x(),m_PackedWidget->m_HistogramsGroup->y(),
                                            m_PackedWidget->m_HistogramsGroup->w(),m_PackedWidget->m_HistogramsGroup->h());
 }

void 
PackedWidgetManager
::Show()
{
  m_PackedWidget->m_Window->show();
  m_InternalCurveWidget->show();
  m_IntenalPixelWidget->show();
  m_InternalFullWidget->show();
  m_InternalScrollWidget->show();
  m_InternalZoomWidget->show();
 }

void 
PackedWidgetManager
::Hide()
{
  m_PackedWidget->m_Window->hide();
}

void 
PackedWidgetManager
::Refresh()
{
 m_InternalScrollWidget ->redraw();
 m_InternalScrollWidget ->redraw();
 m_InternalZoomWidget->redraw();
 m_IntenalPixelWidget->redraw();
 m_InternalCurveWidget->redraw();
}

void 
PackedWidgetManager
::UnRegisterAll()
{
  m_PackedWidget->m_FullGroup->remove(m_InternalFullWidget);
  m_PackedWidget->m_ZoomGroup->remove(m_InternalZoomWidget);
  m_PackedWidget->m_PixelInformationGroup->remove(m_IntenalPixelWidget);
  m_PackedWidget->m_QuicklookGroup->remove(m_InternalScrollWidget);
  m_PackedWidget->m_HistogramsGroup->remove(m_InternalCurveWidget);
}

void 
PackedWidgetManager
::SetLabel(const char * label)
{
  m_PackedWidget->m_Window->copy_label(label);
}



}
#endif
