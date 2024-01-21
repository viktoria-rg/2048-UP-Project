#include <iostream>
#include <iomanip>

using namespace std;

size_t dimension;
int score = 0;

void startGame(char nickname[]) {
    cout << "Enter your nickname:" << endl;
    cin >> nickname;
    cout << endl << "Enter dimension:" << endl;
    cin >> dimension;

    while (dimension < 4 || dimension > 10) {
        cout << endl << "Please, enter dimension between 4 and 10 included" << endl;
        cin >> dimension;
    }
}

int** createMatrix(size_t rows, size_t columns){
    int** matrix = new int*[rows];

    for (size_t i = 0; i < rows; i++) {
        matrix[i] = new int[columns];
    }

    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < columns; j++) {
            matrix[i][j] = 0;
        }
    }

    return matrix;
}

// free dynamically allocated memory
void deleteMatrix (int** board, size_t rows) {
    for (size_t i = 0; i < rows; i++) {
        delete[] board[i];
    }
    delete[] board;
}


void addRandomNum(int** board) {
        size_t k, l;
        size_t freePosCount = 0;
        size_t maxFreePositions = dimension * dimension;
        size_t* freePositionsI = new size_t[maxFreePositions];
        size_t* freePositionsJ = new size_t[maxFreePositions];

        int numsToFill[] = {2, 4};
        for (size_t i = 0; i < dimension; i++) {
            for (size_t j = 0; j < dimension; j++) {
                if (board[i][j] == 0) {
                    freePositionsI[freePosCount] = i;
                    freePositionsJ[freePosCount] = j;
                    freePosCount++;
                }
            }
        }

        // the board is already filled
        if (freePosCount == 0) {
            delete[] freePositionsI;
            delete[] freePositionsJ;
            return;
        }

        else {
            size_t randomIndex = rand() % freePosCount;
            k = freePositionsI[randomIndex];
            l = freePositionsJ[randomIndex];

            size_t randomNumIndex = rand() % 2;

            board[k][l] = numsToFill[randomNumIndex];
        }

        delete[] freePositionsI;
        delete[] freePositionsJ;
}

void slideUp(int** board, bool& hasChanged) {
    // traverse a column from top to bottom
    for (int i = 0; i < dimension; i++) {
        for (int j = 0; j < dimension; j++) {

            // if the tile is empty
            if (board[i][j] == 0) {

                // traverse below to find an element different from 0
                for (int k = i + 1; k < dimension; k++) {

                    // move the element to the empty tile and assign the previous tile 0
                    if (board[k][j]) {
                        board[i][j] = board[k][j];
                        board[k][j] = 0;
                        hasChanged = true;
                        break;
                    }
                }
            }
        }
    }
}


void mergeUp(int** board, bool& hasChanged) {
    for (int j = 0; j < dimension; j++) {
        for (int i = 0; i < dimension - 1; i++) {
            if (board[i][j] && board[i][j] == board[i + 1][j]) {
                board[i][j] *= 2;
                board[i + 1][j] = 0;
                hasChanged = true;
            }
        }
    }
}

// analogically to slideUp
void slideDown(int** board, bool& hasChanged) {
    for (int i = dimension - 1; i >= 0; i--) {
        for (int j = 0; j < dimension; j++) {
            if (board[i][j] == 0) {
                for (int k = i - 1; k >= 0; k--) {
                    if (board[k][j]) {
                        board[i][j] = board[k][j];
                        board[k][j] = 0;
                        hasChanged = true;
                        break;
                    }
                }
            }
        }
    }
}

void mergeDown(int** board, bool& hasChanged) {
    for (int j = 0; j < dimension; j++) {
        for (int i = dimension - 1; i > 0; i--) {
            if (board[i][j] && board[i][j] == board[i - 1][j]) {
                board[i][j] *= 2;
                board[i - 1][j] = 0;
                hasChanged = true;
            }
        }
    }
}

// analogically to slideUp
void slideLeft(int** board, bool& hasChanged) {
    for (int i = 0; i < dimension; i++) {
        for (int j = 0; j < dimension; j++) {
            if (board[i][j] == 0) {
                for (int k = j + 1; k < dimension; k++) {
                    if (board[i][k]) {
                        board[i][j] = board[i][k];
                        board[i][k] = 0;
                        hasChanged = true;
                        break;
                    }
                }
            }
        }
    }
}

void mergeRight(int** board, bool& hasChanged) {
    for (int i = 0; i < dimension; i++) {
        for (int j = dimension - 1; j > 0; j--) {
            if (board[i][j] && board[i][j] == board[i][j - 1]) {
                board[i][j] *= 2;
                board[i][j - 1] = 0;
                hasChanged = true;
            }
        }
    }
}

void mergeLeft(int** board, bool& hasChanged) {
    for (int i = 0; i < dimension; i++) {
        for (int j = 0; j < dimension - 1; j++) {
            if (board[i][j] && board[i][j] == board[i][j + 1]) {
                board[i][j] *= 2;
                board[i][j + 1] = 0;
                hasChanged = true;
            }
        }
    }
}

// analogically to slideUp
void slideRight(int** board, bool& hasChanged) {
    for (int i = 0; i < dimension; i++) {
        for (int j = dimension - 1; j >= 0; j--) {
            if (board[i][j] == 0) {
                for (int k = j - 1; k >= 0; k--) {
                    if (board[i][k]) {
                        board[i][j] = board[i][k];
                        board[i][k] = 0;
                        break;
                    }
                }
            }
        }
    }
}

// check if there is an empty tile
bool hasEmptyTile(int** board) {
    for (size_t i = 0; i < dimension; i++) {
        for (size_t j = 0; j < dimension; j++) {
            if (board[i][j] == 0) {
                return true;
            }
        }
    }
    return false;
}

bool gameOver(int** board) {
    for (size_t i = 0; i < dimension; i++) {
        for (size_t j = 0; j < dimension; j++) {

            // there is an empty tile
            if (board[i][j] == 0) {
                return false;
            }

            // there are two tiles below each other that are equal
            if((i != (dimension - 1)) && (board[i][j] == board[i + 1][j])) {
                return false;
            }

            // there are two tiles above each other that are equal
            if((j != (dimension - 1)) && (board[i][j] == board[i][j + 1])) {
                return false;
            }
        }
    }
    return true;
}


// TODO cout score
void makeMove(char key, int** board) {
    bool hasChanged = false;

    //key from the keyboard as for an operation - up, down, left, right
    switch (key) {

        // move up
        case 'w': {
            // slide up all the existing numbers on the board
            slideUp(board, hasChanged);

            // merge the same valued tiles which are below each other
            mergeUp(board, hasChanged);

            // slide up the new valued tiles
            slideUp(board, hasChanged);

            // if nothing on the board has moved, then don't add a new random tile
            if (!hasChanged) {
                break;
            }

            // in case the board has changed, we add a new random tile
            addRandomNum(board);

            break;
        }

        // down, analogically
        case 's': {
            slideDown(board, hasChanged);
            mergeDown(board, hasChanged);
            slideDown(board, hasChanged);
            if (!hasChanged) {
                break;
            }
            addRandomNum(board);
            break;
        }

        // left
        case 'a': {
            slideLeft(board, hasChanged);
            mergeLeft(board, hasChanged);
            slideLeft(board, hasChanged);
            if (!hasChanged) {
                break;
            }
            addRandomNum(board);
            break;
        }

        // right
        case 'd': {
            slideRight(board, hasChanged);
            mergeRight(board, hasChanged);
            slideRight(board, hasChanged);
            if (!hasChanged) {
                break;
            }
            addRandomNum(board);
            break;
        }
        default:
            break;
    }
}

void printBoard(int** board) {
    for (size_t i = 0; i < dimension; i++) {
        for (size_t j = 0; j < dimension; j++) {
            cout << setw(7);
            cout << board[i][j];
        }
        cout << endl << endl;
    }
}

int main() {
    char nickname[100];
    int menu;

    cout << "Choose from:" << endl
         << "1. Start Game" << endl
         << "2. Leaderboard" << endl
         << "3. Quit" << endl;

    cin >> menu;
    cout << endl;

    switch (menu) {
        case 1: {
            startGame(nickname);

            // create board
            int** board = createMatrix(dimension, dimension);

            // add the very first number
            addRandomNum(board);

            // as long as there is an input as a command for moving
            while (true) {
                cout << endl;
                printBoard(board);

                char key;
                cin >> key;
                makeMove(key, board);

                if (key == 'q') {
                    break;
                }
                if(gameOver(board)){
                    cout << "Game over!" << endl;
                    break;
                }
            }

            deleteMatrix(board, dimension);
        }
            break;

        case 2:
            // TODO add leaderboard
            break;

        case 3:
            // TODO add quit
            break;

        default: {
            cout << "Please, enter 1, 2, or 3:" << endl;
            cin >> menu;
        }
    }

    return 0;
}
