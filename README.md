# Quackle

A personal Scrabble app for iPhone and Mac, built with SwiftUI and the [Quackle](https://github.com/quackle/quackle) C++ engine.

## Features

- **Drag-and-drop** tile placement — drag tiles from rack to board, reposition on board, or drag back to rack
- **Rack reordering** — drag tiles within the rack to rearrange, with animated live preview
- **Shuffle** button to randomize rack tile order
- **Real-time validation** — green tiles for valid moves, red for invalid
- **Tile point values** displayed on every tile (standard Scrabble scoring)
- **AI opponent** using Quackle's NormalPlayer with Gaussian move selection
- **Coin flip** determines who goes first each game
- **Skill slider** — adjust AI difficulty from easy (0.0) to near-perfect (1.0)
- **Blank tile picker** — tap a blank, choose a letter from an A–Z grid
- **Exchange, pass, and new game** support
- **Move history** and **top 50 candidate moves** views
- **Board zoom** — double-tap/click to zoom in, drag to pan (drag-and-drop is zoom-aware)
- **Game persistence** — board, racks, scores, and bag are saved across app launches
- Uses the **TWL06** dictionary

## Requirements

- Xcode 16.3+
- iOS 17.0+ / macOS 14.0+
- [XcodeGen](https://github.com/yonaskolb/XcodeGen) (`brew install xcodegen`)

## Build

```bash
xcodegen generate

# iOS
xcodebuild -scheme QuackleScrabble \
  -destination 'platform=iOS Simulator,name=iPhone 17 Pro' \
  build

# macOS
xcodebuild -scheme QuackleScrabble \
  -destination 'platform=macOS' \
  build
```

## Project Structure

```
QuackleScrabble/
  App/            — SwiftUI app entry point and ContentView (#if os conditionals)
  Bridge/         — Obj-C++ bridge (QuackleBridge) and QuackleEngine
  Model/          — GameState, TilePlacement, MoveHistoryEntry
  Views/
    Board/        — BoardView, SquareView
    Rack/         — RackView
    Game/         — ScoreboardView, MoveInputView
  Assets.xcassets — App icon (iOS + macOS)
libquackle/       — Quackle C++ engine sources
data/             — TWL06 dictionary, alphabet, strategy files
project.yml       — XcodeGen project spec (multiplatform)
```

## Acknowledgments

This app uses the [Quackle](https://github.com/quackle/quackle) crossword game AI engine, created by **Jason Katz-Brown**, **John O'Laughlin**, and **John Fultz**. Quackle is released under the [GPL v3+](https://www.gnu.org/licenses/gpl-3.0.html).
