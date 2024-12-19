# esp8266-sample
esp8266で色々試す

## 使用ツール
- [PlatformIO](https://platformio.org/)
- [Visual Studio Code](https://code.visualstudio.com/)


## セットアップ
1. VSCode上でPlatformIOの拡張機能をインストール
2. esp8266の基板をusb接続する
3. `platformio.ini`の`build_flags`にwifiのssidとpasswordを設定する

## プロジェクトの実行
1. VSCode下部にある「✔︎(チェックマーク)」または、`⌘+p`でコマンドパレットを開いて`PlatformIO: Build`を選択する
2. 下部の「→(右矢印マーク)」または、`⌘+p`でコマンドパレットを開いて`PlatformIO: Upload`を選択する


## ログの確認
1. VSCode下部にある「🔌(ブラグマーク)」または、`⌘+p`でコマンドパレットを開いて`PlatformIO: Serial Monitor`を選択する
