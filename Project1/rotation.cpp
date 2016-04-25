#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#include <cv.h>
#include <highgui.h>

int main(int argc, char **argv) {
	IplImage *src_img = 0, *dst_img = 0;
	CvMat *map_matrix;
	CvPoint2D32f center;
	char path[80];
	double angle = 0, scale = 1.0;
	double x=0,y=0;

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

	//��]���S���摜�̒��S�ŏ����ݒ�
	x = src_img->width / 2 ;
	y = src_img->height / 2;

	//�摜�̉�]���S�̎w��B
	printf("��]���S�̍��W�i��,���j���w�肵�ĉ������B���f�t�H���g�ł͉摜�̒��S\n");
	printf("���摜�̃T�C�Y�́i%d,%d�j�ł��B\n",src_img->width, src_img->height);
	printf("x,y=");
	scanf("%lf, %lf", &x, &y);

	//�C�ӂ̃X�P�[���Ɖ�]�p�x�̎w��
	printf("�C�ӂ̕ϐ����w�肵�Ă�������\n���̒l��,�����v����]\n");
	printf("��]�p�x���w��F\n");
	scanf("%lf", &angle);

	printf("�X�P�[���W�����w��F\n");
	scanf("%lf", &scale	);

	// (1)�摜�̓ǂݍ��݁C�o�͗p�摜�̈�̊m�ۂ��s�Ȃ�
	if (argc >= 2)
		src_img = cvLoadImage(argv[1], CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_ANYCOLOR);
	if (src_img == 0)
		return -1;

	//Iplimaage�̃R�s�[
	dst_img = cvCloneImage(src_img);

	// �p�x�C�X�P�[���W���C��]���S���w�肵��cv2DRotationMatrix��p���ăA�t�B���s������߂�
	map_matrix = cvCreateMat(2, 3, CV_32FC1);
	center = cvPoint2D32f(x,y);
	cv2DRotationMatrix(center, angle, scale, map_matrix);

	// (3)�w�肳�ꂽ�A�t�B���s��ɂ��CcvWarpaffine��p���ĉ摜����]������  
	cvWarpAffine(src_img, dst_img, map_matrix, CV_INTER_LINEAR + CV_WARP_FILL_OUTLIERS, cvScalarAll(255));

	// (4)���ʂ�\������
	cvResetImageROI(src_img);
	cvResetImageROI(dst_img);


	cvNamedWindow("dst", CV_WINDOW_AUTOSIZE);
	
	cvShowImage("dst", dst_img);

	cvWaitKey(0);

	cvSaveImage("rotation.jpg", dst_img);
	
	cvDestroyWindow("dst");
	
	cvReleaseImage(&src_img);
	cvReleaseImage(&dst_img);
	
	cvReleaseMat(&map_matrix);

	return 1;
}
