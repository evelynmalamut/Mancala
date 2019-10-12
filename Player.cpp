//
//  Player.cpp
//  Project_3
//
//  Created by Evelyn Malamut on 5/13/19.
//  Copyright © 2019 Evelyn Malamut. All rights reserved.
//
#include "Player.h"

//constructor
Player::Player(std::string name) {
    m_name = name;
}

//Return false if the player is a computer player. Return true if the player is human. Most kinds of players will be computer players.
bool Player::isInteractive() const {
    return false;
}

//Return the name of the player.
std::string Player::name() const {
    return m_name;
}

//constructor
BadPlayer::BadPlayer(std::string name)
: Player(name)
{}

//Badplayer chooses leftmost hole that isn't empty
int BadPlayer::chooseMove(const Board& b, Side s) const {
    for (int i = 1; i < b.holes() + 1; i++) { //loop through holes
        if (b.beans(s,i) != 0) //if hole is not empty
            return i; //return hole
    }
    return -1; //whole side is empty, no possible moves
}


//constructor
HumanPlayer::HumanPlayer(std::string name)
: Player(name)
{}

bool HumanPlayer::isInteractive() const {
    return true;
}

int HumanPlayer::chooseMove(const Board& b, Side s) const {
    int i = -1;
    bool check = true;
    while(check && b.beansInPlay(NORTH) != 0 && b.beansInPlay(SOUTH) != 0) {
        std::cout << "Chose your next move:" << std::endl; //prompt for hole choice
        std::cin >> i; //cin hole choice
        if (i > 0 && i <= b.holes() && b.beans(s,i) != 0)
            check = false;
    }
    return i;
}

//constructor
SmartPlayer::SmartPlayer(std::string name)
: Player(name)
{}

int SmartPlayer::chooseMove(const Board& b, Side s) const {
    int bestHole = 1;
    int value = -1000;
    int depth = 2;
    helper_chooseMove(s,b,bestHole,value,depth);
    return bestHole;
}

void SmartPlayer::helper_chooseMove(Side s, const Board& b, int& bestHole, int& value, int& depth) const {
    Board myBoard = b;
    int eval = 0;
    int v = 0;
    
    if (myBoard.beansInPlay(NORTH) == 0 || myBoard.beansInPlay(SOUTH) == 0) { //if side is empty (ie. game is over)
        bestHole = -1;
        return;
    }
    for (int i = 1; i < b.holes() + 1; i++) { //loop through holes
        if (b.beans(s,i) != 0) {//if hole is not empty (ie. valid choice)
            Board temp = myBoard;
            eval = evaluate(temp,s,i,depth,v,s);
            if (value <= eval) {
                bestHole = i;
                value = eval;
            }
        }
    }
}

int SmartPlayer::evaluate(Board& myBoard, Side s, int hole, int& depth, int& value, Side player) const {
    Side otherSide;
    Side endSide = SOUTH;
    Side otherPlayer;
    int endHole = 0;
    int eval;
    
    if (s == SOUTH)
        otherSide = NORTH;
    else
        otherSide = SOUTH;
    
    if (player == SOUTH)
        otherPlayer = NORTH;
    else
        otherPlayer = SOUTH;
    
    if (depth == 0)
        return value;
    
    myBoard.sow(s, hole, endSide, endHole);
    
    //capture: ending side is the same as the starting side, the ending hole has 1 bean (newly added), and the hole directly opposite is not empty, and it's not the pot
    if (endSide == s && myBoard.beans(endSide, endHole) == 1 && myBoard.beans(otherSide,endHole) != 0 && myBoard.beansInPlay(s) != 0 && myBoard.beansInPlay(otherSide) != 0) {
        myBoard.moveToPot(endSide,endHole,s); //move bean in ending hole to pot
        myBoard.moveToPot(otherSide,endHole,s); //move beans from other side's hole into pot
    }
    
    if (endHole == 0 && myBoard.beansInPlay(s) != 0 && myBoard.beansInPlay(otherSide) != 0) { //if player ends in the pot
        for (int i = 1; i < myBoard.holes() + 1; i++) {
            if (myBoard.beans(s,i) != 0) { //loop through potential second choices
                if (myBoard.beansInPlay(s) == 0 || myBoard.beansInPlay(otherSide) == 0) { //if game is over
                    return value; //return value
                }
                evaluate(myBoard,s,i,depth,value,player); //else evaluate at those positions
            }
        }
    }
    
    if (myBoard.beansInPlay(s) == 0 || myBoard.beansInPlay(otherSide) == 0) { //if game is over
        for (int i = 1; i < myBoard.holes() + 1; i++) { //sweep beans
            myBoard.moveToPot(s,i,s);
            myBoard.moveToPot(otherSide,i,otherSide);
        }
        if (myBoard.beans(s,0) == myBoard.beans(otherSide,0)) //if tie, value = 0
            value = 0;
        else if (myBoard.beans(player,0) > myBoard.beans(otherPlayer,0)) //if we have more beans in pot, we win
            value = 1000;
        else //we lose
            value = -1000;
        return value;
    }

    
    //game is still in play
    value = myBoard.beans(s,0) - myBoard.beans(otherSide, 0); //take the difference
    
    if (s == player) { //our turn, we want to pick minimum of other player
        int currentMin = 1000;
        depth --; //decrease depth
        for (int i = 1; i < myBoard.holes() + 1; i++) { //loop through holes
            if (myBoard.beans(otherSide,i) != 0) {//if hole is not empty (ie. valid choice)
                    eval = evaluate(myBoard,otherSide,i,depth,value,player);
                    if (currentMin >= eval) //if the current minimum is less than the other options, then the other player will have picked that option
                        currentMin = eval;
            }
        }
        return currentMin;
    }

    else { //other player's turn. We want to pick max of their picks
        int currentMax = -1000;
        depth --;
        for (int i = 1; i < myBoard.holes() + 1; i++) { //loop through holes
            if (myBoard.beans(otherSide,i) != 0) {//if hole is not empty (ie. valid choice) (we want to evaluate the max of our possible turns)
                eval = evaluate(myBoard,otherSide,i,depth,value,player);
                if (currentMax <= eval) //if the eval is larger than the currentMax, we want to choose that
                    currentMax = eval;
            }
        }
        return currentMax;
    }
}
