# C++ OpenCV 車牌辨識


- [x] 影像前處理
- [x] 車牌ROI識別
- [x] 字符切割
- [x] 字符識別
- [ ] UI

* 影像前處理
    * 灰階
    * 自適應二值化
* 車牌ROI識別
    * Canny邊緣偵測
    * findContours尋找輪廓
    * 尋找輪廓所包覆最大區域 (暫時解法)
    * 找尋矩形四個角落的座標點
        * ConvexHull減少候選點
        * HoughLine找線
        * 尋找線與線的交點
    * 調整矩形至方形
        * 透視轉換
    * 調整矩形角度
        * 仿射變換
    
* 字符切割
    * 水平投影
    * 垂直投影

* 字符識別
    * 提取影像HOG特徵
    * SVM
    * KNN (better)

    
    

---

* 輸入影像
![](https://i.imgur.com/2dAkazf.png)

* 二值化
![](https://i.imgur.com/S9yE6yk.png)

* Canny檢測邊緣
![](https://i.imgur.com/1WGmUyg.png)

* 尋找輪廓包覆最大區域 & ConvexHull
![](https://i.imgur.com/c7maJPC.png)

* 霍夫找線
![](https://i.imgur.com/8gGyFid.png)

* 尋找線之交點並進行透視轉換
![](https://i.imgur.com/zqeO3M4.png)

* 仿射轉換調整角度
![](https://i.imgur.com/Oc6XVIP.png)

* 抓取車牌區域
![](https://i.imgur.com/bnOU8GB.png)

* 字符分割
![](https://i.imgur.com/QJSJb0p.png)

* KNN辨識
![](https://i.imgur.com/ahgcyf3.png)



