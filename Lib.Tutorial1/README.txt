Tutorial1

このパッケージではスクリプトからパッケージがロードされた際の挙動について説明します。

MiniKonohaインタプリタ上で以下のコードを入力するとLib.Tutorial1パッケージが呼び出されます。
import("Lib.Tutorial1");

"Lib"はパッケージの分類、"Tutorial1"はパッケージの名前を意味します。ここでは
現在MiniKonohaではパッケージの分類として4つの分類(Syntax, Type, Lib, Common, 言語名)の4つの分類を提供しています。

1. Syntax.* : 文法関連のパッケージ名
    例. Syntax.GlobalVariable (グローバル変数についての文法)
Syntax.JavaClass (JavaStyleのクラス定義の文法)
2. Type.* : データ型に関するパッケージ
    例. Type.Bytes, Type.Floatなど
3. Lib.* : Cライブラリのバインドに関するパッケージ
    例. Lib.Curl, Lib.Libvirt
4. (プログラミング言語名).* : 各プログラミング言語の互換APIを提供するパッケージ
4.1. Java.* : Java互換のライブラリに関するパッケージ
4.2. JavaScript.* : JavaScript互換のライブラリ

5. Common.* : 各言語パッケージで利用するコア機能を提供するパッケージ
    例. Common.StringUtil : 文字列操作に関するライブラリ

さて、Lib.Tutorial1パッケージが呼び出された際、呼び出される手順は以下のとおりです。
1. パッケージの場所を特定する
現在のminikonohaではパッケージのインストール場所として以下の3つのディレクトリを順に探索し、最初にLib.Tutorial1ディレクトリがあった場所からライブラリをロードします。
   $INSTALL_PREFIX/minikonoha/$VERSION/package/
   $HOME/.konoha/minikonoha/$VERSION/package/
   $KONOHA_HOME/minikonoha/$VERSION/package/
   (なお、$INSTALL_PREFIXはデフォルトでは/usr/local, $VERSIONは2013年1月現在0.3が入ります。また、$KONOHA_HOMEは環境変数としてKONOHA_HOMEを指定した場合、探索の対象となります。)

2. 拡張ライブラリを読み込む
  パッケージの場所が特定できたらMinikonohaは拡張ライブラリの探索を行います。
  ライブラリは"パッケージ名_glue.so"(今回の例ではTutorial1_glue.so)という名前を想定しています。(なお、MacOSXの場合拡張子はdylibとなります)
  拡張ライブラリのロードには以下の手順を踏みます。
  まず、ライブラリを初期化するため"パッケージ名"_Init関数(今回の例ではTutorial1_Init)を呼び出します。

  MiniKonohaでは各パッケージはそれぞれ別々の名前空間で区切られており、パッケージ間で定義をimport, exportするための機構が用意されています。
パッケージに新たに定数などを定義する際にはTutorial1_PackupNameSpace関数を利用します。多くのパッケージではメソッド定義や文法定義などを行います。
他のパッケージにシンボルを公開する場合にはTutorial1_ExportNameSpace関数を利用します。定数定義などでこの関数を利用しています。

KDEFINE_PACKAGE *Tutorial1_Init(void)
{
    static KDEFINE_PACKAGE d = {0};
    KSetPackageName(d, "Tutorial1", "0.0");
    d.PackupNameSpace = Tutorial1_PackupNameSpace;
    d.ExportNameSpace = Tutorial1_ExportNameSpace;
    return &d;
}

3. パッケージスクリプトを読み込む
パッケージの読み込みの最終ステップはパッケージ側で定義されているスクリプトをロードすることです。
"パッケージ名"_kick.kのスクリプトがロードされます。(今回の例ではTutorial1_kick.k)
拡張ライブラリ内でCで記述されたメソッドのラッパーメソッドなどはここに記述されます。

