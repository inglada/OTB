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
#include "otbImageLayerRenderingModel.h"
#include "otbVectorImage.h"
#include "itkRGBAPixel.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbImageLayerGenerator.h"
#include "otbImageLayer.h"
#include "otbImageView.h"
#include <FL/Fl.H>
#include "otbImageWidgetController.h"
#include "otbWidgetResizingActionHandler.h"
#include "otbChangeScaledExtractRegionActionHandler.h"
#include "otbChangeExtractRegionActionHandler.h"
#include "otbChangeScaleActionHandler.h"
#include "itkSobelEdgeDetectionImageFilter.h"
#include "otbPerBandVectorImageFilter.h"
#include "otbPixelDescriptionModel.h"
#include "otbPixelDescriptionActionHandler.h"
#include "otbPixelDescriptionView.h"

int otbImageViewerEndToEndTwoLayers( int argc, char * argv[] )
{
  // params
  const char * infname = argv[1];
  const unsigned int scrollSize = atoi(argv[2]);
  const unsigned int fullSize = atoi(argv[3]);
  const unsigned int zoomSize = atoi(argv[4]);
  const double       run      = atoi(argv[5]);


  // typedefs 
  typedef double                                     PixelType;
  typedef itk::RGBAPixel<unsigned char>               RGBPixelType;
  typedef otb::Image<RGBPixelType,2>                 OutputImageType;
  typedef otb::VectorImage<PixelType,2>              ImageType;
  typedef otb::Image<PixelType,2>                    ScalarImageType;
  typedef otb::ImageLayer<ImageType, OutputImageType> LayerType;
  typedef otb::ImageFileReader<ImageType>            ReaderType;
  typedef otb::ImageLayerGenerator<LayerType>        LayerGeneratorType;
  typedef otb::ImageLayerRenderingModel<OutputImageType>     ModelType;
  typedef otb::ImageView<ModelType>                  ViewType;
  typedef otb::ImageWidgetController                 ControllerType;
  typedef otb::WidgetResizingActionHandler
    <ModelType,ViewType>                             ResizingHandlerType;
  typedef otb::ChangeScaledExtractRegionActionHandler
    <ModelType,ViewType>                             ChangeScaledRegionHandlerType;
  typedef otb::ChangeExtractRegionActionHandler
    <ModelType,ViewType>                             ChangeRegionHandlerType;
  typedef otb::ChangeScaleActionHandler
    <ModelType,ViewType>                             ChangeScaleHandlerType;
  typedef otb::PixelDescriptionModel<OutputImageType> PixelDescriptionModelType;
  typedef otb::PixelDescriptionActionHandler
    < PixelDescriptionModelType, ViewType>            PixelDescriptionActionHandlerType;
  typedef otb::PixelDescriptionView
    < PixelDescriptionModelType >                     PixelDescriptionViewType;

  // Filters for the second layer
  typedef itk::SobelEdgeDetectionImageFilter<ScalarImageType,ScalarImageType> FilterType;
  typedef otb::PerBandVectorImageFilter<ImageType,ImageType,FilterType>
  PerBandFilterType;

  // Instantiation
  ModelType::Pointer model = ModelType::New();
  PixelDescriptionModelType::Pointer pixelModel = PixelDescriptionModelType::New();
  pixelModel->SetLayers(model->GetLayers());

  // Reading input image
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(infname);
  
  // Generate the first layer
  LayerGeneratorType::Pointer generator = LayerGeneratorType::New();
  generator->SetImage(reader->GetOutput());
  generator->GenerateLayer();
  generator->GetLayer()->SetName("Image");
  
  // Generate the second layer
  PerBandFilterType::Pointer filter = PerBandFilterType::New();
  filter->SetInput(reader->GetOutput());

  LayerGeneratorType::Pointer generator2 = LayerGeneratorType::New();
  generator2->SetImage(filter->GetOutput());
  generator2->GenerateLayer();
  generator2->GetLayer()->SetName("Gradient");
  
   // Add the layer to the model
  model->AddLayer(generator->GetLayer());
  model->AddLayer(generator2->GetLayer());
  
  // Build a view
  ViewType::Pointer view = ViewType::New();
  view->SetModel(model);

  // Build a controller
  ControllerType::Pointer controller = ControllerType::New();
  view->SetController(controller);

  // Add the resizing handler
  ResizingHandlerType::Pointer resizingHandler = ResizingHandlerType::New();
  resizingHandler->SetModel(model);
  resizingHandler->SetView(view);
  controller->AddActionHandler(resizingHandler);

  // Add the change scaled region handler
  ChangeScaledRegionHandlerType::Pointer changeScaledHandler =ChangeScaledRegionHandlerType::New();
  changeScaledHandler->SetModel(model);
  changeScaledHandler->SetView(view);
  controller->AddActionHandler(changeScaledHandler);

  // Add the change extract region handler
  ChangeRegionHandlerType::Pointer changeHandler =ChangeRegionHandlerType::New();
  changeHandler->SetModel(model);
  changeHandler->SetView(view);
  controller->AddActionHandler(changeHandler);

  // Add the change scaled handler
  ChangeScaleHandlerType::Pointer changeScaleHandler =ChangeScaleHandlerType::New();
  changeScaleHandler->SetModel(model);
  changeScaleHandler->SetView(view);
  controller->AddActionHandler(changeScaleHandler);

  // Add the pixel description action handler
  PixelDescriptionActionHandlerType::Pointer pixelActionHandler = PixelDescriptionActionHandlerType::New();
  pixelActionHandler->SetView(view);
  pixelActionHandler->SetModel(pixelModel);
  controller->AddActionHandler(pixelActionHandler);
  
  // Build a pixel description view
  PixelDescriptionViewType::Pointer pixelView = PixelDescriptionViewType::New();
  pixelView->SetModel(pixelModel);
  
  Fl_Window pixelWindow(fullSize,50);
  if(fullSize > 0)
    {
    pixelWindow.add(pixelView->GetPixelDescriptionWidget());
    pixelWindow.resizable(pixelView->GetPixelDescriptionWidget());
    pixelWindow.show();
    pixelView->GetPixelDescriptionWidget()->show();
    pixelView->GetPixelDescriptionWidget()->resize(0,0,fullSize,50);
    }

  Fl_Window scrollWindow(scrollSize,scrollSize);
  if(scrollSize > 0)
    {
     scrollWindow.add(view->GetScrollWidget());
     scrollWindow.resizable(view->GetScrollWidget());
     scrollWindow.show();
    view->GetScrollWidget()->show();
    view->GetScrollWidget()->resize(0,0,scrollSize,scrollSize);
    }

  Fl_Window fullWindow(fullSize,fullSize);
  if(fullSize > 0)
    {
    fullWindow.add(view->GetFullWidget());
    fullWindow.resizable(view->GetFullWidget());
    fullWindow.show(); 
    view->GetFullWidget()->show();
    view->GetFullWidget()->resize(0,0,fullSize,fullSize);
    }

  Fl_Window zoomWindow(zoomSize,zoomSize);
  if(zoomSize > 0)
    {
    zoomWindow.add(view->GetZoomWidget());
    zoomWindow.resizable(view->GetZoomWidget());
    zoomWindow.show();
    view->GetZoomWidget()->show();
    view->GetZoomWidget()->resize(0,0,zoomSize,zoomSize);
    }

  if(run)
    {
    Fl::run();
    }
  else
    {
    Fl::check();
    }

  zoomWindow.remove(view->GetZoomWidget());
  scrollWindow.remove(view->GetScrollWidget());
  fullWindow.remove(view->GetFullWidget());
  pixelWindow.remove(pixelView->GetPixelDescriptionWidget());
  
  return EXIT_SUCCESS;
}
