import Foundation

struct TilePlacement: Equatable {
    let row: Int
    let col: Int
    let letter: String
    let isBlank: Bool
}

struct MoveHistoryEntry: Identifiable {
    let id = UUID()
    let turn: Int
    let playerName: String
    let moveDescription: String
    let score: Int
    let totalScore: Int
}
