#JIFCSet
はじめに

JIFCSetは、関節独立型運動学計算法[1][2]に基づき開発された運動学計算のためのコマンドセットである。
Linuxで動作し、ロボットの構造データは、共有メモリにロードされ、そのデータに対し
て逆運動学計算を行う各種コマンドを利用して、ロボットの状態を求める。

[1]https://doi.org/10.2493/jjspe.85.585
[2]https://doi.org/10.2493/jjspe.91.526

---------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------
＜導入手順＞
---------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------
このディレクトリには、必要なソースプログラムとインクルードファイルの全てがあります。
インストールの手順は、

$ make
$ sudo make install

で必要な実行コマンドが/usr/local/binにコピーされるので、ここにパスが通っていればOKです。
問題なければ

$ make clean

でオブジェクトファイルなどを消しましょう。
