import SwiftUI

struct RackView: View {
    @Environment(QuackleEngine.self) var engine

    var body: some View {
        HStack(spacing: 3) {
            if engine.isExchangeMode {
                // Exchange mode: show all rack tiles, tap to toggle selection
                ForEach(engine.rack) { tile in
                    let isSelected = engine.exchangeSelectedIds.contains(tile.id)
                    ZStack {
                        RoundedRectangle(cornerRadius: 4)
                            .fill(isSelected
                                  ? Color(red: 1.0, green: 0.6, blue: 0.6)
                                  : Color(red: 1.0, green: 0.92, blue: 0.80))
                            .frame(width: 44, height: 44)
                            .shadow(radius: 1)
                            .overlay(
                                RoundedRectangle(cornerRadius: 4)
                                    .stroke(isSelected ? Color.red : Color.clear, lineWidth: 2)
                            )

                        Text(tile.isBlank ? "?" : tile.letter)
                            .font(.system(size: 24, weight: .bold))
                            .foregroundColor(.black)
                    }
                    .onTapGesture {
                        engine.toggleExchangeTile(tile)
                    }
                }
            } else {
                // Normal mode: show available rack tiles
                ForEach(engine.availableRack) { tile in
                    ZStack {
                        RoundedRectangle(cornerRadius: 4)
                            .fill(tile.id == engine.selectedRackTileId
                                  ? Color(red: 0.7, green: 0.9, blue: 0.7)
                                  : Color(red: 1.0, green: 0.92, blue: 0.80))
                            .frame(width: 44, height: 44)
                            .shadow(radius: 1)
                            .overlay(
                                RoundedRectangle(cornerRadius: 4)
                                    .stroke(tile.id == engine.selectedRackTileId ? Color.green : Color.clear, lineWidth: 2)
                            )

                        Text(tile.isBlank ? "?" : tile.letter)
                            .font(.system(size: 24, weight: .bold))
                            .foregroundColor(.black)
                    }
                    .onTapGesture {
                        engine.selectRackTile(tile)
                    }
                }
            }
        }
        .padding(.vertical, 8)
    }
}
