#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>


//OpenCV�p�̃w�b�_�t�@�C��
#include <cv.h>  
#include <highgui.h>

//�֐��̒�`
void resize(IplImage* &src_img, IplImage* &save_img);
void cut(IplImage* &src_img, IplImage* &save_img);
void flip(IplImage* &src_img, IplImage* &save_img);
void rotation(IplImage* &src_img, IplImage* &save_img);
void moz(IplImage* &src_img, IplImage* &save_img);
void continueProcess(IplImage* &src_img, IplImage* &save_img);
int intro(void);

int main(int argc, char*argv[]) {
	IplImage *src_img = 0;
	IplImage *save_img = 0;
	char path[80];
	char save[80];
	int ss=0;

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
		
		//�����������ɕۑ��̂ݍs���Ƃ��p��
		save_img = cvCloneImage(src_img);
	}

	// (1)�摜��ǂݍ��݁C�����T�C�Y�̏o�͉摜��p�ӂ���
	if (argc >= 2)
		src_img = cvLoadImage(argv[1], CV_LOAD_IMAGE_COLOR);
	if (src_img == 0)
		return -1;

	

	while (1) {
		ss = intro();

		switch (ss) {
		case 1:
			resize(src_img, save_img);
			break;
		case 2:
			cut(src_img, save_img);
			break;
		case 3:
			flip(src_img, save_img);
			break;
		case 4:
			rotation(src_img, save_img);
		case 5:
			moz(src_img, save_img);
			break;
	}

		if (0 < ss < 6) {
			cvNamedWindow("src", CV_WINDOW_AUTOSIZE);
			cvNamedWindow("dst", CV_WINDOW_AUTOSIZE);
			cvShowImage("src", src_img);
			cvShowImage("dst", save_img);

			cvWaitKey(0);

			cvDestroyWindow("src");
			cvDestroyWindow("dst");

			continueProcess(src_img, save_img);

		}
		else if(ss != 100){
			printf("�����������ԍ�����͂��ĉ������B\n");
		}

	if (ss == 6) 	break;
		
	}
		

	//�ۑ���C�ӂ̖��O�Őݒ�
	printf("�ۑ��p�̖��O��ݒ肵�ĉ������B�g���q��Y��Ȃ��ŉ������B��̗�Fsave.jpg\n");
	scanf("%s", &save);
	cvSaveImage(save, save_img);

	cvReleaseImage(&src_img);
	cvReleaseImage(&save_img);

	return 0;
}

//�ŏ��̐����p�֐�
int intro(void){
	int s;

	printf("������I�����ĉ������B\n1:���T�C�Y\n2:�؂���\n3:���]\n4:��]\n5:���U�C�N���H\n");
	printf("6:�ۑ���ƂɈڂ�܂��B\n");
	scanf("%d", &s);

	return s;
}


/*�摜�����p�̊֐�*/
void resize(IplImage* &src_img, IplImage* &save_img) {
	IplImage *dst_img = 0;
	int sw = 0;
	int width=100, height=100;
	double scale = 1.0;

	printf("������w�肵�Ă�������\n�P�F�C�ӂ̃T�C�Y���w��\n�Q�F�C�ӂ̃X�P�[�����w��\n");
	printf("����ԍ��̓��́F\n");
	scanf("%d", &sw);

	if (sw == 1) {
		printf("�C�ӂ̃T�C�Y���w�肵�ĉ������B\n");
		printf("���̃T�C�Y���́F");
		scanf("%d", &width);
		printf("�c�̃T�C�Y���́F");
		scanf("%d", &height);

		//width��height�̒l�����̏ꍇ�̓G���[
		if (width <= 0 || height <= 0) {
			printf("���̒l�͖����ł��B\n");
			return ;
		}

		dst_img = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);
	}
	else if (sw == 2) {
		printf("�C�ӂ̔{�����w�肵�ĉ������B\n");
		scanf("%lf", &scale);

		//scale�̒l��0�ȉ��̏ꍇ�̓G���[
		if (scale <= 0) {
			/* scale��0�̏ꍇ */
			printf("scale��0�͖����ł��B\n");
			return ;
		}

		dst_img = cvCreateImage(cvSize((int)src_img->width*scale, (int)src_img->height*scale), IPL_DEPTH_8U, 3);

	}
	else {
		printf("�I�������ԍ����Ԉ���Ă��܂��B\n");
		return;
	}

	//�T�C�Y��ύX
	cvResize(src_img, dst_img, CV_INTER_CUBIC);

	save_img = cvCloneImage(dst_img);

	return ;
}


void cut(IplImage* &src_img, IplImage* &save_img){
	IplImage *dst_img = 0;
	CvPoint pt1, pt2;
	CvRect rect;
	
	//�؂���̋N�_�������摜�̒��S�ŏ����ݒ�
	rect.x = src_img->width / 2;
	rect.y = src_img->height / 2;

	//�摜�̔C�ӂ̎w�蕔����؂���
	printf("�C�ӂ̐؂���͈͂��w�肵�܂��B\n");
	printf("�؂���͈͂̍���̍��W�i��,���j���w�肵�ĉ������B���f�t�H���g�ł͉摜�̒��S\n");
	printf("���摜�̃T�C�Y�́i%d,%d�j�ł��B\n", src_img->width, src_img->height);
	printf("x,y=");
	scanf("%d, %d", &rect.x, &rect.y);
	printf("���̐؂���T�C�Y����\n");
	scanf("%d", &rect.width);
	printf("�c�̐؂���T�C�Y����\n");
	scanf("%d", &rect.height);

	//�p�����[�^�̒l�����̏ꍇ�̓G���[���o��
	if (rect.x < 0 || rect.y < 0 || rect.width <= 0 || rect.height <= 0) {
		printf("���̒l�͖����ł��B\n");
		exit(0) ;
	}

	dst_img = cvCreateImage(cvSize(rect.width, rect.height), IPL_DEPTH_8U, 3);

	cvSetImageROI(src_img, rect);
	cvCopy(src_img, dst_img);

	cvResetImageROI(src_img);
	cvResetImageROI(dst_img);
	pt1 = cvPoint(rect.x, rect.y);
	pt2 = cvPoint(rect.x + rect.width, rect.y + rect.height);

	//�؎�͈͂̒Z�`����
	cvRectangle(src_img, pt1, pt2, CV_RGB(255, 0, 255), 2, 8, 0);
	
	save_img = cvCloneImage(dst_img);
	
	return ;
}


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


void rotation(IplImage* &src_img, IplImage* &save_img) {
	IplImage *dst_img = 0;
	CvMat *map_matrix;
	CvPoint2D32f center;
	float x = 0, y = 0;
	double angle = 0, scale = 1.0;
	

	//��]���S���摜�̒��S�ŏ����ݒ�
	x = src_img->width / 2;
	y = src_img->height / 2;

	//�摜�̉�]���S�̎w��B
	printf("��]���S�̍��W�i��,���j���w�肵�ĉ������B���f�t�H���g�ł͉摜�̒��S(%d,%d)\n",x,y);
	printf("���摜�̃T�C�Y�́i%d,%d�j�ł��B\n", src_img->width, src_img->height);
	printf("x,y=");
	scanf("%f, %f", &x, &y);

	//�C�ӂ̃X�P�[���Ɖ�]�p�x�̎w��
	printf("�C�ӂ̕ϐ����w�肵�Ă�������\n���̒l��,�����v����]\n");
	printf("��]�p�x���w��F\n");
	scanf("%lf", &angle);

	printf("�X�P�[���W�����w��F\n");
	scanf("%lf", &scale);

	
	//Iplimaage�̃R�s�[
	dst_img = cvCloneImage(src_img);

	// �p�x�C�X�P�[���W���C��]���S���w�肵��cv2DRotationMatrix��p���ăA�t�B���s������߂�
	map_matrix = cvCreateMat(2, 3, CV_32FC1);
	center = cvPoint2D32f(x, y);
	cv2DRotationMatrix(center, angle, scale, map_matrix);

	// (3)�w�肳�ꂽ�A�t�B���s��ɂ��CcvWarpaffine��p���ĉ摜����]������  
	cvWarpAffine(src_img, dst_img, map_matrix, CV_INTER_LINEAR + CV_WARP_FILL_OUTLIERS, cvScalarAll(255));

	// (4)���ʂ�\������
	cvResetImageROI(src_img);
	cvResetImageROI(dst_img);
	cvReleaseMat(&map_matrix);

	save_img = cvCloneImage(dst_img);

	return ;
}


void moz(IplImage* &src_img, IplImage* &save_img) {
	IplImage *dst_img = 0;
	double scale = 0;

	//�֐��̒�`�𒲂ׂĂ��Č����܂���.

	printf("���U�C�N�̓x�������w�肵�ĉ������B���O�ȏ��\n");
	scanf("%lf", &scale);

	//scale�̒l��0�ȉ��̏ꍇ�̓G���[
	if (scale <= 0) {
		/* scale��0�ȉ��̏ꍇ */
		printf("scale��0�͖����ł��B\n");
		return;
	}
	dst_img = cvCreateImage(cvSize((int)src_img->width/scale, (int)src_img->height/scale), IPL_DEPTH_8U, 3);
	cvResize(src_img, dst_img, CV_INTER_CUBIC);
	src_img = cvCloneImage(dst_img);
	dst_img = cvCreateImage(cvSize((int)src_img->width*scale, (int)src_img->height*scale), IPL_DEPTH_8U, 3);
	cvResize(src_img, dst_img, CV_INTER_CUBIC);
	save_img = cvCloneImage(dst_img);
	
}
//�Q�l�ɂ����T�C�g
//http://ithat.me/2015/10/15/opencv-cpp-mosaic-image
//http://tessy.org/wiki/index.php?%B2%E8%C1%FC%A4%CE%A5%EA%A5%B5%A5%A4%A5%BA
//�i2016�N4��25�����_�j


void continueProcess(IplImage* &src_img, IplImage* &save_img) {
	src_img = cvCloneImage(save_img);
}

