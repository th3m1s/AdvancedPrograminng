#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#include <cv.h>
#include <highgui.h>

int main(int argc, char **argv) {
	IplImage *src_img = 0;
	IplImage *dst_img_1, *dst_img_2, *dst_img_3;
	char path[80];
	int sw;

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

	// (1)�摜��ǂݍ��݁C�����T�C�Y�̏o�͉摜��p�ӂ���
	if (argc >= 2)
		src_img = cvLoadImage(argv[1], CV_LOAD_IMAGE_COLOR);
	if (src_img == 0)
		return -1;

	dst_img_1 = cvCloneImage(src_img);
	dst_img_2 = cvCloneImage(src_img);
	dst_img_3 = cvCloneImage(src_img);

	// (2)�摜�̐��������]�E���������]�E�������]���s��
	cvFlip(src_img, dst_img_1, 0);
	cvFlip(src_img, dst_img_2, 1);
	cvFlip(src_img, dst_img_3, -1);

	// �w�肵���������ʂ�\��
	printf("������w�肵�Ă�������\n�P�F���������]\n�Q�F���������]��\n�R�F�������]\n");
	printf("����ԍ��̓��́F\n");
	scanf("%d", &sw);

	if (sw == 1) {
		//�E�B���h�E�̐����E�\���E�ۑ�
		cvNamedWindow("src", CV_WINDOW_AUTOSIZE);
		cvNamedWindow("dst", CV_WINDOW_AUTOSIZE);
		cvShowImage("src", src_img);
		cvShowImage("dst", dst_img_1);
		cvSaveImage("flip.jpg", dst_img_1);
	}
	else if (sw==2){
		cvNamedWindow("src", CV_WINDOW_AUTOSIZE);
		cvNamedWindow("dst_1", CV_WINDOW_AUTOSIZE);
		cvShowImage("src", src_img);
		cvShowImage("dst", dst_img_2);
		cvSaveImage("flip.jpg", dst_img_2);
	}
	else if (sw == 3) {
		cvNamedWindow("src", CV_WINDOW_AUTOSIZE);
		cvNamedWindow("dst", CV_WINDOW_AUTOSIZE);
		cvShowImage("src", src_img);
		cvShowImage("dst", dst_img_3);
		cvSaveImage("flip.jpg", dst_img_3);
	}
	else {
		printf("�I�������ԍ����Ԉ���Ă��܂��B\n");
		return -1;
	}

	cvWaitKey(0);
	
	cvDestroyWindow("src");
	cvDestroyWindow("dst");
	cvReleaseImage(&src_img);
	cvReleaseImage(&dst_img_1);
	cvReleaseImage(&dst_img_2);
	cvReleaseImage(&dst_img_3);

	return 1;
}
