#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>

#include <cv.h>
#include <highgui.h>

int main(int argc, char **argv) {
	IplImage *src_img = 0, *dst_img = 0;

	char path[80];
	int sw = 0;
	double width, height;
	double scale = 1.0;

	printf("�摜�̃p�X����͂��ĉ������B��̗�F../images/01.bmp\n");/*�摜�p�X���͎w��*/
	scanf("%s", &path);

	/* ���摜��ǂݍ��� */
	src_img = cvLoadImage(path, (CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_ANYCOLOR));
	
	/* �摜�����݂��Ȃ��ꍇ�CNULL���Ԃ�l�Ƃ��ēn����� */
	if (src_img == NULL) {
		/* �摜�����݂��Ȃ��ꍇ */
		printf("�摜�����݂��܂���\n");
		return -1;
	}
	else {
		printf("�摜�̓ǂݍ��݂ɐ������܂����B\n");
	}
	
	printf("������w�肵�Ă�������\n�P�F�C�ӂ̃T�C�Y���w��\n�Q�F�C�ӂ̃X�P�[�����w��\n");
	printf("����ԍ��̓��́F\n");
	scanf("%d",&sw);

	if (sw == 1) {
		printf("�C�ӂ̃T�C�Y���w�肵�ĉ������B\n");
		printf("���̃T�C�Y���́F");
		scanf("%lf", &width);
		printf("�c�̃T�C�Y���́F");
		scanf("%lf", &height);

		//width��height�̒l�����̏ꍇ�̓G���[
		if (width<= 0 ||height<=0) {
			printf("���̒l�͖����ł��B\n");
			return -1;
		}

		//)�摜�̓ǂݍ��݁C�o�͗p�摜�̈�̊m�ۂ��s�Ȃ�
		if (argc >= 2)
			src_img = cvLoadImage(argv[1], CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_ANYCOLOR);
		if (src_img == 0)
			return -1;

		dst_img = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);
	}
	else if (sw == 2) {
		printf("�C�ӂ̔{�����w�肵�ĉ������B\n");
		scanf("%lf",&scale);

		//scale�̒l��0�̏ꍇ�̓G���[
		if (scale == 0) {
			/* scale��0�̏ꍇ */
			printf("scale��0�͖����ł��B\n");
			return -1;
		}

		//)�摜�̓ǂݍ��݁C�o�͗p�摜�̈�̊m�ۂ��s�Ȃ�
		if (argc >= 2) 
			src_img = cvLoadImage(argv[1], CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_ANYCOLOR);
		if (src_img == 0)
			return -1;

		dst_img = cvCreateImage(cvSize(src_img->width*scale, src_img->height*scale), IPL_DEPTH_8U, 3);

	}
	else {
		printf("�I�������ԍ����Ԉ���Ă��܂��B\n");
		return -1;
	}

	//�T�C�Y��ύX
	cvResize(src_img, dst_img, CV_INTER_CUBIC);

	//�\���p�E�B���h�E����
	cvNamedWindow("dst", CV_WINDOW_AUTOSIZE);
	cvShowImage("dst", dst_img);

	//�L�[���͂�҂�
	cvWaitKey(0);

	//JPG�ŕۑ�����
	cvSaveImage("resize.jpg", dst_img);

	/* ���������J������ */
	cvReleaseImage(&src_img);
	cvReleaseImage(&dst_img);

	/* �E�C���h�E��j������ */
	cvDestroyWindow("src");
	cvDestroyWindow("dst");
	
	//���������	
	cvReleaseImage(&src_img);
	cvReleaseImage(&dst_img);

	return 0;
}
