#!/usr/bin/env bash
set -euo pipefail

RED='\033[0;31m'; GREEN='\033[0;32m'; CYAN='\033[0;36m'; BOLD='\033[1m'; NC='\033[0m'
ok()  { echo -e "${GREEN}✓${NC} $*"; }
log() { echo -e "${CYAN}→${NC} $*"; }
err() { echo -e "${RED}✗${NC} $*" >&2; exit 1; }

echo ""
echo -e "${BOLD}  Song Starter VST3 — Installer${NC}"
echo "  ────────────────────────────────"
echo ""

[[ "$(uname)" == "Darwin" ]] || err "macOS only. Windows: see README for manual steps."

VST3_DIR="$HOME/Library/Audio/Plug-Ins/VST3"
TMP=$(mktemp -d)
RELEASE_URL="https://github.com/towenwolf/wip-vst-song-starter/releases/latest/download/Song%20Starter.vst3.zip"

log "Downloading..."
curl -fsSL "$RELEASE_URL" -o "$TMP/plugin.zip"
ok "Downloaded"

log "Installing..."
unzip -q "$TMP/plugin.zip" -d "$TMP/extracted"
mkdir -p "$VST3_DIR"
rm -rf "$VST3_DIR/Song Starter.vst3"
cp -r "$TMP/extracted/build/SongStarter_artefacts/Release/VST3/Song Starter.vst3" "$VST3_DIR/"

log "Removing quarantine flag..."
xattr -dr com.apple.quarantine "$VST3_DIR/Song Starter.vst3" 2>/dev/null || true
ok "Installed → $VST3_DIR/Song Starter.vst3"

rm -rf "$TMP"

echo ""
echo -e "${BOLD}  Done!${NC}"
echo "  Open Ableton → Preferences → Plug-Ins → Rescan"
echo "  Then find 'Song Starter' in your VST3 list."
echo ""
