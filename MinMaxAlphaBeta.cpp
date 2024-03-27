#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>

using namespace std;

int N;

int evaluate(const vector<vector<char>>& board) {
    bool brk = false;
    char val;

    auto ret = [&](char val) {
        return val == 'X' ? 10 : -10;
    };

    for (int i = 0; i < N; i++) {
        val = board[i][0];
        for(int j = 1; j < N; j++) {
            
            if(val == ' '){
                brk = true;
                break;
            }
            if(board[i][j] != val){
                brk = true;
                break;
            }
        }
        if(!brk) return ret(val);
        brk = false;
        val = board[0][i];
        for(int j = 1; j < N; j++) {
            
            if(val==' '){
                brk=true;
                break;
            }
            if(board[j][i] != val){
                brk=true;
                break;
            }
        }
        if(!brk) return ret(val);
        brk = false;
    }
    brk = false;

    val = board[0][0];
    if(val != ' ') {
        for(int i = 1; i < N; i++) {
            if(board[i][i] != val) {
                brk = true;
                break;
            }
        }
        if(!brk) return ret(val);
        brk = false;
    }
    val = board[0][N-1];
    if(val != ' ') {
        for(int i = 1; i < N; i++) {
            if(board[i][N-1-i] != val) {
                brk = true;
                break;
            }
        }
        if(!brk) return ret(val);
    }

    return 0;
}

bool isMovesLeft(const vector<vector<char>>& board) {
    for (const auto& row : board) {
        for (char cell : row) {
            if (cell == ' ') return true;
        }
    }
    return false;
}

int minimax(vector<vector<char>>& board, int depth, int alpha, int beta, bool isMax) {
    int score = evaluate(board);

    if (score == 10 || score == -10) return score;

    if (!isMovesLeft(board)) return 0;

    if (isMax) {
        int best = -1000;

        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (board[i][j] == ' ') {
                    board[i][j] = 'X';
                    best = max(best, minimax(board, depth + 1, alpha, beta, !isMax));
                    alpha = max(alpha, best);
                    board[i][j] = ' ';
                    if (beta <= alpha)
                        break;  // Beta cut-off
                }
            }
        }
        return best;
    } else {
        int best = 1000;

        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (board[i][j] == ' ') {
                    board[i][j] = 'O';
                    best = min(best, minimax(board, depth + 1, alpha, beta, !isMax));
                    beta = min(beta, best);
                    board[i][j] = ' ';
                    if (beta <= alpha)
                        break;  // Alpha cut-off
                }
            }
        }
        return best;
    }
}

pair<int, int> bestMove(vector<vector<char>>& board, char player) {
    int bestVal = (player == 'X') ? -1000 : 1000;
    pair<int, int> bestMove = {-1, -1};

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (board[i][j] == ' ') {
                board[i][j] = player;
                int moveVal = minimax(board, 0, -1000, 1000, player == 'O');

                board[i][j] = ' ';
                if ((player == 'X' && moveVal > bestVal) || (player == 'O' && moveVal < bestVal)) {
                    bestMove = {i, j};
                    bestVal = moveVal;
                }
            }
        }
    }
    return bestMove;
}

int simulateGame(vector<vector<char>>& board, char player) {
    while (isMovesLeft(board) && evaluate(board) == 0) {
        auto move = bestMove(board, player);
        if (move.first != -1) {
            board[move.first][move.second] = player;
            player = (player == 'X') ? 'O' : 'X';
        } else {
            break;
        }
    }
    return evaluate(board);
}

void print_board(vector<vector<char>>& board, char player) {

    std::cout << "Turn: " << player << "\n";
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            std::cout << board[i][j];
        }
        std::cout << '\n';
    }

}

int main(int argc, char* argv[]) {

    char filename[256];

    if(argc < 3) {
        std::cout << "Usage: ./tictactoe size dataset.txt\n";
        exit(EXIT_FAILURE);
    }

    sscanf(argv[2], "%s", filename);
    sscanf(argv[1], "%d", &N);

    ifstream file(filename);
    string line;
    int winsForX = 0, winsForO = 0, draw = 0, cpt = 0;
    auto start = chrono::high_resolution_clock::now();

    int line_count = 0; while (getline (file, line)) line_count++;

    file.clear();
    file.seekg(0);

    std::cout << "Parsed " << filename << ", fetched " << line_count << " lines.\n";

    while (getline(file, line)) {
        cpt++;
        char player = line[0];
        vector<vector<char>> board(N, vector<char>(N));
        int index = 1;
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                board[i][j] = line[index++];
            }
        }

        int result = simulateGame(board, player);
        if (result == 10) {
            winsForX++;
            // std::cout << cpt << '\n';
        }
        else if (result == -10) winsForO++;
        else draw++;
        std::cout << cpt << "/" << line_count << "\r";

    }

    std::cout << "\n";

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;
    cout << "Nombre total de parties gagnées par 'X' : " << winsForX <<" soit "<<((float)winsForX/cpt)*100<<"%"<<endl;
    cout << "Nombre total de parties gagnées par 'O' : " << winsForO <<" soit "<<((float)winsForO/cpt)*100<<"%"<<endl;
    cout << "Nombre total de parties nulles : " << draw <<" soit "<<((float)draw/cpt)*100<<"%"<<endl;
    cout << "Temps écoulé : " << elapsed.count() << "s" << endl;

    return 0;
}