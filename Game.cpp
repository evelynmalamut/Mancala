//
//  Game.cpp
//  Project_3
//
//  Created by Evelyn Malamut on 5/13/19.
//  Copyright © 2019 Evelyn Malamut. All rights reserved.
//

#include "Game.h"
#include <iostream>
#include <string>

//Construct a Game to be played with the indicated players on a copy of the board b. The player on the south side always moves first.
Game::Game(const Board& b, Player* south, Player* north)
: m_board(b), pSouth(south), pNorth(north)
{};

//Display the game's board in a manner of your choosing, provided you show the names of the players and a reasonable representation of the state of the board
void Game::display() const {
    std::string spaces = "";
    std::cout <<"South player: " << pSouth->name();
    std::cout <<"  North player: " << pNorth->name() << std::endl;
    
    for (int k = 1; k < m_board.holes() + 1; k++)
    {
        spaces += "  ";
    }
    
    std::cout << "         " << spaces.substr(0,spaces.length()/2) <<"North" << std::endl;
    std::cout << "            ";
    for (int k = 1; k < m_board.holes() + 1; k++)
    {
        std::cout << m_board.beans(NORTH,k) << " ";
    }
    std::cout << std::endl;
    
    std::cout << "North pot " << m_board.beans(NORTH,0) << spaces << m_board.beans(SOUTH,0) << " South pot" << std::endl;
    
    std::cout << "            ";
    for (int k = 1; k < m_board.holes() + 1; k++)
    {
        std::cout << m_board.beans(SOUTH,k) << " ";
    }
    std::cout << std::endl;
    std::cout << "         " << spaces.substr(0,spaces.length()/2) << "South" <<std::endl;
}

//If the game isn't over (i.e., more moves are possible), set over to false and do not change anything else. Otherwise, set over to true and hasWinner to true if the game has a winner, or false if it resulted in a tie. If hasWinner is set to false, leave winner unchanged; otherwise, set it to the winning side.
void Game::status(bool& over, bool& hasWinner, Side& winner) const {
    if (m_board.beansInPlay(SOUTH) != 0 && m_board.beansInPlay(NORTH) != 0) { //if neither side is empty, not over
        over = false;
        return;
    }

    over = true; //if either side is empty --> end of game
    if (beans(SOUTH, 0) == beans(NORTH, 0)) //if tie (same amount of beans in pot), no winner
        hasWinner = false;
    else {
        hasWinner = true; //if more beans in NORTH or SOUTH pot, declare winenr
        if(beans(SOUTH, 0) > beans(NORTH, 0))
            winner = SOUTH;
        else
            winner = NORTH;
    }
}

//If the game is over, return false. Otherwise, make a complete move for the player whose turn it is (so that it becomes the other player's turn) and return true. "Complete" means that the player sows the seeds from a hole and takes any additional turns required or completes a capture. If the player gets an additional turn, you should display the board so someone looking at the screen can follow what's happening.
bool Game::move() {
    bool over,hasWinner;
    Side winner;
    Side endSide = SOUTH;
    int endHole = 0;
    
    status(over,hasWinner,winner); //check status
    if (!over) {
        int hole = currentPlayer->chooseMove(m_board,currentSide); //player whose turn is selects a hole
        m_board.sow(currentSide,hole,endSide,endHole); //beans from hole are sown
        display();
    }
    
    //if last bean is placed in pot
    if (endHole == 0 && !over) {
        while(endHole == 0 && !over) { //repeatedly choose new holes until you land on something that is not a zero (but might end in a capture and then would need to call that
            int hole = currentPlayer->chooseMove(m_board,currentSide);
            status(over,hasWinner,winner);
            if (!over) {
                m_board.sow(currentSide,hole,endSide,endHole);
                display();
            }
        }
    }
    //capture: ending side is the same as the starting side, the ending hole has 1 bean (newly added), and the hole directcly opposite is not empty, and it's not the pot
    if (!over && endSide == currentSide && m_board.beans(endSide, endHole) == 1 && m_board.beans(otherSide,endHole) != 0) {
        m_board.moveToPot(endSide,endHole,currentSide); //move bean in ending hole to pot
        m_board.moveToPot(otherSide,endHole,currentSide); //move beans from other side's hole into pot
    }
    
    status(over,hasWinner,winner); //check status
    if (over) {
        for (int i = 1; i < m_board.holes() + 1; i++) { //sweep beans!
            m_board.moveToPot(SOUTH,i,SOUTH);
            m_board.moveToPot(NORTH,i,NORTH);
        }
        return false; //if game is over, return false
    }
    
    Side temp = currentSide; //switch current side to side of the other player
    currentSide = otherSide;
    otherSide = temp;
    
    if (currentPlayer == pSouth)
        currentPlayer = pNorth; //switch current player with other player (other player's turn)
    else
        currentPlayer = pSouth;
    
    return true;
}


void Game::play() {
    display();
    m_play(false);
}
//Play the game. Display the progress of the game in a manner of your choosing, provided that someone looking at the screen can follow what's happening. If neither player is interactive, then to keep the display from quickly scrolling through the whole game, it would be reasonable periodically to prompt the viewer to press ENTER to continue and not proceed until ENTER is pressed. (The ignore function for input streams is useful here.) Announce the winner at the end of the game. You can apportion to your liking the responsibility for displaying the board between this function and the move function. (Note: If when this function is called, South has no beans in play, so can't make the first move, sweep any beans on the North side into North's pot and act as if the game is thus over.)


int Game::beans(Side s, int hole) const {
    if (hole < 0 || hole > m_board.holes())
        return -1;
    return m_board.beans(s,hole);
}
//Return the number of beans in the indicated hole or pot of the game's board, or −1 if the hole number is invalid. This function exists so that we and you can more easily test your program.

void Game::m_play(bool s) {
    bool over,hasWinner;
    Side winner;
    
    if (s && !pSouth->isInteractive() && !pNorth->isInteractive()) { //both computers, want to make sure human is interacting
        std::cout << "press Enter";
        std::cin.ignore(1000, '\n');
        std::cout << std::endl;
    }
    status(over,hasWinner,winner); //check status
    if (over && hasWinner) { //if over and there is a winner
        display();
        if (winner == SOUTH) {
            std::cout << pSouth->name() << " is the winner!" << std::endl;
            return;
        }
        else {
            std::cout << pNorth->name() << " is the winner!" << std::endl;
            return;
        }
    }
    else if (over && !hasWinner) {
        std::cout << "It's a tie!" << std::endl;
        return;
    }
    else {
        std::cout <<"It's " << currentPlayer->name() << "'s turn" << std::endl;
        move();
        m_play(!s);
    }
}

