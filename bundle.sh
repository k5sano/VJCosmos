#!/bin/bash
set -e
cd "$(dirname "$0")"

APP="bin/fluidTest.app"
RESOURCES="$APP/Contents/Resources"
FRAMEWORKS="$APP/Contents/Frameworks"

# data フォルダごとコピー（data/ ディレクトリとして）
rm -rf "$RESOURCES/data"
mkdir -p "$RESOURCES/data"
rsync -a --exclude='screenshot_*.png' bin/data/ "$RESOURCES/data/"

# Syphon.framework を埋め込む
mkdir -p "$FRAMEWORKS"
cp -R ../../../addons/ofxSyphon/libs/Syphon/lib/osx/Syphon.framework "$FRAMEWORKS/"

# Info.plist を修正
PLIST="$APP/Contents/Info.plist"
/usr/libexec/PlistBuddy -c "Set :CFBundleName VJCosmos" "$PLIST"
/usr/libexec/PlistBuddy -c "Set :CFBundleVersion 1.0" "$PLIST" 2>/dev/null || true
/usr/libexec/PlistBuddy -c "Add :CFBundleDisplayName string VJCosmos" "$PLIST" 2>/dev/null || \
  /usr/libexec/PlistBuddy -c "Set :CFBundleDisplayName VJCosmos" "$PLIST"

# VJCosmos にリネーム
NEWAPP="bin/VJCosmos.app"
rm -rf "$NEWAPP"
mv "$APP" "$NEWAPP"

# アドホック署名
codesign --force --deep --sign - "$NEWAPP"

echo ""
echo "=== Bundle complete: $NEWAPP ==="
echo "Resources/data contents:"
ls "$NEWAPP/Contents/Resources/data/"
echo ""
echo "To install: cp -R $NEWAPP /Applications/"
echo "To run:     open $NEWAPP"
