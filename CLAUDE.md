# CLAUDE.md — openFrameworks + ofxFluid プロジェクト

## 環境
- macOS (Apple Silicon / Intel)
- openFrameworks 0.12.1
- oF のルート: ~/of_v0.12.1_osx_release/
- Xcode でビルド（Cmd + R）
- OpenGL 2.1 コンテキスト（main.cpp で setGLVersion(2, 1)）

## プロジェクト構成

```
 fluidTest/
├── src/
│   ├── main.cpp       # GL 2.1 コンテキスト設定、ウィンドウ 1024x768
│   ├── ofApp.h        # ofxFluid fluid メンバ、oldMouse で速度計算
│   └── ofApp.cpp      # setup/update/draw/mouseDragged 実装
├── addons.make        # ofxFX, ofxFluid
└── CLAUDE.md
```

## アドオン
- **ofxFluid** — GLSL 120 ベースの流体シミュレーション（GL 2.1 必須）
- **ofxFX** — エフェクト補助

## 重要な制約
- GLSL バージョンは **120**（`#version 120`）に固定。150 以上は使わない
- `ofxFluid` は `allocate(w, h, scale)` で初期化。scale=0.5 でハーフ解像度
- シェーダーファイルは ofxFluid アドオン内に存在するため直接編集しない

## 主要パラメータ（ofApp::setup）
| パラメータ | 値 | 説明 |
|---|---|---|
| `fluid.dissipation` | 0.999 | 密度の減衰 |
| `fluid.velocityDissipation` | 0.99 | 速度の減衰 |
| `fluid.setGravity` | (0, -0.0098) | 重力（ほぼ無効） |
| `fluid.setUseObstacles` | false | 障害物なし |

## addTemporalForce の引数順序
```cpp
fluid.addTemporalForce(
    ofVec2f pos,       // スクリーン座標
    ofVec2f vel,       // 速度ベクトル（* 5.0 でスケール）
    ofFloatColor col,  // 注入する色
    float radius,      // 影響半径（px）
    float temperature, // 温度（浮力に影響）
    float density      // 密度
);
```

## GLSL に関する注意
このプロジェクトは GL 2.1 / GLSL 120 で動作している。
`sampler2DRect` / `texture2DRect` / `gl_FragColor` / `gl_TexCoord` を使用。

GL 3.2+ (GLSL 150) に移行する場合は以下の変換が必要:
| GLSL 120 | GLSL 150 |
|---|---|
| `texture2DRect()` | `texture()` |
| `gl_FragColor` | `out vec4 fragColor` |
| `gl_TexCoord[0].st` | `in vec2 texCoordVarying` |
| `glBegin/glEnd` | `ofMesh` |
| `getTextureReference()` | `getTexture()` |

## 既に修正済みの問題
- `ofxFXObject.h`: `operator[]` の返り値なしパスを修正
- `ofxFluid.h` / `ofxFluid.cpp`: `getTextureReference` → `getTexture` に置換済み

## ビルド方法
```bash
# Xcode
# fluidTest.xcodeproj を開いて Cmd + R

# ターミナル
cd ~/of_v0.12.1_osx_release/apps/myApps/" fluidTest"
make && make run
```

## オーディオ入力の方針
- `ofSoundStream` を使用（外部アドオン不要）
- `audioIn` コールバックはオーディオスレッドで呼ばれるので `mutex` で保護
- `bufferSize = 512`, `sampleRate = 44100`, 入力 1ch / 出力 0ch
- スムージングは非対称エンベロープ（アタック即追従、リリース緩やか）

## コーディング規約
- oF のスタイルに準拠（camelCase）
- マジックナンバーはコメントで意味を明記
- シェーダーを外部ファイルにする場合は `bin/data/shaders/` に配置

## ofxFft
- 場所: ~/of_v0.12.1_osx_release/addons/ofxFft/
- KISS FFT をラップしたFFTアドオン
- `ofxFft::create(bufferSize, OF_FFT_WINDOW_HAMMING)` で生成
- `ofxFft::setSignal(float* signal)` で波形を渡す
- `ofxFft::getAmplitude()` で振幅スペクトラム (float*) を取得
- `ofxFft::getBinSize()` でビン数（= bufferSize/2 + 1）
- signalNormalize / signalResize は不要、setSignal に生波形を渡せばOK

## macOS .app バンドル化の注意

### データパス問題（重要）
oF アプリを `.app` バンドルとして `/Applications/` 等に移動すると、`ofToDataPath()` がバンドル外の間違ったパス（例: `/Applications/data`）を参照する。

**原因**: oF のデフォルトのデータパス解決は実行ファイルからの相対パスだが、.app バンドル内では階層が異なる。

**解決策**: `setup()` の最初に `CFBundleGetMainBundle()` でバンドルの Resources パスを取得し `ofSetDataPathRoot()` で設定する。`ofFilePath::getCurrentExeDir()` ベースの方法は .app 内では失敗する場合がある。

```cpp
#ifdef TARGET_OSX
#include <CoreFoundation/CoreFoundation.h>
// CFBundleGetMainBundle → CFBundleCopyResourcesDirectoryURL → ofSetDataPathRoot
#endif
```

### バンドル手順
1. `make Release -j4`
2. `./bundle.sh` で `bin/data/*` を `.app/Contents/Resources/data/` にコピー
3. `xattr -cr bin/VJCosmos.app` で隔離属性を除去
4. `/Applications/` にコピー

### bundle.sh の構造
- コピー先は `Contents/Resources/data/`（`Resources/` 直下ではない）
- Info.plist の `CFBundleName` / `CFBundleDisplayName` を VJCosmos に変更
- `NSMicrophoneUsageDescription` が Info.plist に必要（マイクアクセス）

### よくある失敗
- `bin/data/` を `Resources/` 直下にコピー → oF が見つけられない
- `ofFilePath::getCurrentExeDir()` でパス構築 → .app 内で不正なパスになる
- `xattr` 除去を忘れる → Gatekeeper がリソースアクセスをブロック
- `/Applications/` 移動後にシェーダーだけ動かない → データパス問題
