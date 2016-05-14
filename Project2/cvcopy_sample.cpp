#include <cv.h>
#include <highgui.h>

int
main (int argc, char **argv)
{
  IplImage *src_img = 0, *dst_img = 0;
  IplImage *cpy_img, *dst_img_gray, *msk_img;

  // (1)画像を読み込む
  if (argc != 3 ||
      (src_img = cvLoadImage (argv[1], CV_LOAD_IMAGE_COLOR)) == 0 ||
      (dst_img = cvLoadImage (argv[2], CV_LOAD_IMAGE_COLOR)) == 0)
    return -1;

  cpy_img = cvCloneImage (dst_img);
  if (src_img->width != dst_img->width)
    return -1;
  if (src_img->height != dst_img->height)
    return -1;


  // (2)dst_imgの画像のRedのプレーンだけを2値化してマスクを作成する
  dst_img_gray = cvCreateImage (cvGetSize (dst_img), IPL_DEPTH_8U, 1);
  cvSplit (dst_img, NULL, NULL, dst_img_gray, NULL);

  msk_img = cvCloneImage (dst_img_gray);
  cvSmooth (dst_img_gray, dst_img_gray, CV_GAUSSIAN, 5);
  cvThreshold (dst_img_gray, msk_img, 0, 255, CV_THRESH_BINARY_INV | CV_THRESH_OTSU);

  // (3)src_imgをdst_imgにコピーする．マスキングされている部分は元の値が保持される．
  cvCopy (src_img, cpy_img, msk_img);


  // (4)結果を表示する
  cvNamedWindow ("src", CV_WINDOW_AUTOSIZE);
  cvNamedWindow ("dst", CV_WINDOW_AUTOSIZE);
  cvNamedWindow ("mask", CV_WINDOW_AUTOSIZE);
  cvNamedWindow ("copy", CV_WINDOW_AUTOSIZE);
  cvShowImage ("src", src_img);
  cvShowImage ("dst", dst_img);
  cvShowImage ("mask", msk_img);
  cvShowImage ("copy", cpy_img);
  cvWaitKey (0);

  cvDestroyWindow ("src");
  cvDestroyWindow ("dst");
  cvDestroyWindow ("mask");
  cvDestroyWindow ("copy");
  cvReleaseImage (&src_img);
  cvReleaseImage (&dst_img);
  cvReleaseImage (&msk_img);
  cvReleaseImage (&cpy_img);
  cvReleaseImage (&dst_img_gray);

  return 1;

}