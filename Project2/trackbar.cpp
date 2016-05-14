#include <cv.h>
#include <highgui.h>
#include <stdio.h>

/* グローバル変数 */
CvFont font;
IplImage *img = 0;

/* プロトタイプ宣言 */
void on_trackbar1 (int val);
void on_trackbar2 (int val);

int
main (int argc, char *argv[])
{
  // (1)画像領域を確保し，初期化する
  img = cvCreateImage (cvSize (400, 200), IPL_DEPTH_8U, 3);
  cvZero (img);
  cvInitFont (&font, CV_FONT_HERSHEY_DUPLEX, 1.0, 1.0);

  // (2)ウィンドウ，およびトラックバーを作成する
  cvNamedWindow ("Image", CV_WINDOW_AUTOSIZE);
  cvCreateTrackbar ("Trackbar1", "Image", 0, 100, on_trackbar1);
  cvCreateTrackbar ("Trackbar2", "Image", 0, 100, on_trackbar2);
  cvShowImage ("Image", img);
  cvWaitKey (0);

  cvDestroyWindow ("Image");
  cvReleaseImage (&img);

  return 0;
}

/* コールバック関数 */
void
on_trackbar1 (int val)
{
  char str[64];

  // (3)トラックバー2を，トラックバー1に同期させる
  cvSetTrackbarPos ("Trackbar2", "Image", val);

  // (4)トラックバー1の値を描画する
  cvRectangle (img, cvPoint (0, 0), cvPoint (400, 50), cvScalar (0), CV_FILLED);
  snprintf (str, 64, "%d", val);
  cvPutText (img, str, cvPoint (15, 30), &font, CV_RGB (0, 200, 100));
  cvShowImage ("Image", img);
}

void
on_trackbar2 (int val)
{
  char str[64];
  int pos1, pos2;

  // (5)トラックバー2の移動範囲を，トラックバー1の値±20に限定する
  pos1 = cvGetTrackbarPos ("Trackbar1", "Image");
  if (pos1 > val)
    pos2 = pos1 - 20 < val ? val : pos1 - 20;
  else
    pos2 = pos1 + 20 > val ? val : pos1 + 20;
  cvSetTrackbarPos ("Trackbar2", "Image", pos2);

  // (6)トラックバー2の値を描画する
  cvRectangle (img, cvPoint (0, 50), cvPoint (400, 100), cvScalar (0), CV_FILLED);
  snprintf (str, 64, "%d", val);
  cvPutText (img, str, cvPoint (15, 80), &font, CV_RGB (200, 200, 0));
  cvShowImage ("Image", img);
}

