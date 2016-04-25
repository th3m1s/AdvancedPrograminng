#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#include <cv.h>
#include <highgui.h>

int main(int argc, char **argv) {
	IplImage *src_img = 0, *dst_img = 0;
	CvMat *map_matrix;
	CvPoint2D32f center;
	char path[80];
	double angle = 0, scale = 1.0;
	double x=0,y=0;

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

	//回転中心を画像の中心で初期設定
	x = src_img->width / 2 ;
	y = src_img->height / 2;

	//画像の回転中心の指定。
	printf("回転中心の座標（ｘ,ｙ）を指定して下さい。※デフォルトでは画像の中心\n");
	printf("元画像のサイズは（%d,%d）です。\n",src_img->width, src_img->height);
	printf("x,y=");
	scanf("%lf, %lf", &x, &y);

	//任意のスケールと回転角度の指定
	printf("任意の変数を指定してください\n正の値は,反時計回り回転\n");
	printf("回転角度を指定：\n");
	scanf("%lf", &angle);

	printf("スケール係数を指定：\n");
	scanf("%lf", &scale	);

	// (1)画像の読み込み，出力用画像領域の確保を行なう
	if (argc >= 2)
		src_img = cvLoadImage(argv[1], CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_ANYCOLOR);
	if (src_img == 0)
		return -1;

	//Iplimaageのコピー
	dst_img = cvCloneImage(src_img);

	// 角度，スケール係数，回転中心を指定してcv2DRotationMatrixを用いてアフィン行列を求める
	map_matrix = cvCreateMat(2, 3, CV_32FC1);
	center = cvPoint2D32f(x,y);
	cv2DRotationMatrix(center, angle, scale, map_matrix);

	// (3)指定されたアフィン行列により，cvWarpaffineを用いて画像を回転させる  
	cvWarpAffine(src_img, dst_img, map_matrix, CV_INTER_LINEAR + CV_WARP_FILL_OUTLIERS, cvScalarAll(255));

	// (4)結果を表示する
	cvResetImageROI(src_img);
	cvResetImageROI(dst_img);


	cvNamedWindow("dst", CV_WINDOW_AUTOSIZE);
	
	cvShowImage("dst", dst_img);

	cvWaitKey(0);

	cvSaveImage("rotation.jpg", dst_img);
	
	cvDestroyWindow("dst");
	
	cvReleaseImage(&src_img);
	cvReleaseImage(&dst_img);
	
	cvReleaseMat(&map_matrix);

	return 1;
}
