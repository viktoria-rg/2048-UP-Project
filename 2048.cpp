/**
*
* Solution to course project #4
* Introduction to programming course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2023/2024
*
* @author Viktoria Gardzheva
* @idnumber 4MI0600234
* @compiler GCC
*
* source file with all functions
*
*/

#include <iostream>
#include <iomanip>
#include <fstream>

using namespace std;

size_t dimension = 0;
int SCORE = 0;
bool gameWon = false;
const int MAX_LEADERBOARD = 5;
const int MAX_NAME_LEN = 100;


struct Player
{
    char name[MAX_NAME_LEN];
    int score;
};


void savePlayerToFile(std::ofstream& ofs, Player& p)
{
    ofs << p.name << std::endl;
    ofs << p.score << std::endl;
}

void readPlayerFromFile(std::ifstream& ifs, Player& p)
{
    ifs.getline(p.name, MAX_NAME_LEN);
    ifs >> p.score;
    ifs.ignore();
}

void writeArrayToFile(const char* fileName, Player* students, size_t size)
{
    std::ofstream outputStream(fileName);

    if (!outputStream.is_open())
        return;

    outputStream << size << std::endl;

    for (unsigned i = 0; i < size; i++) {
        savePlayerToFile(outputStream, students[i]);
    }

    outputStream.close();
}

Player* readArrayFromFile(const char* fileName, size_t& size)
{
    std::ifstream inputStream(fileName);

    if (!inputStream.is_open()) {
        return nullptr;
    }

    inputStream >> size;
    inputStream.ignore();

    Player* result = new Player[size];

    for (unsigned int i = 0; i < size; i++)
        readPlayerFromFile(inputStream, result[i]);

    inputStream.close();
    return result;
}

void appendPlayerToFile(const char* fileName, Player& newPlayer) {
    std::ifstream inputStream(fileName);

    if (!inputStream.is_open()) {
        return;
    }

    int currentSize;
    inputStream >> currentSize;
    inputStream.ignore(1000, '\n');

    Player* existingPlayers = new Player[currentSize + 1];

    for (int i = 0; i < currentSize; i++) {
        readPlayerFromFile(inputStream, existingPlayers[i]);
    }

    inputStream.close();

    // find what index to insert the new player at
    int insertIndex = currentSize;
    for (int i = 0; i < currentSize; i++) {
        if (newPlayer.score > existingPlayers[i].score) {
            insertIndex = i;
            break;
        }
    }

    // shift elements to make space for the new player
    for (int i = currentSize; i > insertIndex; i--) {
        existingPlayers[i] = existingPlayers[i - 1];
    }

    // insert the new player at the found index
    existingPlayers[insertIndex] = newPlayer;

    currentSize++;
    writeArrayToFile(fileName, existingPlayers, currentSize);

    delete[] existingPlayers;
}

void copy(const char* source, char* destination) {
    if (source == nullptr || destination == nullptr) {
        return;
    }
    while (*source != '\0') {
        *destination = *source;
        ++source;
        ++destination;
    }
    *destination = '\0';
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

void deleteMatrix (int** board, size_t rows) {
    for (size_t i = 0; i < rows; i++) {
        delete[] board[i];
    }
    delete[] board;
}

const char* getFileName() {
    if(dimension < 4 || dimension > 10){
        return nullptr;
    }
    const char* fileNames [] = { "4.txt", "5.txt", "6.txt", "7.txt", "8.txt", "9.txt", "10.txt"};
    return fileNames[dimension - 4];
}

void showLeaderboard() {
    cout << "Enter dimension to show the leaderboard for:" << endl;
    cin >> dimension;

    while (dimension < 4 || dimension > 10) {
        cout << "Please, enter dimension between 4 and 10 included" << endl;
        cin >> dimension;
    }

    const char* fileName = getFileName();
    size_t size;
    Player* result = readArrayFromFile(fileName, size);

    for (int i = 0; i < 5; i++)
        std::cout << setw(50) << result[i].name << setw(10) << result[i].score << std::endl;

    delete[] result;
}

void startGame(char* nickname) {
    cout << "Enter your nickname:" << endl;
    cin >> nickname;
    cout << endl << "Enter dimension:" << endl;
    cin >> dimension;

    while (dimension < 4 || dimension > 10) {
        cout << endl << "Please, enter dimension between 4 and 10 included" << endl;
        cin >> dimension;
    }

    // update them in case of starting a new game
    gameWon = false;
    SCORE = 0;
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


    size_t randomIndex = rand() % freePosCount;
    k = freePositionsI[randomIndex];
    l = freePositionsJ[randomIndex];

    size_t randomNumIndex = rand() % 2;
    board[k][l] = numsToFill[randomNumIndex];
    SCORE+= numsToFill[randomNumIndex];

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

                if(board[i][j] == 2048){
                    gameWon = true;
                }
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

                if(board[i][j] == 2048){
                    gameWon = true;
                }
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

                if(board[i][j] == 2048){
                    gameWon = true;
                }
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

                if(board[i][j] == 2048){
                    gameWon = true;
                }
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
                        hasChanged = true;
                        break;
                    }
                }
            }
        }
    }
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

int main()  {
    char nickname[MAX_NAME_LEN];
    int menu;

    do {
        cout << "Choose from:" << endl
             << "1. Start Game" << endl
             << "2. Leaderboard" << endl
             << "3. Quit" << endl;

        cin >> menu;
        cout << endl;

        switch (menu) {
            case 1: {
                startGame(nickname);

                int **board = createMatrix(dimension, dimension);

                // add the very first number
                addRandomNum(board);

                // as long as there is an input as a command for moving
                // and the game is not over
                while (true) {
                    cout << endl;
                    cout << "SCORE:  " << SCORE << endl;
                    printBoard(board);

                    char key;
                    cin >> key;
                    makeMove(key, board);

                    if (gameOver(board)) {
                        cout << "Game over!" << endl;
                        cout << endl;
                        cout << "SCORE:  " << SCORE << endl;
                        printBoard(board);

                        Player p;
                        copy(nickname, p.name);
                        p.score = SCORE;
                        const char* fileName = getFileName();

                        appendPlayerToFile(fileName, p);

                        deleteMatrix(board, dimension);
                        board = nullptr;
                        break;
                    }

                    if (gameWon) {
                        cout << "Game won!" << endl;

                        Player p;
                        copy(nickname, p.name);
                        p.score = SCORE;
                        const char* fileName = getFileName();

                        appendPlayerToFile(fileName, p);
                        deleteMatrix(board, dimension);
                        board = nullptr;
                        break;
                    }
                }
                if (board != nullptr) {
                    deleteMatrix(board, dimension);
                    board = nullptr;
                }
                break;
            }

            case 2: {
                showLeaderboard();
                break;
            }

            case 3:{
                return 0;
            }
            default: {
                cout << "Please, enter 1, 2, or 3:" << endl;
                cin >> menu;
                break;
            }
        }
    } while (menu != 3);

    return 0;
}