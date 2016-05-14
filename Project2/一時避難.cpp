#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#include <cv.h>
#include <highgui.h>

//�O���[�o���ϐ�
IplImage *src_img = 0, *cut_img = 0;
CvFont font;
CvRect rect;

//�v���g�^�C�v�錾
void on_mouse(int event, int x, int y, int flags, void *param);
void cut(void);

int main(int argc, char *argv[]) {
	int c;
	void *hwnd;
	char path[80];


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
	}
	else {
		printf("�摜�̓ǂݍ��݂ɐ������܂����B\n");
	}

	while (1) {
		//�E�B���h�E���쐬���C�}�E�X�C�x���g�ɑ΂���R�[���o�b�N�֐���o�^����
		cvNamedWindow("src", CV_WINDOW_AUTOSIZE);
		cvSetMouseCallback("src", on_mouse);
		cvShowImage("src", src_img);

		//�摜�̐؂���E�\��
		cut();

		cvNamedWindow("cut", CV_WINDOW_AUTOSIZE);
		cvShowImage("cut", cut_img);
	}

	cvWaitKey(0);

	//���������
	cvDestroyWindow("src");
	cvDestroyWindow("cut");
	cvReleaseImage(&src_img);
	cvReleaseImage(&cut_img);

	return 0;
}

/* �R�[���o�b�N�֐� */
void on_mouse(int event, int x, int y, int flags, void *param = NULL) {
	char str[64];
	static int line = 0;
	const int max_line = 15, w = 15, h = 30;
	CvPoint origin = { 0, 0 };

	//�}�E�X�C�x���g���擾
	switch (event) {
	case CV_EVENT_MOUSEMOVE:
		snprintf(str, 64, "(%d,%d) %s", x, y, "MOUSE_MOVE");
		break;
	case CV_EVENT_LBUTTONDOWN:
		snprintf(str, 64, "(%d,%d) %s", x, y, "LBUTTON_DOWN");
		origin = cvPoint(x, y);
		rect.x = x;
		rect.y = y;
		break;
	case CV_EVENT_RBUTTONDOWN:
		snprintf(str, 64, "(%d,%d) %s", x, y, "RBUTTON_DOWN");
		break;
	case CV_EVENT_MBUTTONDOWN:
		snprintf(str, 64, "(%d,%d) %s", x, y, "MBUTTON_DOWN");
		break;
	case CV_EVENT_LBUTTONUP:
		snprintf(str, 64, "(%d,%d) %s", x, y, "LBUTTON_UP");
		//�h���b�O��ɐ؂���͈͂̐ݒ�ƒZ�`����
		if (rect, x != NULL || rect.y != NULL) {
			rect.width = rect.x - x;
			rect.height = rect.y - y;

			//�؎�͈͂̒Z�`����
			cvRectangle(src_img, origin, cvPoint(origin.x + rect.width, origin.y + rect.height), CV_RGB(255, 0, 255), 2, 8, 0);
		}
		break;
	case CV_EVENT_RBUTTONUP:
		snprintf(str, 64, "(%d,%d) %s", x, y, "RBUTTON_UP");
		break;
	case CV_EVENT_MBUTTONUP:
		snprintf(str, 64, "(%d,%d) %s", x, y, "MBUTTON_UP");
		break;
	case CV_EVENT_LBUTTONDBLCLK:
		snprintf(str, 64, "(%d,%d) %s", x, y, "LBUTTON_DOUBLE_CLICK");
		break;
	case CV_EVENT_RBUTTONDBLCLK:
		snprintf(str, 64, "(%d,%d) %s", x, y, "RBUTTON_DOUBLE_CLICK");
		break;
	case CV_EVENT_MBUTTONDBLCLK:
		snprintf(str, 64, "(%d,%d) %s", x, y, "MBUTTON_DOUBLE_CLICK");
		break;
	}

	//�}�E�X�{�^���C�C���L�[���擾
	//�r�b�g���Z�q?
	if (flags & CV_EVENT_FLAG_LBUTTON)
		strncat(str, " + LBUTTON", 64);
	if (flags & CV_EVENT_FLAG_RBUTTON)
		strncat(str, " + RBUTTON", 64);
	if (flags & CV_EVENT_FLAG_MBUTTON)
		strncat(str, " + MBUTTON", 64);
	if (flags & CV_EVENT_FLAG_CTRLKEY)
		strncat(str, " + CTRL", 64);
	if (flags & CV_EVENT_FLAG_SHIFTKEY)
		strncat(str, " + SHIFT", 64);
	if (flags & CV_EVENT_FLAG_ALTKEY)
		strncat(str, " + ALT", 64);

	// (6)�}�E�X���W�C�C�x���g�C�C���L�[�Ȃǂ��摜�ɕ`��C�\��
	if (line > max_line) {
		cvGetRectSubPix(src_img, src_img, cvPoint2D32f(320 - 0.5, 240 - 0.5 + h));
		cvPutText(src_img, str, cvPoint(w, 20 + h * max_line), &font, CV_RGB(0, 200, 100));
	}
	else {
		cvPutText(src_img, str, cvPoint(w, 20 + h * line), &font, CV_RGB(0, 200, 100));
	}
	line++;

	cvShowImage("Window", src_img);

}


void cut(void) {
	IplImage *dst_img = 0;

	//�摜��rect�w�蕔����؂���
	//�p�����[�^�̒l�����̏ꍇ�̓G���[���o��
	if (rect.x < 0 || rect.y < 0 || rect.width <= 0 || rect.height <= 0) {
		printf("���̒l�͖����ł��B\n");
		exit(0);
	}

	dst_img = cvCreateImage(cvSize(rect.width, rect.height), IPL_DEPTH_8U, 3);

	cvSetImageROI(src_img, rect);
	cvCopy(src_img, dst_img);

	cvResetImageROI(src_img);
	cvResetImageROI(dst_img);

	cut_img = cvCloneImage(dst_img);

}