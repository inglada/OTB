#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkPointSet.h"
#include "ijBSplineScatteredDataPointSetToImageFilter.h"

/**
 * In this test, we approximate a 2-D scalar field.
 * The scattered data is derived from a segmented
 * image.  We write the output to an image for
 * comparison.
 */
int ijBSplineScatteredDataPointSetToImageFilterTest( int argc, char *argv[] )
{
  const unsigned int ParametricDimension = 2;
  const unsigned int DataDimension = 1;

  typedef int PixelType;
  typedef otb::Image<PixelType, ParametricDimension> InputImageType;
  typedef double RealType;
  typedef itk::Vector<RealType, DataDimension> VectorType;
  typedef otb::Image<VectorType, ParametricDimension> VectorImageType;
  typedef itk::PointSet
    <VectorImageType::PixelType, ParametricDimension> PointSetType;
  PointSetType::Pointer pointSet = PointSetType::New();

  typedef otb::ImageFileReader<InputImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );
  reader->Update();

  itk::ImageRegionIteratorWithIndex<InputImageType>
    It( reader->GetOutput(), reader->GetOutput()->GetLargestPossibleRegion() );

  // Iterate through the input image which consists of multivalued
  // foreground pixels (=nonzero) and background values (=zero).
  // The foreground pixels comprise the input point set.

  for ( It.GoToBegin(); !It.IsAtEnd(); ++It )
    {
    if ( It.Get() != itk::NumericTraits<PixelType>::Zero )
      {
      // We extract both the 2-D location of the point
      // and the pixel value of that point.

      PointSetType::PointType point;
      reader->GetOutput()->TransformIndexToPhysicalPoint( It.GetIndex(), point );

      unsigned long i = pointSet->GetNumberOfPoints();
      pointSet->SetPoint( i, point );

      PointSetType::PixelType V( DataDimension );
      V[0] = static_cast<RealType>( It.Get() );
      pointSet->SetPointData( i, V );
      }
    }

  // Instantiate the B-spline filter and set the desired parameters.
  typedef ij::BSplineScatteredDataPointSetToImageFilter
    <PointSetType, VectorImageType> FilterType;
  FilterType::Pointer filter = FilterType::New();
  filter->SetSplineOrder( 3 );
  FilterType::ArrayType ncps;
  ncps.Fill( 4 );
  filter->SetNumberOfControlPoints( ncps );
  filter->SetNumberOfLevels( 10 );

  // Define the parametric domain.
  filter->SetOrigin( reader->GetOutput()->GetOrigin() );
  filter->SetSpacing( reader->GetOutput()->GetSpacing() );
  filter->SetSize( reader->GetOutput()->GetLargestPossibleRegion().GetSize() );

  filter->SetInput( pointSet );

  try
    {
    filter->Update();
    }
  catch (...)
    {
    std::cerr << "Test 1: itkBSplineScatteredDataImageFilter exception thrown"
              << std::endl;
    return EXIT_FAILURE;
    }

  // Write the output to an image.
  typedef otb::Image<RealType, ParametricDimension> RealImageType;
  RealImageType::Pointer image = RealImageType::New();
  image->SetRegions( reader->GetOutput()->GetLargestPossibleRegion() );
  image->Allocate();
  itk::ImageRegionIteratorWithIndex<RealImageType>
    Itt( image, image->GetLargestPossibleRegion() );

  for ( Itt.GoToBegin(); !Itt.IsAtEnd(); ++Itt )
    {
    Itt.Set( filter->GetOutput()->GetPixel( Itt.GetIndex() )[0] );
    }

  typedef otb::ImageFileWriter<RealImageType> WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput( image );
  writer->SetFileName( argv[2] );
  writer->Update();

  return EXIT_SUCCESS;
};















