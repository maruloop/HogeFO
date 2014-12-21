/* HogeFO.cpp

以下の仕様は飴屋さんによる記述のもの(現在は変わっている可能性もある)

void _stdcall load(char* path);

DLL接続時に呼ばれる。
引数にはDLLの置いてあるパスが渡される。（「\」が終わりに付く）
実装は省略可能。
（初期化処理が不要で音源フォルダをWin32APIを使用して自力で取得する場合等）
現状では処理毎（一音毎）に接続・解除を繰り返すので重い実装をしてはいけない。

void _stdcall unload();

DLL接続解除時に呼ばれる。
実装は省略可能。
現状では処理毎（一音毎）に接続・解除を繰り返すので重い実装をしてはいけない。

int _stdcall exists(char* name, int tone);

存在チェック。
name: 現状では原音設定の１カラム目"あ.wav"等から拡張子を取り除いた文字列が渡される
仕様は確定していない。エイリアスあるいは音符の歌詞をそのまま渡す仕様も考えている
tone: 音階の数値が渡される予定であるが本体側で未実装。常に０が渡る。
その名前でデータを返すことが出来る場合は０以外を、
該当データがない場合は０を返すように実装する。
実装は省略可能。
省略した場合あらゆる発音に対してデータが存在すると仮定される。

void* _stdcall getpcmdata(char* name, int tone);

Waveデータ取得。
引数はexists()と同じ。
“最低限のwavフォーマット”のファイルイメージをメモリに格納し、
そのポインタを返すよう実装する。
※“最低限のwavフォーマット”とは、
WAVEヘッダ・FormatChunk・DataChunkから構成されるwavフォーマットファイル。
余分なChunkを含んではいけない。

返すデータを用意できなかった場合NULLを返すことが出来るが、
それ以外の無効なポインタは返してはいけない。

void _stdcall freepcm(void *p);

getpcmdata()で渡されたメモリを開放するコードを実装する。

void* _stdcall getfrqdata(char* name, int tone);

周波数表データ取得。
引数はexists()と同じ。
実装は必須ではない。
.frqのファイルイメージをメモリに格納し、
そのポインタを返すよう実装する。
省略可能。省略した場合本体側では実ファイルを読みに行く。

返すデータを用意できなかった場合NULLを返すことが出来るが、
それ以外の無効なポインタは返してはいけない。

void _stdcall freefrq(void *p);

getfrqdata()で渡されたメモリを開放するコードを実装する。
getfrqdata()を省略した場合、省略可能。

void _stdcall setup(int hwnd);

音源の設定ダイアログ等を実装する。
音源のプロフィールのSetupボタンから呼ばれる。
省略可能。


■コンパイル

VCの場合
cl HogeFO.cpp HogeFO.def /LD


*/
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fstream>


//モジュール名
char modulepath[512];

/* DLL接続時に呼ばれる。*/
void _stdcall load(char* path)
{
	strcpy(modulepath, path);//渡されるのは"\"付きです。
}

/* 存在チェック */
int _stdcall exists(char* name, int tone)
{
	if (name == 0 || *name == 0)
	{
		return 0;
	}
	char filepath[512];
	strcpy(filepath, modulepath);
	strcat(filepath, name);
	strcat(filepath, ".wav");


	return !access(filepath, 0);

}

void *loadFileAll(char *filename)
{
	FILE *f = fopen(filename, "rb");
	void *p = 0;
	if (f)
	{
		long size;
		fseek(f, 0, SEEK_END);
		size = ftell(f);
		fseek(f, 0, SEEK_SET);
		
		p = malloc(size);
		
		fread(p, size, 1, f);
		
		fclose(f);
	}
	return p;
}

/* Waveデータ取得 */
void* _stdcall getpcmdata(char* name, int tone)
{
	if (name == 0 || *name == 0)
	{
		return 0;
	}
	
	char filepath[512];
	strcpy(filepath, modulepath);
	strcat(filepath, name);
	strcat(filepath, ".wav");
	
	return loadFileAll(filepath);
}

/* データ開放 */
void _stdcall freepcm(void *p)
{
	free(p);
}

/* 周波数表データ取得 */
void* _stdcall getfrqdata(char* name, int tone)
{
	if (name == 0 || *name == 0)
	{
		return 0;
	}
	
	char filepath[512];
	strcpy(filepath, modulepath);
	strcat(filepath, name);
	strcat(filepath, "_wav.frq");
	
	return loadFileAll(filepath);
}

/* 周波数表データ開放 */
void _stdcall freefrq(void *p)
{
	free(p);
}


void _stdcall setup(int hwnd)
{
	// ファイル出力ストリームの初期化
	std::ofstream ofs("hoge.txt", std::ios_base::out | std::ios_base::app);

	// ファイルに1行ずつ書き込み
	ofs << "hoge" << std::endl;
	
}
