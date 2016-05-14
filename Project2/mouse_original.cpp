#include <cv.h>
#include <highgui.h>
#include <stdio.h>

/* �O���[�o���ϐ� */
CvFont font;
IplImage *src_img = 0;

/* �v���g�^�C�v�錾 */
void on_mouse (int event, int x, int y, int flags, void *param);

int main (int argc, char *argv[]){
	int c;
	void *hwnd;
	// (1)�摜�̈���m�ۂ��C����������
	src_img = cvCreateImage (cvSize (640, 480), IPL_DEPTH_8U, 3);
	cvZero (src_img);
	cvInitFont (&font, CV_FONT_HERSHEY_DUPLEX, 0.4, 0.4);
	// (2)�E�B���h�E���쐬���C�}�E�X�C�x���g�ɑ΂���R�[���o�b�N�֐���o�^����
	cvNamedWindow ("Window", CV_WINDOW_AUTOSIZE);
	cvSetMouseCallback ("Window", on_mouse);
	cvShowImage ("Window", src_img);
	// (3)'Esc'�L�[�������ꂽ�ꍇ���E�C���h�E������ꂽ�ꍇ�C�I������
	while (1) {
		hwnd = cvGetWindowHandle("Window");
		c = cvWaitKey (0) & 0xff;
		if (c == 0x1b || hwnd == NULL)
		break;
	}

	cvDestroyWindow ("Window");
	cvReleaseImage (&src_img);
	
	return 0;
}

/* �R�[���o�b�N�֐� */
void on_mouse (int event, int x, int y, int flags, void *param = NULL){
	char str[64];
	static int line = 0;
	const int max_line = 15, w = 15, h = 30;
	// (4)�}�E�X�C�x���g���擾
	switch (event) {
		case CV_EVENT_MOUSEMOVE:
			snprintf (str, 64, "(%d,%d) %s", x, y, "MOUSE_MOVE");
			break;
		case CV_EVENT_LBUTTONDOWN:
			snprintf (str, 64, "(%d,%d) %s", x, y, "LBUTTON_DOWN");
			break;
		case CV_EVENT_RBUTTONDOWN:
			snprintf (str, 64, "(%d,%d) %s", x, y, "RBUTTON_DOWN");
			break;
		case CV_EVENT_MBUTTONDOWN:
			snprintf (str, 64, "(%d,%d) %s", x, y, "MBUTTON_DOWN");
			break;
		case CV_EVENT_LBUTTONUP:
			snprintf (str, 64, "(%d,%d) %s", x, y, "LBUTTON_UP");
			break;
		case CV_EVENT_RBUTTONUP:
			snprintf (str, 64, "(%d,%d) %s", x, y, "RBUTTON_UP");
			break;
		case CV_EVENT_MBUTTONUP:
			snprintf (str, 64, "(%d,%d) %s", x, y, "MBUTTON_UP");
			break;
		case CV_EVENT_LBUTTONDBLCLK:
			snprintf (str, 64, "(%d,%d) %s", x, y, "LBUTTON_DOUBLE_CLICK");
			break;
		case CV_EVENT_RBUTTONDBLCLK:
			snprintf (str, 64, "(%d,%d) %s", x, y, "RBUTTON_DOUBLE_CLICK");
			break;
		case CV_EVENT_MBUTTONDBLCLK:
			snprintf (str, 64, "(%d,%d) %s", x, y, "MBUTTON_DOUBLE_CLICK");
			break;
	}
	// (5)�}�E�X�{�^���C�C���L�[���擾
	if (flags & CV_EVENT_FLAG_LBUTTON)
		strncat (str, " + LBUTTON", 64);
	if (flags & CV_EVENT_FLAG_RBUTTON)
		strncat (str, " + RBUTTON", 64);
	if (flags & CV_EVENT_FLAG_MBUTTON)
		strncat (str, " + MBUTTON", 64);
	if (flags & CV_EVENT_FLAG_CTRLKEY)
		strncat (str, " + CTRL", 64);
	if (flags & CV_EVENT_FLAG_SHIFTKEY)
		strncat (str, " + SHIFT", 64);
	if (flags & CV_EVENT_FLAG_ALTKEY)
		strncat (str, " + ALT", 64);
	// (6)�}�E�X���W�C�C�x���g�C�C���L�[�Ȃǂ��摜�ɕ`��C�\��
	if (line > max_line) {
		cvGetRectSubPix (src_img, src_img, cvPoint2D32f (320 - 0.5, 240 - 0.5 + h));
		cvPutText (src_img, str, cvPoint (w, 20 + h * max_line), &font, CV_RGB (0, 200, 100));
	}
	else {
		cvPutText (src_img, str, cvPoint (w, 20 + h * line), &font, CV_RGB (0, 200, 100));
	}
	line++;
	cvShowImage ("Window", src_img);
}
