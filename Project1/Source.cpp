#include <stdio.h>
#include <cv.h>
#include <highgui.h>

#define PATH "08.jpg"

int main(int argc, char*argv[]) {

	char windowNameSource[] = "Image";

	/* 元画像を読み込む */
	IplImage *sourceImage = cvLoadImage(PATH, (CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_ANYCOLOR));

	/* 画像が存在しない場合，NULLが返り値として渡される */
	if (sourceImage == NULL) {
		/* 画像が存在しない場合 */
		printf("画像が存在しません\n");
		return -1;
	}

	/* ウィンドウを生成する */
	cvNamedWindow(windowNameSource, CV_WINDOW_AUTOSIZE);

	/* 画像を表示する */
	cvShowImage(windowNameSource, sourceImage);

	/* キー入力を待つ */
	cvWaitKey(0);

	/* JPGで保存する*/
	cvSaveImage("save_img.jpg", sourceImage);

	/* メモリを開放する */
	cvReleaseImage(&sourceImage);

	/* ウインドウを破棄する */
	cvDestroyWindow(windowNameSource);

	return 0;
}

