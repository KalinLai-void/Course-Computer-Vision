## 大學課程－「電腦視覺實務」之作業
These are the homeworks of the course "Computer Vision Implementation" in my college.

**該課程為「影像處理實務」之進階課程**，而「影像處理實務」之作業檔案[在此](https://github.com/KalinLai-void/Course_Image-Processing)。

### 環境
- VS 2019
- OpenCV 4.5.5 (CMake Build)
- C++ CLR (.Net Framework)

### 說明
1. Using OpenCV open video and transfer it to Gray (影片灰階)
   1. 使用 OpenCV 將影片檔讀進來，並運用 pictureBox 及 OpenCV 算幀數並顯示。
   2. 將每幀轉換為灰階。
2. Morphology Operation (形態學影像處理)
   1. 非套 OpenCV 的 Function，自己寫的演算法。
   2. 基本形態學影像處理
      1. Dilation
      2. Erosion
      3. Opening
      4. Closing
   3. 可自訂 Structure Element 大小
3. Moving Object Segmentation（移動物切割）
   1. 除了avi開檔套用OpenCV的開檔方式，其他非套 OpenCV 的 Function，皆為自己寫的演算法。
   2. 背景相減法（Background Subtraction）
   3. 相鄰相減法（Frame Difference）
   4. 移動物框選
      1. 使用圖學理論中的「Connected-component labeling」演算法，透過dfs遞迴實作。
   5. Demo
      1. Background Subtraction
         ![](3.%20MovingObjectSegmentation/BackgroundSubtraction.gif)
      2. Frame Difference
         ![](3.%20MovingObjectSegmentation/FrameDifference.gif)
