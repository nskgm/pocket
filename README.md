# **pocket library**

## compiler
***
* VC++
	* ( 12 )
	* 14
* GCC
	* 5.2
	* 5.3 (2016/09/24 ~)

## list
***
* math
* gl

## rake
***
```
$ rake
#-> rake build run
```
***
* cppファイルを全検索して実行ファイルを作成
	* デバッグビルド
	```
	$ rake build (DEBUG=true)
	```
	* リリースビルド
	```
	$ rake build DEBUG=false
	```
* 警告有効無効
```
$ rake WARNING=true|false
```
* 実行ファイル実行
```
$ rake run
```
***
