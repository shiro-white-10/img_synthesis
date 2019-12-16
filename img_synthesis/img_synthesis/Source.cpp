#include <stdio.h>
#include <time.h>
#include <opencv2/opencv.hpp>
#include "Header.h"

using namespace cv;

char TS_folder[] = "./textures/";
//char TS_infile[] = "texture_o_icon.jpg";
//char TS_infile[] = "S27_m.jpg";
//char TS_infile[] = "83.jpg";
char TS_infile[] = "S28_m.jpg";
char TS_outfile[] = "texture_o_icon_out.jpg";

int TS_nbr = 7;

char TS_in_cyc = 1;

int TS_in_x;
int TS_in_y;

unsigned int TS_out_seed = 1;

int TS_out_x = 192;
int TS_out_y = 192;

int main() {
	unsigned char intex[TS_IN_YMAX][TS_IN_XMAX][3];
	unsigned char i_l3[TS_IN_YMAX *  3 / 4][TS_IN_XMAX  * 3/ 4][3];
	unsigned char i_l2[TS_IN_YMAX / 2][TS_IN_XMAX / 2][3];
	unsigned char i_l1[TS_IN_YMAX / 4][TS_IN_XMAX /4][3];
	Mat intex_cv;

	unsigned char outtex[TS_OUT_YMAX][TS_OUT_XMAX][3];

	Mat outtex_cv;

	Mat in_level1, in_level2, in_level3;
	Mat out_level1, out_level2, out_level3;

	char file[512];

	clock_t time_start, time_end;
	double  time_sec;
	int     time_m;
	double  time_s;

	printf("main() ----> start\n");
	time_start = clock();

	strcpy_s(file, sizeof(file), TS_folder);
	strcat_s(file, sizeof(file), TS_infile);
	intex_cv = imread(file);
	TS_in_x = intex_cv.cols;
	TS_in_y = intex_cv.rows;
	namedWindow("input", CV_WINDOW_AUTOSIZE);
	imshow("input", intex_cv);

	printf("main() : TS_infile >> %s\n", TS_infile);
	printf("intex_cv.cols, rows = %d, %d\n", intex_cv.cols, intex_cv.rows);

	
	resize(intex_cv, in_level3,Size(),0.75,0.75 );
	resize(intex_cv, in_level2,Size(),0.5,0.5);
	resize(intex_cv, in_level1,Size(),0.25,0.25);
	printf("in_level3.cols, rows = %d, %d\n", in_level3.cols, in_level3.rows);
	printf("in_level2.cols, rows = %d, %d\n", in_level2.cols, in_level2.rows);
	printf("in_level1.cols, rows = %d, %d\n", in_level1.cols, in_level1.rows);
	imshow("in_level3", in_level3); imwrite("textures/in_level3.jpg", in_level3);
	imshow("in_level2", in_level2); imwrite("textures/in_level2.jpg", in_level2);
	imshow("in_level1", in_level1); imwrite("textures/in_level1.jpg", in_level1);


	for (int y = 0; y < TS_in_y; y++) {
		for (int x = 0; x < TS_in_x; x++) {
			for (int c = 0; c < 3; c++) {
				intex[y][x][c] = intex_cv.at<Vec3b>(y, x)[c];
			}
		}
	}

	for (int y = 0; y < in_level3.rows; y++) {
		for (int x = 0; x < in_level3.cols; x++) {
			for (int c = 0; c < 3; c++) {
				i_l3[y][x][c] = in_level3.at<Vec3b>(y, x)[c];
			}
		}
	}
	for (int y = 0; y < in_level2.rows; y++) {
		for (int x = 0; x < in_level2.cols; x++) {
			for (int c = 0; c < 3; c++) {
				i_l2[y][x][c] = in_level2.at<Vec3b>(y, x)[c];
			}
		}
	}

	for (int y = 0; y < in_level1.rows; y++) {
		for (int x = 0; x < in_level1.cols; x++) {
			for (int c = 0; c < 3; c++) {
				i_l1[y][x][c] = in_level1.at<Vec3b>(y, x)[c];
			}
		}
	}
	int y_level[3],x_level[3];
	y_level[0] = in_level1.rows; x_level[0] = in_level1.cols;
	y_level[1] = in_level2.rows; x_level[1] = in_level2.cols;
	y_level[2] = in_level3.rows; x_level[2] = in_level3.cols;


	/* ‘½d‰ð‘œ“x*/
	if (multiple_resolution(
		TS_nbr,y_level,x_level,
		TS_in_cyc, TS_in_x, TS_in_y, intex,i_l3,i_l2,i_l1,
		TS_out_seed, TS_out_x, TS_out_y, outtex) != 0) {
		printf("err : main() -> multiple_resolution()\n");
		return(1);
	}
	outtex_cv = Mat(TS_out_y, TS_out_x, CV_8UC3);
	for (int y = 0; y < TS_out_y; y++) {
		for (int x = 0; x < TS_out_x; x++) {
			for (int c = 0; c < 3; c++) {
				outtex_cv.at<Vec3b>(y, x)[c] = outtex[y][x][c];
			}
		}
	}

	strcpy_s(file, sizeof(file), TS_folder);
	strcat_s(file, sizeof(file), TS_outfile);
	imwrite(file, outtex_cv);
	namedWindow("output", CV_WINDOW_AUTOSIZE);
	imshow("output", outtex_cv);

	printf("main() : TS_outfile >> %s\n", TS_outfile);
	printf("outtex_cv.cols, rows = %d, %d\n", outtex_cv.cols, outtex_cv.rows);

	time_end = clock();
	time_sec = (double)(time_end - time_start) / (double)CLOCKS_PER_SEC;
	time_m = (int)(time_sec / 60.0);
	time_s = time_sec - (double)time_m * 60.0;
	printf("main() : time >> %lf sec. = %d min. %lf sec.\n", time_sec, time_m, time_s);
	printf("main() <---- end\n");

	waitKey(0);

	return(0);

}