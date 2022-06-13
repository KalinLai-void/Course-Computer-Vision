## 大學課程－「電腦視覺實務」之作業
These are the homeworks of the course "Computer Vision Implementation" in my college.

**該課程為「影像處理實務」之進階課程**，而「影像處理實務」之作業檔案[在此](https://github.com/KalinLai-void/Course_Image-Processing)。

### 環境
#### 第一~四題
- VS 2019
- OpenCV 4.5.5 (CMake Build)
- C++ CLR (.Net Framework)

#### 第五題
- Google Colab
- Python 3
- PyTorch
- ~TensorFlow~（之後補上）

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
      1. 使用圖學理論中的「Connected-component labeling」演算法。使用**非遞迴式 BFS** 實作。
      2. 程式碼中也有遞迴式 DFS 的實作版本。因為最一開始是用遞迴寫，但後來發現有些案例會 Overflow（其實還蠻常的），所以才改成非遞迴式 BFS。
   5. Demo
      1. Background Subtraction
         ![](3.%20MovingObjectSegmentation/BackgroundSubtraction.gif)
      2. Frame Difference
         ![](3.%20MovingObjectSegmentation/FrameDifference.gif)
         - using OpenCV sample "[vtest.avi](https://github.com/opencv/opencv/blob/master/samples/data/vtest.avi)"
         ![](3.%20MovingObjectSegmentation/FrameDifference_OpenCV_vtestAVI.gif) 
4. Fire Detection（火焰偵測）
   1. 還沒寫完...
5. Deep Learning : Image Classification（深度學習─圖像分類） [![Open In Colab](https://colab.research.google.com/assets/colab-badge.svg)](https://colab.research.google.com/github/KalinLai-void/Course-Computer-Vision/blob/master/5.%20DL%20-%20ImageClassification/Pytorch/class_cifar.ipynb)
   1. 目前只有寫 PyTorch 的版本，TensorFlow之後有寫再補上。
      1. 以 [PyTorch 官方的「圖像分類」教學](https://pytorch.org/tutorials/beginner/blitz/cifar10_tutorial.html)修改而成。
      2. 使用 Cifar-10 的資料集訓練。
      3. 該教學的準確率僅約 50% 左右。
   2. 參考 AlexNet 的架構所修改而成，**共8層**（1-5 層為卷積層，6-8 為全連接層）。其中：
      1. 第一、二、五個 **卷積層（Convolutional Layers）** 後使用大小 3x3、stride=2 的 Maxpooling
      2. 第一、二 **全連接層（Fully Connected Layers）** 後使用 Dropout，隨機關閉神經元，藉此讓模型不過度依賴某些特徵，增強模型的泛化能力。
      3. 在輸出前，先用log型的Softmax函數（不過這個影響沒有很大）。
   3. 訓練前的設定：
      1. 損失函數使用 CrossEntropy。
      2. 訓練 25 輪，其實到 10 多輪左右就有到 80% 以上了，抓 25 是因為較穩定，不過也上不太去了。
      3. 優化器（optimizer）改用 Adam。
   4. 最終訓練出的**準確率約 82%** 左右。