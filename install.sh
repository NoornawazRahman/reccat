#!/usr/bin/env bash
set -e

BIN_NAME="reccat"
INSTALL_DIR="/usr/local/bin"

echo "🔨 Building..."
gcc reccat.c -o $BIN_NAME

echo "📦 Installing to $INSTALL_DIR (requires sudo)..."
sudo install -m 755 $BIN_NAME $INSTALL_DIR/$BIN_NAME

echo "✅ Installed! Try: reccat <dir> <output>"
