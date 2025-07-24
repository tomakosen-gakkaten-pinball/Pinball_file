# picのファイルについて
## pic16F1826/27の仕様について
　pic1827_photointerruptor.cにて、主に使われているレジスタと機能を示す。なお、TRISA・TRISB等を総称して、TRISXのように簡略化して示す。

- OSCCON : 内部クロックの設定
- TRISX  : ポートXの入出力を設定(0:出力　1:入力)
- ANSELA : ポートXのアナログ選択
- PORTX  : ポートXの初期化及び代入
- PIRXbits.TMR2IF : Timer2 がオーバーフローしたときに 1 になる割り込みフラグ
- TMRX   : TimerXモジュールのレジスタ
- TXCON  : TimerXがオーバーフローする周期の設定
- PRX    : TimerXの周期を設定
- PIE    : TimerXの割り込みを許可
- INTCOn : 割り込み全体の制御

　その他レジスタと機能については、「PIC16F1827.pdf」を参照してください。

