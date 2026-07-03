#!/usr/bin/env bash
set -euo pipefail

# ─── colors ───────────────────────────────────────────────────────────────────
RED='\033[0;31m'; GREEN='\033[0;32m'; CYAN='\033[0;36m'; BOLD='\033[1m'; NC='\033[0m'
ok()  { echo -e "${GREEN}✓${NC} $*"; }
log() { echo -e "${CYAN}→${NC} $*"; }
err() { echo -e "${RED}✗${NC} $*" >&2; exit 1; }

echo ""
echo -e "${BOLD}  Song Starter VST3 — Installer${NC}"
echo "  ────────────────────────────────"
echo ""

# ─── platform check ───────────────────────────────────────────────────────────
if [[ "$(uname)" != "Darwin" ]]; then
    err "This installer is macOS only. Windows: see README for manual steps."
fi

VST3_DIR="$HOME/Library/Audio/Plug-Ins/VST3"
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# ─── Xcode Command Line Tools ─────────────────────────────────────────────────
if ! xcode-select -p &>/dev/null; then
    log "Installing Xcode Command Line Tools (you may see a dialog)..."
    xcode-select --install 2>/dev/null || true
    echo ""
    echo "  Xcode tools dialog opened. Install them, then re-run:"
    echo "  ./install.sh"
    exit 0
fi
ok "Xcode CLT found"

# ─── Homebrew ─────────────────────────────────────────────────────────────────
if ! command -v brew &>/dev/null; then
    log "Installing Homebrew..."
    /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
    eval "$(/opt/homebrew/bin/brew shellenv)" 2>/dev/null \
      || eval "$(/usr/local/bin/brew shellenv)" 2>/dev/null
fi
ok "Homebrew found"

# ─── cmake ────────────────────────────────────────────────────────────────────
if ! command -v cmake &>/dev/null; then
    log "Installing cmake..."
    brew install cmake
fi
ok "cmake $(cmake --version | head -1 | awk '{print $3}')"

# ─── build ────────────────────────────────────────────────────────────────────
BUILD_DIR="$SCRIPT_DIR/build"
log "Configuring (first run fetches JUCE ~500 MB, grab a coffee)..."
mkdir -p "$BUILD_DIR"
cmake -S "$SCRIPT_DIR" -B "$BUILD_DIR" \
      -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_OSX_ARCHITECTURES="arm64" \
      -Wno-dev \
      --log-level=WARNING

log "Building..."
cmake --build "$BUILD_DIR" --config Release --parallel "$(sysctl -n hw.logicalcpu)"
ok "Build complete"

# ─── install ──────────────────────────────────────────────────────────────────
VST3_SRC=$(find "$BUILD_DIR" -name "Song Starter.vst3" -maxdepth 6 | head -1)

if [[ -z "$VST3_SRC" ]]; then
    err "Could not find Song Starter.vst3 in build output. Check build logs above."
fi

mkdir -p "$VST3_DIR"
rm -rf "$VST3_DIR/Song Starter.vst3"
cp -r "$VST3_SRC" "$VST3_DIR/"
ok "Installed → $VST3_DIR/Song Starter.vst3"

echo ""
echo -e "${BOLD}  Done!${NC}"
echo "  Open Ableton → Preferences → Plug-Ins → Rescan"
echo "  Then find 'Song Starter' in your VST3 list."
echo ""
