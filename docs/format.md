# [命令フォーマット](https://www.intel.co.jp/content/dam/www/public/ijkk/jp/ja/documents/developer/IA32_Arh_Dev_Man_Vol2A_i.pdf)

| prefix | opecode | ModR/M | SIB | disp | imm |

disp,imm: 0,1,2,4バイト

## prefix

### group1
- 0xf0
- 0xf2
- 0xf3

### group2
セグメント・オーバーライド・プリフィックス
- 0x2e: CS
- 0x36: SS
- 0x3e: DS
- 0x26: ES
- 0x64: FS
- 0x65: GS
- 0x2e: 分岐が成立しない(Jcc命令に対してのみ使用)

### group3
- 0x66

### group4
- 0x67

## opecode

## ModR/M
| Mod | reg/op | R/M |

Mod:	2bit
reg/op:	3bit
R/M:	3bit

## disp
| scale | index | base |

scale:	2bit
index:	3bit
base:	3bit
