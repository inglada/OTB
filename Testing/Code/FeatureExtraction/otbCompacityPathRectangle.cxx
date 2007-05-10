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

#include "otbCompacityPathFunction.h"
#include "itkPolyLineParametricPath.h"
#include "itkExceptionObject.h"

int otbCompacityPathRectangle( int argc, char * argv[] )
{
  try 
    { 
        double        A ((double)::atof(argv[1]));
        double        B ((double)::atof(argv[2]));

        const   unsigned int                                   Dimension = 2;
	typedef itk::PolyLineParametricPath< Dimension >       PathType;
	typedef otb::CompacityPathFunction<PathType>           FunctionType;
	typedef FunctionType::RealType                         RealType;
  
	PathType::ContinuousIndexType cindex;
	PathType::Pointer pathElt = PathType::New();

        if(A<0)
	{
             std::cout << "retangle must be greater than 0.0 !" << std::endl; 
             return EXIT_FAILURE;
	} 

        if(B<0)
	{
             std::cout << "rectangle must be greater than 0.0 !" << std::endl; 
             return EXIT_FAILURE;
	} 
	
 	pathElt->Initialize();

       cindex[0]= 100;
       cindex[1]= 100;
       pathElt->AddVertex(cindex);	
       cindex[0]= 100+A;
       cindex[1]= 100;
       pathElt->AddVertex(cindex);	
       cindex[0]= 100+A;
       cindex[1]= 100+B;
       pathElt->AddVertex(cindex);	
       cindex[0]= 100;
       cindex[1]= 100+B;
       pathElt->AddVertex(cindex);	
        

	FunctionType::Pointer function =FunctionType::New();
        function->SetInputPath( pathElt );

	RealType Result = function->Evaluate();
	std::cout << "Compacity result: " << Result <<std::endl;

	RealType Error;
	Error = vcl_abs(Result - static_cast<RealType>(vcl_acos(-1.0) * A*B / (A+B) / (A+B)) );
	
	if(  Error > 1.E-9)
	{
		std::cout << "Error in estimation !" << std::endl;
		return EXIT_FAILURE;
	}
	
    } 
  catch( itk::ExceptionObject & err ) 
    { 
    std::cout << "itk::ExceptionObject catch !" << std::endl; 
    std::cout << err << std::endl; 
    return EXIT_FAILURE;
    } 
  catch( ... ) 
    { 
    std::cout << "unknown Exception catch !" << std::endl; 
    return EXIT_FAILURE;
    } 
  return EXIT_SUCCESS;
}

