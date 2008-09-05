
#     INPUTS: {BrainProtonDensitySlice.png}
#     OUTPUTS: {ThresholdSegmentationLevelSetImageFilterWhiteMatter.png}
#     60 116 5 150 180

#     INPUTS: {BrainProtonDensitySlice.png}
#     OUTPUTS: {ThresholdSegmentationLevelSetImageFilterVentricle.png}
#     81 112 5 210 250

#     INPUTS: {BrainProtonDensitySlice.png}
#     OUTPUTS: {ThresholdSegmentationLevelSetImageFilterGrayMatter.png}
#     107 69 5 180  210

import itk
from sys import argv, stderr, exit
itk.auto_progress(2)

# itk.auto_progress(1)

if len(argv) < 8 :
  print >> stderr, """Missing Parameters
Usage: ThresholdSegmentationLevelSetImageFilter.py inputImage  outputImage seedX seedY InitialDistance LowerThreshold UpperThreshold [CurvatureScaling == 1.0]"""
  exit(1)

InternalPixelType = itk.F
Dimension = 2
InternalImageType = itk.Image[ InternalPixelType, Dimension ]

OutputPixelType = itk.UC
OutputImageType = itk.Image[ OutputPixelType, Dimension ]

thresholder = itk.BinaryThresholdImageFilter[ InternalImageType, OutputImageType ].New()
                      
thresholder.SetLowerThreshold( -1000.0 )
thresholder.SetUpperThreshold(     0.0 )

thresholder.SetOutsideValue(  0  )
thresholder.SetInsideValue(  255 )

ReaderType = itk.ImageFileReader[ InternalImageType ]
WriterType = itk.ImageFileWriter[  OutputImageType  ]

reader = ReaderType.New()
writer = WriterType.New()

reader.SetFileName( argv[1] )
writer.SetFileName( argv[2] )


FastMarchingFilterType = itk.FastMarchingImageFilter[ InternalImageType, InternalImageType ]
fastMarching = FastMarchingFilterType.New()

ThresholdSegmentationLevelSetImageFilterType = itk.ThresholdSegmentationLevelSetImageFilter[ InternalImageType, InternalImageType, InternalPixelType ] 
thresholdSegmentation = ThresholdSegmentationLevelSetImageFilterType.New()
thresholdSegmentation.SetPropagationScaling( 1.0 )
if len(argv) > 8 :
  thresholdSegmentation.SetCurvatureScaling( float(argv[8]) )
else:
  thresholdSegmentation.SetCurvatureScaling( 1.0 )

thresholdSegmentation.SetMaximumRMSError( 0.02 )
thresholdSegmentation.SetNumberOfIterations( 1200 )

thresholdSegmentation.SetUpperThreshold( float(argv[7]) )
thresholdSegmentation.SetLowerThreshold( float(argv[6]) )
thresholdSegmentation.SetIsoSurfaceValue(0.0)

thresholdSegmentation.SetInput( fastMarching.GetOutput() )
thresholdSegmentation.SetFeatureImage( reader.GetOutput() )
thresholder.SetInput( thresholdSegmentation.GetOutput() )
writer.SetInput( thresholder.GetOutput() )

NodeType = itk.LevelSetNode[InternalPixelType, Dimension]
NodeContainer = itk.VectorContainer[itk.UI, NodeType]
seeds = NodeContainer.New()
seedPosition = [int( argv[3] ), int( argv[4] )]

initialDistance = float( argv[5] )

node = NodeType()

seedValue = - initialDistance

node.SetValue( seedValue )
node.SetIndex( seedPosition )

seeds.Initialize()
seeds.InsertElement( 0, node )

fastMarching.SetTrialPoints(  seeds  )

fastMarching.SetSpeedConstant( 1.0 )


reader.Update()
fastMarching.SetOutputSize( 
  reader.GetOutput().GetBufferedRegion().GetSize() )
writer.Update()

itk.echo(thresholdSegmentation)


InternalWriterType = itk.ImageFileWriter[ InternalImageType ] 

mapWriter = InternalWriterType.New()
mapWriter.SetInput( fastMarching.GetOutput() )
mapWriter.SetFileName("fastMarchingImage.mha")
mapWriter.Update()

speedWriter = InternalWriterType.New()
speedWriter.SetInput( thresholdSegmentation.GetSpeedImage() )
speedWriter.SetFileName("speedTermImage.mha")
speedWriter.Update()




