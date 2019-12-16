#pragma once

#ifndef _TS_TEXSYN_H
#define _TS_TEXSYN_H

/* ========
   マクロ */

   // 入力テクスチャの最大解像度
#define TS_IN_XMAX 128
#define TS_IN_YMAX 128

// 出力テクスチャの最大解像度
#define TS_OUT_XMAX 400
#define TS_OUT_YMAX 400

/* ========
   関数宣言 */

   /* --------
	  多重解像度
	*/
int multiple_resolution(
	int nbr,                // 近隣画素群（neighbor）の大きさ
							//   nbr >= 1
							// 近隣画素群の一辺が 2 * nbr + 1 となる
	int y_level[3],
	int x_level[3],
	int in_cyc,             // 入力テクスチャが環状かどうか
							//   0 : 環状でない
							//   1 : 環状である
	int in_x,               // 入力テクスチャの x 解像度
	int in_y,               // 入力テクスチャの y 解像度
	unsigned char intex[TS_IN_YMAX][TS_IN_XMAX][3],     // 入力テクスチャ
	unsigned char i_l3[TS_IN_YMAX  * 3/4][TS_IN_XMAX * 3/4][3],
	unsigned char i_l2[TS_IN_YMAX / 2][TS_IN_XMAX / 2][3],
	unsigned char i_l1[TS_IN_YMAX / 4][TS_IN_XMAX / 4][3],
	unsigned int out_seed,  // 出力テクスチャの初期化のための乱数の種
	int out_x,              // 出力テクスチャの x 解像度
	int out_y,              // 出力テクスチャの y 解像度
	unsigned char outtex[TS_OUT_YMAX][TS_OUT_XMAX][3]   // 出力テクスチャ
);

#endif
