#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

//OpenCV用のヘッダファイル
#include <cv.h>  
#include <highgui.h>

//関数の定義
void flip(IplImage* &src_img, IplImage* &save_img);

int main(int argc, char*argv[]) {
	IplImage *src_img = 0;
	IplImage *save_img = 0;
	char path[80];
	char save[80];

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
	}

	// (1)画像を読み込み，同じサイズの出力画像を用意する
	if (argc >= 2)
		src_img = cvLoadImage(argv[1], CV_LOAD_IMAGE_COLOR);
	if (src_img == 0)
		return -1;
	
	flip(src_img,save_img);

	cvWaitKey(0);

	cvNamedWindow("src", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("dst", CV_WINDOW_AUTOSIZE);
	cvShowImage("src", src_img);
	cvShowImage("dst", save_img);

	//保存を任意の名前で設定
	printf("保存用の名前を設定して下さい。具体例：save.jpg\n");
	scanf("%s", &save);
	cvSaveImage(save, save_img);

	
	//printf("画像の読み込みに成功しました。\n※ENDと入力すると処理を終了します\n");

	cvDestroyWindow("src");
	cvDestroyWindow("dst");
	cvReleaseImage(&src_img);
	cvReleaseImage(&save_img);

	exit(0);
}



/*画像処理用の関数*/

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



//int rotation() {}



