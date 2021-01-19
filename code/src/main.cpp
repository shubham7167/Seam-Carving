#include<iostream>
#include <opencv2/opencv.hpp>
#include <queue>


using namespace cv;
using namespace std;
int test1[2000][2000];
int test2[2000][2000];

class Dot {
public:
	
	Vec3b pixel;
	bool isTraversed;

	Dot() {

		isTraversed = false;
	}
	
	
};

void vs(Mat &pic,Mat &outpic);
void hs(Mat &pic, Mat &outpic);

int main(int argc,char *argv[]) {

	
	Mat pic1, pic;
	pic = imread(argv[1]);

	int x = pic.cols;
	int y = pic.rows;

	Mat outpic;

	int xn= atoi(argv[2]);
	int yn = atoi(argv[3]);
	
	while (x != xn|| y != yn) {

		if (x != xn) {

			vs(pic, outpic);
			x = x - 1;
			pic = outpic;
		}
		
		if (y != yn) {

			hs(pic, outpic);
			y = y - 1;
			pic = outpic;
		}
	}
	imwrite(argv[4], outpic);
	

	namedWindow("Output", WINDOW_AUTOSIZE);
	imshow("Output", outpic);
	waitKey(0);
	return 0;
}

void hs(Mat &pic, Mat &outpic) 
{

	Mat ipic = pic.clone();
	int y = pic.rows;
	int x = pic.cols;
	outpic = Mat(y -1, x , CV_8UC3);

	Mat src, src_gray;
	Mat grad;
	int scale = 1;
	int delta = 0;
	int ddepth = CV_16S;
	src = pic;


	GaussianBlur(src, src, Size(3, 3), 0, 0, BORDER_DEFAULT);

	cvtColor(src, src_gray, CV_BGR2GRAY);

	Mat grad_x, grad_y;
	Mat abs_grad_x, abs_grad_y;

	Sobel(src_gray, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT);
	convertScaleAbs(grad_x, abs_grad_x);

	Sobel(src_gray, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT);
	convertScaleAbs(grad_y, abs_grad_y);

	std::vector< std::vector<Dot> > p(pic.rows, vector<Dot>(pic.cols, Dot()));

	for (int a = 0; a < y; a++) {

		for (int b = 0; b < x; b++) {

			test1[a][b] = (int)abs_grad_x.at<uchar>(a, b) + (int)abs_grad_y.at<uchar>(a, b);

		}
	}

	for (int a = 0; a < y; a++)
		{

		test2[a][0] = test1[a][0];

	}

	for (int b = 1; b < x; b++) {
		for (int a = 0; a < y; a++) {

			int less;			

			if (a == 0) {

				if (test2[a][b-1] < test2[a+1][b-1]) {
					less = test2[a][b-1];
				}
				else {

					less = test2[a+1][b-1];
				}
				test2[a][b] = less + test1[a][b];
			}
			else

				if (a == y - 1) {

					if (test2[a][b-1] < test2[a-1][b - 1]) {

						less = test2[a][b - 1];
					}
					else {

						less = test2[a - 1][b - 1];
					}
					test2[a][b] = less + test1[a][b];
				}
				else
				{
					if (test2[a][b - 1] < test2[a+1][b-1]) {

						less = test2[a][b-1];

					}
					else {

						less = test2[a+1][b -1];



					}
					if (test2[a - 1][b - 1] < less) {


						less = test2[a - 1][b - 1];

					}
					test2[a][b] = less + test1[a][b];
				}

		}
		
	}

	int indexi = 0;

	int less1 = 1000000;

	for (int a = 0; a < y; a++) {


		if (test2[a][x-1] <= less1) {

			less1 = test2[a][x-1];
			indexi = a;

		}
	}


	for (int b = x - 1; b > 0; b--) {

		p[indexi][b].isTraversed = true; 
		int less = 10000000, a;
		a = indexi;
		if (indexi == 0) {

			if (test2[a][b-1] < test2[a+1][b-1]) {
				indexi = a;
			}
			else {

				indexi = a + 1;

			}
		}
		else
			if (a == y - 1) {

				if (test2[a][b-1] < test2[a - 1][b - 1]) {
					indexi = a;

				}
				else {

					indexi = a - 1;
				}
			}
			else
			{
				if (test2[a][b-1] < test2[a + 1][b - 1]) {

					indexi = a;
					less = test2[a][b-1];
				}
				else {

					indexi = a + 1;
					less = test2[a+1][b-1];
				}
				if (test2[a-1][b - 1] < less) {


					indexi = a - 1;

				}

			}

	}
	p[indexi][0].isTraversed = true;

	for (int b = 0; b < x; b++) {

		int in = 0;
		for (int a = 0; a < y; a++) {
			if (p[a][b].isTraversed == true) {

				continue;
			}
			else {

				outpic.at<Vec3b>(in, b) = ipic.at<Vec3b>(a, b);
				in = in + 1;
			}

		}

	}

}


void vs(Mat &pic, Mat &outpic) 
{

	Mat ipic = pic.clone();
	int y = pic.rows;
	int x = pic.cols;
	outpic = Mat(y, x-1, CV_8UC3);

	Mat src, src_gray;
	Mat grad;
	int scale = 1;
	int delta = 0;
	int ddepth = CV_16S;
	src = pic;

	GaussianBlur(src, src, Size(3, 3), 0, 0, BORDER_DEFAULT);

	cvtColor(src, src_gray, CV_BGR2GRAY);

	Mat grad_x, grad_y;
	Mat abs_grad_x, abs_grad_y;

	Sobel(src_gray, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT);
	convertScaleAbs(grad_x, abs_grad_x);

	Sobel(src_gray, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT);
	convertScaleAbs(grad_y, abs_grad_y);
	
	std::vector< std::vector<Dot> > p(pic.rows, vector<Dot>(pic.cols, Dot()));

	for (int a = 0; a < y; a++) {

		for (int b = 0; b < x; b++) {

			test1[a][b] = (int)abs_grad_x.at<uchar>(a, b) + (int)abs_grad_y.at<uchar>(a, b);

		}
	}

		for (int a = 0; a < x; a++) 
		{
			test2[0][a] = test1[0][a];
		}

		for (int a = 1; a < y; a++) {
			for (int b = 0; b < x; b++) {

				int less;
				

				if (b == 0) {

					if (test2[a - 1][b] < test2[a - 1][b + 1]) {
						less = test2[a - 1][b];
						
					}
					else {

						less = test2[a - 1][b + 1];
						

					}
					test2[a][b] = less + test1[a][b];

				}
				else

					if (b == x - 1) {

						if (test2[a - 1][b] < test2[a - 1][b - 1]) {

							less = test2[a - 1][b];
							

						}
						else {

							less = test2[a - 1][b - 1];
							
						}
						test2[a][b] = less + test1[a][b];

					}
					else
					{
						if (test2[a - 1][b] < test2[a - 1][b + 1]) {

							less = test2[a - 1][b];
							
						}
						else {

							less = test2[a - 1][b + 1];
							


						}
						if (test2[a - 1][b - 1] < less) {


							less = test2[a - 1][b - 1];
							
						}
						test2[a][b] = less + test1[a][b];
					}
			}
		}


		int jin = 0;

		int less1 = 1000000;
		for (int b = 0; b < x-1; b++) {


			if (test2[y - 1][b] <= less1) {

				less1 = test2[y - 1][b];
				jin = b;

			}
		}


		for (int a = y - 1; a > 0; a--) {

			p[a][jin].isTraversed = true;
			int less = 1000000, b;
			b = jin;
			if (jin == 0) {

				if (test2[a - 1][b] < test2[a - 1][b + 1]) 
				{
					jin = b;
				}
				else 
				{
					jin = b + 1;
				}
			}
			else
				if (b == x - 1) 
				{
					if (test2[a - 1][b] < test2[a - 1][b - 1])
					{
						jin = b;
					}
					else
					{
						jin = b - 1;
					}
				}
				else
				{
					if (test2[a - 1][b] < test2[a - 1][b + 1]) {

						jin = b;
						less = test2[a - 1][b];

					}
					else 
					{
						jin = b + 1;
						less = test2[a - 1][b + 1];
					}
					if (test2[a - 1][b - 1] < less)
					{
						jin = b - 1;
					}

				}
		}
		p[0][jin].isTraversed = true;

		for (int a = 0; a < y; a++) {

			int newb = 0;
			for (int b = 0; b < x; b++) {


				if (p[a][b].isTraversed == true) {
				
					continue;
				}
				else {
					outpic.at<Vec3b>(a, newb) = ipic.at<Vec3b>(a, b);
					newb=newb+1;
				}

			}

		}

}



