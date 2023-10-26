#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <algorithm>

//TO-DO: Mirror boards
/**
 * Check if file can successfully be opened
 * 
 * @param fileName name of file to be opened
 * 
 * return the opened file stream
 */
template<typename T>
T validateFile(std::string fileName);
/**
 * 
 * 
 */
void printBoard(std::vector<std::vector<char> > &board);
/**
 * Reads the input stream and organize the information into vectors
 * 1st vector: dimension
 * 2nd vector: positive KWs 
 * 3rd vector: negative KWs
 * 
 * @param in_str Input File stream to read data from
 * 
 * @return 2D vector with dimension and positive KWs and negative KWs organized
 */
std::vector<std::vector<std::string> > fetchInfo(std::ifstream &in_str);
/**
 * Initializes a board by setting all cells to (' ')
 * 
 * @param board 2D vector representing the board
 */
void initlaizeBoard(std::vector<std::vector<char> > &board);
/**
 * Check to see if a word can fit within the bounds of the board given the start
 * position and direction
 * 
 * @param board 2D vector representing the board
 * @param word Word to be placed on the board
 * @param startRow Start row of the word
 * @param startCol Start col of the word
 * @param direction Cardinal direction for word to be placed (N, NE, E, SE, S, SW, W, NW, N)
 * 
 * @return True if word fits within the bound. False otherwise
 */
bool isWordInBound(std::vector<std::vector<char> > &board, std::string word, 
                   int startRow, int startCol, std::string direction);
/**
 * Check to see if a word will overlap with existing letters on the board given the start
 * position direction
 * 
 * @param board 2D vector representing the board
 * @param word Word to be placed on the board
 * @param startRow Start row of the word
 * @param startCol Start col of the word
 * @param direction Cardinal direction for word to be placed (N, NE, E, SE, S, SW, W, NW)
 * 
 * @return True is there is an overlap. False otherwise
 */
bool isOverlap(std::vector<std::vector<char> > &board, std::string word, 
               int startRow, int startCol, std::string direction);
/**
 * Recursive function to search for a word starting from a specific position in a given direction.
 *
 * @param board 2D vector representing the board
 * @param word Word to be searched for on the board
 * @param row Current row position on board
 * @param col Current col position on board
 * @param wordIndex Current index of word being searched
 * @param direction Cardinal direction to search in (N, NE, E, SE, S, SW, W, NW)
 * 
 * @return True if the word is found. False otherwise.
 */
bool search(std::vector<std::vector<char> > &board, std::string word, 
            int row, int col, int wordIndex, std::string direction);
/**
 * Checks if a word can be found on the board in any cardinal direction
 * 
 * @param board 2D vector representing the board
 * @param word Word to be searched for on the board
 * 
 * @return True if word is found. False otherwise
 */
bool isWordFound(std::vector<std::vector<char> > &board, std::string word);
/**
 * Checks board for any occurence of negative KWs
 * 
 * @param board 2D vector representing the board
 * @param negativeKWs Vector of strings of negative KWs
 * 
 * @return True if any negative KW is found in the board. False otherwise
 */
bool includesNegativeKWs(std::vector<std::vector<char>> &board, std::vector<std::string> &negativeKWs);
/**
 * Validates board by making sure that all positive KWs are included 
 * and no negative KWs are included
 * 
 * @param board 2D vector representing the board
 * @param positiveKWs Vector of strings of positive KWs
 * @param negativeKWs Vector of strings of negative KWs
 */
bool isValidBoard(std::vector<std::vector<char> > &board, const std::vector<std::string> &positiveKWs, 
                                                          const std::vector<std::string> &negativeKWs);
/**
 * Generates a puzzle board where all positive KWs are on the board, 
 * avoiding negative KWs
 * 
 * @param board 2D vector representing the board
 * @param positiveKWs Vector of strings of positive KWs
 * @param negativeKWs Vector of strings of negative KWs
 * @param index Index of current positive KW
 * @param row Current position of row for placing word
 * @param col Current position of col for placing word
 *
 * @return True if puzzle is successfully generated. False otherwise
 */
bool puzzleGenerator(std::vector<std::vector<char> > &board, 
                     std::vector<std::string> &positiveKWs, 
                     std::vector<std::string> &negativeKWs, 
                     int index, int row, int col);
/**
 * Generates a list of potential boards by filling empty cells with 
 * letters from 'a' to 'z'
 * 
 * @param board 2D vector representing the board
 * @param negativeKWs Vector of strings of negative KWs
 * @param row Current position of row for placing letter
 * @param col Current position of col for placing letter
 * @param boards A 3D vector that stores all valid boards
 */
void generateBoards(std::vector<std::vector<char>> &board, std::vector<std::string> negativeKWs,
                   int row, int col, std::vector<std::vector<std::vector<char> > >&boards);
void reverseRow(std::vector<std::vector<char> > board, int col);
void reverseCol(std::vector<std::vector<char> > board, int row);
/**
 * Prints a single solution to the output stream 
 * 
 * @param boards 3D vector of all possible boards
 * @param out_str Output file stream to write the board to
 */
void printSingleSolution(const std::vector<std::vector<std::vector<char> > > &boards, std::ofstream &out_str);
/**
 * Prints all solutions to the output stream 
 * 
 * @param 3D vector of all possible boards
 * @param Output file stream to write the boards to
 */
void printAllSolution(const std::vector<std::vector<std::vector<char> > > &boards, std::ofstream &out_str);
int main(int argc, char const *argv[])
{
    if(argc != 4) 
    {
        std::cerr << "Invalid Argument Count" << std::endl;
        exit(1);
    }
    std::ifstream inputFile = validateFile<std::ifstream>(argv[1]);
    std::ofstream outputFile = validateFile<std::ofstream>(argv[2]);
    std::vector<std::vector<std::string> > info =  fetchInfo(inputFile);
    std::vector<std::string> dimension = info[0];
    int col = stoi(info[0][0]); 
    int row = stoi(info[0][1]);
    std::vector<std::string> positiveKW = info[1];
    std::vector<std::string> negativeKW = info[2];
    std::string mode = argv[3];

    // Initialize board
    std::vector<std::vector<char> > board(row, std::vector<char>(col, 0));
    initlaizeBoard(board);

    // Generate Board where all +KWs fit with no -KWs
    puzzleGenerator(board, positiveKW, negativeKW, 0, 0, 0);
    
    // Fill it empty blank cells and generate all combinations of letters that can be placed
    std::vector<std::vector<std::vector<char> > > allBoards;
    generateBoards(board, negativeKW, 0, 0, allBoards);

    if(mode == "one_solution")
    {
        printSingleSolution(allBoards, outputFile);
    }
    else if(mode == "all_solutions")
    {
        printAllSolution(allBoards, outputFile);
    }
    else 
    {
        std::cerr << "Invalid Mode" << std::endl;
        exit(1);
    }

    return 0;
}

template<typename T>
T validateFile(std::string fileName)
{
    T stream(fileName);
    if(!stream.good()) std::cerr << "File Error" << std::endl;
    return stream;
}
void printBoard(std::vector<std::vector<char> > &board)
{
    std::cout << "-----" << std::endl;
    for(int x = 0; x < board.size(); x++)
    {
        for(int y = 0; y < board[x].size(); y++)
        {
            std::cout << board[x][y];
        }
        std::cout << std::endl;
    }
    std::cout << "-----" << std::endl;

}
std::vector<std::vector<std::string> > fetchInfo(std::ifstream &in_str)
{
    std::vector<std::vector<std::string> > output;

    std::string col, row, sign, word;
    std::vector<std::string> dimension, positiveKW, negativeKW;

    // Get Dimension in col, row
    in_str >> col >> row;
    dimension.push_back(col);
    dimension.push_back(row);
    
    // Get +KW and -KW
    while(in_str >> sign >> word)
    {
        if(sign == "+")
        {
            positiveKW.push_back(word);
        }
        else if (sign == "-")
        {
            negativeKW.push_back(word);
        }
    }

    output.push_back(dimension);
    output.push_back(positiveKW);
    output.push_back(negativeKW);

    return output;
}
void initlaizeBoard(std::vector<std::vector<char> > &board)
{
    // Iterates through each cell of board
    for(int x = 0; x < board.size(); x++)
    {
        for(int y = 0; y < board[x].size(); y++)
        {
            board[x][y] = ' ';
        }
    }
}
bool isWordInBound(std::vector<std::vector<char> > &board, std::string word, 
                   int startRow, int startCol, std::string direction)
{
    int rowBound = board.size();
    int colBound = board[0].size();

    // Define direction offsets for each cardinal direction
    std::map<std::string,std::pair<int, int> > calculateOffset = 
    {
        {"N" , { -1 * word.length(),  0                }},
        {"NE", { -1 * word.length(),  word.length()    }},
        {"E" , { 0                 ,  word.length()    }},
        {"SE", { word.length()     ,  word.length()    }},
        {"S" , { word.length()     ,  0                }},
        {"SW", { word.length()     , -1 * word.length()}},
        {"W" , { 0                 , -1 * word.length()}},
        {"NW", { -1 * word.length(), -1 * word.length()}}
    };

    // See coord the word will end up if placed
    startRow = startRow + calculateOffset[direction].first;
    startCol = startCol + calculateOffset[direction].second;

    // Check if word is out of bound if placed
    return startRow <= rowBound && startCol <= colBound &&
           startRow >= -1 && startCol >= -1;
}
bool isOverlap(std::vector<std::vector<char> > &board, std::string word, 
               int startRow, int startCol, std::string direction)
{
    // Define direction offsets for each cardinal direction
    std::map<std::string,std::pair<int, int> > calculateOffset = 
    {
        {"N" , { -1,  0}},
        {"NE", { -1,  1}},
        {"E" , {  0,  1}},
        {"SE", {  1,  1}},
        {"S" , {  1,  0}},
        {"SW", {  1, -1}},
        {"W" , {  0, -1}},
        {"NW", { -1, -1}}
    };

    for (char letter : word) 
    {
        // Check bounds 
        if (startRow >= 0 && startRow < board.size() && startCol >= 0 && startCol < board[0].size()) 
        {
            // Check if current cell is a empty cell
            // Check if char on the board matches with current char of word
            if (board[startRow][startCol] != ' ' && board[startRow][startCol] != letter) 
            {
                // If char does not match or cell is not empty, there is an overlap
                return true;
            }
        } 
        else 
        {
            // If word goes out of bound
            return true; 
        }

        // Calculate new position based on direction
        startRow += calculateOffset[direction].first;
        startCol += calculateOffset[direction].second;
    }
    // If no letters overlap
    return false;
}
bool search(std::vector<std::vector<char> > &board, std::string word, int row, int col, int wordIndex, std::string direction)
{
    // When index is equal to word length, it has searched through all chars of string
    if(wordIndex >= word.length()) return true;
 
    // Checking bounds
    if(row < 0 || row >= board.size() ||
       col < 0 || col >= board[0].size()) 
       return false;

    // If char on board does not equal char of word
    if(board[row][col] != word[wordIndex]) return false;

    // Define direction offsets for each cardinal direction
    std::map<std::string,std::pair<int, int> > calculateOffset = 
    {
        {"N" , { -1,  0}},
        {"NE", { -1,  1}},
        {"E" , {  0,  1}},
        {"SE", {  1,  1}},
        {"S" , {  1,  0}},
        {"SW", {  1, -1}},
        {"W" , {  0, -1}},
        {"NW", { -1, -1}}
    };

    // Calculate new position based on direction
    row += calculateOffset[direction].first;
    col += calculateOffset[direction].second;

    // Recursive call with new position and next index
    return search(board, word, row, col, wordIndex + 1, direction);

}
bool isWordFound(std::vector<std::vector<char> > &board, std::string word)
{
        int maxRows= board.size();
        int maxCols = board[0].size();

        // Iterate through board
        for(int row = 0; row < maxRows; row++)
        {
            for(int col = 0; col < maxCols; col++)
            {
                // Search in each cardinal direction of each char in board
                if (search(board, word, row, col, 0, "N")  ||
                    search(board, word, row, col, 0, "NE") ||
                    search(board, word, row, col, 0, "E")  ||
                    search(board, word, row, col, 0, "SE") ||
                    search(board, word, row, col, 0, "S")  ||
                    search(board, word, row, col, 0, "SW") ||
                    search(board, word, row, col, 0, "W")  ||
                    search(board, word, row, col, 0, "NW"))
                // If word is found in any cardinal direction, return true;
                return true;
            }
        }
        // If entire board has been searched
        return false;
}
bool includesNegativeKWs( std::vector<std::vector<char>> &board, std::vector<std::string> &negativeKWs)

{
    // Iterates though all negativeKWs
    for(std::string word : negativeKWs) 
    {
        // Check if the negativeKW is found in the board
        if(isWordFound(board, word)) 
        {
            return true;
        }
    }
    return false;
}
bool isValidBoard(std::vector<std::vector<char> > &board, const std::vector<std::string> &positiveKWs, 
                                                          const std::vector<std::string> &negativeKWs)
{
    for(std::string word : positiveKWs)
    {
        // Positive KW is NOT found
        if(!isWordFound(board, word)) return false;
    }
    for(std::string word : negativeKWs)
    {
        // Negative KW is found
        if(isWordFound(board, word)) return false;
    }
    return true;
}
bool puzzleGenerator(std::vector<std::vector<char> > &board, 
                     std::vector<std::string> &positiveKWs, 
                     std::vector<std::string> &negativeKWs, 
                     int index, int row, int col)
{
    // Authorize board when after going through all positive KWs
    if (index == positiveKWs.size()) 
    {
        // Check for negative KWs in board
        return !includesNegativeKWs(board, negativeKWs);
    }

    int maxRows = board.size();
    int maxCols = board[0].size();

    // Define direction offsets for each cardinal direction
    std::vector<std::string> directions = {"N", "NE", "E", "SE", "S", "SW", "W", "NW"};
    std::map<std::string,std::pair<int, int> > calculateOffset = 
    {
        {"N" , { -1,  0}},
        {"NE", { -1,  1}},
        {"E" , {  0,  1}},
        {"SE", {  1,  1}},
        {"S" , {  1,  0}},
        {"SW", {  1, -1}},
        {"W" , {  0, -1}},
        {"NW", { -1, -1}}
    };
    
    int rowOriginal = row;
    int colOriginal = col;
    // Iterate through all possible direction in each cell
    for(std::string direction : directions)
    {
        for(int x = 0; x < maxRows; x++)
        {
            for(int y = 0; y < maxCols; y++)
            {   
                // Check to see if word can be placed in a specific direction
                // Check to see if word can be placed without conflicting with another word on the board
                if(isWordInBound(board, positiveKWs[index], x, y, direction) &&
                   !isOverlap(board, positiveKWs[index], x, y, direction))
                {
                    int row = x;
                    int col = y;

                    // Keeps track of shared letters
                    // These letter(s) should not be removed when word is removed
                    std::vector<char> cannotRemove;

                    // Place letter of word
                    for(char letter : positiveKWs[index])
                    {
                        if(board[row][col] == ' ' ) board[row][col] = letter;
                        else cannotRemove.push_back(letter);
                        // Calculate new position based on direction
                        row += calculateOffset[direction].first;
                        col += calculateOffset[direction].second;
                    }
                    
                    // Recursively call with next positive keyword
                    if (puzzleGenerator(board, positiveKWs, negativeKWs, index + 1, rowOriginal, colOriginal)) 
                    {
                        // Return if next positive keyword is successfully placed
                        return true;
                    }

                    // Backtrack and reset the cell back to (' ')
                    row = x;
                    col = y;
                    for(char letter : positiveKWs[index])
                    {
                        // Prevents shared letter from another word to be removed
                        std::vector<char>::const_iterator it = std::find(cannotRemove.begin(), cannotRemove.end(), letter);
                        if(it == cannotRemove.end()) 
                        {
                            board[row][col] = ' ';
                        }
                        // Calculate new position based on direction
                        row += calculateOffset[direction].first;
                        col += calculateOffset[direction].second;
                    }
                }
            }
        }
    }
    return false;
}
void generateBoards(std::vector<std::vector<char>> &board, std::vector<std::string> negativeKWs,
                    int row, int col, std::vector<std::vector<std::vector<char> > > &boards)
{
    // Add board to boards once it has iterated through all rows of the board
    if(row == board.size())
    {
        std::vector<std::vector<char> > boardCopy = board;
        // Check to see if generated version of board inlcludes negative KWs
        if(!includesNegativeKWs(board, negativeKWs))
        {
            // Valid board will be added to the list of valid boards;
            boards.push_back(boardCopy);
        }
        return;
    }

    // Itterate through all cells
    int nextRow = row;
    int nextCol = col + 1;
    // Last cell of Col, move to next row
    if(nextCol == board[row].size()) 
    {
        nextRow = row + 1;
        nextCol = 0;
    }

    // Makes sure cell is empty
    if(board[row][col] == ' ')
    {
        // Places letter from a-z
        for(char character = 'a'; character <= 'z'; character++) 
        {
            board[row][col] = character;
            // Recur to the next cell
            generateBoards(board, negativeKWs, nextRow, nextCol, boards);
            // Reset the cell back to empty for the next iteration
            board[row][col] = ' ';
        }
    }
    else
    {
        // Move onto next cell if cell already has a letter
        generateBoards(board, negativeKWs, nextRow, nextCol, boards);
    }
}

void generateMirrorBoards(std::vector<std::vector<char>> &boards)
{
    int numBoards = boards.size();
    for(int x = 0; x < numBoards; x++)
    {
        
    }
}
void printSingleSolution(const std::vector<std::vector<std::vector<char> > > &boards, std::ofstream &out_str)
{
    out_str << "Board: \n";
    for(std::vector<char> row : boards[0])
    {
        out_str << "  ";
        for(char letter : row)
        {
            out_str << letter;
        }
        out_str << "\n";
    }
    
}
void printAllSolution(const std::vector<std::vector<std::vector<char> > > &boards, std::ofstream &out_str)
{
    out_str << boards.size() << " solution(s)\n";
    for(std::vector<std::vector<char> > board : boards)
    {
        out_str << "Board: \n";
        for(std::vector<char> row : board)
        {
            out_str << "  ";
            for(char letter : row)
            {
                out_str << letter;
            }
            out_str << "\n";
        }
    }
}