#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#include <cv.h>
#include <highgui.h>

//グローバル変数
CvFont font;
IplImage *src_img = 0,*cut_img=0;
CvRect rect;
bool drawing_box = false;


//プロトタイプ宣言
void on_mouse(int event, int x, int y, int flags, void *param);
void cut(IplImage* &dst_img2);

int main(int argc, char *argv[]) {

	char path[80];
	char save[80];

	cvInitFont(&font, CV_FONT_HERSHEY_DUPLEX, 0.4, 0.4);

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

	//ウィンドウを作成し，マウスイベントに対するコールバック関数を登録する
	cvNamedWindow("src", CV_WINDOW_AUTOSIZE);
	cvSetMouseCallback("src", on_mouse, src_img);
	cvShowImage("src", src_img);

	printf("何度でも切り取り範囲は選択可能です。\n切り取り範囲を決定した場合は任意のボタンを押して下さい。\n");
	cvWaitKey(0);

	printf("\nここで、ウィンドウ名cutの画像が保存できます。\n");
	

	cvDestroyWindow("src");
	cvDestroyWindow("cut");

	//保存を任意の名前で設定
	printf("保存用の名前を設定して下さい。拡張子を忘れないで下さい。具体例：save.jpg\n");
	scanf("%s", &save);
	cvSaveImage(save, cut_img);

	//メモリ解放
	cvReleaseImage(&src_img);
	cvReleaseImage(&cut_img);

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
		
	cvNamedWindow("cut", CV_WINDOW_AUTOSIZE);
	cvShowImage("cut", cut_img);
	
}
