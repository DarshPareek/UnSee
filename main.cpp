#include <iostream>
#include <opencv2/core/base.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>

#include "fastGauss.cpp"
int main() {
  int bs, ch;
  cv::Mat image;
  std::string fpath;
  std::cout << "Enter the File Path : ";
  std::cin >> fpath;
  std::cout << "Enter a number between 1(min) and 100(max) to decide the blurs "
               "strength"
            << std::endl;
  std::cin >> bs;
  std::cout << "You Selected blur strength of " << bs << std::endl;
  while (true) {
    fastGauss(fpath, bs);
    image = cv::imread(fpath);
    cv::imshow("Original Image", image);
    cv::waitKey(0);
    std::cout << "Enter: \n1 to close \n2 to choose a different blur strength "
                 "\n3 to choose a different image"
              << std::endl;
    std::cin >> ch;
    if (ch == 1) {
      cv::destroyAllWindows();
      return 0;
    } else if (ch == 2) {
      cv::destroyAllWindows();
      std::cout
          << "Enter a number between 1(min) and 100(max) to decide the blurs "
             "strength"
          << std::endl;
      std::cin >> bs;
      std::cout << "You Selected blur strength of " << bs << std::endl;
      fastGauss(fpath, bs);
      image = cv::imread(fpath);
      cv::imshow("Original Image", image);
      cv::waitKey(0);
      std::cout
          << "Enter: \n1 to close \n2 to choose a different blur strength "
             "\n3 to choose a different image"
          << std::endl;
      std::cin >> ch;
    } else if (ch == 3) {
      cv::destroyAllWindows();
      std::cout << "Enter the File Path : ";
      std::cin >> fpath;
      std::cout << "You Selected blur strength of " << bs << std::endl;
      fastGauss(fpath, bs);
      image = cv::imread(fpath);
      cv::imshow("Original Image", image);
      cv::waitKey(0);
      std::cout
          << "Enter: \n1 to close \n2 to choose a different blur strength "
             "\n3 to choose a different image"
          << std::endl;
      std::cin >> ch;
    }
  }
}
