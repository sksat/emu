# レジスタ
- 汎用データレジスタ
- セグメント・レジスタ
- EFLAGS

## 汎用データ・レジスタ

* Vol2A 3.1.1.1
| 番号 | 8bit | 16bit | 32bit |
|------|------|-------|-------|
|0|AL|AX|EAX|
|1|CL|CX|ECX|
|2|DL|DX|EDX|
|3|BL|BX|EBX|
|4|AH|SP|ESP|
|5|CH|BP|EBP|
|6|DH|SI|ESI|
|7|BH|DI|EDI|

## セグメント・レジスタ


# 特権レベル(IA32 Arch Dev Vol1 p169)
Level 0		OS Kernel
Level 1		OS Service (ex: device driver)
Level 2		OS Service (ex: device driver)
Level 3		Application

# 割り込み/例外(Vol1 p174)
ベクタ	mnemonic	説明			原因
0	#DE		除算エラー		DIV,IDIV命令
1	#DB		debug			任意のコード・データの参照
2			NMI			マスク不可能な外部割り込み
3	#BP		break point		INT 3
4	#OF		over flow		INTO
5	#BR		BOUND範囲外		BOUND
6	#UD		未定義opcode		UD2・予約オペコード
7	#NM		デバイス使用不能	浮動小数点命令・WAIT/FWAIT
8	#DF		ダブルフォルト		例外,NMI,INTRを生成できる任意の命令
9	#MF		予約			浮動小数点命令 (386以降)
10	#TS		無効TSS			タスクスイッチ,TSSアクセス
11	#NP		セグメント不在		セグメントレジスタ/システムセグメントレジスタのアクセス
12	#SS		Stack Segment Fault	スタック操作・SSレジスタのロード
13	#GP		一般保護		任意のメモリ参照,その他の保護チェック
14	#PF		ページフォルト		任意のメモリ参照
15			予約
16	#MF		浮動小数点エラー	浮動小数点命令,WAIT/FWAIT
17	#AC		アラインメントチェック	メモリ内の任意のデータ参照 (486で導入)
18	#MC		マシンチェック		エラーコード、ソースがモデルに依存 (Pentiumで導入,PRファミリで拡張)
19	#XF		SIMD浮動小数点例外	SIMD浮動小数点命令 (PentiumⅡ で導入)
20~31			予約
32~255			マスク可能割り込み	INTRピンによる外部割り込み,INT n命令


# Jcc -- Jump If Condition Is Met
70 jo
71 jno
72 jb,jc,jnae
73 jae,jnb,jnc
74 je,jz
75 jne,jnz
76 jbe,jna
77 ja,jnbe
78 js
79 jns
7a jp,jpe
7b jpo
7c jl,jnge
7d jge,jnl
7e jle,jng
7f jg,jnle
e3 jcxz,jecxz(16,32bit)
