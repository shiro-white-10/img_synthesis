#pragma once

#ifndef _TS_TEXSYN_H
#define _TS_TEXSYN_H

/* ========
   �}�N�� */

   // ���̓e�N�X�`���̍ő�𑜓x
#define TS_IN_XMAX 128
#define TS_IN_YMAX 128

// �o�̓e�N�X�`���̍ő�𑜓x
#define TS_OUT_XMAX 400
#define TS_OUT_YMAX 400

/* ========
   �֐��錾 */

   /* --------
	  ���d�𑜓x
	*/
int multiple_resolution(
	int nbr,                // �ߗ׉�f�Q�ineighbor�j�̑傫��
							//   nbr >= 1
							// �ߗ׉�f�Q�̈�ӂ� 2 * nbr + 1 �ƂȂ�
	int y_level[3],
	int x_level[3],
	int in_cyc,             // ���̓e�N�X�`�����󂩂ǂ���
							//   0 : ��łȂ�
							//   1 : ��ł���
	int in_x,               // ���̓e�N�X�`���� x �𑜓x
	int in_y,               // ���̓e�N�X�`���� y �𑜓x
	unsigned char intex[TS_IN_YMAX][TS_IN_XMAX][3],     // ���̓e�N�X�`��
	unsigned char i_l3[TS_IN_YMAX  * 3/4][TS_IN_XMAX * 3/4][3],
	unsigned char i_l2[TS_IN_YMAX / 2][TS_IN_XMAX / 2][3],
	unsigned char i_l1[TS_IN_YMAX / 4][TS_IN_XMAX / 4][3],
	unsigned int out_seed,  // �o�̓e�N�X�`���̏������̂��߂̗����̎�
	int out_x,              // �o�̓e�N�X�`���� x �𑜓x
	int out_y,              // �o�̓e�N�X�`���� y �𑜓x
	unsigned char outtex[TS_OUT_YMAX][TS_OUT_XMAX][3]   // �o�̓e�N�X�`��
);

#endif
