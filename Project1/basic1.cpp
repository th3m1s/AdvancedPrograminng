#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>


//OpenCV用のヘッダファイル
#include <cv.h>  
#include <highgui.h>

//関数の定義
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

	//画像パス入力指示
	printf("画像のパスを入力して下さい。具体例：../images/01.bmp\n");
	scanf("%s", &path);

	//元画像を読み込む
	src_img = cvLoadImage(path, (CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_ANYCOLOR));

	/* 画像が存在しない場合，NULLが返り値として渡される */
	if (src_img == NULL) {
		/* 画像が存在しない場合 */
		printf("画像が存在しません\n");
		return -1;
	}else {
		printf("画像の読み込みに成功しました。\n");
		
		//処理をせずに保存のみ行うとき用に
		save_img = cvCloneImage(src_img);
	}

	// (1)画像を読み込み，同じサイズの出力画像を用意する
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
			printf("正しい処理番号を入力して下さい。\n");
		}

	if (ss == 6) 	break;
		
	}
		

	//保存を任意の名前で設定
	printf("保存用の名前を設定して下さい。拡張子を忘れないで下さい。具体例：save.jpg\n");
	scanf("%s", &save);
	cvSaveImage(save, save_img);

	cvReleaseImage(&src_img);
	cvReleaseImage(&save_img);

	return 0;
}

//最初の説明用関数
int intro(void){
	int s;

	printf("処理を選択して下さい。\n1:リサイズ\n2:切り取り\n3:反転\n4:回転\n5:モザイク加工\n");
	printf("6:保存作業に移ります。\n");
	scanf("%d", &s);

	return s;
}


/*画像処理用の関数*/
void resize(IplImage* &src_img, IplImage* &save_img) {
	IplImage *dst_img = 0;
	int sw = 0;
	int width=100, height=100;
	double scale = 1.0;

	printf("操作を指定してください\n１：任意のサイズを指定\n２：任意のスケールを指定\n");
	printf("操作番号の入力：\n");
	scanf("%d", &sw);

	if (sw == 1) {
		printf("任意のサイズを指定して下さい。\n");
		printf("横のサイズ入力：");
		scanf("%d", &width);
		printf("縦のサイズ入力：");
		scanf("%d", &height);

		//widthとheightの値が負の場合はエラー
		if (width <= 0 || height <= 0) {
			printf("その値は無効です。\n");
			return ;
		}

		dst_img = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);
	}
	else if (sw == 2) {
		printf("任意の倍率を指定して下さい。\n");
		scanf("%lf", &scale);

		//scaleの値が0以下の場合はエラー
		if (scale <= 0) {
			/* scaleが0の場合 */
			printf("scale＝0は無効です。\n");
			return ;
		}

		dst_img = cvCreateImage(cvSize((int)src_img->width*scale, (int)src_img->height*scale), IPL_DEPTH_8U, 3);

	}
	else {
		printf("選択した番号が間違っています。\n");
		return;
	}

	//サイズを変更
	cvResize(src_img, dst_img, CV_INTER_CUBIC);

	save_img = cvCloneImage(dst_img);

	return ;
}


void cut(IplImage* &src_img, IplImage* &save_img){
	IplImage *dst_img = 0;
	CvPoint pt1, pt2;
	CvRect rect;
	
	//切り取りの起点部分を画像の中心で初期設定
	rect.x = src_img->width / 2;
	rect.y = src_img->height / 2;

	//画像の任意の指定部分を切り取る
	printf("任意の切り取り範囲を指定します。\n");
	printf("切り取り範囲の左上の座標（ｘ,ｙ）を指定して下さい。※デフォルトでは画像の中心\n");
	printf("元画像のサイズは（%d,%d）です。\n", src_img->width, src_img->height);
	printf("x,y=");
	scanf("%d, %d", &rect.x, &rect.y);
	printf("横の切り取りサイズ入力\n");
	scanf("%d", &rect.width);
	printf("縦の切り取りサイズ入力\n");
	scanf("%d", &rect.height);

	//パラメータの値が負の場合はエラーを出力
	if (rect.x < 0 || rect.y < 0 || rect.width <= 0 || rect.height <= 0) {
		printf("その値は無効です。\n");
		exit(0) ;
	}

	dst_img = cvCreateImage(cvSize(rect.width, rect.height), IPL_DEPTH_8U, 3);

	cvSetImageROI(src_img, rect);
	cvCopy(src_img, dst_img);

	cvResetImageROI(src_img);
	cvResetImageROI(dst_img);
	pt1 = cvPoint(rect.x, rect.y);
	pt2 = cvPoint(rect.x + rect.width, rect.y + rect.height);

	//切取範囲の短形生成
	cvRectangle(src_img, pt1, pt2, CV_RGB(255, 0, 255), 2, 8, 0);
	
	save_img = cvCloneImage(dst_img);
	
	return ;
}


void flip(IplImage* &src_img, IplImage* &save_img) {
	int sw;
	IplImage *dst_img = 0;
	
	// 指定した処理結果を表示
	printf("操作を指定してください\n１：水平軸反転\n２：垂直軸反転\n３：両軸反転\n");
	printf("操作番号の入力：\n");
	scanf("%d", &sw);

	if (sw == 1) {
		//画像の水平軸反転
		dst_img= cvCloneImage(src_img);
		cvFlip(src_img, dst_img, 0);
		save_img = cvCloneImage(dst_img);
	}
	else if (sw == 2) {
		//画像の垂直軸反転
		dst_img = cvCloneImage(src_img);
		cvFlip(src_img, dst_img, 1);
		save_img = cvCloneImage(dst_img);
	}
	else if (sw == 3) {
		//画像の両軸反転
		dst_img = cvCloneImage(src_img);
		cvFlip(src_img, dst_img, -1);
		save_img = cvCloneImage(dst_img);
	}
	else {
		printf("選択した番号が間違っています。\n");
	}
	
	return ;

}


void rotation(IplImage* &src_img, IplImage* &save_img) {
	IplImage *dst_img = 0;
	CvMat *map_matrix;
	CvPoint2D32f center;
	float x = 0, y = 0;
	double angle = 0, scale = 1.0;
	

	//回転中心を画像の中心で初期設定
	x = src_img->width / 2;
	y = src_img->height / 2;

	//画像の回転中心の指定。
	printf("回転中心の座標（ｘ,ｙ）を指定して下さい。※デフォルトでは画像の中心(%d,%d)\n",x,y);
	printf("元画像のサイズは（%d,%d）です。\n", src_img->width, src_img->height);
	printf("x,y=");
	scanf("%f, %f", &x, &y);

	//任意のスケールと回転角度の指定
	printf("任意の変数を指定してください\n正の値は,反時計回り回転\n");
	printf("回転角度を指定：\n");
	scanf("%lf", &angle);

	printf("スケール係数を指定：\n");
	scanf("%lf", &scale);

	
	//Iplimaageのコピー
	dst_img = cvCloneImage(src_img);

	// 角度，スケール係数，回転中心を指定してcv2DRotationMatrixを用いてアフィン行列を求める
	map_matrix = cvCreateMat(2, 3, CV_32FC1);
	center = cvPoint2D32f(x, y);
	cv2DRotationMatrix(center, angle, scale, map_matrix);

	// (3)指定されたアフィン行列により，cvWarpaffineを用いて画像を回転させる  
	cvWarpAffine(src_img, dst_img, map_matrix, CV_INTER_LINEAR + CV_WARP_FILL_OUTLIERS, cvScalarAll(255));

	// (4)結果を表示する
	cvResetImageROI(src_img);
	cvResetImageROI(dst_img);
	cvReleaseMat(&map_matrix);

	save_img = cvCloneImage(dst_img);

	return ;
}


void moz(IplImage* &src_img, IplImage* &save_img) {
	IplImage *dst_img = 0;
	double scale = 0;

	//関数の定義を調べていて見つけました.

	printf("モザイクの度合いを指定して下さい。※０以上で\n");
	scanf("%lf", &scale);

	//scaleの値が0以下の場合はエラー
	if (scale <= 0) {
		/* scaleが0以下の場合 */
		printf("scale＝0は無効です。\n");
		return;
	}
	dst_img = cvCreateImage(cvSize((int)src_img->width/scale, (int)src_img->height/scale), IPL_DEPTH_8U, 3);
	cvResize(src_img, dst_img, CV_INTER_CUBIC);
	src_img = cvCloneImage(dst_img);
	dst_img = cvCreateImage(cvSize((int)src_img->width*scale, (int)src_img->height*scale), IPL_DEPTH_8U, 3);
	cvResize(src_img, dst_img, CV_INTER_CUBIC);
	save_img = cvCloneImage(dst_img);
	
}
//参考にしたサイト
//http://ithat.me/2015/10/15/opencv-cpp-mosaic-image
//http://tessy.org/wiki/index.php?%B2%E8%C1%FC%A4%CE%A5%EA%A5%B5%A5%A4%A5%BA
//（2016年4月25日時点）


void continueProcess(IplImage* &src_img, IplImage* &save_img) {
	src_img = cvCloneImage(save_img);
}

