はじめに

RoboShellは、HS法フレームワークに基づき開発された運動学計算シミュレーションのプログラムである。
Linuxで動作し、ロボットの構造データは、共有メモリにロードされ、そのデータに対して逆運動学計算を
行う各種コマンドを利用して、ロボットの状態を求める。

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

simukationsディレクトリには、サンプルがあります。





---------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------
＜ジョイントデータ＞
---------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------
ジョイントのデータは、以下の構造です。
プログラム中で自由にアクセスできます。

typedef struct _joint_ {
	double 	ApData[3];	//位置
	double 	AaData[3];	//　方向
	double 	dtData;		//求めた変位
	double 	NumberOfDtData;	//このジョイントを使うEFの数
	double 	dtAmount;	//積算変位
	double 	AxisLimit[2];	//軸の変位リミット
	int 	upper[10];	//上位リンクの番号
	int 	lower[10];	//下位リンクの番号
	double 	kp[2];		// 協調パラメータ
	int	TypeOfJoint; 	// 0x00:回転, 0x01:並進

} JOINT ;
#define LINER	0x01
#define STOP	0x02

---------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------
＜ジョイント定義ファイル＞
---------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------

ジョイントデータは、以下のフォーマットで作成します。
ファイルの最初には、EFと各ジョイントのデータを記述します。以下は例です。

4				ー＞ジョイントの数（EFもジョイントの１つとして記述する。）
#0				ー＞ジョイントの番号を記号’＃’に続けて書く
0.0	0.0	0.0		ー＞ジョイントの位置（回転軸軸の中心／並進軸の原点）
0.0	0.0	1.0		ー＞回転軸の方向ベクトル／並進軸の方向
-1.570796327	1.570796327	ー＞変位の最大値／最小値（制限を設けない場合は、-99999 99999などとしておく）
1	-1			ー＞上位軸の番号リスト（最大１０個）。リストの終わりは?１を書く
-1				ー＞下位軸の番号リスト（最大１０個）。リストの終わりは?１を書く
 0.01	 0.01 			ー＞協調パラメータの初期値（プログラム中で変更可）
0x00				ー＞回転軸：０ｘ００、並進軸：０ｘ０１、停止：0ｘ02　その他

//Joint０の定義			ー＞１つのジョイントの設定のあと、次の＃まで、自由にコメントを書ける。
#1
0.0	10.0	0.0
0.0	0.0	1.0
-1.570796327	1.570796327
2	-1
0	-1
 0.01	 0.01 
0x00


---------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------
＜ロボシェルコマンド＞
---------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------



＜ロボットの構造データのロード＞

コマンド	LoadRobo  <キーコード> <ロボットの構造データファイル名>

キーコード：
キーコードは１台のロボットに対して付けられる識別子であり、このコードを帰ることで複数台の
ロボットも取り扱うことができる。

ex)
	$ ./LoadRobo  100  UpperBody.dat
	
	UpperBody.datに記述されているロボットの構造データをメモリにロードする。
	

＜ロボットの構造データの消去＞

コマンド	ipcrm -M  <キーコード>
	
このコマンドは、Linuxのシェルコマンドであり、共有メモリを削除する。
また、ipcs -m　で現在の共有メモリが表示される。キーコードは16進数で表示される。
	

＜ロボットの操作コマンド１＞

コマンド：	RoboCnt	<キーコード> ( < -d > < -s 数字>)

キーコードで指定されるロボットの逆運動学計算を行う。
標準入力からタスク点の数、タスク点の番号と目標座標をタスク点数分入力する。
リダイレクションを使ってファイルのデータを入力すると便利である。

-d　オプションを指定すると、局所逆運動学計算がタスク点であるジョイントで終了する。（分担モード）
指定していなければ、ベースまで計算が進む。（混合モード）

-s　オプションはその後に＜数字＞が続き、タスク点から下位＜数字＞分の局所逆運動学計算で終了する。

ex)
	$ ./RoboCnt 100 < FP31.txt
	
	キーコード100のロボットの逆運動学計算を行う。FP31.datは、タスク点の数と番号及び目標座標が
	記述されたファイルである。
	
直接入力の例）

	[satake@core:RoboShell]$ ./RoboCnt 100 
	2
	4	50	-20.0	100
	10	-50	-20.0	0

	[satake@core:RoboShell]$ 

以下の例では、?dオプションを指定しているので、タスク点であるジョイント10に対する局所逆運動学計算は、
下位のタスク点であるジョイント４の計算が終わると下位のジョイントの計算は行わない。
	[satake@core:RoboShell]$ ./RoboCnt 100 -d
	2
	4	50	-20.0	100
	10	-50	-20.0	0

	[satake@core:RoboShell]$ 


＜ロボットの操作コマンド２＞

コマンド：	RoboCntCP	<キーコード> ( < -d > < -s 数字>)

キーコードで指定されるロボットの逆運動学計算を行うと同時に、繰り返し計算過程の途中結果（距離の評価）をファイルに出力する。
ファイル名は、"CnvPro.dat"と"CnvProth.dat"である。それぞれ、距離、変位が格納される。
その他の使用法は、RoboCntと同様である。

＜ロボットの操作コマンド３＞

コマンド：	RoboCntEx1000	<キーコード> ( < -d > < -s 数字>)

使用法は、RoboCntと同様であるが、収束条件を評価せずに１０００回の繰り返し計算を行う。

＜姿勢表示用のデータの出力＞

コマンド	PosePlotS　  <キーコード>

キーコードで指定されたロボットの現在の状態を出力する。
データは、ジョイント間を線分として表示するためのデータで、gnuplotのsplotコマンドなどで表示することができる。

ex)
	$  ./PosePlotS 100 > ooo1.txt
	
	キーコード100のロボットのデータを出力。リダイレクションでooo1.txtに書き込んでいる。このデータは、gnuplotで、splotコマンドを使用して表示することができる。
	
	gnuplot> splot "ooo1.txt" with lp

	
	

＜ロボットモデルの保存＞

コマンド	SaveRobo  <キーコード>

キーコードのロボットのモデルデータを出力する。ファイルに保存したい場合はリダイレクションを使う。


＜ロボットモデルの再ロード＞

コマンド	ReLoadRobo  <キーコード>　<ロボットの構造データファイル名>

キーコードで表されるロボットをメモリに読み込む。ロボット構造データは、変位量が加えられているだけで、LoadRoboのファイルと同じフォーマットである。
SaveRoboで保存したロボットに加えられている変位量をそのまま読み込むことができるようになったLoadRobo。


＜ロボットモデルの座標、ベクトル、変位を継承＞

コマンド　Inherit　<キーコード>　<継承したいデータをもつロボットの構造データファイル名>

LoadRobo　もしくは　ReLoadRobo　でメモリに読み込んだキーコードのロボットに他のロボットの座標、ベクトル、変位を継承する。


＜ロボットを平行移動＞

コマンド	PMove  <キーコード>　<Xの変位>　<Yの変位>　<Zの変位>

キーコードのロボットを（X,Y,Z)だけ平行移動する。
	
＜順運動学計算＞

コマンド	F_Kine  <キーコード>　

キーコードのロボットの順運動学計算を行う。コマンド入力後、入力するデータの数、ジョイント番号と変位（度）の組をデータ数分入力する。

ex)
	$  ./F_Kine 100
	2
	10 30.0
	11 45.0
	
	キーコード100のロボットのジョイント10に30度と11に45度を加える。リダイレクションを使えば、ファイルに書き込んだデータを入力することもできる。
	
＜タスク点（ジョイント番号）の座標値を取り出す＞

コマンド	GetPositions  <キーコード>　

キーコードのロボットの任意の番号のタスク点（ジョイント）の位置を取り出す。

ex)
	$ ./GetPositions 120
	5
	0 1 2 3 4
	0	100.000000	100.000000	10.000000
	1	138.105118	122.000000	180.000000
	2	143.301270	125.000000	180.000000
	3	148.497422	128.000000	180.000000
	4	151.961524	130.000000	180.000000

	
	上の2行は、入力データである。５つのタスク点０，１，２，３，４の座標を取り出している。リダイレクションを使うと入力が表示されない。
	
	$ ./GetPositions 120 < ooo.txt 
	0	100.000000	100.000000	10.000000
	1	138.105118	122.000000	180.000000
	2	143.301270	125.000000	180.000000
	3	148.497422	128.000000	180.000000
	4	151.961524	130.000000	180.000000

	出力にもリダイレクションを使って、
	
	$ ./GetPositions 120 < ooo.txt > ooo1.txt 
	
	$ cat ooo1.txt 
	0	100.000000	100.000000	10.000000
	1	138.105118	122.000000	180.000000
	2	143.301270	125.000000	180.000000
	3	148.497422	128.000000	180.000000
	4	151.961524	130.000000	180.000000
	
	$ cat ooo.txt 
	5
	0 1 2 3 4

＜全ジョイントの現在の値（変位）を取り出す＞

コマンド	OutPutTh  <キーコード>　

全ジョイントの現在の変位を表示する。リダイレクションを使ってファイルに出力すると良い。

	
＜全ジョイントの現在の値（変位）をジョイントの番号付きで取り出す＞

コマンド	OutPutThFkine  <キーコード>　

全ジョイントの現在の変位を表示する。リダイレクションを使ってファイルに出力すると良い。
各行の先頭にジョイント数をつけるとF_kineコマンドの入力になる

	
＜ジョイントの連鎖データを出力＞

コマンド	OutputSt  <キーコード>　

全ジョイントの上位、下位の接続関係を出力する。この出力データを変更し、次のChgstructで読み込むと接続関係が変更される。最初に出力される
値は　＄＜ベース番号＞で＄マークの後にベースとなるジョイント番号が書かれている。



＜ジョイントの連鎖データの変更＞

コマンド	Chgstruct  <キーコード>　

OUtputStの出力データを変更したのち、Chgstructで読み込むと接続関係が変更される。ベースを変更する場合は、
＄マークの後にベースとなるジョイント番号を変更する。


	
＜gnuplotにジョイント番号表示を行うデータの出力＞

コマンド	OutLabel  <キーコード>　

リダイレクションでファイルに保存し、それをgnuplotにloadコマンドで読み込み、ロボットのポースデータと共に表示する。

	$ ../../OutLabel 100 > label.dat
	
	gnuplot> load "label.dat"
	
	
---------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------
＜経路データ＞

typedef struct Pathdata {
	int no;			//タスク店の番号
	double p[3];		//経路の点座標
	double dist;
} PATH ;


＜狭隘路進入シミュレーション用コマンド＞
---------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------

＜侵入経路データを共有メモリに登録するコマンド＞

コマンド	LoadPath  <キーコード> <経路データファイル名>

キーコード：
キーコードは１つの経路に対して付けられる識別子、他の経路及び、ロボットのコードと異なるコードを設定しなければならない。

ex)
	$ ./LoadPath 200 Path30.dat
	Path30.datに記述されている経路データをメモリにロードする。
	
	フアイル形式（テキストファイル）
	ファイルの先頭行には、点の数を記述し、以下の行にはそれぞれの座標を記述する。
	
	＜データ数＞
	＜X座標＞　＜Y座標＞　＜Z座標＞
	＜X座標＞　＜Y座標＞　＜Z座標＞
	＜X座標＞　＜Y座標＞　＜Z座標＞
	＜X座標＞　＜Y座標＞　＜Z座標＞
	
	EX)
	
	200
	0.0	0.0	0.0
	10.0	0.0	0.0
	
	
＜ロボットのタスク点を１つ進め、その目標を表示するコマンド＞

コマンド	OutPutTraceNext  <ロボットのキーコード> <経路のキーコード>

ロボットの先端部から、経路に侵入させるため、ロボットのデータと、経路のデータを使って次に位置決めすべきタスク点と目標座標を出力する。
この出力データを、コマンドRoboCntの入力値として用いて計算を実行する。例えば、以下のように

	../../OutPutTraceNext 100 200 | ../../RoboCnt 100 -d >> ResultF.txt
	
ロボットのキーコードと経路のキーコードを指定してOutPutTraceNextコマンドを実行すると、進入しているタスク点の情報があるので、
それを使用して、次に位置決めすべきタスク点（複数）とその目標値を出力する。それを、OSのパイプコマンドを利用して、RoboCntコマンド
の入力としている。


＜ロボットの現在のタスク点と全ての経路点を表示するコマンド＞

コマンド	OutPutPath  <経路のキーコード>

タスク点と経路の点列座標からなる情報を出力するコマンド

	$ OutPutPath 200
	
	20
	-1      8.000000        180.000000      0.0000001.000000e+08
	-1      10.000000       180.000000      0.0000001.000000e+08
	-1      12.000000       180.000000      0.0000001.000000e+08
	99      14.000000       180.000000      0.0000001.000000e+08


＜タスク点番号と経路点列の組が記述されたファイルを読み共有メモリに登録するコマンド＞

コマンド	ReLoadPath  <キーコード> <経路データファイル名>

キーコード：
キーコードは１つの経路に対して付けられる識別子、他の経路及び、ロボットのコードと異なるコードを設定しなければならない。

ex)
	$ ./ReLoadPath 200 Path30.dat
	Path30.datに記述されている経路データをメモリにロードする。
	
	フアイル形式（テキストファイル）
	ファイルの先頭行には、点の数を記述し、以下の行にはそれぞれの座標を記述する。
	
	＜データ数＞
	＜タスク点番号＞　＜X座標＞　＜Y座標＞　＜Z座標＞
	＜タスク点番号＞　＜X座標＞　＜Y座標＞　＜Z座標＞
	＜タスク点番号＞　＜X座標＞　＜Y座標＞　＜Z座標＞
	＜タスク点番号＞　＜X座標＞　＜Y座標＞　＜Z座標＞
	
	EX)
	
	200
	-1	0.0	0.0	0.0
	20	10.0	0.0	0.0

	
	−1は、対応するタスク店がないケース、20はタスク点の番号で、その後にXYZ座標が続き1つの組となる。
	この情報は、OutPutPathコマンドで出力されたデータと同じ構造であり、OutPutPathで出力された
	データを加工して利用する際などに用いれば、ロボットの形態を変更することができる。
	

＜多点位置決め（形状制御）において、各タスク点と目標点との距離を出力する＞

コマンド	MultiDist  <キーコード> 

キーコード：
キーコードは１つの経路に対して付けられる識別子、他の経路及び、ロボットのコードと異なるコードを設定しなければならない。

	下記のようにコマンド入力後、ジョイント（タスク点）の番号と位置を入力する。
ex)
	$ ./MultiDist 100 
	10
	11	13.900000	107.700000	0.000000
	12	14.400000	86.900000	0.000000
	13	17.800000	62.500000	0.000000
	14	29.200000	42.300000	0.000000	
	15	48.000000	36.500000	0.000000
	16	67.000000	34.900000	0.000000
	17	84.400000	34.700000	0.000000
	18	96.200000	47.800000	0.000000
	19	100.800000	69.800000	0.000000
	20	109.500000	89.500000	0.000000

以下のような結果が出力される。
11	9.717375e+01	13.900000	107.700000	0.000000	72.615032	185.129213	0.000000
12	1.310455e+02	14.400000	86.900000	0.000000	90.856399	193.329977	0.000000
13	1.650047e+02	17.800000	62.500000	0.000000	109.912803	199.400683	0.000000
14	1.892258e+02	29.200000	42.300000	0.000000	129.644413	202.666193	0.000000
15	1.944926e+02	48.000000	36.500000	0.000000	149.641422	202.320314	0.000000
16	1.921284e+02	67.000000	34.900000	0.000000	169.090848	197.659859	0.000000
17	1.845639e+02	84.400000	34.700000	0.000000	186.755523	188.281200	0.000000
18	1.644387e+02	96.200000	47.800000	0.000000	201.147942	174.393857	0.000000
19	1.404504e+02	100.800000	69.800000	0.000000	210.938462	156.954084	0.000000
20	1.159137e+02	109.500000	89.500000	0.000000	215.061120	137.383604	0.000000

予め、ファイルに作っておき次のように入力しても良い。

	$ ./MultiDist 100 < Points.txt > ResultDist.txt
	

