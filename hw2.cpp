#include "itkImage.h"
#include "itkImageFileReader.h" 
#include "itkImageFileWriter.h" 
#include "itkResampleImageFilter.h" 
#include "itkAffineTransform.h" 

/**
 * Use ITK capabilities to implement a linear transform program. 
 * Geometrically transform a 3D raw image using translation, 
 * scaling, rotation and similarity transforms. You design these
 * matrices yourself. Outfit the geometrically transformed images. 
 * 
 * Translation: transform object by transforming each vertice. 
 * Scaling: alter any object by a scaling factor. 
 * Rotation: x-roll, y-roll, z-roll. 
 */

int main (int argc, char * argv[]) {

  //make sure that the correct number of arguments have been put in
  if (argc != 3) {
    std::cerr << "Error: " << std::endl;
    std::cerr << "Using: " << argv[0] << std::endl;
    std::cerr << "<InputFileName> <OutputFileName>" << std::endl;
    return EXIT_FAILURE;
  }//if

  //set image file names
  const char * InputFileName = argv[1];
  const char * OutputFileName = argv[2];

  //set dimension, scalar type and pixel type for all transforms
  constexpr unsigned int Dimension = 3;
  using ScalarType = double;
  using PixelType = unsigned char;
  
  //image type
  using ImageType = itk::Image<PixelType, Dimension>;
  ImageType::SizeType size;
  ImageType::PointType origin;
  ImageType::SpacingType spacing;

  //use reader to read input image
  using ReaderType = itk::ImageFileReader<ImageType>;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(InputFileName);
  reader->Update();

  //create affine transform
  using TransformType = itk::AffineTransform<ScalarType, Dimension>;
  TransformType::Pointer affineTransform = TransformType::New();

  //create new resample image filter
  using FilterType = itk::ResampleImageFilter<ImageType, ImageType>;
  FilterType::Pointer resample = FilterType::New();

  size = (reader->GetOutput()->GetLargestPossibleRegion().GetSize());

  //translation transform
  TransformType::OutputVectorType translationVector;
  translationVector[0] = 6.0;
  translationVector[1] = 2.0;
  translationVector[2] = 4.0;
  affineTransform->Translate(translationVector);

  //scale transform in three dimensions
  TransformType::OutputVectorType scaleParams(Dimension);
  scaleParams[0] = 0.5; //x dimension
  scaleParams[1] = 0.75; //y dimension
  scaleParams[2] = 0.9; //z dimension
  affineTransform->Scale(scaleParams);

  TransformType::InputPointType center;
  center[0] = size[0] / 2.0;
  center[1] = size[1] / 2.0;
  center[2] = size[2] / 2.0;
  affineTransform->SetCenter(center);

  const double degreesToRadians = atan(1.0) / 45.0;
  const double angle = 60 * degreesToRadians;

  TransformType::OutputVectorType axis;
  axis[0] = 1;
  axis[1] = 0;
  axis[2] = 0;

  affineTransform->Rotate3D(axis, angle, false);

  resample->SetInput(reader->GetOutput());
  resample->SetTransform(affineTransform);
  resample->SetSize(size);
  resample->Update();

  //image file writer taking input from resample filter
  using WriterType = itk::ImageFileWriter<ImageType>;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(OutputFileName);
  writer->SetInput(resample->GetOutput());

  //try catch for writing output imahe
  try {
    writer->Update();
  }catch(itk::ExceptionObject & error) {
    std::cerr<< "Error: " << error << std::endl;
    return EXIT_FAILURE;
  }//catch
  //return success if no exit failures
  return EXIT_SUCCESS;
}//main
