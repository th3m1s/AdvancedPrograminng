#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#include <cv.h>
#include <highgui.h>

int main(int argc, char **argv) {
	IplImage *src_img = 0, *dst_img = 0;
	CvPoint pt1, pt2;
	CvRect rect;
	char path[80];
	
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

	// (1)�摜�̓ǂݍ��݁C�o�͗p�摜�̈�̊m�ۂ��s�Ȃ�
	if (argc >= 2)
		src_img = cvLoadImage(argv[1], CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_ANYCOLOR);
	if (src_img == 0)
		return -1;

	//�؂���̋N�_�������摜�̒��S�ŏ����ݒ�
	rect.x = src_img->width / 2;
	rect.y = src_img->height / 2;

	//�摜�̔C�ӂ̎w�蕔����؂���
	printf("�C�ӂ̐؂���͈͂��w�肵�܂��B\n");
	printf("�؂���͈͂̍���̍��W�i��,���j���w�肵�ĉ������B���f�t�H���g�ł͉摜�̒��S\n");
	printf("���摜�̃T�C�Y�́i%d,%d�j�ł��B\n", src_img->width, src_img->height);
	printf("x,y=");
	scanf("%d, %d", &rect.x, &rect.y);
	printf("���̐؂���T�C�Y���́��f�t�H���g�ł�100\n");
	scanf("%d",&rect.width);
	printf("�c�̐؂���T�C�Y���́��f�t�H���g�ł�100\n");
	scanf("%d", &rect.height);

	//�p�����[�^�̒l�����̏ꍇ�̓G���[���o��
	if (rect.x<= 0 || rect.y <= 0||rect.width <= 0 || rect.height <= 0) {
		printf("���̒l�͖����ł��B\n");
		return -1;
	}

	dst_img = cvCreateImage(cvSize(rect.width, rect.height), IPL_DEPTH_8U, 3);

	cvSetImageROI(src_img, rect);
	cvCopy(src_img, dst_img);

	// (3)���ʂ�\������
	cvResetImageROI(src_img);
	cvResetImageROI(dst_img);
	pt1 = cvPoint(rect.x, rect.y);
	pt2 = cvPoint(rect.x + rect.width, rect.y + rect.height);
	
	//�؎�͈͂̒Z�`����
	cvRectangle(src_img, pt1, pt2, CV_RGB(255, 0, 255), 2, 8, 0);
	cvNamedWindow("src", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("dst", CV_WINDOW_AUTOSIZE);
	cvShowImage("src", src_img);
	cvShowImage("dst", dst_img);
	
	//JPG�ŕۑ�����
	cvSaveImage("cut.jpg", dst_img);
	
	cvWaitKey(0);
	cvDestroyWindow("src");
	cvDestroyWindow("dst");
	cvReleaseImage(&src_img);
	cvReleaseImage(&dst_img);

	return 1;
}
