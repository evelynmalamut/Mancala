//
//  Player.h
//  Project_3
//
//  Created by Evelyn Malamut on 5/13/19.
//  Copyright © 2019 Evelyn Malamut. All rights reserved.
//

#ifndef Player_h
#define Player_h

#include <string>
#include "Board.h"
#include "Side.h"
#include <iostream>

class Player {
public:
    //Create a Player with the indicated name.
    Player(std::string name);

    //Return the name of the player.
    std::string name() const;

    //Return false if the player is a computer player. Return true if the player is human. Most kinds of players will be computer players.
    virtual bool isInteractive() const;

    //Every concrete class derived from this class must implement this function so that if the player were to be playing side s and had to make a move given board b, the function returns the move the player would choose. If no move is possible, return −1.
    virtual int chooseMove(const Board& b, Side s) const = 0;

    //Since this class is designed as a base class, it should have a virtual destructor.
    virtual ~Player() {};
    
private:
    std::string m_name;
};


//A BadPlayer is a computer player that chooses an arbitrary valid move and returns that choice. "Arbitrary" can be what you like: leftmost, nearest to pot, fewest beans, random, etc.. The point of this class is to have an easy-to-implement class that at least plays legally.
class BadPlayer : public Player {
public:
    BadPlayer(std::string name); //constructor
    
    virtual int chooseMove(const Board& b, Side s) const; //Badplayer chooses leftmost hole that isn't empty
    
    virtual ~BadPlayer() {}
    
};

//A HumanPlayer chooses its move by prompting a person running the program for a move (reprompting if necessary until the person enters a valid hole number), and returning that choice. We will never test for a situation where the user doesn't enter an integer when prompted for a hole number.
class HumanPlayer : public Player {
public:
    HumanPlayer(std::string name);
    
    virtual bool isInteractive() const; //HumanPlayer is only interactive class
    
    virtual int chooseMove(const Board& b, Side s) const;
    
    virtual ~HumanPlayer() {}
};

class SmartPlayer : public Player {
public:
    SmartPlayer(std::string name);
    
    virtual int chooseMove(const Board& b, Side s) const;
    
    virtual ~SmartPlayer() {}

private:
    void helper_chooseMove(Side s, const Board& b, int& bestHole, int& value, int& depth) const;
    int evaluate(Board& myBoard,Side s, int hole, int& depth, int& value, Side player) const;
    //void displayBoard(Board b) const;
    
};

#endif /* Player_h */
