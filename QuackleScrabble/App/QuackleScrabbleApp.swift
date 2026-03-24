import SwiftUI

@main
struct QuackleScrabbleApp: App {
    @State private var engine = QuackleEngine()

    var body: some Scene {
        WindowGroup {
            ContentView()
                .environment(engine)
                .onAppear {
                    engine.initialize()
                }
        }
    }
}
