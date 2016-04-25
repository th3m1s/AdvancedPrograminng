#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#include <cv.h>
#include <highgui.h>

int main(int argc, char **argv) {
	IplImage *src_img = 0, *dst_img = 0;
	CvPoint pt1, pt2;
	CvRect rect;
	char path[80];
	
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

	// (1)画像の読み込み，出力用画像領域の確保を行なう
	if (argc >= 2)
		src_img = cvLoadImage(argv[1], CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_ANYCOLOR);
	if (src_img == 0)
		return -1;

	//切り取りの起点部分を画像の中心で初期設定
	rect.x = src_img->width / 2;
	rect.y = src_img->height / 2;

	//画像の任意の指定部分を切り取る
	printf("任意の切り取り範囲を指定します。\n");
	printf("切り取り範囲の左上の座標（ｘ,ｙ）を指定して下さい。※デフォルトでは画像の中心\n");
	printf("元画像のサイズは（%d,%d）です。\n", src_img->width, src_img->height);
	printf("x,y=");
	scanf("%d, %d", &rect.x, &rect.y);
	printf("横の切り取りサイズ入力※デフォルトでは100\n");
	scanf("%d",&rect.width);
	printf("縦の切り取りサイズ入力※デフォルトでは100\n");
	scanf("%d", &rect.height);

	//パラメータの値が負の場合はエラーを出力
	if (rect.x<= 0 || rect.y <= 0||rect.width <= 0 || rect.height <= 0) {
		printf("その値は無効です。\n");
		return -1;
	}

	dst_img = cvCreateImage(cvSize(rect.width, rect.height), IPL_DEPTH_8U, 3);

	cvSetImageROI(src_img, rect);
	cvCopy(src_img, dst_img);

	// (3)結果を表示する
	cvResetImageROI(src_img);
	cvResetImageROI(dst_img);
	pt1 = cvPoint(rect.x, rect.y);
	pt2 = cvPoint(rect.x + rect.width, rect.y + rect.height);
	
	//切取範囲の短形生成
	cvRectangle(src_img, pt1, pt2, CV_RGB(255, 0, 255), 2, 8, 0);
	cvNamedWindow("src", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("dst", CV_WINDOW_AUTOSIZE);
	cvShowImage("src", src_img);
	cvShowImage("dst", dst_img);
	
	//JPGで保存する
	cvSaveImage("cut.jpg", dst_img);
	
	cvWaitKey(0);
	cvDestroyWindow("src");
	cvDestroyWindow("dst");
	cvReleaseImage(&src_img);
	cvReleaseImage(&dst_img);

	return 1;
}
