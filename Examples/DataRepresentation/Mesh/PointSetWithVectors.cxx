/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Some parts of this code are derived from ITK. See ITKCopyright.txt
  for details.


     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

//  Software Guide : BeginLatex
//
//  This example illustrates how a point set can be parameterized to manage a
//  particular pixel type. It is quite common to associate vector values with
//  points for producing geometric representations or storing
//  multi-band informations.  The following code shows
//  how vector values can be used as pixel type on the PointSet class.  The
//  \doxygen{itk::Vector} class is used here as the pixel type. This class is
//  appropriate for representing the relative position between two points. It
//  could then be used to manage displacements in disparity map
//  estimations, for example. 
//  
//  \index{itk::PointSet!Vector pixels}
//  
//  In order to use the vector class it is necessary to include its header file
//  along with the header of the point set.
//
//  Software Guide : EndLatex 

// Software Guide : BeginCodeSnippet
#include "itkVector.h"
#include "itkPointSet.h"
// Software Guide : EndCodeSnippet


int main(int, char *[])
{
  //  Software Guide : BeginLatex
  //
  //  \itkpiccaption[PointSet with Vectors as PixelType]{Vectors as PixelType.\label{fig:PointSetWithVectors}}
  //  \parpic(6cm,4cm)[r]{\includegraphics[width=4cm]{PointSetWithVectors.eps}}
  //
  //  The Vector class is templated over the type used to represent
  //  the spatial coordinates and over the space dimension.  Since the
  //  PixelType is independent of the PointType, we are free to select any
  //  dimension for the vectors to be used as pixel type. However, for the
  //  sake of producing an interesting example, we will use vectors that 
  //  represent displacements of the points in the PointSet. Those vectors
  //  are then selected to be of the same dimension as the PointSet.
  //
  //  \index{itk::Vector!itk::PointSet}
  //
  //  Software Guide : EndLatex 

  // Software Guide : BeginCodeSnippet
  const unsigned int Dimension = 2;
  typedef itk::Vector< float, Dimension >    PixelType;
  // Software Guide : EndCodeSnippet


  //  Software Guide : BeginLatex
  //
  //  Then we use the PixelType (which are actually Vectors) to instantiate the
  //  PointSet type and subsequently create a PointSet object.
  //
  //  Software Guide : EndLatex 

  // Software Guide : BeginCodeSnippet
  typedef itk::PointSet< PixelType, Dimension > PointSetType;
  PointSetType::Pointer  pointSet = PointSetType::New();
  // Software Guide : EndCodeSnippet


  //  Software Guide : BeginLatex
  //
  //  The following code is generating a circle and assigning vector values
  //  to the points. The components of the vectors in this example are
  //  computed to represent the tangents to the circle as shown in
  //  Figure~\ref{fig:PointSetWithVectors}.
  //
  //  \index{itk::PointSet!SetPoint()}
  //  \index{itk::PointSet!SetPointData()}
  //
  //  Software Guide : EndLatex 

  // Software Guide : BeginCodeSnippet
  PointSetType::PixelType   tangent;
  PointSetType::PointType   point;

  unsigned int pointId =  0;
  const double radius = 300.0;

  for(unsigned int i=0; i<360; i++)
    {
    const double angle = i * atan(1.0) / 45.0;
    point[0] = radius * sin( angle );
    point[1] = radius * cos( angle );
    tangent[0] =  cos(angle);
    tangent[1] = -sin(angle);
    pointSet->SetPoint( pointId, point );   
    pointSet->SetPointData( pointId, tangent );   
    pointId++;
    }
  // Software Guide : EndCodeSnippet


  //  Software Guide : BeginLatex
  //
  //  We can now visit all the points and use the vector on the pixel values to
  //  apply a displacement on the points. This is along the spirit of what a
  //  deformable model could do at each one of its iterations. 
  //
  //  \index{itk::PointSet!PointIterator}
  //  \index{itk::PointSet!GetPoints()}
  //  \index{itk::PointSet!GetPointData()}
  //
  //  Software Guide : EndLatex 


  // Software Guide : BeginCodeSnippet
  typedef  PointSetType::PointDataContainer::ConstIterator PointDataIterator;
  PointDataIterator pixelIterator = pointSet->GetPointData()->Begin();
  PointDataIterator pixelEnd      = pointSet->GetPointData()->End();

  typedef  PointSetType::PointsContainer::Iterator     PointIterator;
  PointIterator pointIterator = pointSet->GetPoints()->Begin();
  PointIterator pointEnd      = pointSet->GetPoints()->End();

  while( pixelIterator != pixelEnd  && pointIterator != pointEnd ) 
    {
    pointIterator.Value() = pointIterator.Value() + pixelIterator.Value();
    ++pixelIterator;
    ++pointIterator;
    }
  // Software Guide : EndCodeSnippet


  //  Software Guide : BeginLatex
  //
  //  Note that the \code{ConstIterator} was used here instead of the normal
  //  \code{Iterator} since the pixel values are only intended to be read and
  //  not modified. ITK supports const-correctness at the API level.
  //
  //  \index{ConstIterator}
  //  \index{const-correctness}
  //
  //  Software Guide : EndLatex 


  //  Software Guide : BeginLatex
  //
  //  The \doxygen{itk::Vector} class has overloaded the \code{+} operator with
  //  the \doxygen{itk::Point}. In other words, vectors can be added to points in
  //  order to produce new points.  This property is exploited in the center
  //  of the loop in order to update the points positions with a single
  //  statement.
  //
  //  \index{itk::PointSet!PointIterator}
  //
  //  We can finally visit all the points and print out the new values
  //
  //  Software Guide : EndLatex 

  // Software Guide : BeginCodeSnippet
  pointIterator = pointSet->GetPoints()->Begin();
  pointEnd      = pointSet->GetPoints()->End();
  while( pointIterator != pointEnd ) 
    {
    std::cout << pointIterator.Value() << std::endl;
    ++pointIterator;
    }
  // Software Guide : EndCodeSnippet


  //  Software Guide : BeginLatex
  //
  //  Note that \doxygen{itk::Vector} is not the appropriate class for
  //  representing normals to surfaces and gradients of functions. This is due
  //  to the way in which vectors behave under affine transforms. ITK has a
  //  specific class for representing normals and function gradients. This is
  //  the \doxygen{itk::CovariantVector} class.
  //
  //  Software Guide : EndLatex 

  return 0;
}



