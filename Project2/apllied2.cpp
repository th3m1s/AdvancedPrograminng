#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#include <cv.h>
#include <highgui.h>

//グローバル変数
CvFont font;
IplImage *src_img = 0,*cut_img=0, *zoom_img = 0;
CvRect rect;
bool drawing_box = false;


//プロトタイプ宣言
void on_mouse(int event, int x, int y, int flags, void *param);
void cut(IplImage* &dst_img2);
void pip_zoom();
IplImage*  resize(double scale);

int main(int argc, char *argv[]) {

	char path[80]="08.jpg";
	char save[80]; "save.jpg";

	cvInitFont(&font, CV_FONT_HERSHEY_DUPLEX, 0.4, 0.4);

	//画像パス入力指示
	printf("画像のパスを入力して下さい。具体例：../images/01.bmp\n");
	//scanf("%s", &path);

	//元画像を読み込む
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

	//ウィンドウを作成し，マウスイベントに対するコールバック関数を登録する
	cvNamedWindow("src", CV_WINDOW_AUTOSIZE);
	cvSetMouseCallback("src", on_mouse, src_img);
	cvShowImage("src", src_img);

	printf("何度でも切り取り範囲は選択可能です。\n切り取り範囲を決定した場合は任意のボタンを押して下さい。\n");
	
	cvWaitKey(0);

	printf("次に拡大・縮小を行います。\n");
	pip_zoom();

	cvDestroyWindow("src");
	cvDestroyWindow("cut");
	

	//保存を任意の名前で設定
	printf("保存用の名前を設定して下さい。拡張子を忘れないで下さい。具体例：save.jpg\n");
	//scanf("%s", &save);
	cvSaveImage(save, zoom_img);

	//メモリ解放
	cvReleaseImage(&src_img);
	cvReleaseImage(&cut_img);
	cvReleaseImage(&zoom_img);

	return 0;
}

/* コールバック関数 */
void on_mouse(int event, int x, int y, int flags, void *param = NULL) {
	char str[64];
	IplImage *dst_img1;
	IplImage *dst_img2;
	
	// (4)マウスイベントを取得
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
	//マウスボタン，修飾キーを取得
	if (flags & CV_EVENT_FLAG_LBUTTON)
		strncat(str, " + LBUTTON", 64);
	if (flags & CV_EVENT_FLAG_RBUTTON)
		strncat(str, " + RBUTTON", 64);
	if (flags & CV_EVENT_FLAG_MBUTTON)
		strncat(str, " + MBUTTON", 64);



	//マウス座標，イベント，修飾キーなどを画像dst_img1に描画，表示
	//表示画像がずれるのを防ぐために、cvPutText()の表示位置を固定
	//フォントが重なることを防ぐために、毎回元画像src_imgの状態に戻す
	cvPutText(dst_img1 =cvCloneImage(src_img), str, cvPoint(5, src_img->height-10), &font, CV_RGB(0, 200, 100));
	cvShowImage("src", dst_img1);
	cvCopy(src_img, dst_img1);

}


void cut(IplImage* &dst_img2) {
	dst_img2 = cvCloneImage(src_img);

	//処理用画像dst_img2に切り取り範囲表示
	//元画像src_imgに切り取り範囲表示をすると何度も切り取りを行うと選択範囲の線が消えずに残るため
	//何度でも、切り取り範囲の指定ができるように工夫
	//短形の消去ができなかったので、その都度、ウィンドウが生成してしまう。
	cvRectangle(dst_img2, cvPoint(rect.x, rect.y), cvPoint(rect.x + rect.width, rect.y + rect.height), cvScalar(0xff, 0x00, 0x00)/* red */);

	cvNamedWindow("dst", CV_WINDOW_AUTOSIZE);
	cvShowImage("dst", dst_img2);

	cvWaitKey(0);


	cvDestroyWindow("dst");
	
	//切り取り画像の出力先画像cut_imgを作成
	cut_img = cvCreateImage(cvSize(rect.width, rect.height), IPL_DEPTH_8U, 3);

	//注目領域ROIを元画像src_imgにセット
	cvSetImageROI(src_img, rect);

	//出力先画像cut_imgに元画像の注目範囲をコピー
	cvCopy(src_img, cut_img);

	dst_img2 = cvCloneImage(src_img);

	cvResetImageROI(src_img);
		
	//cvNamedWindow("cut", CV_WINDOW_AUTOSIZE);
	//cvShowImage("cut", cut_img);
	
}

/*------------------------------------------------------------*

* @fn          OpenCVのピクチャーインピクチャ

* @brief       画像内に画像を貼り付ける（位置を座標で指定）

* @par[in ]    srcImg  背景画像

* @par[in ]    smallImg    前景画像

* @par[in ]    p0  前景画像の左上座標

* @par[in ]    p1  前景画像の右下座標

*------------------------------------------------------------*/
void pip_zoom(){
	IplImage *dst_img = 0;
	CvPoint2D32f center, pt;
	CvRect zoom;
	uchar p[3],dst[3];
	int x, y;
	double scale = 0.1;

	pt =cvPoint2D32f(0, 0);

	zoom_img = cvCloneImage(src_img);
	
	printf("任意の倍率を指定して下さい。\n");
	//scanf("%lf", &scale);

	//scaleの値が0以下の場合はエラー
	if (scale <= 0) {
		/* scaleが0の場合 */
		printf("scale＝0は無効です。\n");
		return;
	}

	//resize後のcut_imgを用意
	dst_img = resize(scale);

	// set ROI
	center = cvPoint2D32f(rect.x + dst_img->width*0.5, rect.y + dst_img->height*0.5);
	pt = cvPoint2D32f(center.x - (dst_img->width*scale*0.5), center.y + (dst_img->height*scale*0.5));
	
	zoom.x = (int)pt.x;
	zoom.y = (int)pt.y;
	zoom.width = dst_img->width*scale;
	zoom.height =dst_img->height*scale;

	cvSetImageROI(zoom_img, zoom);

	for (y =(int) pt.y; y < dst_img->height; y++) {
		for (x = (int)pt.x; x < dst_img->width; x++) {
			/* 画素値を直接操作 */
			dst[0] = dst_img->imageData[dst_img->widthStep * y + x * 3];		// B
			dst[1] = dst_img->imageData[dst_img->widthStep * y + x * 3 + 1];	// G
			dst[2] = dst_img->imageData[dst_img->widthStep * y + x * 3 + 2];	// R
			
			zoom_img->imageData[zoom_img->widthStep * y + x * 3] = cvRound(dst[0]);
			zoom_img->imageData[zoom_img->widthStep * y + x * 3 + 1] = cvRound(dst[1]);
			zoom_img->imageData[zoom_img->widthStep * y + x * 3 + 2] = cvRound(dst[2]);
		}
	}

	

	// (5)reset ROI, then show source and destination image with a rectangle indicating ROI
	cvResetImageROI(zoom_img);
	cvResetImageROI(dst_img);

	cvNamedWindow("zoom", CV_WINDOW_AUTOSIZE);
	cvShowImage("zoom", zoom_img);
	
	cvWaitKey(0);

	cvDestroyWindow("zoom");

	return;

}


IplImage*  resize(double scale) {
	IplImage *dst_img = 0;

	//拡大・縮小後のIplimageを容易
	dst_img = cvCreateImage(cvSize((int)cut_img->width*scale, (int)cut_img->height*scale), IPL_DEPTH_8U, 3);
	
	//サイズを変更
	cvResize(cut_img, dst_img, CV_INTER_CUBIC);

	return dst_img;
}