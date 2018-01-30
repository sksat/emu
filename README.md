# emu

master: [![Build Status](https://api.travis-ci.org/sk2sat/emu.svg?branch=master)](https://travis-ci.org/sk2sat/emu)
develop: [![Build Status](https://api.travis-ci.org/sk2sat/emu.svg?branch=develop)](https://travis-ci.org/sk2sat/emu)

https://github.com/sk2sat/vm の設計とか色々見直して新しく作っているx86エミュレータ

# ビルド・実行

```
make
```

```
make run
```

実行すると、エミュレータ操作用のシェル(もどき)が起動します。

このシェルはまず初めに、.emurcに書いてあるコマンドを実行します。
起動時に自動的に実行したいコマンドがある場合は、.emurcを編集してください。

デフォルトの.emurcでは、メモリ1MB,EIP=0x7c00,ESP=0x7c00で、0x7c00から512バイト分のsample/test.binをロードした状態にします。
そのため、runコマンドを使うだけでsample/test.binのエミュレーションが行えます。

# エラー
全然実装が進んでいないため、よくエラーが発生します。ここにはエラーの種類のメモをしておきます。

```
not implemented
```

このエラーは、実装されていない動作が要求された時に発生します。このエラーのうち、

```
not implemented: code = 16進数
```

となっているものは、16進数が示す機械語が実装されていない、というエラーです。
