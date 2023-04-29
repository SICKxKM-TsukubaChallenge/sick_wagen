sick_whill
===
基本的にこのsick_whillリポジトリのconfig,src,launchに記入していく。（元のgit cloneしたリポジトリのコードは極力変更しない）
srcフォルダにC++やpythonでノードを作成した場合、CMakeLists.txtにビルド情報を付け足す。
### ROSの勉強について
* ROS講座00 
入門　基礎編・発展編を学習し、適宜参照する。
https://qiita.com/srs/items/5f44440afea0eb616b4a
* ROSチュートリアル（公式サイト）
http://wiki.ros.org/ja/ROS/Tutorials
* navigation stackを理解する
https://qiita.com/MoriKen/items/0b75ab291ab0d95c37c2
* 車椅子ロボットWHILLをROSで自立走行させる（MoriokaLab）
https://moriokalab.com/news/103

### メガローバーの操作
メカナムローバーVer2.1 ROS 制御モード 取扱説明書
* [https://www.vstone.co.jp/products/wheelrobot/pdf/mecanumrover21_ROSmanual_20200819.pdf](https://www.vstone.co.jp/products/wheelrobot/pdf/mecanumrover21_ROSmanual_20200819.pdf)

### WHILLの操作
GitHub WHILL/ros_whill
* [https://github.com/WHILL/ros_whill](https://github.com/WHILL/ros_whill)

## Description
### install
### sick_whillに必要なROSパッケージとリポジトリ
packageのインストールには基本的にros-ROSのバージョン-パッケージ名で記載する。
* ros-melodic-serial
* ros-melodic-tf2-sensor-msgs
* ros-melodic-joy
* ros-melodic-joystick-drivers
* ros-melodic-amcl
* ros-melodic-map-server
* ros-melodic-move-base

```
sudo apt-get install PACKAGE_NAME
```
 `example` 
```
sudo apt-get install ros-melodic-serial ros-melodic-tf2-sensor-msgs
```
Ubuntu 18.04で `melodic` のブランチ・バージョンのリポジトリ
* robot_localization
* robot_pose_ekf
* sick_scan
* ros_whill
* megarover_samples
* ublox
```
git clone PACKAGE_URL

``` 
## メガローバーが動かないor通信できない(デバイス名(ttyUSB0)が認識しない), コントローラーがスリープ状態に入ってしまった時
* メガローバーの電源は入っている
* デバイス名は正しいか(コマンドで確認)
```
ls /dev/tty*
```
```
dmesg
```
* 上記を確認しても動かない場合(順番が非常に重要)
1. メガローバーの電源を切る
2. PCとメガローバーの電源基盤に挿しているシリアルコードを抜く
3. メガローバーの電源を入れる
4. 電源基板にmicro USBを挿入
5. PCにUSB Type-Aを挿入
6. 再びROSノードを起動
```
rosrun rosserial_python serial_node.py _port:=/dev/ttyUSB0 _baud:=115200
```

## Usage
catkin_make
```
catkin_make install --cmake-args -DROS_VERSION=1
```
### メガローバーでnavigationしてみる(GPS有り,robot_localizationを利用)
launch/robot_localization下のlaunchファイルで操作。

```
roslaunch sick_whill gps_move_base.launch
```

下記ノードを起動する。
*  `sick_whill pub_ekf_odom` subscribe(/rover_odo),publish(/odom)
*  `ublox` publish(/ublox/) 
*  `robot_localization navsat_transform_node` subscribe(/odometry/filterd, /ublox/fix),publish(/odometry/gps) 
*  `robot_localization ekf_localization_node` subscribe(/odometry/gps, /odom, /amcl_pose),service(/set_pose), publish(/odometry/filterd,/tf) 
*  `amcl` remap /initialpose -> /set_pose
*  `move_base` publish(/rover_twist)  
*  `rviz` remap /initialpose -> /set_pose
*  `sick_scan sick_generic_caller` 
*  `map_server` 
*  `tf` 

#### sick_whill/launch/robot_localization/gps_move_base.launch起動時のrqt-graphとtf
<img src="/uploads/d6e7bfc2fec3cdc01a3a005beaf831f3/iOS_の画像__1_.jpg" width="450">
<img src="/uploads/d7f174df945db4758960d7161953dbc9/iOS_の画像.jpg" width="450">

### configフォルダ
config/move_base/以下にそれぞれプランナーを用いたパラメータファイルを保存。
config/robot_localization/以下にnavsat_transform_node,ekf_localization_nodeのパラメータファイル。
### launchフォルダ
*  `launch/gps/` 
ublox_device.launchでsick_whill/config/ublox_gps/のyamlファイルを読み込みublox_gpsノードを起動
### srcフォルダ
* cmd_vel_pub0825.cpp　- 
whillを操作するcmd_velの優先順位("/whill/state/joy" > "joy" > "cmd_vel_nav(move_base)" )
* pub_odom.cpp - 
megarover_samplesのソースコードのコピー
* pub_none_odom_tf.cpp - 
tf(odom -> base_link)をbroadcastしない。odomトピック（navmsgs/Odometry,frame_id="odom"）をpublishする。
* pub_ekf_odom.cpp - 
共分散有り。tf(odom_combined -> base_footprint)をbroadcastしない。odomトピック（navmsgs/Odometry,frame_id="odom" child_frame_id="base_link"）をpublishする。


