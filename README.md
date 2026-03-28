# Quackle

A personal Scrabble app for iPhone and Mac, built with SwiftUI and the [Quackle](https://github.com/quackle/quackle) C++ engine.

## Features

### Three Game Modes
- **Play vs AI** — Quackle's NormalPlayer with adjustable skill slider (easy to near-perfect)
- **Pass & Play** — Two players on the same device, with a handoff screen between turns
- **Play Online** — Game Center turn-based multiplayer via programmatic auto-match

### Gameplay
- **Drag-and-drop** tile placement — drag tiles from rack to board, reposition on board, or drag back to rack
- **Rack reordering** — drag tiles within the rack to rearrange, with animated live preview
- **Shuffle** button to randomize rack tile order
- **Real-time validation** — green tiles for valid moves, red for invalid
- **Tile point values** displayed on every tile (standard Scrabble scoring)
- **Blank tile picker** — tap a blank, choose a letter from an A–Z grid
- **Exchange, pass, and new game** support
- **Move history** and **top 50 candidate moves** views
- **Board zoom** — double-tap/click to zoom in, drag to pan (drag-and-drop is zoom-aware)
- **Game persistence** — AI games save board, racks, scores, and bag across app launches
- **AI move animation** — opponent tiles flip face-up then fly to board positions
- **Coin flip** determines who goes first in AI games
- Uses the **CSW19** dictionary

## Requirements

- Xcode 16.3+
- iOS 17.0+ / macOS 14.0+
- [XcodeGen](https://github.com/yonaskolb/XcodeGen) (`brew install xcodegen`)
- Apple Developer account (for Game Center multiplayer)

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
  -allowProvisioningUpdates \
  build
```

## Project Structure

```
QuackleScrabble/
  App/            — SwiftUI app entry point, ContentView, GameView, HandoffView
  Bridge/         — Obj-C++ bridge (QuackleBridge) and QuackleEngine
  Model/          — GameState, TilePlacement, MoveHistoryEntry
  Multiplayer/    — GameCenterManager, MultiplayerGameState
  Views/
    Board/        — BoardView, SquareView
    Rack/         — RackView
    Game/         — ScoreboardView, MoveInputView, ModeSelectionView
  Assets.xcassets — App icon (iOS + macOS)
libquackle/       — Quackle C++ engine sources
data/             — CSW19 dictionary, alphabet, strategy files
project.yml       — XcodeGen project spec (multiplatform)
```

## Acknowledgments

This app uses the [Quackle](https://github.com/quackle/quackle) crossword game AI engine, created by **Jason Katz-Brown**, **John O'Laughlin**, and **John Fultz**. Quackle is released under the [GPL v3+](https://www.gnu.org/licenses/gpl-3.0.html).
