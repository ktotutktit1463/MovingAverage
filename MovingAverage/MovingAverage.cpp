// MovingAverage.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define TRUE 1
#define FALSE 0
#define MAXRECORDS 1000


#pragma warning(disable : 4996)


/* 渡された名前のファイルを移動平均をとって上書き保存する */
/* 第1引数:ファイル名　第2引数:平均するデータ数 */
int MovingAverage(char *filename, int M);


int _tmain(int argc, _TCHAR* argv[])
{

	FILE *fp;
	char filename[20], tempin[20];
	char i_char[5];
	int i;
	int M, FileNum;
	char a;


	printf("Kinectモーションデータの移動平均のプログラム\n");
	printf("ファイルは上書き保存されるので元のファイルは必ずバックアップをとっておくこと\n");
	printf("入力ファイルのヘッダーは2行とすること\n");
	printf("入力ファイルはcsv形式とし、フォーマットはsample.csvを参照\n");
	printf("ファイル名を連番(1～n)にしておくと連続で処理します\n\n");


	printf("入力ファイル名（連番の数字以外の部分） -> ");
	scanf("%s", filename);
	strcpy(tempin, filename);

	printf("ファイル数 -> ");
	scanf("%d", &FileNum);

	printf("平均するデータ数 -> ");
	scanf("%d", &M);


	for (i = 1; i <= FileNum; i++) {

		/* ファイル名を完成させる */
		strcpy(filename, tempin);
		sprintf(i_char, "%d", i);
		strcat(filename, i_char);
		strcat(filename, ".csv");


		if (MovingAverage(filename, M) == FALSE) {
			printf("%s の処理に失敗しました。\n", filename);
		}
		else {
			printf("%s の処理に成功しました。\n", filename);
		}
		
	}


	return 0;
}




int MovingAverage(char *filename, int M) {

	int i, j, k, flag;
	int RecordNum;
	int id[MAXRECORDS];
	double x[MAXRECORDS][20], y[MAXRECORDS][20], z[MAXRECORDS][20];
	double averageX, averageY, averageZ;
	char header1[256], header2[256];
	FILE *fp;


	if ((fp = fopen(filename, "r")) == NULL) {
		return FALSE;
	}


	/* 全てのデータを読み込んで配列に格納 */
	j = 0;
	i = 0;
	flag = 0;
	fscanf(fp, "%s", header1);
	fscanf(fp, "%s", header2);

	while (flag == 0) {

		/* idの読み込み */
		if (j == 0) {

			/* idが読み込めなかったら終了 */
			if (fscanf(fp, "%d,", &id[i]) == EOF) flag = 1;
			j++;

		}
		/* １行全部読み込んだらiをインクリメント */
		else if (j == 21) {
			j = 0;
			i++;
		}
		else {
			fscanf(fp, "%lf,%lf,%lf,", &x[i][j-1], &y[i][j-1], &z[i][j-1]);
			j++;
		}

	}
	RecordNum = i;
	fclose(fp);


	/* calculate average and save */
	fp = fopen(filename, "w");
	fprintf(fp, "%s\n", header1);
	fprintf(fp, "%s\n", header2);


	for (i = 0; i < RecordNum; i++) {
		for (j = 0; j < 22; j++) {

			if (j == 0) {
				fprintf(fp, "%d", id[i]);
			}

			else if (j == 21) {
				fprintf(fp, "\n");
			}
			else {

				/* 平均を出せない最初のレコード */
				if (i < M / 2) {
					fprintf(fp, ",%lf,%lf,%lf", x[i][j-1], y[i][j-1], z[i][j-1]);
				}

				/* 平均を出せない最後のレコード */
				else if (i >= RecordNum - M / 2) {
					fprintf(fp, ",%lf,%lf,%lf", x[i][j-1], y[i][j-1], z[i][j-1]);
				}

				/* calculate average and save */
				else {
					averageX = averageY = averageZ = 0;
					for (k = -(M / 2); k <= (M / 2); k++) {
						averageX += x[i + k][j-1];
						averageY += y[i + k][j-1];
						averageZ += z[i + k][j-1];
					}
					averageX = averageX / M;
					averageY = averageY / M;
					averageZ = averageZ / M;
					fprintf(fp, ",%lf,%lf,%lf", averageX, averageY, averageZ);
				}
			}

		}
	}

	fclose(fp);

	return TRUE;
}