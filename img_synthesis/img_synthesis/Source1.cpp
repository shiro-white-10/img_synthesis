#include <stdio.h>
#include <stdlib.h>
#include "Header.h"
#include <opencv2/opencv.hpp>

using namespace cv;

int multiple_resolution(
	int nbr,
	int y_level[3],
	int x_level[3],
	int in_cyc,
	int in_x,
	int in_y,
	unsigned char intex[TS_IN_YMAX][TS_IN_XMAX][3],
	unsigned char i_l3[TS_IN_YMAX * 3 / 4][TS_IN_XMAX * 3 / 4][3],
	unsigned char i_l2[TS_IN_YMAX / 2][TS_IN_XMAX / 2][3],
	unsigned char i_l1[TS_IN_YMAX / 4][TS_IN_XMAX / 4][3],
	unsigned int out_seed,
	int out_x,
	int out_y,
	unsigned char outtex[TS_OUT_YMAX][TS_OUT_XMAX][3]
)
{
	/* ========
   初期処理 */
	
	//printf("y_level[0] = %d\n", y_level[0]);


   // 入力テクスチャの探索範囲
	int in_x_min, in_x_max;
	int in_y_min, in_y_max;

	int in_x_min_l1, in_x_max_l1;
	int in_y_min_l1, in_y_max_l1;

	int in_x_min_l2, in_x_max_l2;
	int in_y_min_l2, in_y_max_l2;

	int in_x_min_l3, in_x_max_l3;
	int in_y_min_l3, in_y_max_l3;

	if (in_cyc == 0) { // 環状でない
		in_x_min = nbr;
		in_x_max = in_x - nbr;
		in_y_min = nbr;
		in_y_max = in_y;

		in_x_min_l1 = nbr;
		in_x_max_l1 = x_level[0] - nbr;
		in_y_min_l1 = nbr;
		in_y_max_l1 = y_level[0] - nbr;

		in_x_min_l2 = nbr;
		in_x_max_l2 = x_level[1] - nbr;
		in_y_min_l2 = nbr;
		in_y_max_l2 = y_level[1] - nbr;

		in_x_min_l3 = nbr;
		in_x_max_l3 = x_level[2] - nbr;
		in_y_min_l3 = nbr;
		in_y_max_l3 = y_level[2] - nbr;
	}
	else {              // 環状である
		in_x_min = 0;
		in_x_max = in_x;
		in_y_min = 0;
		in_y_max = in_y;

		in_x_min_l1 = 0;
		in_x_max_l1 = x_level[0];
		in_y_min_l1 = 0;
		in_y_max_l1 = y_level[0];

		in_x_min_l2 = 0;
		in_x_max_l2 = x_level[1];
		in_y_min_l2 = 0;
		in_y_max_l2 = y_level[1];

		in_x_min_l3 = 0;
		in_x_max_l3 = x_level[2];
		in_y_min_l3 = 0;
		in_y_max_l3 = y_level[2];
	}

	

	//各レベルの出力用
	Mat out_level1, out_level2, out_level3;
	unsigned char o_level1[TS_OUT_YMAX / 4][TS_OUT_XMAX / 4][3], o_level2[TS_OUT_YMAX  /2][TS_OUT_XMAX / 2][3], o_level3[TS_OUT_YMAX * 3 / 4][TS_OUT_XMAX  * 3/ 4][3];

	int out_y_level[3];
	int out_x_level[3];

	out_y_level[0] = out_y / 4; out_y_level[1] = out_y / 2; out_y_level[2] = out_y * 3 / 4;
	out_x_level[0] = out_x / 4; out_x_level[1] = out_x / 2; out_x_level[2] = out_x * 3 / 4;

	srand(out_seed);



	//o_level1~3の初期化	
	for (int oy = 0; oy < out_y_level[0]; oy++) {
		for (int ox = 0; ox < out_x_level[0]; ox++) {
			for (int c = 0; c < 3; c++) {
				o_level1[oy][ox][c] = i_l1[rand() % in_y][rand() % in_x][c];
			}
		}
	}

	for (int oy = 0; oy < out_y_level[1]; oy++) {
		for (int ox = 0; ox < out_x_level[1]; ox++) {
			for (int c = 0; c < 3; c++) {
				o_level2[oy][ox][c] = i_l2[rand() % in_y][rand() % in_x][c];
			}
		}
	}

	for (int oy = 0; oy < out_y_level[2]; oy++) {
		for (int ox = 0; ox < out_x_level[2]; ox++) {
			for (int c = 0; c < 3; c++) {
				o_level3[oy][ox][c] = i_l3[rand() % in_y][rand() % in_x][c];
			}
		}
	}

	//出力テクスチャの初期化
	for (int oy = 0; oy < out_y; oy++) {
		for (int ox = 0; ox < out_x; ox++) {
			for (int c = 0; c < 3; c++) {
				outtex[oy][ox][c] = intex[rand() % in_y][rand() % in_x][c];
			}
		}
	}

	int pxn = ((2 * nbr + 1) * (2 * nbr + 1)) / 2;

	int pxn2 = ((2 * nbr + 1) * (2 * nbr + 1)) - 1;

	printf("multiple_resolution() : nbr = %d, pxn = %d\n", nbr, pxn);


	int ssd_max = 3 * 255 * 255 * pxn;
	
	printf("%d\n",in_y_max_l1);

	//level1
	for (int oy = 0; oy < out_y_level[0]; oy++) {
		for (int ox = 0; ox < out_x_level[0]; ox++) {
			int ssd_min = ssd_max;
			//printf("oy = %d,ox = %d",oy,ox);
			int ix_s = -1;
			int iy_s = -1;

			for (int iy = in_y_min_l1; iy < in_y_max_l1; iy++) {
				for (int ix = in_x_min_l1; ix < in_x_max_l1; ix++) {
					int s;
					int ssd = 0;
					for (int ny = (-nbr); ny <= 0; ny++) {
						for (int nx = (-nbr); nx <= 0; nx++) {
							if ((ny == 0) && (nx == 0)) {
								break;
							}

							for (int c = 0; c < 3; c++) {
								s = (int)o_level1[(oy + ny + out_y_level[0]) % out_y_level[0]][(ox + nx + out_x_level[0]) % out_x_level[0]][c]
									- (int)i_l1[(iy + ny + y_level[0]) % y_level[0]][(ix + nx + x_level[0]) % x_level[0]][c];
								ssd += (s*s);
								//printf("s = %d\n", s);
							}
						}
					}

					if (ssd_min > ssd) {
						ssd_min = ssd;
						ix_s = ix;
						iy_s = iy;
					}


				}//ix

			}//iy
			
			if (ssd_min == ssd_max) {
				for (int c = 0; c < 3; c++) {
					o_level1[oy][ox][c] = 0;
				}
			}
			else {
				for (int c = 0; c < 3; c++) {
					o_level1[oy][ox][c] = i_l1[iy_s][ix_s][c];
				}
			}
		}
	}

	out_level1 = Mat(out_y_level[0], out_x_level[0], CV_8UC3);
	for (int y = 0; y < out_y_level[0]; y++) {
		for (int x = 0; x < out_x_level[0]; x++) {
			for (int c = 0; c < 3; c++) {
				out_level1.at<Vec3b>(y, x)[c] = o_level1[y][x][c];
			}
		}
	}
	imshow("out_level1",out_level1);
	cv::imwrite("textures/out_level1.jpg", out_level1);
	printf("finished level1\n");

	//level2
	for (int oy = 0; oy < out_y_level[1]; oy++) {
		for (int ox = 0; ox < out_x_level[1]; ox++) {
			//printf("oy = %d,ox = %d\n", oy, ox);
			int ssd_min = ssd_max;

			int ix_s = -1;
			int iy_s = -1;
			int ssd_frag = -1;

			for (int iy = in_y_min_l2; iy < in_y_max_l2; iy++) {
				for (int ix = in_x_min_l2; ix < in_x_max_l2; ix++) {
					int s;
					int ssd = 0;

					for (int ny = (-nbr); ny <= 0; ny++) {
						for (int nx = (-nbr); nx <= 0; nx++) {
							if ((ny == 0) && (nx == 0)) {
								break;
							}

							for (int c = 0; c < 3; c++) {
								s = (int)o_level2[(oy + ny + out_y_level[1]) % out_y_level[1]][(ox + nx + out_x_level[1]) % out_x_level[1]][c]
									- (int)i_l2[(iy + ny + y_level[1]) % y_level[1]][(ix + nx + x_level[1]) % x_level[1]][c];
								ssd += (s*s);
								
							}
						}
					}
					ssd = ssd / pxn / pxn;

					if (ssd_min > ssd) {
						ssd_min = ssd;

						ix_s = ix;
						iy_s = iy;
						ssd_frag = 0;
					}


				}//ix

			}//iy

			for (int oy_level1 = 0; oy_level1 < out_y_level[0]; oy_level1++) {
				for (int ox_level1 = 0; ox_level1 < out_x_level[0]; ox_level1++) {
					int s;
					int ssd = 0;

					for (int ny = (-nbr); ny <= nbr; ny++) {
						for (int nx = (-nbr); nx <= nbr; nx++) {
							if ((ny == 0) && (nx == 0)) {
							}
							else {
								for (int c = 0; c < 3; c++) {
									s = (int)o_level2[(oy + ny + out_y_level[1]) % out_y_level[1]][(ox + nx + out_x_level[1]) % out_x_level[1]][c]
										- (int)o_level1[(oy_level1 + ny + out_y_level[0]) % out_y_level[0]][(ox_level1 + nx + out_x_level[0]) % out_x_level[0]][c];
									ssd += (s*s);
								}
							}
						}
					}
					ssd = ssd / pxn2 / pxn2;
					if (ssd_min > ssd) {
						ssd_min = ssd;
						ix_s = ox_level1;
						iy_s = oy_level1;
						ssd_frag = 1;
					}
				}//ox_level1
			}//oy_level1
		
			if (ssd_frag == 0) {
				if (ssd_min == ssd_max) {
					for (int c = 0; c < 3; c++) {
						o_level2[oy][ox][c] = 0;
					}
				}
				else {
					for (int c = 0; c < 3; c++) {
						o_level2[oy][ox][c] = i_l2[iy_s][ix_s][c];
					}
				}
			}
			else if (ssd_frag == 1) {
				if (ssd_min == ssd_max) {
					for (int c = 0; c < 3; c++) {
						o_level2[oy][ox][c] = 0;
					}
				}
				else {
					for (int c = 0; c < 3; c++) {
						o_level2[oy][ox][c] = o_level1[iy_s][ix_s][c];
					}
				}
			}//else if
			//printf("ssd_frag = %d", ssd_frag);
		}

	}
	out_level2 = Mat(out_y_level[1], out_x_level[1], CV_8UC3);
	for (int y = 0; y < out_y_level[1]; y++) {
		for (int x = 0; x < out_x_level[1]; x++) {
			for (int c = 0; c < 3; c++) {
				out_level2.at<Vec3b>(y, x)[c] = o_level2[y][x][c];
			}
		}
	}
	imshow("out_level2", out_level2);
	cv::imwrite("textures/out_level2.jpg", out_level2);
	printf("finished level2\n");
	

	//level3
	for (int oy = 0; oy < out_y_level[2]; oy++) {
		for (int ox = 0; ox < out_x_level[2]; ox++) {
			//printf("oy = %d,ox = %d\n", oy, ox);
			int ssd_min = ssd_max;

			int ix_s = -1;
			int iy_s = -1;
			int ssd_frag = -1;

			for (int iy = in_y_min_l3; iy < in_y_max_l3; iy++) {
				for (int ix = in_x_min_l3; ix < in_x_max_l3; ix++) {
					int s;
					int ssd = 0;

					for (int ny = (-nbr); ny <= 0; ny++) {
						for (int nx = (-nbr); nx <= 0; nx++) {
							if ((ny == 0) && (nx == 0)) {
								break;
							}

							for (int c = 0; c < 3; c++) {
								s = (int)o_level3[(oy + ny + out_y_level[2]) % out_y_level[2]][(ox + nx + out_x_level[2]) % out_x_level[2]][c]
									- (int)i_l3[(iy + ny + y_level[2]) % y_level[2]][(ix + nx + x_level[2]) % x_level[2]][c];
								ssd += (s*s);

							}
						}
					}
					ssd = ssd / pxn / pxn;

					if (ssd_min > ssd) {
						ssd_min = ssd;

						ix_s = ix;
						iy_s = iy;
						ssd_frag = 0;
					}


				}//ix

			}//iy

			for (int oy_level2 = 0; oy_level2 < out_y_level[1]; oy_level2++) {
				for (int ox_level2 = 0; ox_level2 < out_x_level[1]; ox_level2++) {
					int s;
					int ssd = 0;

					for (int ny = (-nbr); ny <= nbr; ny++) {
						for (int nx = (-nbr); nx <= nbr; nx++) {
							if ((ny == 0) && (nx == 0)) {
							}
							else {
								for (int c = 0; c < 3; c++) {
									s = (int)o_level3[(oy + ny + out_y_level[2]) % out_y_level[2]][(ox + nx + out_x_level[2]) % out_x_level[2]][c]
										- (int)o_level2[(oy_level2 + ny + out_y_level[1]) % out_y_level[1]][(ox_level2 + nx + out_x_level[1]) % out_x_level[1]][c];
									ssd += (s*s);
								}
							}
						}
					}
					ssd = ssd / pxn2 / pxn2;
					if (ssd_min > ssd) {
						ssd_min = ssd;
						ix_s = ox_level2;
						iy_s = oy_level2;
						ssd_frag = 1;
					}
				}//ox_level2
			}//oy_level2

			if (ssd_frag == 0) {
				if (ssd_min == ssd_max) {
					for (int c = 0; c < 3; c++) {
						o_level3[oy][ox][c] = 0;
					}
				}
				else {
					for (int c = 0; c < 3; c++) {
						o_level3[oy][ox][c] = i_l3[iy_s][ix_s][c];
					}
				}
			}
			else if (ssd_frag == 1) {
				if (ssd_min == ssd_max) {
					for (int c = 0; c < 3; c++) {
						o_level3[oy][ox][c] = 0;
					}
				}
				else {
					for (int c = 0; c < 3; c++) {
						o_level3[oy][ox][c] = o_level2[iy_s][ix_s][c];
					}
				}
			}//else if
			//printf("ssd_frag = %d", ssd_frag);
		}

	}
	out_level3 = Mat(out_y_level[2], out_x_level[2], CV_8UC3);
	for (int y = 0; y < out_y_level[2]; y++) {
		for (int x = 0; x < out_x_level[2]; x++) {
			for (int c = 0; c < 3; c++) {
				out_level3.at<Vec3b>(y, x)[c] = o_level3[y][x][c];
			}
		}
	}
	imshow("out_level3", out_level3);
	cv::imwrite("textures/out_level3.jpg", out_level3);
	printf("finished level3\n");


	for (int oy = 0; oy < out_y; oy++) {
		for (int ox = 0; ox < out_x; ox++) {
			
			 int ssd_min = ssd_max;

			int ix_s = -1;
			int iy_s = -1;
			int ssd_frag = -1;

			for (int iy = in_y_min; iy < in_y_max; iy++) {
				for (int ix = in_x_min; ix < in_x_max; ix++) {

					int s;
					int ssd = 0;

					

					for (int ny = (-nbr); ny <= 0; ny++) {
						for (int nx = (-nbr); nx <= nbr; nx++) {

							if ((ny == 0) && (nx == 0)) {
								break;
							}

							for (int c = 0; c < 3; c++) {
								s = (int)outtex[(oy + ny + out_y) % out_y][(ox + nx + out_x) % out_x][c]
									- (int)intex[(iy + ny + in_y) % in_y][(ix + nx + in_x) % in_x][c];
								ssd += (s * s);
							}
						}
					}

					ssd = ssd / pxn / pxn;

							if (ssd_min > ssd) {
								ssd_min = ssd;
								ix_s = ix;
								iy_s = iy;

								ssd_frag = 0;
							}



				}//ix
			}//iy

			for (int oy_level3 = 0; oy_level3 < out_y_level[2]; oy_level3++) {
				for (int ox_level3 = 0; ox_level3 < out_x_level[2]; ox_level3++) {
					int s;
					int ssd = 0;

					for (int ny = (-nbr); ny <= nbr; ny++) {
						for (int nx = (-nbr); nx <= nbr; nx++) {
							if ((ny == 0) && (nx == 0)) {
							}
							else {
								for (int c = 0; c < 3; c++) {
									s = (int)outtex[(oy + ny + out_y) % out_y][(ox + nx + out_x) % out_x][c]
										- (int)o_level3[(oy_level3 + ny + out_y_level[2]) % out_y_level[2]][(ox_level3 + nx + out_x_level[2]) % out_x_level[2]][c];
									ssd += (s*s);
								}
							}
						}
					}
					ssd = ssd / pxn2 / pxn2;
					if (ssd_min > ssd) {
						ssd_min = ssd;
						ix_s = ox_level3;
						iy_s = oy_level3;
						ssd_frag = 1;
					}
				}//ox_level2
			}//oy_level2




			if (ssd_frag == 0) {
				if (ssd_min == ssd_max) {
					for (int c = 0; c < 3; c++) {
						outtex[oy][ox][c] = 0;
					}
				}
				else {
					for (int c = 0; c < 3; c++) {
						outtex[oy][ox][c] = intex[iy_s][ix_s][c];

					}
				}
			}
			else if (ssd_frag == 1) {
				if (ssd_min == ssd_max) {
					for (int c = 0; c < 3; c++) {
						outtex[oy][ox][c] = 0;
					}
				}
				else {
					for (int c = 0; c < 3; c++) {
						outtex[oy][ox][c] = o_level3[iy_s][ix_s][c];
					}
				}
			}
			//printf("ssd_frag = %d", ssd_frag);
		}//ox


	}//oy

	return(0);

}











