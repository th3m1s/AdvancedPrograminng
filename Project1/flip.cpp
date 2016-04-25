#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#include <cv.h>
#include <highgui.h>

int main(int argc, char **argv) {
	IplImage *src_img = 0;
	IplImage *dst_img_1, *dst_img_2, *dst_img_3;
	char path[80];
	int sw;

	printf("画像のパスを入力して下さい。具体例：../images/01.bmp\n");/*画像パス入力指示*/
	scanf("%s", &path);

	/* 元画像を読み込む */
	src_img = cvLoadImage(path, (CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_ANYCOLOR));

	/* 画像が存在しない場合，NULLが返り値として渡される */
	if (src_img == NULL) {
		/* 画像が存在しない場合 */
		printf("画像が存在しません\n");
		return -1;
	}
	else {
		printf("画像の読み込みに成功しました。\n");
	}

	// (1)画像を読み込み，同じサイズの出力画像を用意する
	if (argc >= 2)
		src_img = cvLoadImage(argv[1], CV_LOAD_IMAGE_COLOR);
	if (src_img == 0)
		return -1;

	dst_img_1 = cvCloneImage(src_img);
	dst_img_2 = cvCloneImage(src_img);
	dst_img_3 = cvCloneImage(src_img);

	// (2)画像の水平軸反転・垂直軸反転・両軸反転を行う
	cvFlip(src_img, dst_img_1, 0);
	cvFlip(src_img, dst_img_2, 1);
	cvFlip(src_img, dst_img_3, -1);

	// 指定した処理結果を表示
	printf("操作を指定してください\n１：水平軸反転\n２：垂直軸反転ｎ\n３：両軸反転\n");
	printf("操作番号の入力：\n");
	scanf("%d", &sw);

	if (sw == 1) {
		//ウィンドウの生成・表示・保存
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
		printf("選択した番号が間違っています。\n");
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
