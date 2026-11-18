// game.cpp - Logika inti game Batu Kertas Gunting (Rock Paper Scissors)
// Dikompilasi ke WebAssembly memakai Emscripten.

#include <emscripten.h>
#include <cstdlib>
#include <ctime>

namespace {

// Kode pilihan: 0 = Batu, 1 = Kertas, 2 = Gunting
enum Choice { ROCK = 0, PAPER = 1, SCISSORS = 2 };

// Kode hasil: 0 = Seri, 1 = Pemain menang, 2 = Komputer menang
enum Result { DRAW = 0, PLAYER_WIN = 1, COMPUTER_WIN = 2 };

class Game {
public:
    Game() { reset(); }

    void reset() {
        playerScore_ = 0;
        computerScore_ = 0;
        draws_ = 0;
        lastComputer_ = ROCK;
    }

    // Memainkan satu ronde. Mengembalikan kode hasil.
    int play(int player) {
        if (player < 0 || player > 2) return -1;  // input tidak valid

        lastComputer_ = std::rand() % 3;
        int result = decide(player, lastComputer_);

        if (result == PLAYER_WIN) playerScore_++;
        else if (result == COMPUTER_WIN) computerScore_++;
        else draws_++;

        return result;
    }

    int lastComputer() const { return lastComputer_; }
    int playerScore() const { return playerScore_; }
    int computerScore() const { return computerScore_; }
    int draws() const { return draws_; }

private:
    // Aturan: Batu > Gunting, Gunting > Kertas, Kertas > Batu
    // Pilihan "i" dikalahkan oleh pilihan "(i + 1) % 3".
    static int decide(int player, int computer) {
        if (player == computer) return DRAW;
        if ((player + 1) % 3 == computer) return COMPUTER_WIN;
        return PLAYER_WIN;
    }

    int playerScore_ = 0;
    int computerScore_ = 0;
    int draws_ = 0;
    int lastComputer_ = ROCK;
};

Game game;  // satu objek game global

}  // namespace

// Fungsi-fungsi di bawah ini yang bisa dipanggil dari JavaScript.
extern "C" {

EMSCRIPTEN_KEEPALIVE
void init_game() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    game.reset();
}

EMSCRIPTEN_KEEPALIVE
int play_round(int player) { return game.play(player); }

EMSCRIPTEN_KEEPALIVE
int get_computer_choice() { return game.lastComputer(); }

EMSCRIPTEN_KEEPALIVE
int get_player_score() { return game.playerScore(); }

EMSCRIPTEN_KEEPALIVE
int get_computer_score() { return game.computerScore(); }

EMSCRIPTEN_KEEPALIVE
int get_draws() { return game.draws(); }

EMSCRIPTEN_KEEPALIVE
void reset_game() { game.reset(); }

}  // extern "C"
