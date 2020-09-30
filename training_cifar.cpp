/* cifar training program in c++ by menascii
   
   compile:
      g++ training_cifar.cpp

   run:
   	  ./a.out

   input:
      hardcoded file name for 60,000 cifar training images  
      download training data from https://www.cs.toronto.edu/~kriz/cifar.html
         train-images.bin

      output:
      	  write updated model weights to file   

  	  note:
  	     this program just reads the cifar training images and print the images 
  	     in a local directory. i will update the convolutional neural network process soon.
*/

#include <iostream>
#include <fstream>
#include <cmath>
#include <iomanip>

using namespace std;

void init_layers(double *&layer_one, double *&layer_two, double *&layer_three);
double** set_weights(int input_layer_size, int output_layer_size);
double ** init_deltas(int input_layer_size, int output_layer_size);
void read_image(ifstream &training_image, double image_object[32][32][3], double *layer_one, double expected[], int &label);
void write_image(double image_object[32][32][3], int image_index, int label);
void training_process(double *layer_one, double *layer_two, double *layer_three, 
                     double **layer_one_weights, double **layer_two_weights,
                     double **layer_one_deltas, double **layer_two_deltas,
                     double *layer_two_thetas, double *layer_three_thetas,
                     double expected[]);

int main()
{
	// cifar binary training images file name                                       
  	string training_images = "train-images.bin";
  	ifstream training_image;

	training_image.open(training_images.c_str(), ios::in | ios::binary);

	// cifar image object 32 x 32 x 3
	// rgb object
	double image_object[32][32][3];
	// expected value as output array
  	// value in array is 7
  	// {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}
	double expected[10];
	int label;

	// neural network layers
  	double *layer_one, *layer_two, *layer_three;
  	// neural network weights
	double **layer_one_weights, **layer_two_weights;
	// neural network derivatives
	double **layer_one_deltas, **layer_two_deltas;
	// neural network biases
	double *layer_two_thetas, *layer_three_thetas;

  	// initialize neural network layers
  	init_layers(layer_one, layer_two, layer_three);

	// seed random number
	srand(time(0));
	// initialize and set neural network weights
	layer_one_weights = set_weights(1024, 256);
	layer_two_weights = set_weights(256, 10);

	// initialize neural network derivatives
	layer_one_deltas = init_deltas(1024, 256);
	layer_two_deltas = init_deltas(256, 10);

	// initialize neural network biases
	layer_two_thetas = new double [256];
	layer_three_thetas = new double [10];


	cout << "............training cifar dataset" << endl;
	for (int image_index = 0; image_index < 10; image_index++)
	{
		cout << "cifar training image #: " << image_index << endl; 

		read_image(training_image, image_object, layer_one, expected, label);
		write_image(image_object, image_index, label);
		training_process(layer_one, layer_two, layer_three, 
                     layer_one_weights, layer_two_weights, 
                     layer_one_deltas, layer_two_deltas,
                     layer_two_thetas, layer_three_thetas,
                     expected);
	}
	return 0;
}

void init_layers(double *&layer_one, double *&layer_two, double *&layer_three)
{
  layer_one = new double[1024];
  layer_two = new double[256];
  layer_three = new double[10];
}

double **set_weights(int input_layer_size, int output_layer_size)
{
  // set weights values from input layer to output layer
  double **layer_weights = new double *[input_layer_size];  
  for (int i = 0; i < input_layer_size; i++)
  {
    layer_weights[i] = new double [output_layer_size];
    for (int j = 0; j < output_layer_size; j++)
    {
      int sign = rand() % 2;
      layer_weights[i][j] = (double)(rand() % 6) / 10.0;
      if (sign == 1)
      {
        layer_weights[i][j] = - layer_weights[i][j];
      }
    }
  }
  return layer_weights;
}

double **init_deltas(int input_layer_size, int output_layer_size)
{
  double **layer_deltas = new double *[input_layer_size];
  for (int i = 0; i < input_layer_size; i++)
  {
    layer_deltas[i] = new double [output_layer_size];
    for (int j = 0; j < output_layer_size; j++)
    {
      layer_deltas[i][j] = 0.0;
    }
  }
  return layer_deltas;
}

void read_image(ifstream &training_image, double image_object[32][32][3], double *layer_one, double expected[], int &label)
{
	// read 32x32 image one character at a time
	char number;
	training_image.read(&number, sizeof(char));
	label = (signed int)number;
	for (int k = 0; k < 3; k++)
	{
		for (int j = 0; j < 32; j++)
		{
			for (int i = 0; i < 32; i++)
			{
				int layer_index = i + j * 32;
				training_image.read(&number, sizeof(char));
				image_object[j][i][k] = (double)number;
			}
		}
	}
}

void write_image(double image_object[32][32][3], int image_index, int label)
{
	string display_images = to_string(label) + "_cifar_image_output.ppm";
	FILE *display_image = fopen(display_images.c_str(), "wb");
	fprintf(display_image, "P6\n%i %i 255\n", 32, 32);
	for (int j = 0; j < 32; j++) 
	{
		for (int i = 0; i < 32; i++) 
		{
	        fputc(image_object[j][i][0], display_image);
	        fputc(image_object[j][i][1], display_image);
	        fputc(image_object[j][i][2], display_image);
		}
	}		
	fclose(display_image);
}

void training_process(double *layer_one, double *layer_two, double *layer_three, 
                     double **layer_one_weights, double **layer_two_weights, 
                     double **layer_one_deltas, double **layer_two_deltas,
                     double *layer_two_thetas, double *layer_three_thetas,
                     double expected[])
{

}
