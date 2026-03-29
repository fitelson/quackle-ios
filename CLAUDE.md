# QuackleScrabble

Multiplatform (iOS + macOS) Scrabble game powered by the Quackle C++ engine.

## Build

```bash
# iOS Simulator
xcodebuild -project QuackleScrabble.xcodeproj -scheme QuackleScrabble -destination 'platform=iOS Simulator,name=iPhone 17 Pro' build

# macOS (needs -allowProvisioningUpdates for Game Center entitlement)
xcodebuild -project QuackleScrabble.xcodeproj -scheme QuackleScrabble -destination 'platform=macOS' -allowProvisioningUpdates build
```

## Project structure

- `QuackleScrabble/App/` — ContentView, GameView, HandoffView, WaitingForOpponentView, sheet views (BlankPicker, History, TopMoves, SkillSlider)
- `QuackleScrabble/Bridge/` — QuackleEngine.swift (main Observable engine), QuackleBridge (Obj-C++ bridge to C++ Quackle)
- `QuackleScrabble/Views/Board/` — BoardView, SquareView
- `QuackleScrabble/Views/Rack/` — RackView
- `QuackleScrabble/Model/` — GameState (TilePlacement, MoveHistoryEntry, AIAnimTile, SavedGameState)
- `QuackleScrabble/Views/Game/` — OpponentRackView, AIAnimationOverlay, ScoreboardView, MoveInputView, ModeSelectionView
- `QuackleScrabble/Multiplayer/` — GameCenterManager, MultiplayerGameState
- `QuackleScrabble/QuackleScrabble.entitlements` — Game Center capability
- `QuackleScrabbleTests/` — Unit tests (ModelTests: Codable roundtrips, tile points, UUID identity)

## Key conventions

- Tile placement uses immediate DragGesture (not tap-to-place or system onDrag/onDrop)
- The "game" named coordinate space is defined on GameView's VStack and used by all drag gestures
- Board geometry (grid origin, square size, zoom state) is reported to the engine for drop-target calculation
- Drag-and-drop is zoom-aware: endDrag() inverse-transforms coordinates through scaleEffect + offset
- Rack tiles can be reordered by dragging within the rack (animated live preview)
- Game state persists across launches via UserDefaults (auto-saves after each move)
- LetterString is FixedLengthString (max 40 chars); use LongLetterString (std::string) for bag-sized data
- Bag() default constructor calls prepareFullBag(); always call clear() before toss() when restoring
- Bundle ID: `com.bef.quacklescrabble`
- Lexicon: CSW19
- GameMode enum: .ai (vs computer), .multiplayer (via Game Center), .passAndPlay (two humans, one device)
- Multiplayer uses GKTurnBasedMatch with programmatic auto-match (GKTurnBasedMatch.find(for:)), no matchmaker UI
- findOrCreateMatch() only removes ended/finished matches; reuses open matches (pending or in-progress) to preserve auto-match pairing
- Only the currentParticipant initializes a new match; the other player waits for first move
- Opponent display names resolved from match participants when loading state (handles late-join)
- GameCenterManager conforms to GKLocalPlayerListener for turn event callbacks
- receivedTurnEventFor only clears isWaitingForOpponent when match data exists (prevents spurious callback race)
- Both WaitingForOpponentView and GameView poll Game Center every 3s via `.task`-based async loops (auto-cancelled when conditions change)
- QuackleEngine.onMultiplayerMoveCommitted callback wired in QuackleScrabbleApp to submit turns (weak captures to avoid retain cycle)
- isLocalPlayerTurn is a stored property updated in refreshState(), not computed (bridge calls aren't tracked by @Observable)
- Multiplayer move history managed via MultiplayerGameState serialization, not bridge (bridge only has moves since last restore)
- appendLatestMoveToHistory() reads bridge history post-commit and appends to accumulated moveHistory
- Opponent moves animate (3-phase flip+fly) in multiplayer via board diff in loadMultiplayerState
- Hypothetical moves: players can place tiles and see scores while waiting for opponent's turn
  - Drag allowed when !isLocalPlayerTurn in multiplayer; validation ignores rack check (InvalidTiles 0x0001)
  - Shows "Score: N" label (orange) instead of Submit button; Clear button available
  - Bridge method scoreMoveStringIgnoringRack: scores valid board placements regardless of rack ownership
- Submit button shows score preview: "Submit (N)" for valid tentative moves
- Pass & Play uses HandoffView overlay between turns to hide the rack during device handoff
- Bridge has separate methods for AI games (startNewGame/restoreGame) and two-human games (startNewTwoHumanGame/restoreTwoHumanGame)
- Game state persistence (UserDefaults) only applies to AI mode; multiplayer state lives in GameKit match data
- ModeSelectionView shown on first launch (no saved game) or when user taps New
- "…" Menu next to New button: AI Skill Level (always), Switch to AI Game (in multiplayer), Resume Online Game (in AI with active match)
- Game switching preserves both games: AI saves to UserDefaults, multiplayer lives in Game Center match data
- switchToAIGame() loads saved AI game or starts new; preserves onMultiplayerMoveCommitted callback
- resumeCurrentMatch() refreshes match from Game Center and calls handleMatchFound
- Turn event callbacks (receivedTurnEventFor, matchEnded) only switch to multiplayer when already in multiplayer mode; otherwise silently update match reference
- loadActiveMatch() called after Game Center authentication; queries for open matches so "Resume Online Game" works across app restarts and devices
- Same online game can be open on multiple devices (iPhone + Mac) — Game Center match data is server-side; both see same state
- QuackleBridge critical methods (startNewGame, haveComputerPlay, kibitzMoves, commitMove, restore*, moveHistory) are wrapped in C++ try/catch to prevent exceptions from crossing the ObjC boundary
- QuackleEngine uses a serial `bridgeQueue` (DispatchQueue) for background bridge work (init, AI play) via `withCheckedContinuation`, avoiding `Task.detached`
- AI/opponent move animations tracked via `animationTask` property; previous animation cancelled before starting new one
- initStage3LoadGaddag returns BOOL (NO if GADDAG file not found; move generation still works, just slower)
- Board restoration validates rowBlanks array bounds before access (guards against mismatched array sizes)
- RNG in haveComputerPlay seeded via `std::random_device` (not `std::time`)
- loadMatchState tracks matched players with flags to prevent double-assignment when player2GameCenterID is empty
- forfeitMatch uses do/catch with error reporting (not silent try?)
- BlankPickerView sets engine state directly (no dismiss()+asyncAfter delay)
- buildMoveString uses guard-let for UnicodeScalar (no force unwrap)
- Module name is "Scrabble" (matches PRODUCT_NAME), use `@testable import Scrabble` in tests
- DataManager/ComputerPlayer ownership documented in QuackleBridge.mm comments
- Bridge return contract: collections return empty arrays, single objects return nullable nil (documented in QuackleBridge.h)
- Submit button uses explicit Text label with `.fixedSize()` to prevent macOS button text truncation (score was clipped without it)
- When copying .app to /Applications on macOS, `rm -rf` the old bundle first — macOS caches the old binary and may launch the stale version if you just `cp` over it
