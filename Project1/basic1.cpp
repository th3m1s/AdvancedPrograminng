#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

//OpenCV�p�̃w�b�_�t�@�C��
#include <cv.h>  
#include <highgui.h>

//�֐��̒�`
void flip(IplImage* &src_img, IplImage* &save_img);

int main(int argc, char*argv[]) {
	IplImage *src_img = 0;
	IplImage *save_img = 0;
	char path[80];
	char save[80];

	//�摜�p�X���͎w��
	printf("�摜�̃p�X����͂��ĉ������B��̗�F../images/01.bmp\n");
	scanf("%s", &path);

	//���摜��ǂݍ���
	src_img = cvLoadImage(path, (CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_ANYCOLOR));

	/* �摜�����݂��Ȃ��ꍇ�CNULL���Ԃ�l�Ƃ��ēn����� */
	if (src_img == NULL) {
		/* �摜�����݂��Ȃ��ꍇ */
		printf("�摜�����݂��܂���\n");
		return -1;
	}else {
		printf("�摜�̓ǂݍ��݂ɐ������܂����B\n");
	}

	// (1)�摜��ǂݍ��݁C�����T�C�Y�̏o�͉摜��p�ӂ���
	if (argc >= 2)
		src_img = cvLoadImage(argv[1], CV_LOAD_IMAGE_COLOR);
	if (src_img == 0)
		return -1;
	
	flip(src_img,save_img);

	cvWaitKey(0);

	cvNamedWindow("src", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("dst", CV_WINDOW_AUTOSIZE);
	cvShowImage("src", src_img);
	cvShowImage("dst", save_img);

	//�ۑ���C�ӂ̖��O�Őݒ�
	printf("�ۑ��p�̖��O��ݒ肵�ĉ������B��̗�Fsave.jpg\n");
	scanf("%s", &save);
	cvSaveImage(save, save_img);

	
	//printf("�摜�̓ǂݍ��݂ɐ������܂����B\n��END�Ɠ��͂���Ə������I�����܂�\n");

	cvDestroyWindow("src");
	cvDestroyWindow("dst");
	cvReleaseImage(&src_img);
	cvReleaseImage(&save_img);

	exit(0);
}



/*�摜�����p�̊֐�*/

void flip(IplImage* &src_img, IplImage* &save_img) {
	int sw;
	IplImage *dst_img = 0;
	
	// �w�肵���������ʂ�\��
	printf("������w�肵�Ă�������\n�P�F���������]\n�Q�F���������]\n�R�F�������]\n");
	printf("����ԍ��̓��́F\n");
	scanf("%d", &sw);

	if (sw == 1) {
		//�摜�̐��������]
		dst_img= cvCloneImage(src_img);
		cvFlip(src_img, dst_img, 0);
		save_img = cvCloneImage(dst_img);
	}
	else if (sw == 2) {
		//�摜�̐��������]
		dst_img = cvCloneImage(src_img);
		cvFlip(src_img, dst_img, 1);
		save_img = cvCloneImage(dst_img);
	}
	else if (sw == 3) {
		//�摜�̗������]
		dst_img = cvCloneImage(src_img);
		cvFlip(src_img, dst_img, -1);
		save_img = cvCloneImage(dst_img);
	}
	else {
		printf("�I�������ԍ����Ԉ���Ă��܂��B\n");
	}
	
	return ;

}



//int rotation() {}



