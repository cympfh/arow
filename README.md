# AROW

AROWの実装を与える.

## AROW の原理

- [http://cympfh.cc/study/ai/arow.pdf](http://cympfh.cc/study/ai/arow.pdf)

特に `r` パラメタはこのスライドのものと一致して用いる.

## できること

1. 学習ファイル (正解ラベル附) をオンライン学習して結果をモデルとして吐く.
1. モデルを読み込んで別な学習ファイル (同じ次元であること) を加えて学習してモデルを吐く. (入出力に同じモデルを指定しないこと!)
1. モデルを読み込んで、新しい予測、あるいはテストを行う.

```
$ ./arow -?
  --diag or -D       set diagonalize=true (in default: false). ignored when -f
  -r <double>        set the parameter `r` (in default: 1.0). ignored when -f
  -f <model-path>    load a model file
  --test             test (or predict) with a model (requires -f)
  --verbose or -V    noisy
```

## オプション

### `-diag` `-D`
対角モード.
共分散行列の対角成分だけを計算して用いる.
精度を多少失うかわりに、使用するメモリと計算量がquadraticだったのが線形になる.
また、 後述する `-f` によってモデルを読み込む場合、このオプションによる指定は無視される.

### `-r`
続けて数値を指定することで、 パラメータ `r` を指定する.
数値は `long double` で読み込む.
また、 後述する `-f` によってモデルを読み込む場合、このオプションによる指定は無視される.

### `-f`
前回に本プログラムによって作成されたモデルを読み込む.
`-f` に続けて、モデルのファイルパスを指定する.
モデルには、パラメータ`p` と、対角モードを行うかどうかの情報が含まれており、
強制的にそれらを用いる.

### `--test`

標準入力から、学習データと同じフォーマットのものを読み (ラベルが不明な場合は0など)、
`-f` からモデルを読み (実際には `mu` のみ用いる)、
実際に予測をした結果を標準出力する.
標準エラーにAccとF1を出力する (ラベルとの一致を確認する).

### `-v`
冗長モード. 小うるさいモード

## ファイルのフォーマット

### 訓練データ

`sample/linear` `sample/noise` をサンプルとして与える.

学習データのフォーマットは2種類.
1つは、素性ベクトル (実数列) を並べまくるもの.
もう1つは、'idx:(実数)' を並べまくる libSVM とかでお馴染みなあれ.

ターゲットを `y` (`1` または `-1` とすること) 、
素性を n次元 `x1 x2 .. xn` とすると、

```
y_1 x_11 x_12 ... x_1n
 :
y_n x_n1 x_n2 ... x_nn
```

または

```
y_1 1:x_11 2:x_12 3:x_13 .. n:x_1n
y_2 1:x_21 2:x_22 3:x_23 .. n:x_2n
y_3 1:x_n1 2:x_n2 3:x_n3 .. n:x_nn
```

余計な空白については考慮しない.
ただし空行と `#` から始まる行は無視する.

特に後者については、
`i:x`
が省略されていた場合については、
`i` 番目は `0.0` だとして解釈する.
疎でインデックスがでかい場合には後者がよい
(AROWはSVMみたいに次元の数に強いというわけではないけれど).

### テストデータ

訓練と同様であるが、
ターゲットについては、正しいものが書かれてるとみなして、最後に精度やFスコアといったテスト結果を示す.
それ以外に利用することはないので、テスト結果が不要ならば、
未知の場合は `0` などと書いておけばよい.

### モデル

基本的に標準出力で吐いたものをそのまま使うことを想定するが、
単なるテキストファイルであって、見れば意味を読み解くのは容易である.

`sample/linear.diag.model` など

## 例

### Makefile より

```bash
./arow -r 0.1     < sample/linear > sample/linear.model
./arow -f sample/linear.model --test < sample/linear > sample/linear.out

./arow -D -r 0.01 < sample/noise  > sample/noise.diag.model
./arow -f sample/noise.diag.model --test < sample/noise > sample/noise.diag.out
```

これは訓練事例とテスト事例とに同じ物を用いている
(closed test ってゆうんだって).

### データの前半で学習して後半でテストしてみる

```bash
% wc -l sample/linear
1001 sample/linear
```
最初の一行は次元数、続く1000行にデータ.

```bash
% head -n 501 sample/linear | ./arow > hoge.model
```

頭の500データで学習

```bash
% ( head -1 sample/linear; tail -n 500 sample/linear ) | ./arow -f hoge.model --test >/dev/null
Acc 0.994
Rec 0.970874
Prec 1
F1 0.985222
```

末尾の500データでテスト.

