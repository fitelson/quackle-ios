import Foundation

struct MultiplayerGameState: Codable {
    let version: Int
    var player1GameCenterID: String
    var player2GameCenterID: String
    var player1DisplayName: String
    var player2DisplayName: String
    let board: [[SavedTile?]]
    let playerScores: [Int]
    let playerRacks: [[String]]
    let bag: [String]
    let currentPlayerIndex: Int
    let moveHistory: [MoveHistoryEntry]
    let isGameOver: Bool
    let consecutiveScorelessTurns: Int

    init(
        player1GameCenterID: String,
        player2GameCenterID: String,
        player1DisplayName: String,
        player2DisplayName: String,
        board: [[SavedTile?]],
        playerScores: [Int],
        playerRacks: [[String]],
        bag: [String],
        currentPlayerIndex: Int,
        moveHistory: [MoveHistoryEntry],
        isGameOver: Bool,
        consecutiveScorelessTurns: Int
    ) {
        self.version = 1
        self.player1GameCenterID = player1GameCenterID
        self.player2GameCenterID = player2GameCenterID
        self.player1DisplayName = player1DisplayName
        self.player2DisplayName = player2DisplayName
        self.board = board
        self.playerScores = playerScores
        self.playerRacks = playerRacks
        self.bag = bag
        self.currentPlayerIndex = currentPlayerIndex
        self.moveHistory = moveHistory
        self.isGameOver = isGameOver
        self.consecutiveScorelessTurns = consecutiveScorelessTurns
    }
}
