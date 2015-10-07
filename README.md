# AROW

AROWの実装を与える.

## AROW の原理

- [http://cympfh.cc/study/ai/arow.pdf](http://cympfh.cc/study/ai/arow.pdf)

特に `r` パラメタはこのスライドのものと一致して用いる.

## 使い方 (コマンドライン)

できることは、

1. 学習ファイル (正解ラベル附) をオンライン学習して結果をモデルとして吐く.
1. モデルを読み込んで別な学習ファイル (同じ次元であること) を加えて学習してモデルを吐く.
1. モデルを読み込んで、新しい予測、あるいはテストを行う.

```
$ ./arow -?
  --diag or -D       set diagonalize=true (in default: false). ignored when -f
  -r <double>        set the parameter `r` (in default: 1.0). ignored when -f
  -f <model-path>    load a model file
  --test             test (or predict) with a model (requires -f)
  --verbose or -V    noisy
```

### makefile より

```bash
./arow -v -r 0.1 <sample/linear 2>log/linear.log >sample/linear.model
./arow -v -r 0.01 <sample/noise 2>log/noise.log >sample/noise.model
./arow -v -D -r 0.1 <sample/linear 2>log/linear.diag.log >sample/linear.diag.model
./arow -v -D -r 0.01 <sample/noise 2>log/noise.diag.log >sample/noise.diag.model
./arow -v -f sample/linear.model < sample/noise 2>log/linear2noise.log
```

## オプション

### `-diag` `-D`
対角モード.
共分散行列の対角成分だけを計算して用いる.
精度を多少失うかわりに、使用するメモリと計算量がquadraticだったのが厳密に線形になる.
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

### 学習データ

`sample/linear` `sample/noise` をサンプルとして与える.

形式的には次のようなフォーマットが学習データ

```
n
y_1 x_11 x_12 ... x_1n
 :
y_n x_n1 x_n2 ... x_nn
```

`n` は自然数.
`y_i` は `1` または `-1`.
`x_ij` は `long double` で記述できる数値.

意味として、`n`は点`x`が存在する空間の次元数.
`x = (x1, .. xn)`
であり
`x`のラベルが`y`のとき、

```
y x1 .. xn
```

と一行に記述したものを並べたものである.

### モデル

基本的に標準出力で吐いたものをそのまま使うことを想定するが、
単なるテキストファイルであって、見れば意味を読み解くのは容易である.

`sample/linear.diag.model` など

# お遊び

```bash
make test
cd log
./visualize
```

`sample/linear` は、ランダムに発生させた `(x,y,z)` に
`2x+y = 0` を境界にしてラベルを付けただけの単純な人工データである.

`sample/noise` は、全く同様に作成した人工データであるが、
10% の確率で、ラベルを逆転させた、ノイズ付きである.

`./visualize`
は、オンライン学習の過程で分類誤りが蓄積していく様子をプロットする.
ある時点で完全に正しく学習したのなら、
プロットはある時点から収束して横ばいになるはずである.
10%のノイズ付きなら、正しく学習して、
仮にその時点で正しく学習できたと分かったのなら、それ以上更新する必要はないので、
10%の割合で分類を誤るだけのものが出来上がる.
現実的に、自身がいつ正しく学習できたかは、知ることはできないため、
分類が誤った時点で、自身を更新してしまう.
正しく学習できたのに更新すると、当然ながら、誤った学習をするわけである.
従って、10%よりも高い割合で常に誤り続けることになる.

`make test` は、

- `sample/linear` を普通にオンライン学習
- `sample/linear` を対角モードでオンライン学習
- `sample/noise` を普通にオンライン学習
- `sample/noise` を対角モードでオンライン学習
- `sample/linear` を普通に学習したあと、 `sample/noise` を普通にオンライン学習する

## 例

データの前半で学習して後半でテストしてみる.

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



