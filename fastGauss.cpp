#include <opencv2/core/hal/interface.h>

#include <algorithm>
#include <cmath>
#include <iostream>
#include <opencv2/core/base.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/matx.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <ostream>
#include <string>

#include "opencv2/imgcodecs.hpp"

struct Pixel {
  int r, g, b;
};

void loadImageData(cv::Mat img, int h, int w, int *imgIn) {
  unsigned char *input = (unsigned char *)(img.data);
  for (int i = 0; i < h * w * 3; i++) {
    imgIn[i] = input[i];
  }
}

void badGauss(int *imgIn, int *imgOut, int w, int h, double r) {
  int rs = std::ceil(r * 2.57);
  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) {
      double val = 0, wsum = 0;
      for (int iy = i - rs; iy < rs + i + 1; iy++) {
        for (int ix = j - rs; ix < rs + j + 1; ix++) {
          int x = std::min(w - 1, std::max(0, ix));
          int y = std::min(h - 1, std::max(0, iy));
          int dsq = (ix - j) * (ix - j) + (iy - i) * (iy - i);
          double wgt = std::exp(-dsq / (2 * r * r)) / (3.14 * 2 * r * r);
          val += imgIn[y * w + x] * wgt;

          wsum += wgt;
        }
      }
      imgOut[i * w + j] = std::round(val / wsum);
    }
  }
}

void splitChannels(int *big, int *b, int *g, int *r, int w, int h) {
  int c = 0;
  for (int i = 0; i < w * h * 3; i = i + 3) {
    if (c == w * h) {
      std::cout << "AwHellNah" << std::endl;
    }
    b[c] = big[i];
    g[c] = big[i + 1];
    r[c] = big[i + 2];
    c++;
  }
}

void combineChannels(int *b, int *g, int *r, int *big, int w, int h) {
  int c = 0;
  for (int i = 0; i < w * h * 3; i = i + 3) {
    if (c == w * h) {
      std::cout << "AwHellNah" << std::endl;
    }
    big[i] = b[c];
    big[i + 1] = g[c];
    big[i + 2] = r[c];
    c++;
  }
}

void copyToImage(int *image, unsigned char *toImg, int w, int h) {
  for (int i = 0; i < h * w * 3; i++) {
    toImg[i] = image[i];
  }
}

void testfn(int *data, int w, int h) {
  for (int i = 0; i < w * h * 3; i++) {
    if (i % 3 == 0 and i % 5 == 0) {
      data[i] = 0;
    }
  }
}

int *boxesForGauss(double sigma, int n) {
  int *size = new int[n];
  double wIdeal = std::sqrt((12 * sigma * sigma / n) + 1);
  int wl = std::floor(wIdeal);
  if (wl % 2 == 0) wl--;
  int wu = wl + 2;
  double mIdeal =
      (12 * sigma * sigma - n * wl * wl - 4 * n * wl - 3 * n) / (-4 * wl - 4);
  int m = std::round(mIdeal);
  for (int i = 0; i < n; i++) {
    if (i < m) {
      size[i] = wl;
    } else {
      size[i] = wu;
    }
  }
  return size;
}

void bBlurH(int *in, int *out, int w, int h, int r) {
  double iarr = 1.0 / (r + r + 1);
  for (int i = 0; i < h; i++) {
    int ti = i * w, li = ti, ri = ti + r;
    int fv = in[ti], lv = in[ti + w - 1], val = (r + 1) * fv;
    for (int j = 0; j < r; j++) val += in[ti + j];
    for (int j = 0; j <= r; j++) {
      val += in[ri++] - fv;
      out[ti++] = std::round(val * iarr);
    }
    for (int j = r + 1; j < w - r; j++) {
      val += in[ri++] - in[li++];
      out[ti++] = std::round(val * iarr);
    }
    for (int j = w - r; j < w; j++) {
      val += lv - in[li++];
      out[ti++] = std::round(val * iarr);
    }
  }
}

void bBlurT(int *in, int *out, int w, int h, int r) {
  double iarr = 1.0 / (r + r + 1);
  for (int i = 0; i < w; i++) {
    int ti = i, li = ti, ri = ti + r * w;
    int fv = in[ti], lv = in[ti + w * (h - 1)], val = (r + 1) * fv;
    for (int j = 0; j < r; j++) val += in[ti + j * w];
    for (int j = 0; j <= r; j++) {
      val += in[ri] - fv;
      out[ti] = std::round(val * iarr);
      ri += w;
      ti += w;
    }
    for (int j = r + 1; j < h - r; j++) {
      val += in[ri] - in[li];
      out[ti] = std::round(val * iarr);
      ri += w;
      ti += w;
      li += w;
    }
    for (int j = h - r; j < h; j++) {
      val += lv - in[li];
      out[ti] = std::round(val * iarr);
      ti += w;
      li += w;
    }
  }
}

void bBlur(int *in, int *out, int w, int h, int r) {
  for (int i = 0; i < w * h; i++) out[i] = in[i];
  bBlurH(out, in, w, h, r);
  bBlurT(in, out, w, h, r);
}

void gaussBlur(int *in, int *out, int w, int h, double r) {
  int *bxs = boxesForGauss(r, 3);
  bBlur(in, out, w, h, (bxs[0] - 1) / 2);
  bBlur(out, in, w, h, (bxs[1] - 1) / 2);
  bBlur(in, out, w, h, (bxs[2] - 1) / 2);
}

int fastGauss(std::string fPath, int bs) {
  cv::Mat image;
  image = cv::imread(fPath);
  int w, h, type;
  double rad = bs;
  w = image.cols;
  h = image.rows;
  int *imgIn = new int[w * h * 3], *imgOut = new int[w * h * 3],
      *imgOut2 = new int[w * h * 3];
  int *b = new int[w * h], *g = new int[w * h], *r = new int[w * h];
  int *bt = new int[w * h], *gt = new int[w * h], *rt = new int[w * h];
  loadImageData(image, h, w, imgIn);
  type = image.type();
  splitChannels(imgIn, b, g, r, w, h);
  gaussBlur(b, bt, w, h, rad);
  gaussBlur(g, gt, w, h, rad);
  gaussBlur(r, rt, w, h, rad);
  combineChannels(bt, gt, rt, imgOut, w, h);
  unsigned char *toImg = new unsigned char[w * h * 3];
  copyToImage(imgOut, toImg, w, h);
  cv::Mat io1 = cv::Mat(h, w, type, toImg);
  cv::imshow("Blurred Image", io1);
  return 0;
}