# Song Starter — VST3

Kick-start a session. One click gives you a random **genre**, **key**, and **BPM**. Open Ableton and go.

## Install (macOS)

```bash
curl -fsSL https://raw.githubusercontent.com/towenwolf/wip-vst-song-starter/main/install.sh | bash
```

That's it. Downloads the pre-built bundle (~2 MB) and installs it. No Xcode, no Homebrew, no build step.

After install: **Ableton → Preferences → Plug-Ins → Rescan**, then drop `Song Starter` on any track.

---

## What it does

| Field | Range |
|-------|-------|
| Genre | House, Techno, DnB, Trap, Future Bass, Lo-Fi, Synthwave, and 9 more |
| Key   | All 12 notes × Major / Minor |
| BPM   | Genre-appropriate range (e.g. 165–180 for DnB, 70–90 for Lo-Fi) |

Hit **RANDOMIZE** for a new combo. Hit **COPY** to paste the summary into your DAW notes or a group track name.

---

## Manual build (Windows / advanced)

Requirements: cmake ≥ 3.22, Visual Studio 2022 (Windows) or Xcode (macOS)

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

VST3 output: `build/SongStarter_artefacts/Release/VST3/Song Starter.vst3`

Copy to:
- **macOS**: `~/Library/Audio/Plug-Ins/VST3/`
- **Windows**: `C:\Program Files\Common Files\VST3\`
