#include <iostream>
#include <vector>
#include <algorithm>
#include <random>


class Square
{
    public:

    int x;
    int y;
    int piece;

    Square(int x, int y) : x(x), y(y)
    {
        piece = 0;
    }
};

class Ship
{
public:

    int size;
    int hits;
    bool orientation; //true = down, false = right
    std::vector<Square> wholeship;
    Ship(int size) : size(size)
    {
        hits = 0;
        std::mt19937 rng(std::random_device{}());
        if (rng() % 2 == 0)
            orientation = true;
        else
            orientation = false;
    }

};
std::vector<Ship> shipcontainer;

class Board
{
public:

    std::vector<Square> board;
    std::vector<Square> randboard;
    int boardsize;

    Board(int size)
    {
        boardsize = size;
        for (int i = 0; i < size; ++i)
        {
            for (int j = 0; j < size; ++j)
            {
                board.emplace_back(Square(j, i));
                randboard.emplace_back(Square(j, i));
            }
        }
        std::mt19937 rng(std::random_device{}());
        std::shuffle(randboard.begin(), randboard.end(), rng);
    }

    void printboard()
    {
        std::cout << "\n\n\n  ";
        for (int j = 0; j < boardsize; ++j)
            std::cout << j;
        std::cout << "\n0 ";
        for (int i = 0; i < board.size(); ++i)
        {
            /*if (board[i].piece == 1)      //testing purposes
                std::cout << "ship";
            else if (board[i].piece == 2)
                std::cout << "****";
            else
                std::cout << " " << board[i].x << board[i].y << " ";*/
            

            if (board[i].piece == 2)
                std::cout << "*";
            else
                std::cout << "~";

            if (i % boardsize == boardsize - 1 && i != board.size() - 1)
                std::cout << "\n" << i / boardsize + 1 << " ";
        }
        std::cout << "\n\n\n";
    }

    bool fire()
    {
        int x, y;
        std::cin >> x >> y;
        bool flag = false;
        for (Ship& ship : shipcontainer)
        {
            for (Square& sqr : ship.wholeship)
            {
                if (sqr.x == x && sqr.y == y && sqr.piece == 1)
                {
                    std::cout << "Hit!\n";
                    ++ship.hits;
                    flag = true;
                    sqr.piece = 2;
                    board[x + boardsize * y].piece = 2;
                        if (ship.hits == ship.size)
                            std::cout << "You sank my battleship!\n";
                    return true;
                }
            }
        }
        if (flag == false)
        {
            std::cout << "Miss!\n";
            return false;
        }

    }

};



bool placeship(Board& board, Ship& ship)
{
    bool check = false;
    while (check == false)
    {
        int x1 = board.randboard[board.randboard.size() - 1].x;
        int y1 = board.randboard[board.randboard.size() - 1].y;
        board.randboard.pop_back();
        if (board.randboard.empty() == true)
        {
            for (int i = 0; i < board.boardsize; ++i)
            {
                for (int j = 0; j < board.boardsize; ++j)
                {
                    board.randboard.emplace_back(Square(j, i));
                }
            }
            return false;
        }

        int it = x1 + y1 * board.boardsize;
        bool check2 = true;
        for (int j = 0; j < 2; ++j)
        {
            check2 = true;
            for (int i = 0; i < ship.size; ++i)
            {
                if (ship.orientation == false && x1 + ship.size > board.boardsize)
                {
                    check2 = false;
                    break;
                }
                if (ship.orientation == true && y1 + ship.size > board.boardsize)
                {
                    check2 = false;
                    break;
                }
                if (ship.orientation == false)
                {
                    if (board.board[it - 1].piece == 1 && it - 1 >= 0)
                    {
                        check2 = false;
                        break;
                    }
                    if (it + ship.size < board.boardsize * board.boardsize 
                    && board.board[it + ship.size].piece == 1)
                    {
                        check2 = false;
                        break;
                    }
                    if ((board.board[it + i].piece == 1 && it + i < board.boardsize * board.boardsize)
                    || (board.board[it - board.boardsize + i].piece == 1 && it - board.boardsize + i >= 0)
                    || (board.board[it + board.boardsize + i].piece == 1 && it + board.boardsize + i < board.boardsize * board.boardsize))
                    {
                        check2 = false;
                        break;
                    }
                }
                else if (ship.orientation == true)
                {
                    if (it - board.boardsize >= 0 && board.board[it - board.boardsize].piece == 1)
                    {
                        
                        check2 = false;
                        break;
                    }
                    if (it + board.boardsize * ship.size <= board.boardsize * board.boardsize 
                    && board.board[it + board.boardsize * ship.size].piece == 1)
                    {
                        check2 = false;
                        break;
                    }

                    if ((board.board[it + i * board.boardsize].piece == 1 && it + i * board.boardsize < board.boardsize * board.boardsize)
                    || (board.board[it + board.boardsize * i - 1].piece == 1 && it + board.boardsize * i - 1 < board.boardsize * board.boardsize)
                    || (board.board[it + board.boardsize * i + 1].piece == 1 && it + board.boardsize * i + 1 < board.boardsize * board.boardsize))
                    {
                        check2 = false;
                        break;
                    }
                }
            }
            if (check2 == true)
                break;
            else
            {
                ship.orientation = !ship.orientation;
            }
            
        }    
        if (check2 == false)
            continue;
        check = true;
        if (ship.orientation == true)
        {
            for (int i = 0; i < ship.size; ++i)
            {
                board.board[it + board.boardsize * i].piece = 1;
                ship.wholeship.emplace_back(board.board[it + board.boardsize * i]);
            }
        }
        if (ship.orientation == false)
        {
            for (int i = 0; i < ship.size; ++i)
            {
                board.board[it + i].piece = 1;
                ship.wholeship.emplace_back(board.board[it + i]);
            }
        }
        if (check == true)
            return true;
    }
}


int main()
{
    Board test(6);
    for (int i = 5; i > 1; --i)
    {
        shipcontainer.emplace_back(Ship(i));
    }

    for (Ship& ship : shipcontainer)
    {
        if (placeship(test, ship) == false)
            std::cout << "Couldn't place the ship.";
    }

    int totalhits = 14;

    for (int i = 0; i < 20; ++i)
    {   
        test.printboard();
        std::cout <<  "Shots left: " << 20 - i << "\n" << "Choose coordinates to fire: \n";
        if (test.fire() == true)
        {
            --totalhits;
            --i;
        }
        if (totalhits == 0)
        {
            std::cout << "You won!";
            break;
        }
        if (i == 20)
        {
            std::cout << "Game over, you lost!";
        }

    }



}