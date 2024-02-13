<h1>UnSee : Blur any Image you like!!</h1>
<h4>This repository has source files for UnSee which was just a C++ Project (Nothing Fancy). It uses OpenCV to read images and a custom implementation of Gaussian Blur inspired from <a href = "https://blog.ivank.net/fastest-gaussian-blur.html">this article</a> I found while looking for fast blurring methods because the default one provided by OpenCV was not fast enough..</h4>
<hr>
<h1>1. Building....</h1>
<h2>&emsp; 1. Pre-requisites :</h2>
<div>You need to have <b>Linux(preferably Ubuntu 22.04) because I used WSL2</b>, then install  <code>cmake, OpenCV, and build-essentials</code> etc.</div>
<div>Some resilience because OpenCV generally has linking problems :D</div>
<h2>&emsp; 2. Actual Building and Running:</h2>
<div>Use the following commands : </div>
<pre>
<code>git clone https://github.com/DarshPareek/UnSee
  cd UnSee
  cmake --build .</code>
</pre>
<div>This Should generate a file(executable) named UnSee <br> To run it :</div>
<pre><code>./UnSee</code></pre>
<div>Then just follow the prompts :D.</div>
<br>
<h1>2. Release....</h1>
<div>You can directly download <code>program.o</code> from the releases and run it using the following commands : </div>
<pre><code>chmod +x program.o<br>./program.o</code></pre>
<h1>3. About Source Files </h1>
<div><code>main.cpp</code> : contains the basic loop to spare you from re-running the executable again and again for different images and blur strengths also makes calls to <code>fastGauss.cpp</code>.<br><br><code>fastGauss.cpp</code> : accepts the image path and blur strength from <code>main.cpp</code> and does all the crazy math that is required to do a <a href = "https://en.wikipedia.org/wiki/Gaussian_blur">Gaussian Blur</a>. I implemented <a href = "https://blog.ivank.net/fastest-gaussian-blur.html">this</a> js-version into C++. Call me crazy but this is way faster from what OpenCV provides and there is still scope for heavy optimization in my code.<br><br><code>CMakeList.txt</code> : contains the bare-minimum stuff to build this project. It links OpenCV libraries and does a whole lot of other stuff.</div>

<h1>4. Future....</h1>
<div>1. I might build a windows version. (windows is a niche operating system used by gamers)<br><br>
2. I might build a backend server (in C++ or Python) and a frontend GUI in HTML, CSS and JS to allow users to upload images on the localhost and choose blur strength using a slider while the image updates in real-time.</div>
