#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>

#include <cv.h>
#include <highgui.h>

int main(int argc, char **argv) {
	IplImage *src_img = 0, *dst_img = 0;

	char path[80];
	int sw = 0;
	double width, height;
	double scale = 1.0;

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
	
	printf("操作を指定してください\n１：任意のサイズを指定\n２：任意のスケールを指定\n");
	printf("操作番号の入力：\n");
	scanf("%d",&sw);

	if (sw == 1) {
		printf("任意のサイズを指定して下さい。\n");
		printf("横のサイズ入力：");
		scanf("%lf", &width);
		printf("縦のサイズ入力：");
		scanf("%lf", &height);

		//widthとheightの値が負の場合はエラー
		if (width<= 0 ||height<=0) {
			printf("その値は無効です。\n");
			return -1;
		}

		//)画像の読み込み，出力用画像領域の確保を行なう
		if (argc >= 2)
			src_img = cvLoadImage(argv[1], CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_ANYCOLOR);
		if (src_img == 0)
			return -1;

		dst_img = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);
	}
	else if (sw == 2) {
		printf("任意の倍率を指定して下さい。\n");
		scanf("%lf",&scale);

		//scaleの値が0の場合はエラー
		if (scale == 0) {
			/* scaleが0の場合 */
			printf("scale＝0は無効です。\n");
			return -1;
		}

		//)画像の読み込み，出力用画像領域の確保を行なう
		if (argc >= 2) 
			src_img = cvLoadImage(argv[1], CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_ANYCOLOR);
		if (src_img == 0)
			return -1;

		dst_img = cvCreateImage(cvSize(src_img->width*scale, src_img->height*scale), IPL_DEPTH_8U, 3);

	}
	else {
		printf("選択した番号が間違っています。\n");
		return -1;
	}

	//サイズを変更
	cvResize(src_img, dst_img, CV_INTER_CUBIC);

	//表示用ウィンドウ生成
	cvNamedWindow("dst", CV_WINDOW_AUTOSIZE);
	cvShowImage("dst", dst_img);

	//キー入力を待つ
	cvWaitKey(0);

	//JPGで保存する
	cvSaveImage("resize.jpg", dst_img);

	/* メモリを開放する */
	cvReleaseImage(&src_img);
	cvReleaseImage(&dst_img);

	/* ウインドウを破棄する */
	cvDestroyWindow("src");
	cvDestroyWindow("dst");
	
	//メモリ解放	
	cvReleaseImage(&src_img);
	cvReleaseImage(&dst_img);

	return 0;
}
