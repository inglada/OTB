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
#ifndef __otbImageView_txx
#define __otbImageView_txx

#include "otbImageView.h"

namespace otb
{
template < class TViewerModel >
ImageView<TViewerModel>
::ImageView() : m_ScrollWidget(), m_FullWidget(), m_ZoomWidget(),
                m_Model(), m_Controller(), m_ExtractRegionGlComponent(), m_ScaledExtractRegionGlComponent()
{
  // Initializing the widgets
  m_ScrollWidget = ImageWidgetType::New();
  m_FullWidget   = ImageWidgetType::New();
  m_ZoomWidget   = ImageWidgetType::New();

  // Extract regions gl components
  m_ExtractRegionGlComponent = RegionGlComponentType::New();
  m_ScaledExtractRegionGlComponent = RegionGlComponentType::New();
  m_ExtractRegionGlComponent->SetVisible(false);
  m_ScaledExtractRegionGlComponent->SetVisible(false);
  m_ScrollWidget->AddGlComponent(m_ExtractRegionGlComponent);
  m_FullWidget->AddGlComponent(m_ScaledExtractRegionGlComponent);

  // Set the widget identifiers
  m_ScrollWidget->SetIdentifier("Scroll");
  m_FullWidget->SetIdentifier("Full");
  m_ZoomWidget->SetIdentifier("Zoom");
}

template < class TViewerModel >
ImageView<TViewerModel>
::~ImageView()
{
  
}

template < class TViewerModel >
void
ImageView<TViewerModel>
::SetModel(ModelType * model)
{
  // Unregister from previous model if nay
  if(m_Model.IsNotNull())
    {
    m_Model->UnRegisterListener(this);
    }
  
  // Set and register with new model
  m_Model = model;
  m_Model->RegisterListener(this);
}

template < class TViewerModel >
void
ImageView<TViewerModel>
::SetController(ControllerType * controller)
{
  m_Controller = controller;
  m_ScrollWidget->SetController(m_Controller);
  m_FullWidget->SetController(m_Controller);
  m_ZoomWidget->SetController(m_Controller);
}

template < class TViewerModel>
void
ImageView<TViewerModel>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  // Call the superclass implementation
  Superclass::PrintSelf(os,indent);
}


template < class TViewerModel >
void
ImageView<TViewerModel>
::Notify()
{
  this->Update();
}

template < class TViewerModel >
void
ImageView<TViewerModel>
::Update()
{
  this->UpdateScrollWidget();
  this->UpdateFullWidget();
  this->UpdateZoomWidget();
}

template < class TViewerModel >
void
ImageView<TViewerModel>
::UpdateScrollWidget()
{
  // If the model has a quicklook
  if(m_Model->GetHasQuicklook())
    {
      otbMsgDevMacro(<<"ImageView::UpdateScrollWidget(): redrawing scroll widget");

    // Read the buffer
    const RegionType qlRegion =  m_Model->GetRasterizedQuicklook()
      ->GetLargestPossibleRegion(); 
    m_ScrollWidget->ReadBuffer(m_Model->GetRasterizedQuicklook(),qlRegion);

    // Compute the appropriate scale
    const double wratio = static_cast<double>(m_ScrollWidget->w())/static_cast<double>(qlRegion.GetSize()[0]);
    const double hratio = static_cast<double>(m_ScrollWidget->h())/static_cast<double>(qlRegion.GetSize()[1]);
    const double scale = std::min(wratio,hratio);
    m_ScrollWidget->SetIsotropicZoom(scale);
    m_ScrollWidget->SetSubsamplingRate(m_Model->GetSubsamplingRate());

    // Setting widget label
    std::string label = m_ScrollWidget->GetIdentifier();
    label+=(" - ");
    label+=m_Model->GetName();
    m_ScrollWidget->label(label.c_str());

    // display the zoom rectangle if necessary
    if(m_Model->GetHasExtract())
      {
      m_ExtractRegionGlComponent->SetVisible(true);
      m_ExtractRegionGlComponent->SetRegion(m_Model->GetExtractRegion());
      }
    else
      {
      m_ExtractRegionGlComponent->SetVisible(false);
      }
    }

    // Redraw
    m_ScrollWidget->redraw();
}


template < class TViewerModel >
void
ImageView<TViewerModel>
::UpdateFullWidget()
{
  if(m_Model->GetHasExtract())
    { 
    otbMsgDevMacro(<<"ImageView::UpdateFullWidget(): redrawing full widget");
    m_FullWidget->ReadBuffer(m_Model->GetRasterizedExtract(),m_Model->GetRasterizedExtract()
                             ->GetLargestPossibleRegion());

   // Setting widget label
    std::string label = m_FullWidget->GetIdentifier();
    label+=(" - ");
    label+=m_Model->GetName();
    m_FullWidget->label(label.c_str());

    // display the zoom rectangle if necessary
    if(m_Model->GetHasScaledExtract())
      {
      m_ScaledExtractRegionGlComponent->SetVisible(true);
      m_ScaledExtractRegionGlComponent->SetRegion(m_Model->GetScaledExtractRegion());
      }
    else
      {
      m_ScaledExtractRegionGlComponent->SetVisible(false);
      }
    // redraw the widget
    m_FullWidget->redraw();
    }
 }

template < class TViewerModel >
void
ImageView<TViewerModel>
::UpdateZoomWidget()
{
  if(m_Model->GetHasScaledExtract())
    {
    otbMsgDevMacro(<<"ImageView::UpdateZoomWidget(): redrawing zoom widget");
    m_ZoomWidget->ReadBuffer(m_Model->GetRasterizedScaledExtract(),m_Model->GetRasterizedScaledExtract()
                             ->GetLargestPossibleRegion());

    // Setting widget label
    std::string label = m_ZoomWidget->GetIdentifier();
    label+=(" - ");
    label+=m_Model->GetName();
    m_ZoomWidget->label(label.c_str());

    m_ZoomWidget->redraw();
    }  
}


}
#endif
