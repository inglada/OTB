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

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "otbMacro.h"
#include "itkExceptionObject.h"
#include "itkImage.h"
#include "itkImageFileWriter.h"
#include <iostream>
#include "itkParametricPath.h"
#include "itkPolyLineParametricPath.h"
#include "itkVectorContainer.h"

#include "otbImageFileReader.h"
#include "otbImageToPathListAlignFilter.h"

#include "itkPath.h"
#include "itkPathSource.h"
#include "otbPathListSource.h"
#include "otbImageToPathListFilter.h"
#include "itkContinuousIndex.h"

#include <stdio.h>

int otbAlignImageToPath( int argc, char * argv[] )
{
  try 
    { 
        const char * inputFilename  = argv[1];
        const char * outputFilename = argv[2];
       

        typedef double                                   	InputPixelType;
        typedef double   	                        	OutputPixelType;
	typedef double						RealPixelType;
        const   unsigned int        	                        Dimension = 2;

        typedef itk::Image< InputPixelType,  Dimension >	InputImageType;
        typedef itk::Image< RealPixelType,  Dimension >		RealImageType;

	typedef itk::PolyLineParametricPath< Dimension >	PathType;
	
	typedef PathType::Pointer PathTypePointer;
	PathType::Pointer ltoto = PathType::New();

        typedef itk::Image< OutputPixelType, Dimension >        OutputImageType;

        typedef otb::ImageFileReader< InputImageType  >         ReaderType;  

	typedef otb::ImageToPathListAlignFilter<InputImageType,PathType> ListAlignFilterType;
        typedef itk::ImageFileWriter< OutputImageType >         WriterType;

        ReaderType::Pointer reader = ReaderType::New();
        WriterType::Pointer writer = WriterType::New();
	InputImageType::Pointer ImageIn = InputImageType::New();
	
        typedef otb::ImageToPathListAlignFilter<InputImageType,PathType>  TestType;
	TestType::Pointer testList = TestType::New();
	
        reader->SetFileName( inputFilename  );

//OTB-FA-00010-CS
        testList->SetInput( reader->GetOutput() );
	
	typedef ListAlignFilterType::OutputPathListType   ListAlignFilterOutputPathListType;
	
	
	otbGenericMsgDebugMacro(<< "Before update");
	testList->Update(); 
	otbGenericMsgDebugMacro(<< "After update");
	ListAlignFilterOutputPathListType * sortiePath = testList->GetOutput();
	
	otbGenericMsgDebugMacro(<< "Writing :");
	
	FILE *file = fopen(outputFilename,"w");
  	if (file == NULL) {
    		fprintf(stderr,"Error, can't open file");
    		exit(-1);
  	}
	typedef itk::ContinuousIndex< double,2>              VertexType; 
	typedef itk::VectorContainer< unsigned,VertexType >  VertexListType;
	typedef VertexListType::ConstPointer                 VertexListTypePointer;
	VertexListTypePointer vertexList;
	VertexType cindex;
	double x1,y1,x2,y2;
	  
	int nbPath = sortiePath->Size();
	otbGenericMsgDebugMacro(<< "NbSegment: "<<nbPath);
	fprintf(file,"Nb Segment: %d\n",nbPath);
	for (int i =0 ; i<nbPath ;i++){
	   vertexList = sortiePath->GetNthElement(i)->GetVertexList();
	   cindex = vertexList->GetElement(0);
	   x1 = cindex[0];
	   y1 = cindex[1];
	   cindex = vertexList->GetElement(1);
	   x2 = cindex[0];
	   y2 = cindex[1];
	   fprintf(file,"%8.3f %8.3f\n",x1,y1);
	}
  	fclose(file);
        
//        writer->Update(); 

    } 
  catch( itk::ExceptionObject & err ) 
    { 
    std::cout << "Exception itk::ExceptionObject levee !" << std::endl; 
    std::cout << err << std::endl; 
    return EXIT_FAILURE;
    } 
  catch( ... ) 
    { 
    std::cout << "Exception levee inconnue !" << std::endl; 
    return EXIT_FAILURE;
    } 
  return EXIT_SUCCESS;
}


