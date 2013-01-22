Tutorial3 クラスを定義する

このパッケージではクラスの作成方法について説明していきます。

目次
1. スクリプトで定義するクラス、C言語で定義するクラス
2. クラスの定義
3. Konoha言語への登録
4. 注意事項

1. スクリプトで定義するクラス、C言語で定義するクラス
2. クラスの定義
3. Konoha言語への登録


あとで書く

4. 注意事項
C言語で定義されたクラスはFinalなクラスとして定義されます。
C言語で定義されたクラスをスクリプト側でextendしたい場合には以下のようにラップするオブジェクトを作成することで回避する方法をとっています。

/* defined by C exntension */
class CDefinedClass;

class MyClass /* exnteds CDefinedClass */ {
    CDefinedClass superObject;
    MyClass() {
        superObject = new CDefinedClass();
        ...
    }
}

