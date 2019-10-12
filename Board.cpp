//
//  Board.cpp
//  Project_3
//
//  Created by Evelyn Malamut on 5/13/19.
//  Copyright © 2019 Evelyn Malamut. All rights reserved.
//
#include <iostream>
#include "Board.h"

//Construct a Board with the indicated number of holes per side (not counting the pot) and initial number of beans per hole. If nHoles is not positive, act as if it were 1; if nInitialBeansPerHole is negative, act as if it were 0.
Board::Board(int nHoles, int nInitialBeansPerHole) {
    if (nHoles > 0) //if nHoles is positive
        m_nHoles = nHoles; //assign value
    else //if negative number or 0 for holes
        m_nHoles = 1; //assign 1
    if (nInitialBeansPerHole > 0) //if positive
        m_BeansPerHole = nInitialBeansPerHole; //assign value
    else //if negtaive
        m_BeansPerHole = 0; //assign 0
    
    m_totalBeans = 2*m_BeansPerHole*m_nHoles; //assign total beans, 2 times number of holes times beans per hole
    
    //create new dynamically allocated arrays for the two sides w/ size nHoels
    m_south = new int[m_nHoles];
    m_north = new int[m_nHoles];
    
    for (int i = 0; i < m_nHoles; i++) { //fill both sides with initial beans per hole
        m_south[i] = m_BeansPerHole;
        m_north[i] = m_BeansPerHole;
    }
}

//Destructor
Board::~Board() {
    delete [] m_north;
    delete [] m_south;
}

//copy constructor
Board::Board(const Board& other) {
    m_BeansPerHole = other.m_BeansPerHole; //assign built in types to other built in types
    m_nHoles = other.m_nHoles;
    m_totalBeans = other.m_totalBeans;
    m_northPot = other.m_northPot;
    m_southPot = other.m_southPot;
 
    m_north = new int[m_nHoles]; //create new dynamically allocated arrays
    m_south = new int[m_nHoles];
    
    for (int i = 0; i < m_nHoles; i++) { //fill new arrays with values from other's arrays
        m_north[i] = other.m_north[i];
        m_south[i] = other.m_south[i];
    }
}

//assignment operator
Board& Board::operator=(const Board& other) {
    if (this != &other) //protects against alias
    {
        m_BeansPerHole = other.m_BeansPerHole; //assign built in types to other built in types
        m_nHoles = other.m_nHoles;
        m_totalBeans = other.m_totalBeans;
        m_northPot = other.m_northPot;
        m_southPot = other.m_southPot;
        
        delete [] m_north; //delete current dynamically allocated arrays
        delete [] m_south;
        
        m_north = new int[m_nHoles]; //create new dynamically allocated arrays
        m_south = new int[m_nHoles];
        
        for (int i = 0; i < m_nHoles; i++) { //fill new arrays with values from other's arrays
            m_north[i] = other.m_north[i];
            m_south[i] = other.m_south[i];
        }
    }
    return *this;
}

//return the number of holes on one side (should stay constant throughout the game)
int Board::holes() const {
    return m_nHoles;
}

//Return the number of beans in the indicated hole or pot, or −1 if the hole number is invalid.
int Board::beans(Side s, int hole) const {
    int* side = Board::side(s); //pointer to first element in m_north or m_south depending on side
    
    if (hole < 0 || hole > m_nHoles) //if negtive or greater than holes assigned, return -1
        return -1;
    if (hole == POT) { //if pot, checks south or north and returns amount in desired pot
        if (s == NORTH)
            return m_northPot;
        else
            return m_southPot;
    }
    return side[hole - 1]; //returns amount in hole
}

//Return the total number of beans in all the holes on the indicated side, not counting the beans in the pot.
int Board::beansInPlay(Side s) const {
    int count = 0;
    int* side = Board::side(s); //pointer to first element in m_north or m_south depending on side

    for (int i = 0; i < m_nHoles; i++) //iterates through side and adds the number of beans in each hole
        count += side[i];
    return count; //returns value
}

// Return the total number of beans in the game, including any in the pots.
int Board::totalBeans() const {
    return m_totalBeans;
}

//If the hole indicated by (s,hole) is empty or invalid or a pot, this function returns false without changing anything. Otherwise, it will return true after sowing the beans: the beans are removed from hole (s,hole) and sown counterclockwise, including s's pot if encountered, but skipping s's opponent's pot. The parameters endSide and endHole are set to the side and hole where the last bean was placed. (This function does not make captures or multiple turns; different Kalah variants have different rules about these issues, so dealing with them should not be the responsibility of the Board class.)

bool Board::sow(Side s, int hole, Side& endSide, int& endHole) {
    int numBeans = 0;
    int* side = Board::side(s); //sets side to north or south side

    if (hole < 0 || hole > m_nHoles) //checks that hole is positive and less than the number of holes assigned
        return false;
    else if (hole == POT) //if pot, return false
        return false;
    if (side[hole -1] == 0) //if empty, return false
        return false;

    numBeans = side[hole - 1]; //sets numBeans to amount in hole
    side[hole - 1] = 0; //empties current hole
    
    while (numBeans > 0) {
        if (s == SOUTH) { //if on south side, call sowSouth
            hole ++; //increase hole (so that we add beans to next hole, not current hole)
            numBeans = sowSouth(numBeans, hole, endSide, endHole, s);
            if (numBeans == 0) //if no more beans, break
                break;
            numBeans = sowNorth(numBeans, m_nHoles, endSide, endHole, s); //call sowNorth starting from last position (want to add to last hole)
            if (numBeans == 0) //if no more beans, break
                break;
            hole = 0; //set hole to 0 (will call South again starting from first position)
        }
        else {
            hole --; //decrease hole (so that we add beans to next hole, not current hole)
            numBeans = sowNorth(numBeans, hole, endSide, endHole, s); //if on north side, call sowNorth
            if (numBeans == 0) //if no more beans, break
                break;
            numBeans = sowSouth(numBeans, 1, endSide, endHole, s); //call sowSouth starting from first position (want to add to first hole)
            if (numBeans == 0) //if no more beans, break
                break;
            hole = m_nHoles + 1; //set hole to element after last (will call sowNorth again starting from last position after decrement)
        }
    }
    return true;
}

//If the indicated hole is invalid or a pot, return false without changing anything. Otherwise, move all the beans in hole (s,hole) into the pot belonging to potOwner and return true.
bool Board::moveToPot(Side s, int hole, Side potOwner) {
    int* side = Board::side(s);
    
    if (hole < 0 || hole > m_nHoles) //checks that hole is positive and less than the number of holes assigned
        return false;
    else if (hole == POT) //if hole is the pot return false
        return false;
    if (potOwner == NORTH) //add hole amount to pot
        m_northPot += side[hole - 1];
    else
        m_southPot += side[hole - 1];
    side[hole - 1] = 0; //empty hole
    return true;
}

//If the indicated hole is invalid or beans is negative, this function returns false without changing anything. Otherwise, it will return true after setting the number of beans in the indicated hole or pot to the value of the third parameter. (This may change what beansInPlay and totalBeans return if they are called later.) This function exists solely so that we and you can more easily test your program: None of your code that implements the member functions of any class is allowed to call this function directly or indirectly. (We'll show an example of its use below.)
bool Board::setBeans(Side s, int hole, int beans) {
    int* side = Board::side(s);
    
    if (hole < 0 || hole > m_nHoles) //checks that hole is positive and less than the number of holes assigned
        return false;
    if (beans < 0) //checks that beans is positive
        return false;
    
    if (hole == POT) {//if hole is a pot, set pot to have the amount of beans given
        if (s == NORTH) {
            m_totalBeans -= m_northPot;
            m_northPot = beans;
        }
        else {
            m_totalBeans -= m_southPot;
            m_southPot = beans;
        }
    }
    else {
        m_totalBeans -= side[hole - 1];
        side[hole - 1] = beans; //set hole to have amount of beans given
    }
    m_totalBeans += beans;
    return true;
}


//PRIVATE MEMBER FUNCTIONS

int* Board::side(Side s) const { //given s, returns pointer to the start of m_north or m_south
    if (s == NORTH) {
        return m_north;
    }
    else {
        return m_south;
    }
}

int Board::sowSouth(int numBeans, int hole, Side& endSide, int& endHole, Side startingSide) {
    while (0 < numBeans && (hole - 1) < m_nHoles) { //while there are still beans and we're still adding to holes (not pot)
        m_south[hole - 1] ++; //add to hole
        numBeans --; //reduce bean amount
        hole++; //move to next hole
    }
    if (numBeans != 0 && startingSide == SOUTH) { //if still more beans and correct pot add beans to pot
        m_southPot ++; //add to pot
        numBeans --; //reduce bean amount
        endHole = 0;
    }
    else { //if no more beans, or not correct pot, set endSide to SOUTH and endHole to the last hole that beans went in
        endSide = SOUTH;
        endHole = hole - 1;
    }
    return numBeans; //return amount of beans
}

int Board::sowNorth(int numBeans, int hole, Side& endSide, int& endHole, Side startingSide) {
    while (0 < numBeans && hole > 0) { //while there are still beans and we're still adding to holes (not pot)
        m_north[hole - 1] ++; //add to hole
        numBeans --; //reduce bean amount
        hole --; //move to next hole
    }
    if (numBeans != 0 && startingSide == NORTH) { //if still more beans and correct pot add beans to pot
        m_northPot ++; //add to pot
        numBeans --; //reduce bean amount
        endHole = 0;
    }
    else { //if no more beans, or not correct pot, set endSide to NORTH and endHole to the last hole that beans went in
        endSide = NORTH;
        endHole = hole + 1;
    }
    return numBeans; //return amount of beans
}
