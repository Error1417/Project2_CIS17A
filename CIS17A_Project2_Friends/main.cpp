/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: Ismael Perez
 * Purpose:
 * 
 *
 * 
 * Created on December, 2022, 2:27 PM
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include "BlackJack.h"

using namespace std;

// Main function
int main() {
        int choice;
        //class objects
	Game g;
        Decks dks;
        Card c;
        
        Extra e;
        ExtraTwo ee;
        ExtraThree eee;
        
        Data d;
        FakePlayerInfo *fp=&d;
        
        DataTwo dd;
        
        dd.setAge(21);
        dd.setAdres("12345 Home St.");
        dd.setName("Joe");
        
        // dev class has access to all the private data in the program
        Dev dev;
        
        
	try {
		g.play();
	}
	catch(BadSuit e) {
		cout << "*** Bad card suit is given.\n";
		exit(1);
	}
	catch(BadNumberDecks e) {
		cout << "*** Bad number of decks is given.\n";
		exit(1);
	};
        
        //menu for concepts
        cout << endl;
        cout << endl;
        cout << "This part of the program is all the Extra stuff such as Inheritance, " << endl;
        cout << "Friends, Aggregation, protected members, and Abstract Classes." << endl;
        cout << "Enter in the number you want to see." <<endl;
        cout << "1) First Example of inheritance." << endl;
        cout << "2) Second Example of inheritance." << endl;
        cout << "3) Final Example of inheritance." << endl;
        cout << "4) Example of inheritance utilizing protected members." << endl;
        cout << "5) Example of abstract class." << endl;
        cout << "6) Example of Friend class." << endl;
        cin >> choice;
        
        switch(choice){
            case 1:
                //this examples of inheritance will always show no cards since this takes place after the game 
                // has already ended
                //example of inheritance 
                e.disclamer();
                //card will show no cards since this is called after the cards have been removed
                //from the player and dealer
                e.showHands();
                break;
                
            case 2:
                //second class to show private data
                ee.disclamer();
                //prints the cards in hand at this given time
                ee.print();
                break;
                
            case 3:
                //Last example of inheritance 
                eee.disclamer();
                // this prints out all the cards in the deck
                eee.print();
                break;
                
            case 4:
                //protected member example with a fake person created
                cout << endl;
                cout << "**Fake Player Data**" << endl;
                fp->setInfo("Bob",19);
                cout << d.dataAge();
                cout << endl;
                cout << d.dataName();
                break;
                
            case 5:
                // abstract class example
                cout << endl;
                cout << "**Fake Player Data with Abstract classes**" << endl;
                cout << dd.dataNameTwo();
                cout << endl;
                cout << dd.dataAdrsTwo();
                cout << endl;
                cout << dd.dataAgeTwo();
                break;
                
            case 6:
                cout << endl;
                cout << "**This is a example of the Friends concept.**" << endl;
                cout << "I decided to use it as a sort of developers tool " << endl;
                cout << "where you can see all the private data of any class you want." << endl;
                cout << endl;
                
                dev.displayGame(g);
                cout << endl;
                cout << endl;
                dev.displayDecks(dks);
                cout << endl;
                cout << endl;
                dev.displayCard(c);
                break;
                
            default:{
                cout << "You didn't enter any of the choices ;(" << endl;
                return 0;
            }
                
        }
	return 0;
}