#ifndef __ijBSplineScatteredDataPointSetToImageFilter_txx
#define __ijBSplineScatteredDataPointSetToImageFilter_txx

#include "ijBSplineScatteredDataPointSetToImageFilter.h"
#include "itkImageRegionConstIteratorWithIndex.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkImageDuplicator.h"
#include "itkCastImageFilter.h"
#include "itkNumericTraits.h"
#include "itkTimeProbe.h"

#include "vnl/vnl_math.h"
#include "vnl/algo/vnl_matrix_inverse.h"
#include "vnl/vnl_vector.h"
namespace ij
{

template <class TInputPointSet, class TOutputImage>
BSplineScatteredDataPointSetToImageFilter<TInputPointSet, TOutputImage>
::BSplineScatteredDataPointSetToImageFilter()
{
  this->m_SplineOrder.Fill( 3 );
  for ( unsigned int i = 0; i < ImageDimension; i++ )
    { 
    this->m_NumberOfControlPoints[i] = ( this->m_SplineOrder[i]+1 );
    this->m_Kernel[i] = KernelType::New();
    this->m_Kernel[i]->SetSplineOrder( this->m_SplineOrder[i] );
    }

  this->m_CloseDimension.Fill( 0 );
  this->m_DoMultilevel = false;
  this->m_GenerateOutputImage = true;
  this->m_NumberOfLevels.Fill( 1 );
  this->m_MaximumNumberOfLevels = 1;
   
  this->m_PhiLattice = PointDataImageType::New();
  this->m_PsiLattice = PointDataImageType::New();  
  this->m_InputPointData = PointDataContainerType::New();
  this->m_OutputPointData = PointDataContainerType::New();
  
  this->m_PointWeights = WeightsContainerType::New();
  this->m_UsePointWeights = false;
}

template <class TInputPointSet, class TOutputImage>
BSplineScatteredDataPointSetToImageFilter<TInputPointSet, TOutputImage>
::~BSplineScatteredDataPointSetToImageFilter()
{  
}

template <class TInputPointSet, class TOutputImage>
void
BSplineScatteredDataPointSetToImageFilter<TInputPointSet, TOutputImage>
::SetSplineOrder( unsigned int order )
{ 
  this->m_SplineOrder.Fill( order );
  this->SetSplineOrder( this->m_SplineOrder );
} 

template <class TInputPointSet, class TOutputImage>
void
BSplineScatteredDataPointSetToImageFilter<TInputPointSet, TOutputImage>
::SetSplineOrder( ArrayType order )
{ 
  itkDebugMacro(<< "Setting m_SplineOrder to " << order);

  this->m_SplineOrder = order;
  for ( int i = 0; i < ImageDimension; i++ )
    { 
    if ( this->m_SplineOrder[i] == 0 )
      {
      itkExceptionMacro( "The spline order in each dimension must be greater than 0" );
      }

    this->m_Kernel[i] = KernelType::New();
    this->m_Kernel[i]->SetSplineOrder( this->m_SplineOrder[i] );
  
    if ( this->m_DoMultilevel )
      {
      typename KernelType::MatrixType R;
      typename KernelType::MatrixType C;
      C = this->m_Kernel[i]->GetShapeFunctionsInZeroToOneInterval();
      R = C;
      for ( unsigned int j = 0; j < C.cols(); j++ )
        {
        RealType c = pow( 2.0, static_cast<RealType>( C.cols()-j-1 ) );
        for ( unsigned int k = 0; k < C.rows(); k++)
          {
          R(k, j) *= c;
          }
        }  
      R = R.transpose();  
      R.flipud();
      C = C.transpose();  
      C.flipud();      
      this->m_RefinedLatticeCoefficients[i] 
          = ( vnl_svd<RealType>( R ).solve( C ) ).extract( 2, C.cols() );
      }  
    } 
  this->Modified();
}  

template <class TInputPointSet, class TOutputImage>
void 
BSplineScatteredDataPointSetToImageFilter<TInputPointSet, TOutputImage>
::SetNumberOfLevels( unsigned int levels )
{
  this->m_NumberOfLevels.Fill( levels );
  this->SetNumberOfLevels( this->m_NumberOfLevels );
}     	

template <class TInputPointSet, class TOutputImage>
void 
BSplineScatteredDataPointSetToImageFilter<TInputPointSet, TOutputImage>
::SetNumberOfLevels( ArrayType levels )
{
  this->m_NumberOfLevels = levels;
  this->m_MaximumNumberOfLevels = 1;
  for ( unsigned int i = 0; i < ImageDimension; i++ )
    {
    if ( this->m_NumberOfLevels[i] == 0 )
      {
      itkExceptionMacro( "The number of levels in each dimension must be greater than 0" );
      }
    if ( this->m_NumberOfLevels[i] > this->m_MaximumNumberOfLevels )
      {
      this->m_MaximumNumberOfLevels = this->m_NumberOfLevels[i];
      } 
    }

  itkDebugMacro( "Setting m_NumberOfLevels to " <<  
                 this->m_NumberOfLevels );
  itkDebugMacro( "Setting m_MaximumNumberOfLevels to " <<
                 this->m_MaximumNumberOfLevels );

  if ( this->m_MaximumNumberOfLevels > 1 ) 
    {
    this->m_DoMultilevel = true;
    }
  else
    {  
    this->m_DoMultilevel = false;
    }
  this->SetSplineOrder( this->m_SplineOrder );
  this->Modified();
}     	

template <class TInputPointSet, class TOutputImage>
void 
BSplineScatteredDataPointSetToImageFilter<TInputPointSet, TOutputImage>
::SetPointWeights( typename WeightsContainerType::Pointer weights )
{
  this->m_UsePointWeights = true; 
  this->m_PointWeights = weights;
  this->Modified();
}
  
template <class TInputPointSet, class TOutputImage>
void
BSplineScatteredDataPointSetToImageFilter<TInputPointSet, TOutputImage>
::GenerateData()
{
  if ( this->GetInput()->GetNumberOfPoints() == 0 )
    {
    itkExceptionMacro( "The number of points must be greater than 0." );
    }
  if ( this->m_UsePointWeights && 
       ( this->m_PointWeights->Size() != this->GetInput()->GetNumberOfPoints() ) )
    {
    itkExceptionMacro( "The number of weight points and input points must be equal." );   
    }


  /**
   *  Create the output image
   */

  for ( unsigned int i = 0; i < ImageDimension; i++)
    {
    if ( this->m_Size[i] == 0 )
      {
      itkExceptionMacro( "Size must be specified." );
      }
    }
  this->GetOutput()->SetOrigin( this->m_Origin );
  this->GetOutput()->SetSpacing( this->m_Spacing );
  this->GetOutput()->SetRegions( this->m_Size );
  this->GetOutput()->Allocate();

  this->m_InputPointData->Initialize();
  this->m_OutputPointData->Initialize();

  typename PointSetType::PointDataContainer::ConstIterator It;
  It = this->GetInput()->GetPointData()->Begin();
  while ( It != this->GetInput()->GetPointData()->End() )
    {
    if ( !this->m_UsePointWeights )
      {
      this->m_PointWeights->InsertElement( It.Index(), 1.0 );
      }
    this->m_InputPointData->InsertElement( It.Index(), It.Value() );
    this->m_OutputPointData->InsertElement( It.Index(), It.Value() );
    ++It;
    }

  this->m_CurrentLevel = 0; 
  this->m_CurrentNumberOfControlPoints = this->m_NumberOfControlPoints;    
  this->GenerateControlLattice();
  this->UpdatePointSet();

  itkDebugMacro( << "Current Level = " << this->m_CurrentLevel );
  itkDebugMacro( << "  Number of control points = " 
                 << this->m_CurrentNumberOfControlPoints );

  if ( this->m_DoMultilevel ) 
    {   
    this->m_PsiLattice->SetRegions( this->m_PhiLattice->GetLargestPossibleRegion() );
    this->m_PsiLattice->Allocate();
    PointDataType P( 0.0 );
    this->m_PsiLattice->FillBuffer( P );
    }
  
  for ( this->m_CurrentLevel = 1; 
        this->m_CurrentLevel < this->m_MaximumNumberOfLevels; this->m_CurrentLevel++ )
    {
  
    itk::ImageRegionIterator<PointDataImageType> ItPsi( this->m_PsiLattice, 
            this->m_PsiLattice->GetLargestPossibleRegion() );
    itk::ImageRegionIterator<PointDataImageType> ItPhi( this->m_PhiLattice, 
            this->m_PhiLattice->GetLargestPossibleRegion() );
    for ( ItPsi.GoToBegin(), ItPhi.GoToBegin(); 
          !ItPsi.IsAtEnd(); ++ItPsi, ++ItPhi )
      {
      ItPsi.Set(ItPhi.Get()+ItPsi.Get());
      }  
    this->RefineControlLattice();

    for ( unsigned int i = 0; i < ImageDimension; i++ )
      {
      if ( this->m_CurrentLevel < this->m_NumberOfLevels[i] )
        {
        this->m_CurrentNumberOfControlPoints[i] = 
	         2*this->m_CurrentNumberOfControlPoints[i]-this->m_SplineOrder[i];
        }
      }

    itkDebugMacro( << "Current Level = " << this->m_CurrentLevel );
    itkDebugMacro( << "  No. of control points = " 
                   << this->m_CurrentNumberOfControlPoints );

    RealType avg_p = 0.0;
    RealType totalWeight = 0.0;

    typename PointDataContainerType::Iterator  ItIn, ItOut;
    ItIn = this->m_InputPointData->Begin();
    ItOut = this->m_OutputPointData->Begin();
    while ( ItIn != this->m_InputPointData->End() )
      {
      this->m_InputPointData->InsertElement( ItIn.Index(), ItIn.Value()-ItOut.Value() );
      if ( this->GetDebug() )
   	    {
        RealType weight = this->m_PointWeights->GetElement( ItIn.Index() );
	       avg_p += ( ItIn.Value()-ItOut.Value() ).GetNorm() * weight;	 
        totalWeight += weight;
        }  	
      ++ItIn;
      ++ItOut;
      }
    itkDebugMacro( << "The average weighted difference norm of the point set is " 
                   << avg_p / totalWeight );

    this->GenerateControlLattice();
    this->UpdatePointSet();
    }

  if (this->m_DoMultilevel) 
    {   
    itk::ImageRegionIterator<PointDataImageType> 
      ItPsi( this->m_PsiLattice, this->m_PsiLattice->GetLargestPossibleRegion() );
    itk::ImageRegionIterator<PointDataImageType> 
      ItPhi( this->m_PhiLattice, this->m_PhiLattice->GetLargestPossibleRegion() );
    for ( ItPsi.GoToBegin(), ItPhi.GoToBegin(); !ItPsi.IsAtEnd(); ++ItPsi, ++ItPhi )
      {
      ItPsi.Set( ItPhi.Get()+ItPsi.Get() );
      } 

    typedef itk::ImageDuplicator<PointDataImageType> ImageDuplicatorType;
    typename ImageDuplicatorType::Pointer Duplicator = ImageDuplicatorType::New();  
    Duplicator->SetInputImage( this->m_PsiLattice );
    Duplicator->Update();
    this->m_PhiLattice = Duplicator->GetOutput();      
    this->UpdatePointSet();
    }

  if ( this->m_GenerateOutputImage )
    {  
    //this->GenerateOutputImage();
    this->GenerateOutputImageFast();
    }
}

template <class TInputPointSet, class TOutputImage>
void
BSplineScatteredDataPointSetToImageFilter<TInputPointSet, TOutputImage>
::RefineControlLattice() 
{
  ArrayType NumberOfNewControlPoints = this->m_CurrentNumberOfControlPoints;
  for ( unsigned int i = 0; i < ImageDimension; i++ )
    {
    if ( this->m_CurrentLevel < this->m_NumberOfLevels[i] )
      {  
      NumberOfNewControlPoints[i] = 2*NumberOfNewControlPoints[i]-this->m_SplineOrder[i];
      }  
    }
  typename RealImageType::RegionType::SizeType size;
  for ( unsigned int i = 0; i < ImageDimension; i++ )
    {
    if ( this->m_CloseDimension[i] )
      {
      size[i] = NumberOfNewControlPoints[i] - this->m_SplineOrder[i];
      }
    else
      {
      size[i] = NumberOfNewControlPoints[i];
      }
    }
  
  typename PointDataImageType::Pointer RefinedLattice = PointDataImageType::New();
  RefinedLattice->SetRegions( size );
  RefinedLattice->Allocate();
  PointDataType data;
  data.Fill( 0.0 );
  RefinedLattice->FillBuffer( data );  

  typename PointDataImageType::IndexType idx;
  typename PointDataImageType::IndexType idx_Psi;
  typename PointDataImageType::IndexType tmp;
  typename PointDataImageType::IndexType tmp_Psi;
  typename PointDataImageType::IndexType off;
  typename PointDataImageType::IndexType off_Psi;
  typename PointDataImageType::RegionType::SizeType size_Psi;

  size.Fill( 2 );
  int N = 1;
  for ( unsigned int i = 0; i < ImageDimension; i++ )
    {
    N *= ( this->m_SplineOrder[i]+1 );
    size_Psi[i] = this->m_SplineOrder[i] + 1;  
    }

  itk::ImageRegionIteratorWithIndex<PointDataImageType> 
    It( RefinedLattice, RefinedLattice->GetLargestPossibleRegion() );

  It.GoToBegin();
  while ( !It.IsAtEnd() )
    {
    idx = It.GetIndex();
    for ( unsigned int i = 0; i < ImageDimension; i++ )
      {
      if ( this->m_CurrentLevel < this->m_NumberOfLevels[i] )
        {
        idx_Psi[i] = static_cast<unsigned int>( 0.5*idx[i] );
	       }
      else
        {
        idx_Psi[i] = static_cast<unsigned int>( idx[i] );    
        }
      }
    for ( unsigned int i = 0; 
          i < pow( 2.0, static_cast<RealType>( ImageDimension ) ); i++ )
      {
      PointDataType sum( 0.0 );
      PointDataType val;
      off = this->IndexToSubscript( i, size );

      bool OutOfBoundary = false;
      for ( unsigned int j = 0; j < ImageDimension; j++ )
        {
        tmp[j] = idx[j] + off[j];
        if ( tmp[j] >= static_cast<long>(NumberOfNewControlPoints[j]) && (!this->m_CloseDimension[j]) )
          {
             OutOfBoundary = true;
             break;
          }
        if ( this->m_CloseDimension[j] )
          {
          tmp[j] %=  RefinedLattice->GetLargestPossibleRegion().GetSize()[j];
          } 
        }	
      if ( OutOfBoundary )
        {
        continue;
        }      
 
      for ( unsigned int j = 0; j < static_cast<unsigned int>(N); j++ )
        {
        off_Psi = this->IndexToSubscript( j, size_Psi );

        bool OutOfBoundary = false;
        for ( unsigned int k = 0; k < ImageDimension; k++ )
          {
          tmp_Psi[k] = idx_Psi[k] + off_Psi[k];
          if ( tmp_Psi[k] >= static_cast<long>(this->m_CurrentNumberOfControlPoints[k] )
                  && (!this->m_CloseDimension[k]) )
            {
            OutOfBoundary = true;
            break;
            }
          if ( this->m_CloseDimension[k] )
            {
           tmp_Psi[k] %= this->m_PsiLattice->GetLargestPossibleRegion().GetSize()[k];
            } 
          }	
          if ( OutOfBoundary )
             {
               continue;
             }      
          RealType coeff = 1.0;
          for ( unsigned int k = 0; k < ImageDimension; k++ )
  	         {
            coeff *= this->m_RefinedLatticeCoefficients[k]( off[k], off_Psi[k] );
	           }  
          val = this->m_PsiLattice->GetPixel( tmp_Psi );  
	         val *= coeff;
          sum += val;
          }
        RefinedLattice->SetPixel( tmp, sum );
        }  

    bool IsEvenIndex = false;
    while ( !IsEvenIndex && !It.IsAtEnd() )
      {      
      ++It;  
      idx = It.GetIndex();
      IsEvenIndex = true;
      for ( unsigned int i = 0; i < ImageDimension; i++ )
        {
        if ( idx[i] % 2 )
          {
          IsEvenIndex = false;
          }
        }
      }    
    }

  typedef itk::ImageDuplicator<PointDataImageType> ImageDuplicatorType;
  typename ImageDuplicatorType::Pointer Duplicator = ImageDuplicatorType::New();  
  Duplicator->SetInputImage( RefinedLattice );
  Duplicator->Update();
  this->m_PsiLattice = Duplicator->GetOutput();  
}

template <class TInputPointSet, class TOutputImage>
void
BSplineScatteredDataPointSetToImageFilter<TInputPointSet, TOutputImage>
::GenerateControlLattice() 
{
  typename RealImageType::RegionType::SizeType size;
  for ( unsigned int i = 0; i < ImageDimension; i++ )
    {
    if ( this->m_CloseDimension[i] )
      {  
      size[i] = this->m_CurrentNumberOfControlPoints[i]-this->m_SplineOrder[i];
      }
    else
      {
      size[i] = this->m_CurrentNumberOfControlPoints[i];
      }
    }

  this->m_PhiLattice->SetRegions( size );
  this->m_PhiLattice->Allocate();
  this->m_PhiLattice->FillBuffer( 0.0 );

  typename RealImageType::Pointer omega;
  omega = RealImageType::New();
  omega->SetRegions( size );
  omega->Allocate();
  omega->FillBuffer( 0.0 );

  typename PointDataImageType::Pointer delta;
  delta = PointDataImageType::New();  
  delta->SetRegions( size );
  delta->Allocate();
  delta->FillBuffer( 0.0 );
  
  for ( unsigned int i = 0; i < ImageDimension; i++ )
    {
    size[i] = this->m_SplineOrder[i]+1;  
    }

  typename RealImageType::Pointer w;
  w = RealImageType::New();  
  w->SetRegions( size );
  w->Allocate();

  typename PointDataImageType::Pointer phi;
  phi = PointDataImageType::New();  
  phi->SetRegions( size );
  phi->Allocate();

  itk::ImageRegionIteratorWithIndex<RealImageType> 
     Itw( w, w->GetBufferedRegion() );
  itk::ImageRegionIteratorWithIndex<PointDataImageType> 
     Itp( phi, phi->GetBufferedRegion() );

  vnl_vector<RealType> p( ImageDimension );
  vnl_vector<RealType> r( ImageDimension );  
  for ( unsigned int i = 0; i < ImageDimension; i++ )
    {
    r[i] = static_cast<RealType>( this->m_CurrentNumberOfControlPoints[i]-this->m_SplineOrder[i] )
           /static_cast<RealType>( this->m_Size[i]*this->m_Spacing[i]+1e-10 );
    }  
    
  typename PointDataContainerType::ConstIterator It;
  for ( It = this->m_InputPointData->Begin(); It != this->m_InputPointData->End(); ++It )
    {
    PointType point;
    this->GetInput()->GetPoint( It.Index(), &point );

    for ( unsigned int i = 0; i < ImageDimension; i++ )
      {
      p[i] = ( point[i] - this->m_Origin[i] ) * r[i];
      }

    RealType w2_sum = 0.0;
    for ( Itw.GoToBegin(); !Itw.IsAtEnd(); ++Itw )
      {
      RealType B = 1.0;
      typename RealImageType::IndexType idx = Itw.GetIndex();
      for ( unsigned int i = 0; i < ImageDimension; i++ )
        {	 
	       RealType u = static_cast<RealType>( p[i] - 
	         static_cast<unsigned>( p[i] ) - idx[i] ) 
	         + 0.5*static_cast<RealType>( this->m_SplineOrder[i]-1 );
        B *= this->m_Kernel[i]->Evaluate( u );
        }  
      Itw.Set( B );
      w2_sum += B*B;
      }

    for ( Itp.GoToBegin(), Itw.GoToBegin(); !Itp.IsAtEnd(); ++Itp, ++Itw )
      {
      typename RealImageType::IndexType idx = Itw.GetIndex();
      for ( unsigned int i = 0; i < ImageDimension; i++ )
        {
        idx[i] += static_cast<unsigned>( p[i] );
        if ( this->m_CloseDimension[i] )
          {
          idx[i] %= delta->GetLargestPossibleRegion().GetSize()[i];
          }  
        }
						RealType wc = this->m_PointWeights->GetElement( It.Index() );
						RealType t = Itw.Get();
						PointDataType data = It.Value();
						data *= ( t/w2_sum ); 
						Itp.Set( data );
						data *= ( t*t*wc );       
						delta->SetPixel( idx, delta->GetPixel( idx ) + data );
						omega->SetPixel( idx, omega->GetPixel( idx ) + wc*t*t );
      }
    }  

  itk::ImageRegionIterator<PointDataImageType> 
      Itl( this->m_PhiLattice, this->m_PhiLattice->GetBufferedRegion() );
  itk::ImageRegionIterator<PointDataImageType> 
      Itd( delta, delta->GetBufferedRegion() );  
  itk::ImageRegionIterator<RealImageType> 
      Ito( omega, omega->GetBufferedRegion() );
  
  for ( Itl.GoToBegin(), Ito.GoToBegin(), Itd.GoToBegin(); 
           !Itl.IsAtEnd(); ++Itl, ++Ito, ++Itd )
    {   
    PointDataType P;   
    P.Fill( 0 );
    if ( Ito.Get() != 0 )
      {
      P = Itd.Get()/Ito.Get();
      for ( unsigned int i = 0; i < P.Size(); i++ )
        {
        if ( vnl_math_isnan( P[i] ) || vnl_math_isinf( P[i] ) )
          {
          P[i] = 0;
          } 
        }         
      Itl.Set( P );
      }
    }
}

template <class TInputPointSet, class TOutputImage>
void
BSplineScatteredDataPointSetToImageFilter<TInputPointSet, TOutputImage>
::UpdatePointSet() 
{
  typename PointDataContainerType::ConstIterator ItIn;

  ItIn = this->m_InputPointData->Begin();
  while ( ItIn != this->m_InputPointData->End() )
    {
    PointType point(itk::NumericTraits<PointType>::Zero);
    PointDataType dataOut(itk::NumericTraits<PointDataType>::Zero);

    this->GetInput()->GetPoint( ItIn.Index(), &point );
    this->EvaluateAtPoint( point, dataOut );
    this->m_OutputPointData->InsertElement( ItIn.Index(), dataOut );
    ++ItIn;
    }
}

template <class TInputPointSet, class TOutputImage>
void
BSplineScatteredDataPointSetToImageFilter<TInputPointSet, TOutputImage>
::GenerateOutputImage() 
{
  itk::ImageRegionIteratorWithIndex<ImageType> 
     It( this->GetOutput(), this->GetOutput()->GetBufferedRegion() );

  for ( It.GoToBegin(); !It.IsAtEnd(); ++It )
    {
    PointDataType data;
    this->EvaluateAtIndex( It.GetIndex(), data );
    It.Set( data );
    }
}

template <class TInputPointSet, class TOutputImage>
void
BSplineScatteredDataPointSetToImageFilter<TInputPointSet, TOutputImage>
::GenerateOutputImageFast() 
{
  typename PointDataImageType::Pointer collapsedPhiLattices[ImageDimension+1];
  for ( unsigned int i = 0; i < ImageDimension; i++ )
    {
    collapsedPhiLattices[i] = PointDataImageType::New();
    collapsedPhiLattices[i]->SetOrigin( this->m_PhiLattice->GetOrigin() );
    collapsedPhiLattices[i]->SetSpacing( this->m_PhiLattice->GetSpacing() );
    typename PointDataImageType::SizeType size;
    size.Fill( 1 );
    for ( unsigned int j = 0; j < i; j++ )
      {
      size[j] = this->m_PhiLattice->GetLargestPossibleRegion().GetSize()[j];
      }
    typename PointDataImageType::IndexType index;
    index.Fill(0);

    typename PointDataImageType::RegionType region;
    region.SetIndex(index);
    region.SetSize(size);
    
    collapsedPhiLattices[i]->SetRegions( region );
    collapsedPhiLattices[i]->Allocate();
    collapsedPhiLattices[i]->FillBuffer(typename PointDataImageType::PixelType(0.));
    } 
  collapsedPhiLattices[ImageDimension] = this->m_PhiLattice;
  ArrayType totalNumberOfSpans;
  for ( unsigned int i = 0; i < ImageDimension; i++ )
    {
    totalNumberOfSpans[i] = this->m_CurrentNumberOfControlPoints[i] - this->m_SplineOrder[i];
    }
  itk::FixedArray<RealType, ImageDimension> U;
  itk::FixedArray<RealType, ImageDimension> currentU;
  currentU.Fill( -1 );
  typename ImageType::IndexType startIndex 
    = this->GetOutput()->GetRequestedRegion().GetIndex();
  typename PointDataImageType::IndexType startPhiIndex
    = this->GetOutput()->GetLargestPossibleRegion().GetIndex();

  itk::ImageRegionIteratorWithIndex<ImageType> 
     It( this->GetOutput(), this->GetOutput()->GetRequestedRegion() );
  for ( It.GoToBegin(); !It.IsAtEnd(); ++It )
    {
    typename ImageType::IndexType idx = It.GetIndex();
    for ( unsigned int i = 0; i < ImageDimension; i++ )
      {
      U[i] = static_cast<RealType>( totalNumberOfSpans[i] ) * 
             static_cast<RealType>( idx[i] - startIndex[i] ) /
             static_cast<RealType>( this->m_Size[i] - 1.0 );
      if ( U[i] == 1.0 )
        {
        U[i] -= vnl_math::float_eps;
        }  
      } 
    for ( int i = ImageDimension-1; i >= 0; i-- )
      {
      if ( U[i] != currentU[i] )
        {    
        for ( int j = i; j >= 0; j-- )
          { 
          this->CollapsePhiLattice( collapsedPhiLattices[j+1], collapsedPhiLattices[j], U[j], j );
          currentU[j] = U[j];
          }
        break;
        }
      }
    It.Set( collapsedPhiLattices[0]->GetPixel( startPhiIndex ) ); 
    }   
}

template <class TInputPointSet, class TOutputImage>
void
BSplineScatteredDataPointSetToImageFilter<TInputPointSet, TOutputImage>
::CollapsePhiLattice( PointDataImageType *lattice, 
                      PointDataImageType *collapsedLattice,
                      RealType u, unsigned int dimension ) 
{
  itk::ImageRegionIteratorWithIndex<PointDataImageType> It
    ( collapsedLattice, collapsedLattice->GetLargestPossibleRegion() );

  for ( It.GoToBegin(); !It.IsAtEnd(); ++It )
    { 
    PointDataType data;
    data.Fill( 0.0 );
    typename PointDataImageType::IndexType idx = It.GetIndex();
    for ( unsigned int i = 0; i < this->m_SplineOrder[dimension] + 1; i++ )
      {
      idx[dimension] = static_cast<unsigned int>( u ) + i;         
      if ( this->m_CloseDimension[dimension] )
        {
        idx[dimension] %= lattice->GetLargestPossibleRegion().GetSize()[dimension]; 
        }
      RealType B = this->m_Kernel[dimension]->Evaluate
        ( u - idx[dimension] + 0.5*static_cast<RealType>( this->m_SplineOrder[dimension] - 1 ) ); 

      if(lattice->GetLargestPossibleRegion().IsInside(idx))
	{
	  data += ( lattice->GetPixel( idx ) * B );
	}
      }    
    It.Set( data );
    }  
}

template <class TInputPointSet, class TOutputImage>
void
BSplineScatteredDataPointSetToImageFilter<TInputPointSet, TOutputImage>
::EvaluateAtPoint( PointType point, PointDataType &data ) 
{
  for ( unsigned int i = 0; i < ImageDimension; i++)
    {
    point[i] -= this->m_Origin[i];
    point[i] /= ( static_cast<RealType>( ( this->m_Size[i] - 1 )*this->m_Spacing[i] ) );
    }  
  this->Evaluate( point, data );
}

template <class TInputPointSet, class TOutputImage>
void
BSplineScatteredDataPointSetToImageFilter<TInputPointSet, TOutputImage>
::EvaluateAtIndex( IndexType idx, PointDataType &data ) 
{
  PointType point;
  this->GetOutput()->TransformIndexToPhysicalPoint( idx, point );
  this->EvaluateAtPoint( point, data );
}

template <class TInputPointSet, class TOutputImage>
void
BSplineScatteredDataPointSetToImageFilter<TInputPointSet, TOutputImage>
::EvaluateAtContinuousIndex( ContinuousIndexType idx, PointDataType &data ) 
{
  PointType point;
  this->GetOutput()->TransformContinuousIndexToPhysicalPoint( idx, point );
  this->EvaluateAtPoint( point, data );
}

template <class TInputPointSet, class TOutputImage>
void
BSplineScatteredDataPointSetToImageFilter<TInputPointSet, TOutputImage>
::Evaluate( PointType params, PointDataType &data ) 
{
  vnl_vector<RealType> p( ImageDimension );
  for ( unsigned int i = 0; i < ImageDimension; i++ )
    {
    if ( params[i] < 0.0 || params[i] > 1.0 )
      {
      itkExceptionMacro( "The specifed point is outside the image domain." );
      }
    if ( params[i] == 1.0 )
      {
      params[i] = 1.0 - vnl_math::float_eps;  
      }   
    p[i] = static_cast<RealType>( params[i] ) 
         * static_cast<RealType>( this->m_CurrentNumberOfControlPoints[i]
	                          - this->m_SplineOrder[i] );
    }

  typename RealImageType::RegionType::SizeType size;
  for ( unsigned int i = 0; i < ImageDimension; i++ )
    {
    size[i] = this->m_SplineOrder[i] + 1;
    } 
  typename RealImageType::Pointer w;
  w = RealImageType::New();  
  w->SetRegions( size );
  w->Allocate(); 

  PointDataType val;
  data.Fill( 0.0 );

  itk::ImageRegionIteratorWithIndex<RealImageType> Itw( w, w->GetLargestPossibleRegion() );

  for ( Itw.GoToBegin(); !Itw.IsAtEnd(); ++Itw )
    {
    RealType B = 1.0;
    typename RealImageType::IndexType idx = Itw.GetIndex();
    for ( unsigned int i = 0; i < ImageDimension; i++ )
      {	  
      RealType u = p[i] - static_cast<RealType>( static_cast<unsigned>(p[i]) + idx[i] ) 
                        + 0.5*static_cast<RealType>( this->m_SplineOrder[i] - 1 );
      B *= this->m_Kernel[i]->Evaluate( u );
      }  
    for ( unsigned int i = 0; i < ImageDimension; i++ )
      {
      idx[i] += static_cast<unsigned int>( p[i] );
      if ( this->m_CloseDimension[i] )
        {
        idx[i] %= this->m_PhiLattice->GetLargestPossibleRegion().GetSize()[i];
        }  
      }      
    if ( this->m_PhiLattice->GetLargestPossibleRegion().IsInside( idx ) )
      {
      val = this->m_PhiLattice->GetPixel( idx );  
	     val *= B;
      data += val;
      } 
    }
}

template <class TInputPointSet, class TOutputImage>
void
BSplineScatteredDataPointSetToImageFilter<TInputPointSet, TOutputImage>
::EvaluateGradientAtPoint( PointType point, GradientType &gradient ) 
{
  for ( unsigned int i = 0; i < ImageDimension; i++)
    {
    point[i] -= this->m_Origin[i];
    point[i] /= ( static_cast<RealType>( ( this->m_Size[i] - 1 )*this->m_Spacing[i] ) );
    }  
  this->EvaluateGradient( point, gradient );
}

template <class TInputPointSet, class TOutputImage>
void
BSplineScatteredDataPointSetToImageFilter<TInputPointSet, TOutputImage>
::EvaluateGradientAtIndex( IndexType idx, GradientType &gradient ) 
{
  PointType point;
  this->GetOutput()->TransformIndexToPhysicalPoint( idx, point );
  this->EvaluateGradientAtPoint( point, gradient );
}

template <class TInputPointSet, class TOutputImage>
void
BSplineScatteredDataPointSetToImageFilter<TInputPointSet, TOutputImage>
::EvaluateGradientAtContinuousIndex( ContinuousIndexType idx, GradientType &gradient ) 
{
  PointType point;
  this->GetOutput()->TransformContinuousIndexToPhysicalPoint( idx, gradient );
  this->EvaluateGradientAtPoint( point, gradient );
}

template <class TInputPointSet, class TOutputImage>
void
BSplineScatteredDataPointSetToImageFilter<TInputPointSet, TOutputImage>
::EvaluateGradient( PointType params, GradientType &gradient ) 
{
  vnl_vector<RealType> p( ImageDimension );
  for ( unsigned int i = 0; i < ImageDimension; i++ )
    {
    if ( params[i] < 0.0 || params[i] > 1.0 )
      {
      itkExceptionMacro( "The specifed point is outside the image domain." );
      }
    if ( params[i] == 1.0 )
      {
      params[i] = 1.0 - vnl_math::float_eps;  
      }   
    p[i] = static_cast<RealType>( params[i] ) 
         * static_cast<RealType>( this->m_CurrentNumberOfControlPoints[i]
	                          - this->m_SplineOrder[i] );
    }

  typename RealImageType::RegionType::SizeType size;
  for ( unsigned int i = 0; i < ImageDimension; i++ )
    {
    size[i] = this->m_SplineOrder[i]+1;
    } 
  typename RealImageType::Pointer w;
  w = RealImageType::New();  
  w->SetRegions( size );
  w->Allocate(); 

  PointDataType val;
  gradient.SetSize( val.Size(), ImageDimension );
  gradient.Fill( 0.0 );
  
  itk::ImageRegionIteratorWithIndex<RealImageType> 
     Itw( w, w->GetLargestPossibleRegion() );

  for ( unsigned int j = 0; j < gradient.Rows(); j++ )
    {
    for ( Itw.GoToBegin(); !Itw.IsAtEnd(); ++Itw )
      {
      RealType B = 1.0;
      typename RealImageType::IndexType idx = Itw.GetIndex();
      for ( unsigned int k = 0; k < ImageDimension; k++ )
        {	  
        RealType u = p[k] - static_cast<RealType>( static_cast<unsigned>( p[k] ) + idx[k] ) 
                          + 0.5*static_cast<RealType>( this->m_SplineOrder[k] - 1 );
        if ( j == k )
          { 
          B *= this->m_Kernel[k]->EvaluateDerivative( u );
          }
        else
          {
          B *= this->m_Kernel[k]->Evaluate( u );
          }	
        }  
      for ( unsigned int k = 0; k < ImageDimension; k++ )
        {
        idx[k] += static_cast<unsigned int>( p[k] );
        if ( this->m_CloseDimension[k] )
          {
          idx[k] %= this->m_PhiLattice->GetLargestPossibleRegion().GetSize()[k];
          }  
        }      
      if ( this->m_PhiLattice->GetLargestPossibleRegion().IsInside( idx ) )
        {
        val = this->m_PhiLattice->GetPixel( idx );  
        val *= B;
        for ( unsigned int k = 0; k < val.Size(); k++ )
          {
          gradient( j, k ) += val[k];
          }
        } 
      }
  	 }  
 
}

/**
 * Standard "PrintSelf" method
 */
template <class TInputPointSet, class TOutputImage>
void
BSplineScatteredDataPointSetToImageFilter<TInputPointSet, TOutputImage>
::PrintSelf(
  std::ostream& os, 
  itk::Indent indent) const
{
  Superclass::PrintSelf( os, indent );
  for ( unsigned int i = 0; i < ImageDimension; i++ )
    {    
    this->m_Kernel[i]->Print( os, indent );
    }
  os << indent << "B-spline order: " 
     << this->m_SplineOrder << std::endl;
  os << indent << "Number Of control points: " 
     << this->m_NumberOfControlPoints << std::endl;
  os << indent << "Close dimension: " 
     << this->m_CloseDimension << std::endl;
  os << indent << "Number of levels " 
     << this->m_NumberOfLevels << std::endl;
}
} // end namespace ij

#endif
