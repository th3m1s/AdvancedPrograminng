#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#include <cv.h>
#include <highgui.h>

//グローバル変数
IplImage *src_img = 0, *cut_img = 0;
CvFont font;
CvRect rect;

//プロトタイプ宣言
void on_mouse(int event, int x, int y, int flags, void *param);
void cut(void);

int main(int argc, char *argv[]) {
	int c;
	void *hwnd;
	char path[80];


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
	}
	else {
		printf("画像の読み込みに成功しました。\n");
	}

	while (1) {
		//ウィンドウを作成し，マウスイベントに対するコールバック関数を登録する
		cvNamedWindow("src", CV_WINDOW_AUTOSIZE);
		cvSetMouseCallback("src", on_mouse);
		cvShowImage("src", src_img);

		//画像の切り取り・表示
		cut();

		cvNamedWindow("cut", CV_WINDOW_AUTOSIZE);
		cvShowImage("cut", cut_img);
	}

	cvWaitKey(0);

	//メモリ解放
	cvDestroyWindow("src");
	cvDestroyWindow("cut");
	cvReleaseImage(&src_img);
	cvReleaseImage(&cut_img);

	return 0;
}

/* コールバック関数 */
void on_mouse(int event, int x, int y, int flags, void *param = NULL) {
	char str[64];
	static int line = 0;
	const int max_line = 15, w = 15, h = 30;
	CvPoint origin = { 0, 0 };

	//マウスイベントを取得
	switch (event) {
	case CV_EVENT_MOUSEMOVE:
		snprintf(str, 64, "(%d,%d) %s", x, y, "MOUSE_MOVE");
		break;
	case CV_EVENT_LBUTTONDOWN:
		snprintf(str, 64, "(%d,%d) %s", x, y, "LBUTTON_DOWN");
		origin = cvPoint(x, y);
		rect.x = x;
		rect.y = y;
		break;
	case CV_EVENT_RBUTTONDOWN:
		snprintf(str, 64, "(%d,%d) %s", x, y, "RBUTTON_DOWN");
		break;
	case CV_EVENT_MBUTTONDOWN:
		snprintf(str, 64, "(%d,%d) %s", x, y, "MBUTTON_DOWN");
		break;
	case CV_EVENT_LBUTTONUP:
		snprintf(str, 64, "(%d,%d) %s", x, y, "LBUTTON_UP");
		//ドラッグ後に切り取り範囲の設定と短形生成
		if (rect, x != NULL || rect.y != NULL) {
			rect.width = rect.x - x;
			rect.height = rect.y - y;

			//切取範囲の短形生成
			cvRectangle(src_img, origin, cvPoint(origin.x + rect.width, origin.y + rect.height), CV_RGB(255, 0, 255), 2, 8, 0);
		}
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
	//ビット演算子?
	if (flags & CV_EVENT_FLAG_LBUTTON)
		strncat(str, " + LBUTTON", 64);
	if (flags & CV_EVENT_FLAG_RBUTTON)
		strncat(str, " + RBUTTON", 64);
	if (flags & CV_EVENT_FLAG_MBUTTON)
		strncat(str, " + MBUTTON", 64);
	if (flags & CV_EVENT_FLAG_CTRLKEY)
		strncat(str, " + CTRL", 64);
	if (flags & CV_EVENT_FLAG_SHIFTKEY)
		strncat(str, " + SHIFT", 64);
	if (flags & CV_EVENT_FLAG_ALTKEY)
		strncat(str, " + ALT", 64);

	// (6)マウス座標，イベント，修飾キーなどを画像に描画，表示
	if (line > max_line) {
		cvGetRectSubPix(src_img, src_img, cvPoint2D32f(320 - 0.5, 240 - 0.5 + h));
		cvPutText(src_img, str, cvPoint(w, 20 + h * max_line), &font, CV_RGB(0, 200, 100));
	}
	else {
		cvPutText(src_img, str, cvPoint(w, 20 + h * line), &font, CV_RGB(0, 200, 100));
	}
	line++;

	cvShowImage("Window", src_img);

}


void cut(void) {
	IplImage *dst_img = 0;

	//画像のrect指定部分を切り取る
	//パラメータの値が負の場合はエラーを出力
	if (rect.x < 0 || rect.y < 0 || rect.width <= 0 || rect.height <= 0) {
		printf("その値は無効です。\n");
		exit(0);
	}

	dst_img = cvCreateImage(cvSize(rect.width, rect.height), IPL_DEPTH_8U, 3);

	cvSetImageROI(src_img, rect);
	cvCopy(src_img, dst_img);

	cvResetImageROI(src_img);
	cvResetImageROI(dst_img);

	cut_img = cvCloneImage(dst_img);

}