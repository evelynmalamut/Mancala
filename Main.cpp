//
//  main.cpp
//  Project_3
//
//  Created by Evelyn Malamut on 5/13/19.
//  Copyright © 2019 Evelyn Malamut. All rights reserved.
//
#include "Board.h"
#include "Player.h"
#include "Side.h"
#include "Game.h"
#include <iostream>
#include <cassert>
#include <string>

using namespace std;

void Smallberg1() {
    HumanPlayer hp("Marge");
    BadPlayer bp("Homer");
    Board b(3, 2);
    Game g(b, &hp, &bp);
    g.play();
    
    cout << "Passed Smallberg1" << endl;
}

void Smallberg2() {
    BadPlayer bp1("Bart");
    BadPlayer bp2("Homer");
    Board b(3, 2);
    Game g(b, &bp1, &bp2);
    g.play();
    
    cout << "Passed Smallberg2" << endl;
}

void Smallberg3()
{
    Board b(3, 2);
    assert(b.holes() == 3  &&  b.totalBeans() == 12  &&
           b.beans(SOUTH, POT) == 0  &&  b.beansInPlay(SOUTH) == 6);
    b.setBeans(SOUTH, 1, 1);
    b.moveToPot(SOUTH, 2, SOUTH);
    assert(b.totalBeans() == 11  &&  b.beans(SOUTH, 1) == 1  &&
           b.beans(SOUTH, 2) == 0  &&  b.beans(SOUTH, POT) == 2  &&
           b.beansInPlay(SOUTH) == 3);
    Side es;
    int eh;
    b.sow(SOUTH, 3, es, eh);
    assert(es == NORTH  &&  eh == 3  &&  b.beans(SOUTH, 3) == 0  &&
           b.beans(NORTH, 3) == 3  &&  b.beans(SOUTH, POT) == 3  &&
           b.beansInPlay(SOUTH) == 1  &&  b.beansInPlay(NORTH) == 7);
    
    cout << "Passed Smallberg3" << endl;
}

void Smallberg4() {
    HumanPlayer hp("Marge");
    assert(hp.name() == "Marge"  &&  hp.isInteractive());
    BadPlayer bp("Homer");
    assert(bp.name() == "Homer"  &&  !bp.isInteractive());
    SmartPlayer sp("Lisa");
    assert(sp.name() == "Lisa"  &&  !sp.isInteractive());
    Board b(3, 2);
    b.setBeans(SOUTH, 2, 0);
    cout << "=========" << endl;
    int n = hp.chooseMove(b, SOUTH);
    cout << "=========" << endl;
    assert(n == 1  ||  n == 3);
    n = bp.chooseMove(b, SOUTH);
    assert(n == 1  ||  n == 3);
    n = sp.chooseMove(b, SOUTH);
    assert(n == 1  ||  n == 3);
    
    cout << "Passed Smallberg4" << endl;
}

void Smallberg5() {
    BadPlayer bp1("Bart");
    BadPlayer bp2("Homer");
    Board b(3, 0);
    b.setBeans(SOUTH, 1, 2);
    b.setBeans(NORTH, 2, 1);
    b.setBeans(NORTH, 3, 2);
    Game g(b, &bp1, &bp2);
    bool over;
    bool hasWinner;
    Side winner;
    //    Homer
    //   0  1  2
    // 0         0
    //   2  0  0
    //    Bart
    g.status(over, hasWinner, winner);
    assert(!over && g.beans(NORTH, POT) == 0 && g.beans(SOUTH, POT) == 0 &&
           g.beans(NORTH, 1) == 0 && g.beans(NORTH, 2) == 1 && g.beans(NORTH, 3) == 2 &&
           g.beans(SOUTH, 1) == 2 && g.beans(SOUTH, 2) == 0 && g.beans(SOUTH, 3) == 0);
    
    g.move();
    //   0  1  0
    // 0         3
    //   0  1  0
    g.status(over, hasWinner, winner);
    assert(!over && g.beans(NORTH, POT) == 0 && g.beans(SOUTH, POT) == 3 &&
           g.beans(NORTH, 1) == 0 && g.beans(NORTH, 2) == 1 && g.beans(NORTH, 3) == 0 &&
           g.beans(SOUTH, 1) == 0 && g.beans(SOUTH, 2) == 1 && g.beans(SOUTH, 3) == 0);
    
    g.move();
    //   1  0  0
    // 0         3
    //   0  1  0
    g.status(over, hasWinner, winner);
    assert(!over && g.beans(NORTH, POT) == 0 && g.beans(SOUTH, POT) == 3 &&
           g.beans(NORTH, 1) == 1 && g.beans(NORTH, 2) == 0 && g.beans(NORTH, 3) == 0 &&
           g.beans(SOUTH, 1) == 0 && g.beans(SOUTH, 2) == 1 && g.beans(SOUTH, 3) == 0);
    
    g.move();
    //   1  0  0
    // 0         3
    //   0  0  1
    g.status(over, hasWinner, winner);
    assert(!over && g.beans(NORTH, POT) == 0 && g.beans(SOUTH, POT) == 3 &&
           g.beans(NORTH, 1) == 1 && g.beans(NORTH, 2) == 0 && g.beans(NORTH, 3) == 0 &&
           g.beans(SOUTH, 1) == 0 && g.beans(SOUTH, 2) == 0 && g.beans(SOUTH, 3) == 1);
    
    g.move();
    //   0  0  0
    // 1         4
    //   0  0  0
    g.status(over, hasWinner, winner);
    assert(over && g.beans(NORTH, POT) == 1 && g.beans(SOUTH, POT) == 4 &&
           g.beans(NORTH, 1) == 0 && g.beans(NORTH, 2) == 0 && g.beans(NORTH, 3) == 0 &&
           g.beans(SOUTH, 1) == 0 && g.beans(SOUTH, 2) == 0 && g.beans(SOUTH, 3) == 0);
    assert(hasWinner && winner == SOUTH);
    
    cout << "Passed Smallberg5" << endl;
}


void doBoardTests()
{
    Board e(0,-1); //test empty board (1 hole no beans)
    assert(e.holes() == 1); //tests constructor default holes to 1
    assert(e.totalBeans() == 0); //tests negative value constructor totalBeans
    assert(e.beans(SOUTH, POT) == 0); //test beans with empty pot
    assert(e.beans(SOUTH, 1) == 0); //test beans with empty hole
    assert(e.beansInPlay(SOUTH) == 0); //test beans in play w/ no beans
    
    Board e1(-1,3); //test 1 hole with beans
    assert(e1.holes() == 1); //tests constructor default holes to 1
    assert(e1.totalBeans() == 6); //tests totalBeans
    assert(e1.beans(SOUTH, POT) == 0); //test beans with empty pot
    assert(e1.beansInPlay(SOUTH) == 3); //test beans in play w/ beans
    
    assert(e1.beans(SOUTH, 7) == -1); //test beans with out of bounds
    assert(e1.beans(SOUTH, -3) == -1); //test beans with negative hole given
    
    Board b(3,2); //test board with multiple beans and holes
    assert(b.holes() == 3); //test holes with multiple holes
    assert(b.totalBeans() == 12); //test beans with multiple beans and holes
    assert(b.beans(SOUTH, POT) == 0); //test beans with empty pot
    assert(b.beansInPlay(SOUTH) == 6 && b.beansInPlay(NORTH) == 6); //tests beans in play (at starting North and South should be equal
    
    Board c(3,2); //test copy constructor
    assert(c.holes() == 3); //test holes with multiple holes
    assert(c.totalBeans() == 12); //test beans with multiple beans and holes
    assert(c.beans(SOUTH, POT) == 0); //test beans with empty pot
    assert(c.beansInPlay(SOUTH) == 6 && c.beansInPlay(NORTH) == 6); //tests beans in play (at starting North and South should be equal
    
    e = c; //test assignment operator
    assert(e.holes() == 3); //test holes with multiple holes
    assert(e.totalBeans() == 12); //test beans with multiple beans and holes
    assert(e.beans(SOUTH, POT) == 0); //test beans with empty pot
    assert(e.beansInPlay(SOUTH) == 6 && e.beansInPlay(NORTH) == 6); //tests beans in play (at starting North and South should be equal
    
    assert(!b.setBeans(SOUTH, 5, 2)); //test setBeans out of range hole
    assert(!b.setBeans(SOUTH, -1, 2)); //test setBeans with negative hole
    assert(!b.setBeans(SOUTH, 1, -4)); //test setBeans with negative beans
    
    assert(b.setBeans(SOUTH, 1, 1)); //test setBeans with valid hole and bean number
    assert(b.totalBeans() == 11); //test totalBeans after setBeans
    assert(b.beansInPlay(SOUTH) == 5); //test beansInPlay after setBeans
    assert(b.beans(SOUTH, 1) == 1); //test beans after setBeans
    
    
    assert(!b.moveToPot(SOUTH, -2, SOUTH)); //test moveToPot with negative hole
    assert(!b.moveToPot(SOUTH, 7, SOUTH)); //test moveToPot with out of range hole
    assert(!b.moveToPot(SOUTH, 0, SOUTH)); //test moveToPot with pot
    
    assert(b.moveToPot(SOUTH, 2, SOUTH)); //test moveToPot with valid arguments
    assert(b.totalBeans() == 11); //test totalBeans after moveToPot (should stay the same)
    assert(b.beans(SOUTH, 2) == 0); //test beans with hole after moveToPot (shold be empty)
    assert(b.beans(SOUTH, POT) == 2); //test beans with pot after moveToPot (should have amount of beans from hole)
    assert(b.beansInPlay(SOUTH) == 3); //test beansInPlay after moveToPot (should be less))

    
    Side es;
    int eh;
    
    assert(!b.sow(SOUTH, 7, es, eh)); //test sow with out of bounds
    assert(!b.sow(SOUTH, -7, es, eh)); //test sow with negative
    assert(!b.sow(SOUTH, 0, es, eh)); //test sow with pot
    assert(!b.sow(SOUTH, 2, es, eh)); //test sow with empty hole
    
    assert(b.sow(SOUTH, 3, es, eh)); //test sow with valid inputs
    assert(es == NORTH); //checks that it made it to other side
    assert(eh == 3); //checks last hole
    assert(b.beans(SOUTH, 3) == 0); //empty because removed beans
    assert(b.beans(NORTH, 3) == 3); //added bean
    assert(b.beans(SOUTH, POT) == 3); //pot increased
    assert(b.beansInPlay(SOUTH) == 1); //only 1 bean on south side
    assert(b.beansInPlay(NORTH) == 7); //7 beans on North side
    
    b.sow(NORTH, 3, es, eh); //test sow on North side
    assert(es == NORTH); //checks that it made it to other side
    assert(eh == 0); //checks last hole
    assert(b.beans(SOUTH, 3) == 0); //empty because removed beans
    assert(b.beans(NORTH, 3) == 0); //added bean
    assert(b.beans(SOUTH, POT) == 3); //pot stayed the same
    assert(b.beans(NORTH, POT) == 1); //pot increased
    assert(b.beansInPlay(SOUTH) == 1); //only 1 bean on south side
    assert(b.beansInPlay(NORTH) == 6); //7 beans on North side
    
    b.setBeans(SOUTH,2,10);
    
    b.sow(SOUTH, 2, es, eh); //test sow going around the board multiple times starting SOUTH
    assert(es == NORTH);
    assert(eh == 3);
    assert(b.totalBeans() == 21); //test totalBeans increase
    //testing each hole
    assert(b.beans(SOUTH, 1) == 2);
    assert(b.beans(SOUTH, 2) == 1);
    assert(b.beans(SOUTH, 3) == 2);
    
    assert(b.beans(NORTH, 1) == 4);
    assert(b.beans(NORTH, 2) == 4);
    assert(b.beans(NORTH, 3) == 2);
    
    assert(b.beans(NORTH, POT) == 1); //stayed the same
    assert(b.beans(SOUTH, POT) == 5); //pot increased
    
    b.setBeans(NORTH,2,10);
    
    b.sow(NORTH, 2, es, eh); //test sow going around the board multiple times starting NORTH
    assert(es == SOUTH);
    assert(eh == 1);
    assert(b.totalBeans() == 27); //test totalBeans increase
    //testing each hole
    assert(b.beans(SOUTH, 1) == 4);
    assert(b.beans(SOUTH, 2) == 2);
    assert(b.beans(SOUTH, 3) == 3);
    
    assert(b.beans(NORTH, 1) == 6);
    assert(b.beans(NORTH, 2) == 1);
    assert(b.beans(NORTH, 3) == 3);
    
    assert(b.beans(NORTH, POT) == 3); //pot increased
    assert(b.beans(SOUTH, POT) == 5); //stayed the same
    
    assert(b.moveToPot(SOUTH, 2, NORTH)); //test moveToPot with valid arguments (opposite sides)
    assert(b.beans(SOUTH, 2) == 0); //test beans with hole after moveToPot (shold be empty)
    assert(b.beans(SOUTH, POT) == 5); //test beans with pot after moveToPot (should have amount of beans from hole)
    assert(b.beans(NORTH, POT) == 5); //test beans with pot after moveToPot (should have amount of beans from hole)
    assert(b.beansInPlay(SOUTH) == 7); //test beansInPlay after moveToPot (should be less))
    assert(b.beansInPlay(NORTH) == 10); //test beansInPlay after moveToPot (should be less))
    
    cout << "Passed all Board Tests" << endl;
}

void doPlayerTests() {
    Player* player[3]; //create array of pointers to class
    player[0] = new BadPlayer("penny"); //test inheritance (superclass pointer can point to subclass)
    player[1] = new HumanPlayer("george");
    player[2] = new SmartPlayer("smartie pants");
    
    assert(player[0]->name() == "penny"); //test inheritance of name member function
    assert(player[1]->name() == "george");
    assert(player[2]->name() == "smartie pants");
    
    assert(!player[0]->isInteractive()); //test isInteractive virtual function
    assert(player[1]->isInteractive());
    assert(!player[2]->isInteractive());
    
    Board b(3,2); //test chooseMove (BadPlayer will pick leftmost, non-empty hole)
    assert(player[0]->chooseMove(b, SOUTH) == 1);
    
    b.setBeans(SOUTH, 1, 0);
    b.setBeans(SOUTH, 2, 0);
    b.setBeans(SOUTH, 3, 0);
    //shouldn't be able to pick 2
    assert(player[0]->chooseMove(b,SOUTH) == -1);
    
    for (int k = 0; k < 3; k++)
        delete player[k];
    
    cout << "Passed all Player test" << endl;
}

void doGameTests() {
    BadPlayer p1("bad");
    BadPlayer p2("alsoBad");
    Board b(3,1);
    bool hasWinner = false;
    bool isOver = false;
    Side winner = NORTH;
    
    Game g(b,&p1,&p2); //test constructor
    assert(g.beans(NORTH, 7) == -1); //invalid hole #
    
    assert(g.beans(NORTH, 1) == 1); //test beans
    assert(g.beans(NORTH, 2) == 1);
    assert(g.beans(NORTH, 3) == 1);
    
    assert(g.beans(SOUTH, 1) == 1);
    assert(g.beans(SOUTH, 2) == 1);
    assert(g.beans(SOUTH, 3) == 1);
    g.display(); //test display
    /*
    South player: bad  North player: alsoBad
                North
                1 1 1
     North pot 0     0 South Pot
                1 1 1
                South
    */
    g.status(isOver,hasWinner,winner);
    assert(isOver == false && hasWinner == false); //test status when game is not over
    
    g.move(); //testing move, SOUTH is first so it shoud move
    assert(g.beans(NORTH, 1) == 1); //test beans
    assert(g.beans(NORTH, 2) == 1);
    assert(g.beans(NORTH, 3) == 1);
    assert(g.beans(NORTH, 0) == 0);
    
    assert(g.beans(SOUTH, 1) == 0);
    assert(g.beans(SOUTH, 2) == 2);
    assert(g.beans(SOUTH, 3) == 1);
    assert(g.beans(SOUTH, 0) == 0);
    
    g.move(); //currentPlayer should switch to NORTH, which will move twic, b/c it lands in a hole the first time
    assert(g.beans(NORTH, 1) == 1); //test beans
    assert(g.beans(NORTH, 2) == 0);
    assert(g.beans(NORTH, 3) == 1);
    assert(g.beans(NORTH, 0) == 1);
    
    assert(g.beans(SOUTH, 1) == 0);
    assert(g.beans(SOUTH, 2) == 2);
    assert(g.beans(SOUTH, 3) == 1);
    assert(g.beans(SOUTH, 0) == 0);
    
    g.play();
    //should be prompting me to press enter to make sure that I keeo up with the game
    /*
    South player: bad  North player: alsoBad
                North
                1 0 1
    North pot 1      0 South Pot
                0 2 1
                South
    
    South player: bad  North player: alsoBad
                North
                1 0 2
    North pot 1      2 South Pot
                0 0 0
                South
     
     South player: bad  North player: alsoBad
                North
                0 0 0
     North pot 4      2 South Pot
                0 0 0
                South
    */
    assert(g.beans(NORTH, 1) == 0);
    assert(g.beans(NORTH, 2) == 0);
    assert(g.beans(NORTH, 3) == 0);
    assert(g.beans(NORTH, 0) == 4);
    
    assert(g.beans(SOUTH, 1) == 0);
    assert(g.beans(SOUTH, 2) == 0);
    assert(g.beans(SOUTH, 3) == 0);
    assert(g.beans(SOUTH, 0) == 2);
    g.display(); //test display
    g.status(isOver,hasWinner,winner);
    assert(g.move() == false);
    assert(isOver == true && hasWinner == true && winner == NORTH); //test status when game is over
    
    g.play(); //since nothing to play in SOUTH side, all beans should be swept in the NORTH side and game is over
    assert(g.beans(NORTH, 1) == 0); //test beans
    assert(g.beans(NORTH, 2) == 0);
    assert(g.beans(NORTH, 3) == 0);
    assert(g.beans(NORTH, 0) == 4);
    
    assert(g.beans(SOUTH, 1) == 0);
    assert(g.beans(SOUTH, 2) == 0);
    assert(g.beans(SOUTH, 3) == 0);
    assert(g.beans(SOUTH, 0) == 2);
    g.display(); //test display
    g.status(isOver,hasWinner,winner);
    assert(isOver == true && hasWinner == true && winner == NORTH);
    
    cout << "Passed all Game Tests" << endl;
    
}

void doSmartPlayerTests() {
    SmartPlayer s("smartie pants");
    HumanPlayer e("evelyn");

    bool hasWinner = false;
    bool isOver = false;
    Side winner = NORTH;
    
    //GAME 1
    //should choose 1, b/c will deposit in pot, then choose 2 deposit in pot, then choose 1 deposit in pot..etc until all beans on that side are in the pot
    Board b1(3, 1);
    b1.setBeans(NORTH, 1, 1);
    b1.setBeans(NORTH, 2, 0);
    b1.setBeans(NORTH, 3, 0);
    
    b1.setBeans(SOUTH, 1, 0);
    b1.setBeans(SOUTH, 2, 2);
    b1.setBeans(SOUTH, 3, 1);
    
    /*
      1 0 0
     0     0
      0 2 1
     */
 
    Game g1(b1, &s, &e);
    assert(s.chooseMove(b1, SOUTH) == 3);
    g1.play();
    assert(g1.beans(SOUTH, 0) == 3);
    g1.status(isOver, hasWinner, winner);
    assert(isOver == true && hasWinner == true && winner == SOUTH);
    
    //GAME 2
    //should choose 2, b/c will capture the 1 on the other side, and end game since the holes on the north side will be empty
    Board b2(3, 1);
    b2.setBeans(NORTH, 1, 0);
    b2.setBeans(NORTH, 2, 0);
    b2.setBeans(NORTH, 3, 1);
    
    b2.setBeans(SOUTH, 1, 1);
    b2.setBeans(SOUTH, 2, 1);
    b2.setBeans(SOUTH, 3, 0);
    
    /*
      0 0 1
     0     0
      1 1 0
     */
    Game g2(b2, &s, &e);
    assert(s.chooseMove(b2, SOUTH) == 2);
    g2.play();
    assert(g2.beans(SOUTH, 0) == 3);
    g2.status(isOver, hasWinner, winner);
    assert(isOver == true && hasWinner == true && winner == SOUTH);

    //GAME 3
    //should pick 1 so that it can capture 2 and win the game
    Board b3(4,1);
    b3.setBeans(NORTH, 1, 0);
    b3.setBeans(NORTH, 2, 2);
    b3.setBeans(NORTH, 3, 0);
    b3.setBeans(NORTH, 4, 0);
    
    b3.setBeans(SOUTH, 1, 1);
    b3.setBeans(SOUTH, 2, 0);
    b3.setBeans(SOUTH, 3, 1);
    b3.setBeans(SOUTH, 4, 0);
    
    /*
      0 2 0 0
     0       0
      1 0 1 0
     */
    Game g3(b3, &s, &e);
    assert(s.chooseMove(b3, SOUTH) == 1);
    g3.play();
    assert(g3.beans(SOUTH, 0) == 4);
    g3.status(isOver, hasWinner, winner);
    assert(isOver == true && hasWinner == true && winner == SOUTH);
    
    //GAME 4
    //should pick 2 so that it can choose move again
    Board b4(4,1);
    b4.setBeans(NORTH, 1, 0);
    b4.setBeans(NORTH, 2, 2);
    b4.setBeans(NORTH, 3, 0);
    b4.setBeans(NORTH, 4, 0);
    
    b4.setBeans(SOUTH, 1, 0);
    b4.setBeans(SOUTH, 2, 1);
    b4.setBeans(SOUTH, 3, 2);
    b4.setBeans(SOUTH, 4, 0);
    
    /*
      0 2 0 0
     0       0
      0 1 2 0
     */
    Game g4(b4, &s, &e);
    assert(s.chooseMove(b4, SOUTH) == 3);
    g4.play();
    assert(g4.beans(SOUTH, 0) == 3);
    g4.status(isOver, hasWinner, winner);
    //after playing my turn (pick 2 and then pick one and tie)
    assert(isOver == true && hasWinner == true && winner == SOUTH); //after sweep south wins
    
    //GAME 5
    //empty board
    Board b5(4,1);
    b5.setBeans(NORTH, 1, 0);
    b5.setBeans(NORTH, 2, 0);
    b5.setBeans(NORTH, 3, 0);
    b5.setBeans(NORTH, 4, 0);
    
    b5.setBeans(SOUTH, 1, 0);
    b5.setBeans(SOUTH, 2, 0);
    b5.setBeans(SOUTH, 3, 0);
    b5.setBeans(SOUTH, 4, 0);
    
    /*
      0 0 0 0
     0       0
      0 0 0 0
     */
    Game g5(b5, &s, &e);
    assert(s.chooseMove(b5, SOUTH) == -1);
    g5.status(isOver, hasWinner, winner);
    assert(isOver == true && hasWinner == false); //it's a tie
    
    //GAME 6
    //should capture other side's hole
    Board b6(5,1);
    b6.setBeans(NORTH, 1, 0);
    b6.setBeans(NORTH, 2, 0);
    b6.setBeans(NORTH, 3, 1);
    b6.setBeans(NORTH, 4, 0);
    b6.setBeans(NORTH, 5, 0);
    
    b6.setBeans(SOUTH, 1, 0);
    b6.setBeans(SOUTH, 2, 1);
    b6.setBeans(SOUTH, 3, 0);
    b6.setBeans(SOUTH, 4, 1);
    b6.setBeans(SOUTH, 5, 0);
    
    /*
      0 0 1 0 0
     0         0
      0 1 0 1 0
     */
    Game g6(b6, &s, &e);
    assert(s.chooseMove(b6, SOUTH) == 2);
    g6.play();
    assert(g6.beans(SOUTH, 0) == 3);
    g6.status(isOver, hasWinner, winner);
    assert(isOver == true && hasWinner == true && winner == SOUTH); //wins b/c capture
    
    //GAME 7
    //should pick hole that will let them go again
    Board b7(5,1);
    b7.setBeans(NORTH, 1, 0);
    b7.setBeans(NORTH, 2, 0);
    b7.setBeans(NORTH, 3, 1);
    b7.setBeans(NORTH, 4, 0);
    b7.setBeans(NORTH, 5, 0);
    
    b7.setBeans(SOUTH, 1, 0);
    b7.setBeans(SOUTH, 2, 1);
    b7.setBeans(SOUTH, 3, 0);
    b7.setBeans(SOUTH, 4, 1);
    b7.setBeans(SOUTH, 5, 0);
    
    /*
      0 0 1 0 0
     0         0
      0 4 0 1 0
     */
    Game g7(b7, &s, &e);
    assert(s.chooseMove(b7, SOUTH) == 2);
    
    Board b8(6,4);
    Game g8(b8,&s,&e);
    g8.play();

    cout << "Passed all Smart Player Tests" << endl;
}

int main()
{
    doBoardTests();
    doPlayerTests();
    doGameTests();
    doSmartPlayerTests();
    Smallberg1();
    Smallberg2();
    Smallberg3();
    Smallberg4();
    Smallberg5();
    cout << "Passed all tests" << endl;
}
