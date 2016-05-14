#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#include <cv.h>
#include <highgui.h>

//�O���[�o���ϐ�
CvFont font;
IplImage *src_img = 0,*cut_img=0;
CvRect rect;
bool drawing_box = false;


//�v���g�^�C�v�錾
void on_mouse(int event, int x, int y, int flags, void *param);
void cut(IplImage* &dst_img2);

int main(int argc, char *argv[]) {

	char path[80];
	char save[80];

	cvInitFont(&font, CV_FONT_HERSHEY_DUPLEX, 0.4, 0.4);

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

	//�E�B���h�E���쐬���C�}�E�X�C�x���g�ɑ΂���R�[���o�b�N�֐���o�^����
	cvNamedWindow("src", CV_WINDOW_AUTOSIZE);
	cvSetMouseCallback("src", on_mouse, src_img);
	cvShowImage("src", src_img);

	printf("���x�ł��؂���͈͂͑I���\�ł��B\n�؂���͈͂����肵���ꍇ�͔C�ӂ̃{�^���������ĉ������B\n");
	cvWaitKey(0);

	printf("\n�����ŁA�E�B���h�E��cut�̉摜���ۑ��ł��܂��B\n");
	

	cvDestroyWindow("src");
	cvDestroyWindow("cut");

	//�ۑ���C�ӂ̖��O�Őݒ�
	printf("�ۑ��p�̖��O��ݒ肵�ĉ������B�g���q��Y��Ȃ��ŉ������B��̗�Fsave.jpg\n");
	scanf("%s", &save);
	cvSaveImage(save, cut_img);

	//���������
	cvReleaseImage(&src_img);
	cvReleaseImage(&cut_img);

	return 0;
}

/* �R�[���o�b�N�֐� */
void on_mouse(int event, int x, int y, int flags, void *param = NULL) {
	char str[64];
	IplImage *dst_img1;
	IplImage *dst_img2;
	
	// (4)�}�E�X�C�x���g���擾
	switch (event) {
	case CV_EVENT_MOUSEMOVE:
		snprintf(str, 64, "(%d,%d) %s", x, y, "MOUSE_MOVE");
		if (drawing_box) {
			rect.width = x - rect.x;
			rect.height = y - rect.y;
		}
		break;
	case CV_EVENT_LBUTTONDOWN:
		snprintf(str, 64, "(%d,%d) %s", x, y, "LBUTTON_DOWN");
		drawing_box = true;
		rect = cvRect(x, y, 0, 0);
		break;
	case CV_EVENT_LBUTTONUP:
		snprintf(str, 64, "(%d,%d) %s", x, y, "LBUTTON_UP");
		drawing_box = false;
		if (rect.width<0) {
			rect.x += rect.width;
			rect.width *= -1;
		}
		if (rect.height<0) {
			rect.y += rect.height;
			rect.height *= -1;
		}
		if (rect.width !=0 && rect.height!=0) {
			cut(dst_img2);
		}
		break;
	case CV_EVENT_RBUTTONDOWN:
		snprintf(str, 64, "(%d,%d) %s", x, y, "RBUTTON_DOWN");
		break;
	case CV_EVENT_MBUTTONDOWN:
		snprintf(str, 64, "(%d,%d) %s", x, y, "MBUTTON_DOWN");
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
	if (flags & CV_EVENT_FLAG_LBUTTON)
		strncat(str, " + LBUTTON", 64);
	if (flags & CV_EVENT_FLAG_RBUTTON)
		strncat(str, " + RBUTTON", 64);
	if (flags & CV_EVENT_FLAG_MBUTTON)
		strncat(str, " + MBUTTON", 64);



	//�}�E�X���W�C�C�x���g�C�C���L�[�Ȃǂ��摜dst_img1�ɕ`��C�\��
	//�\���摜�������̂�h�����߂ɁAcvPutText()�̕\���ʒu���Œ�
	//�t�H���g���d�Ȃ邱�Ƃ�h�����߂ɁA���񌳉摜src_img�̏�Ԃɖ߂�
	cvPutText(dst_img1 =cvCloneImage(src_img), str, cvPoint(5, src_img->height-10), &font, CV_RGB(0, 200, 100));
	cvShowImage("src", dst_img1);
	cvCopy(src_img, dst_img1);

}


void cut(IplImage* &dst_img2) {
	dst_img2 = cvCloneImage(src_img);

	//�����p�摜dst_img2�ɐ؂���͈͕\��
	//���摜src_img�ɐ؂���͈͕\��������Ɖ��x���؂�����s���ƑI��͈͂̐����������Ɏc�邽��
	//���x�ł��A�؂���͈͂̎w�肪�ł���悤�ɍH�v
	//�Z�`�̏������ł��Ȃ������̂ŁA���̓s�x�A�E�B���h�E���������Ă��܂��B
	cvRectangle(dst_img2, cvPoint(rect.x, rect.y), cvPoint(rect.x + rect.width, rect.y + rect.height), cvScalar(0xff, 0x00, 0x00)/* red */);

	cvNamedWindow("dst", CV_WINDOW_AUTOSIZE);
	cvShowImage("dst", dst_img2);

	cvWaitKey(0);


	cvDestroyWindow("dst");
	
	//�؂���摜�̏o�͐�摜cut_img���쐬
	cut_img = cvCreateImage(cvSize(rect.width, rect.height), IPL_DEPTH_8U, 3);

	//���ڗ̈�ROI�����摜src_img�ɃZ�b�g
	cvSetImageROI(src_img, rect);

	//�o�͐�摜cut_img�Ɍ��摜�̒��ڔ͈͂��R�s�[
	cvCopy(src_img, cut_img);

	dst_img2 = cvCloneImage(src_img);

	cvResetImageROI(src_img);
		
	cvNamedWindow("cut", CV_WINDOW_AUTOSIZE);
	cvShowImage("cut", cut_img);
	
}
