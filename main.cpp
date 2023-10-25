#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <map>

template<typename T>
T validateFile(std::string fileName);
void printBoard(std::vector<std::vector<char> > &board);

std::vector<std::vector<std::string> > fetchInfo(std::ifstream &in_str);
bool isWordInBound(std::vector<std::vector<char>> &board, std::string word, 
                   int startRow, int startCol, std::string direction);
bool isOverlap(std::vector<std::vector<char>> &board, std::string word, 
               int startRow, int startCol, std::string direction);
bool search(std::vector<std::vector<char>> &board, std::string word, 
            int row, int col, int index, std::string direction);
bool isWordFound(std::vector<std::vector<char> > &board, std::string word);


bool puzzleGenerator(std::vector<std::vector<char>> &board, 
                     std::vector<std::string> &positiveKWs, 
                     std::vector<std::string> &negativeKWs, 
                     int index, int row, int col);

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
    std::cout << "BOARD: " << std::endl;
    printBoard(board);
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
bool isWordInBound(std::vector<std::vector<char>> &board, std::string word, 
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
bool isOverlap(std::vector<std::vector<char>> &board, std::string word, 
               int startRow, int startCol, std::string direction)
{
    // define direction offsets for each cardinal direction
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
            // Check if the current character on the board matches the character from the word
            if (board[startRow][startCol] != ' ' && board[startRow][startCol] != letter) 
            {
                return true;
            }
        } 
        else 
        {
            // The word goes out of bounds, return true
            return true; 
        }

        startRow += calculateOffset[direction].first;
        startCol += calculateOffset[direction].second;
    }
    return false;


}
bool search(std::vector<std::vector<char>> &board, std::string word, int row, int col, int index, std::string direction)
{
    // when index is equal to word length, search has reached end of word
    if(index >= word.length()) return true;
 
    // checking bounds checking
    if(row < 0 || row >= board.size() ||
       col < 0 || col >= board[0].size()) 
       return false;

    // if current char on board does not equal current char of word
    if(board[row][col] != word[index]) return false;

    // define direction offsets for each cardinal direction
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
    row = row + calculateOffset[direction].first;
    col = col + calculateOffset[direction].second;

    // Recursive call with updated row, col and index
    return search(board, word, row, col, index + 1, direction);

}
bool isWordFound(std::vector<std::vector<char> > &board, std::string word)
{
        int rows = board.size();
        int cols = board[0].size();
        // Iterate through board
        for(int row = 0; row < rows; row++)
        {
            for(int col = 0; col < cols; col++)
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
                return true;
            }
        }
        // if reach end, word cannot be found
        return false;
}
bool puzzleGenerator(std::vector<std::vector<char>> &board, 
                     std::vector<std::string> &positiveKWs, 
                     std::vector<std::string> &negativeKWs, 
                     int index, int row, int col)
{
    // Check board when after going through all positive KWs
    if (index == positiveKWs.size()) 
    {
        // Check for negative KWs in board
        for (std::string word : negativeKWs) 
        {
            if (isWordFound(board, word)) 
            {
                std::cout << "NEGATIVE KW FOUND" << std::endl;
                return false;
            }
        }
        return true;
    }
    std::cout << "WORD: " << positiveKWs[index] << std:: endl;
    int rows = board.size();
    int cols = board[0].size();
    std::vector<std::string> directions = {"N", "NE", "E", "SE", "S", "SW", "W", "NW"};
    // define direction offsets for each cardinal direction
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

    for(std::string direction : directions)
    {
        for(int x = 0; x < rows; x++)
        {
            for(int y = 0; y < cols; y++)
            {   
                if(isWordInBound(board, positiveKWs[index], x, y, direction) &&
                   !isOverlap(board, positiveKWs[index], x, y, direction))
                {
                    int row = x;
                    int col = y;

                    // Place letter of word
                    for(char letter : positiveKWs[index])
                    {
                        // std::cout << "Placing Char: " << letter << " From Word: " << positiveKWs[index] << std::endl;
                        board[row][col] = letter;
                        // printBoard(board);
                        row += calculateOffset[direction].first;
                        col += calculateOffset[direction].second;
                    }
                    printBoard(board);

                    // Recursively call with next positive keyword
                    if (puzzleGenerator(board, positiveKWs, negativeKWs, index + 1, rowOriginal, colOriginal)) 
                    {
                        // True if word is successfully placed
                        return true;
                    }

                    //Backtrack and set letters of word back to blank
                    row = x;
                    col = y;
                    for(char letter : positiveKWs[index])
                    {
                        // std::cout << "Replacing Char: " << letter << " From Word: " << positiveKWs[index] << std::endl;
                        board[row][col] = ' ';
                        // printBoard(board);
                        row = row + calculateOffset[direction].first;
                        col = col + calculateOffset[direction].second;
                    }
                    printBoard(board);
                }
            }
        }
    }
    // row = rowOriginal;
    // col = colOriginal;  
    return false;
}