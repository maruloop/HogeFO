HogeFO
======

UTAU dll voicebank Sample (very very very very simply)

## What is This
It's UTAU DLL voicebank Sample.

When you click "setup" button in voicebank's profile dialog, create text file and write "hoge" in your voicebank directory.

HogeFO.cpp based on WavLoader.cpp(written by Ameya)

I edited only setup function.

## Compile

VC

```
cl HogeFO.cpp HogeFO.def /LD
```

## Usage

1. Compile this source
2. HogeFO.dll and plugin.txt into your voicebank
3. Open your voicebank in UTAU and click "setup" button
4. Check hoge.txt in your voicebank

## Attention
UTAU works only CRLF(line feed code)

## UTAU Version

UTAU v0.2.90 and above

