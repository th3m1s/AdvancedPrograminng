#include <stdio.h>
#include <cv.h>
#include <highgui.h>

#define PATH "08.jpg"

int main(int argc, char*argv[]) {

	char windowNameSource[] = "Image";

	/* ���摜��ǂݍ��� */
	IplImage *sourceImage = cvLoadImage(PATH, (CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_ANYCOLOR));

	/* �摜�����݂��Ȃ��ꍇ�CNULL���Ԃ�l�Ƃ��ēn����� */
	if (sourceImage == NULL) {
		/* �摜�����݂��Ȃ��ꍇ */
		printf("�摜�����݂��܂���\n");
		return -1;
	}

	/* �E�B���h�E�𐶐����� */
	cvNamedWindow(windowNameSource, CV_WINDOW_AUTOSIZE);

	/* �摜��\������ */
	cvShowImage(windowNameSource, sourceImage);

	/* �L�[���͂�҂� */
	cvWaitKey(0);

	/* JPG�ŕۑ�����*/
	cvSaveImage("save_img.jpg", sourceImage);

	/* ���������J������ */
	cvReleaseImage(&sourceImage);

	/* �E�C���h�E��j������ */
	cvDestroyWindow(windowNameSource);

	return 0;
}

