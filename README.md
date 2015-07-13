# AROW

AROWの実装を与える.
学習の部分だけである.
最終的に得られた `mu` を使って勝手に分類すればいい.

## AROW 自体の説明

線形分類器のオンライン学習の一つである.
詳しい説明は [pdf](http://cympfh.cc/study/ai/arow.pdf) にある.
pdf 中の `r` というパラメータ変数は本プログラムでも共通.

## 使い方 (コマンドライン)

学習 (ラベルと点の集合) ファイルと、モデルファイルとがある.

できることは、

1. 学習ファイル (正解ラベルがついた) をオンライン学習して結果をモデルとして吐く.
1. 以前の学習結果を読み込んで、続けて別な学習ファイルをオンライン学習して結果をモデルとして吐く.

```bash
./arow < datum > model # 新しく datum を学習して結果を model として保存
./arow -f model < datum2 > model2 # modelを読み込んで datum2 を学習して結果を model2 として保存する (datumとdatum2を結合してから学習する結果と厳密に一致する)
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

```bash
./arow --help
--help or -?
--diag or -D: diagonalize (in default: no). ignored when -f
-r <long double>: configure the parameter `r` (in default: 1.0). ignored when -f
-f <file>: load a model file
--verbose or -V
```

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
0
```

`n` は自然数.
`y_i` は `1` または `-1`.
`x_ij` は long double で記述できる数値.

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

