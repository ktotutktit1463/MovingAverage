// MovingAverage.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define TRUE 1
#define FALSE 0
#define MAXRECORDS 1000


#pragma warning(disable : 4996)


/* �n���ꂽ���O�̃t�@�C�����ړ����ς��Ƃ��ď㏑���ۑ����� */
/* ��1����:�t�@�C�����@��2����:���ς���f�[�^�� */
int MovingAverage(char *filename, int M);


int _tmain(int argc, _TCHAR* argv[])
{

	FILE *fp;
	char filename[20], tempin[20];
	char i_char[5];
	int i;
	int M, FileNum;
	char a;


	printf("Kinect���[�V�����f�[�^�̈ړ����ς̃v���O����\n");
	printf("�t�@�C���͏㏑���ۑ������̂Ō��̃t�@�C���͕K���o�b�N�A�b�v���Ƃ��Ă�������\n");
	printf("���̓t�@�C���̃w�b�_�[��2�s�Ƃ��邱��\n");
	printf("���̓t�@�C����csv�`���Ƃ��A�t�H�[�}�b�g��sample.csv���Q��\n");
	printf("�t�@�C������A��(1�`n)�ɂ��Ă����ƘA���ŏ������܂�\n\n");


	printf("���̓t�@�C�����i�A�Ԃ̐����ȊO�̕����j -> ");
	scanf("%s", filename);
	strcpy(tempin, filename);

	printf("�t�@�C���� -> ");
	scanf("%d", &FileNum);

	printf("���ς���f�[�^�� -> ");
	scanf("%d", &M);


	for (i = 1; i <= FileNum; i++) {

		/* �t�@�C���������������� */
		strcpy(filename, tempin);
		sprintf(i_char, "%d", i);
		strcat(filename, i_char);
		strcat(filename, ".csv");


		if (MovingAverage(filename, M) == FALSE) {
			printf("%s �̏����Ɏ��s���܂����B\n", filename);
		}
		else {
			printf("%s �̏����ɐ������܂����B\n", filename);
		}
		
	}


	return 0;
}




int MovingAverage(char *filename, int M) {

	int i, j, k;
	int RecordNum;
	double x[MAXRECORDS][20], y[MAXRECORDS][20], z[MAXRECORDS][20];
	double averageX, averageY, averageZ;
	char header1[256], header2[256];
	FILE *fp;


	if ((fp = fopen(filename, "r")) == NULL) {
		return FALSE;
	}


	/* �S�Ẵf�[�^��ǂݍ���Ŕz��Ɋi�[ */
	j = 0;
	i = 0;
	fscanf(fp, "%s", header1);
	fscanf(fp, "%s", header2);
	while ( fscanf(fp, "%lf,%lf,%lf,", &x[i][j], &y[i][j], &z[i][j]) != EOF) {

		if (j == 19) {
			j = 0;
			i++;
		}
		else {
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
		for (j = 0; j < 20; j++) {

			if (j != 0) fprintf(fp, ",");

			/* ���ς��o���Ȃ��ŏ��̃��R�[�h */
			if (i < M / 2) {
				fprintf(fp, "%lf,%lf,%lf", x[i][j], y[i][j], z[i][j]);
			}

			/* ���ς��o���Ȃ��Ō�̃��R�[�h */
			else if (i >= RecordNum - M / 2) {
				fprintf(fp, "%lf,%lf,%lf", x[i][j], y[i][j], z[i][j]);
			}

			/* calculate average and save */
			else {
				averageX = averageY = averageZ = 0;
				for (k = -(M / 2); k <= (M / 2); k++) {
					averageX += x[i + k][j];
					averageY += y[i + k][j];
					averageZ += z[i + k][j];
				}
				averageX = averageX / M;
				averageY = averageY / M;
				averageZ = averageZ / M;
				fprintf(fp, "%lf,%lf,%lf", averageX, averageY, averageZ);
			}

			if (j == 19) fprintf(fp, "\n");
		}
	}

	fclose(fp);

	return TRUE;
}