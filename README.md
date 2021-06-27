# m5stack_GPS

シリアル接続したGPSモジュールからGPS NMEAフォーマット情報を取得、パース処理し、Bluetooth SPPで送信します。  
> Webアプリケーションでの可視化に対応予定です。

## Arduino依存ライブラリ

### TinyGPS++
GPS NMEAフォーマットのパース処理には以下ライブラリを利用しています。  
http://arduiniana.org/libraries/tinygpsplus/

### M5Stack_ScrollTextWindow
M5Stackディスプレイ表示には以下ライブラリを利用しています。  
https://github.com/totsucom/M5Stack_ScrollTextWindow

## M5Stack操作

**左ボタン**  
パースしたNMEA情報を表示します。  

| 項目           | 表示フォーマット         |
| :------------: | :----------------------- |
| 時刻 (UTC)     | yyyy/mm/dd hh/mm/ss      |
| 緯度           | lat : 緯度（度）         |
| 経度           | lng : 経度（度）         |
| 高度           | altitude : 高度（m）     |
| 移動速度       | speed : 速度（km/h）     |
| 移動方向       | course : 移動方向（度）  |

**中央ボタン**  
NMEAログを表示します。  

**右ボタン**  
NMEAログ表示のスクロールを一時停止します。停止中にボタン押下すると再度スクロール状態に戻ります。  

## Bluetooth SPP メッセージフォーマット

| 項目           | メッセージフォーマット                       |
| :------------: | :------------------------------------------- |
| 時刻 (UTC)     | time,UNIX時間                                |
| 緯度経度       | latlng,受信時刻（UTC）,緯度（度）,経度（度） |
| 高度           | altitude,受信時刻（UTC）,高度（m）           |
| 移動速度       | speed,受信時刻（UTC）,速度（km/h）           |
| 移動方向       | cource,受信時刻（UTC）,移動方向（度）        |
