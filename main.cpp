#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <algorithm>

template<typename T>
T validateFile(std::string fileName);
void printBoard(std::vector<std::vector<char> > &board);

std::vector<std::vector<std::string> > fetchInfo(std::ifstream &in_str);
bool isWordInBound(std::vector<std::vector<char> > &board, std::string word, 
                   int startRow, int startCol, std::string direction);
bool isOverlap(std::vector<std::vector<char> > &board, std::string word, 
               int startRow, int startCol, std::string direction);
bool search(std::vector<std::vector<char> > &board, std::string word, 
            int row, int col, int wordIndex, std::string direction);
bool isWordFound(std::vector<std::vector<char> > &board, std::string word);
bool includesNegativeKWs(std::vector<std::vector<char>> &board, std::vector<std::string> &negativeKWs);
bool puzzleGenerator(std::vector<std::vector<char> > &board, 
                     std::vector<std::string> &positiveKWs, 
                     std::vector<std::string> &negativeKWs, 
                     int index, int row, int col);
void generateBoard(std::vector<std::vector<char>> &board, std::vector<std::string> negativeKWs,
                   int row, int col, std::vector<std::vector<std::vector<char> > >&boards);
int main(int argc, char const *argv[])
{
    std::ifstream inputFile = validateFile<std::ifstream>(argv[1]);
    std::ofstream outputFile = validateFile<std::ofstream>(argv[2]);
    std::vector<std::vector<std::string> > info =  fetchInfo(inputFile);
    std::vector<std::string> dimension = info[0];
    int col = stoi(info[0][0]); 
    int row = stoi(info[0][1]);
    std::vector<std::string> positiveKW = info[1];
    std::vector<std::string> negativeKW = info[2];
    
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;

    // Initialize board
    std::vector<std::vector<char> > board(row, std::vector<char>(col, 0));
    for(int x = 0; x < board.size(); x++)
    {
        for(int y = 0; y < board[x].size(); y++)
        {
            board[x][y] = ' ';
        }
    }

    puzzleGenerator(board, positiveKW, negativeKW, 0, 0, 0);
    std::vector<std::vector<std::vector<char> > > allBoards;


    std::cout << std::endl;
    std::cout << "BOARD: " << std::endl;
    printBoard(board);

    generateBoard(board, negativeKW, 0, 0, allBoards);
    int count = 1;
    for(auto board : allBoards)
    {
        std::cout << "======" << std::endl;
        std::cout << count << std::endl;
        printBoard(board);
        std::cout << "======" << std::endl;
        count++;    

    }

    return 0;
}

template<typename T>
T validateFile(std::string fileName)
{
    T stream(fileName);
    if(!stream.good()) std::cerr << "Infile File" << std::endl;
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
bool isWordInBound(std::vector<std::vector<char> > &board, std::string word, 
                   int startRow, int startCol, std::string direction)
{
    int rowBound = board.size();
    int colBound = board[0].size();

    // define direction offsets for each cardinal direction
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
        // If reach end, word cannot be found
        return false;
}
bool includesNegativeKWs(std::vector<std::vector<char>> &board, std::vector<std::string> &negativeKWs)
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
void generateBoard(std::vector<std::vector<char>> &board, std::vector<std::string> negativeKWs,
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
            generateBoard(board, negativeKWs, nextRow, nextCol, boards);
            // Reset the cell back to empty for the next iteration
            board[row][col] = ' ';
        }
    }
    else
    {
        // Move onto next cell if cell already has a letter
        generateBoard(board, negativeKWs, nextRow, nextCol, boards);
    }
}
