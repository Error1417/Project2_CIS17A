/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   BlackJack.h
 * Author: error
 *
 * Created on December 7, 2022, 9:05 PM
 */

#ifndef BLACKJACK_H
#define BLACKJACK_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>

using namespace std;

// structs for error exceptions.
struct BadSuit {};
struct BadNumberDecks {};

// Some global constants.
const int PLAYER_CHIP = 100;	// Initial number of player's chips.
const int DEALER_CHIP = 10000;	// Initial number of dealer's chips.
const int MAX_BET = 5;		// Maximum number of chips for a bet in a round.
const int MAX_CHARACTER = 1000;	// Maximum number of characters in a line
				// 	to be used for standard input.


// Class that represents a card. It needs a value (rank) and a suit.
class Card {
    private:
        int num;	// The value (rank) of the card.
        char suit;	// The suit of the card (c, s, h, d).
    public:
        // Constructor.
        // num should be in the range of 1<= num <=13 
        // 	(1: Ace, 11:J, 12:Q, 13:K),
        // suits: 'c' (club), 's' (spade), 'h' (heart), 'd' (diamond).
        Card(int num1 = 1, char suit1 = 's') {
            // Assigning the rank of the card using a number.
            if (num1 < 1) num = 1;
            else if (num1 > 13) num = 13;
            else num=num1 ;
            // Assigning the suit of the card.
            if (suit1 == 'c' || suit1 == 'C') suit = 'c';
            else if (suit1 == 's' || suit1 == 'S') suit = 's';
            else if (suit1 == 'h' || suit1 == 'H') suit = 'h';
            else if (suit1 == 'd' || suit1 == 'D') suit = 'd';
            else throw BadSuit(); // Error exception for bad input.
        }

        // Get the value of the card (1~13).
        int getValue() const {
                return num;
        }

        // Get the rank of the card (A, 2,..., 10, J, Q, K) as strings.
        string getRank() const {
            if (num == 1) return "A";
            else if (num == 2) return "2";
            else if (num == 3) return "3";
            else if (num == 4) return "4";
            else if (num == 5) return "5";
            else if (num == 6) return "6";
            else if (num == 7) return "7";
            else if (num == 8) return "8";
            else if (num == 9) return "9";
            else if (num == 10) return "10";
            else if (num == 11) return "J";
            else if (num == 12) return "Q";
            else if (num == 13) return "K";
        }

        // Get the suit of the given card.
        char getSuit() const {
            return suit;
        }
        
        friend class Dev;// example of friend class
};		

// outstream operator overloading for the Card class.
ostream & operator<<(ostream &s, const Card &c) {
    return s << c.getRank() << "(" << c.getSuit() << ')';
}

// Class that represents the decks of cards for the game of blackjack.
// Number of decks allowed here are 1, 2, and 4.		
class Decks {
    private:
        vector<Card> cards;// Array for all cards.
        int current;		// The pointer for the current card.
    public:
        // Constructor. The number of decks should be given (default=1).
        Decks(int nDeck = 1): cards(), current(0) {
            create(nDeck); 	// create all cards.
        }

        // Create all cards of given number of decks.
        void create(int nDeck) {
            if (nDeck != 1 && nDeck != 2 && nDeck != 4) 
                throw BadNumberDecks();
            cards.resize(nDeck*52); // size of the array resized. 
            int index = 0;
            for (int i = 0; i < nDeck; i++)
                // Creating and storing cards for nDeck decks.
                for (int n = 1; n <= 13; n++) { 
                    cards[index++] = Card(n, 'c');
                    cards[index++] = Card(n, 's');
                    cards[index++] = Card(n, 'h');
                    cards[index++] = Card(n, 'd');
                }
        }

        // Randomly shuffle all cards in the deck and set current as 0.
        void shuffle() {
            // using the standard library.
            random_shuffle(cards.begin(), cards.end()); 
            current = 0;
        }

        // Deal a card that are pointed by current and add 1 to current.
        Card deal() {
            // If all cards are dealt, shuffle cards again.
            if (current == cards.size()) shuffle();
            return cards[current++];
        }


        // Print all cards at the current shuffled state.
        void print() const {
            for (int i = 0; i < cards.size(); i++){ 
                cout << cards[i] << endl;
            }
        }
        
        friend class Dev;// example of friend class
};

// Class that represents a hand of a player or a dealer.
class Hand {
    private:
        vector<Card> crdsAtHnd;	// Array of cards in a hand.
    public:
        // Constructor.
        Hand(): crdsAtHnd() {}
        
        //function isn't called because it causes come type of error in the code
        //so I destroy the array of cards in another function
        //this is just here for demonstration purposes
        ~Hand(){
            crdsAtHnd.clear();
        }

        // Add a card to the hand.
        void addCard(Card card) {
            crdsAtHnd.push_back(card);
        }

        int getValue() const {
            int value = 0;
            // true if an ace exists in the hand.
            bool ifAce = false;  
            for (int i = 0; i < crdsAtHnd.size(); i++) {
                int cardValue = crdsAtHnd[i].getValue();
                // Give 10 for all face cards (J, Q, K).
                if (cardValue > 10) cardValue = 10; 	
                if (cardValue == 1) ifAce=true;
                value += cardValue;
            }
            // Ace is always counted as 11 if doing so dose not 
            // make the hand bust (S17 rule for dealer's hand).
            if (ifAce && value < 12) value += 10;	
            return value;
        }

        // Return the number of cards of the hand.
        int size() const {
            return crdsAtHnd.size();
        }

        // Returns true if the hand is "blackjack" 
        // (getting the value 21 with 2 cards).
        bool blackjack() const {
            return (crdsAtHnd.size() == 2 && getValue() == 21);
        }

        // destructor for my vector
        void removeAllCards() {
            crdsAtHnd.clear();
        }

        // print the cards in the hand.
        // (if hideFirst==true, the first card will not be shown).
        void print(bool hideFirst=false) const {
            if (crdsAtHnd.size() == 0) 
                    cout << "No card." << endl;
            for (int i = 0; i < crdsAtHnd.size(); i++) 
                if (i == 0 && hideFirst) cout << "?(?) ";
                else cout << crdsAtHnd[i] << ' ';
            cout << endl;
        }
}; 

/* Class that represents the game (displaying texts using standard output, 
* 	and managing the flow of the game using 5 stages).
* User inputs are obtained also using standard input, and only the first
* 	character (excluding white spaces) of an input (a line ending with \n)
* 	is assumed to be the only user input to reduce confusions.
*/
class Game {
    private:
        Decks myDecks;	// All cards for the game.
        Hand playerHand, 
             dealerHand; // Hands for the game.
        int nPlayerChip, 
            nDealerChip; // Numbers of chips for players.
        int nBet; 	// Bet of the current round.
        char temp[MAX_CHARACTER+1]; // C-string for temporary storages.
    public:
        // Constructor.
        Game(): myDecks(), 
            playerHand(), 
            dealerHand() {
            nPlayerChip = PLAYER_CHIP;
            nDealerChip = DEALER_CHIP;
        }

        // Manage the flow of the game.
        void play() {
            // Setting the random seed.
            srand(static_cast<unsigned int>(time(0))); 
            // Starting the game
            char input = beginGame();
            while (input == 'n') {
                beginRound(); // beginning of round
                do 
                {
                    input = inRound();
                } while (input == 'h');
                input = endRound(input);
            }
            endGame();
        }

        // Beginning of the game (stage 1).
        // Returns the input (n, r, q).
        // (n: next round, r: rule, q: quit.)
        char beginGame() {
                cout << "----------------------------" << endl;
                cout << "|  Welcome  to  Blackjack  |" << endl;
                cout << "----------------------------" << endl;
                cout << endl;
                
                int nDeck = 1;
                myDecks.create(nDeck); // Creating the decks of cards.
                cout << nDeck << " deck (52 cards) has";
                cout << " been created and shuffled." << endl;
                cout << "Your balance is: " << PLAYER_CHIP;
                cout << " chips now, and you can bet";
                cout << " upto " << MAX_BET;
                cout << " chips for each round." << endl << endl;

                char input; // For user input.
                do { // (will be repeated until a right input is given).
                    cout << "Type n for a new round, r for rules, ";
                    cout << "and q to quit [n/r/q] (default: n): ";
                    cin.getline(temp, MAX_CHARACTER);
                    // extract the first char.
                    input = firstChar(temp, MAX_CHARACTER); 
                    if (input == '0') input = 'n';
                    if (input == 'r') displayRules();
                } while (input != 'n' && input != 'q');
                cout << endl;
                return input;
        }

        // Beginning of a round (stage 2)
        // Returns the input (amount of bet; 1-5).
        void beginRound() {
                cout << "===================================" << endl;
                cout << "Starting a New Round (your chips: ";
                cout << nPlayerChip << ")." << endl << endl;

                bool noChips = false; 
                        // Used when checking (remaining chips) < nBet.
                do { // (will be repeated until a right input is given).
                    if (noChips) {
                            cout << "You only have " << nPlayerChip;
                            cout << " chip(s)." << endl; 
                    };
                    cout << "How many chips do you want to bet? ";
                    cout << "[1-5] (default: 1): ";
                    noChips = false; // Set it for a new input.

                    // Read a line and store in temp.
                    cin.getline(temp, MAX_CHARACTER); 
                    // extract a number from the first char.
                    nBet = firstChar(temp, MAX_CHARACTER) - '0';
                    if (nBet == 0) nBet = 1; // Default is 1.
                    // Check if chips are sufficient.
                    if (nBet > nPlayerChip || nBet > nDealerChip) 
                                noChips = true;
                } while (nBet < 1 || nBet > MAX_BET || noChips);
                cout << "You bet " << nBet << " chip";
                cout << (nBet == 1 ? "." : "s.") << endl;

                dealerHand.removeAllCards(); // return all cards
                playerHand.removeAllCards(); // return all cards
                myDecks.shuffle(); // Shuffle the cards.

                // Dealing two cards to each player.
                dealerHand.addCard(myDecks.deal());
                playerHand.addCard(myDecks.deal());
                dealerHand.addCard(myDecks.deal());
                playerHand.addCard(myDecks.deal());
        }

        char inRound() {
                int playerValue = playerHand.getValue();
                // Check for the blackjack.
                // Then stop the round automatically, and return 'j'
                if (playerHand.blackjack()) {
                        cout << "You got the blackjack!" << endl;
                        return 'j'; 
                };

                // Check for the player busting 
                // (if busted, stop the round and return 'b').
                if (playerValue > 21) return 'b'; 

                // Show hands (with dealer's first card hidden).
                showHands(true);	

                char input; // user input in this stage.
                do { // (will be repeated until a right input is given).
                        cout << "Type h for Hit, s for Stand, r for ";
                        cout << "rules, q to quit [h/s/r/q] ";
                        cout << "(default: h): ";
                        cin.getline(temp, MAX_CHARACTER);
                        // extract the first char.
                        input = firstChar(temp, MAX_CHARACTER);
                        // Default is h(Hit).
                        if (input == '0') input = 'h'; 
                        if (input == 'r') displayRules(); // for rules.
                } while (input != 'h' && input != 's' && input != 'q');

                // Add a card for "Hit".
                if (input == 'h') playerHand.addCard(myDecks.deal()); 
                return input;
        }

        // End of a round (stage 4)
        // (busted=true if the player got busted, false if not).
        char endRound(char in) {
                int playerValue = playerHand.getValue();
                if (in == 'b') {	// if the player is busted.
                    cout << "\nYou have:\t "; 
                    playerHand.print();
                    cout << "You got busted, and lost ";
                    cout << nBet << " chips." << endl;
                    takeChips(-nBet);
                }
                else if (in == 'q') { // if the player quits the game.
                    cout << "\nYou lost " << nBet << " chips.";
                    cout << endl;
                    takeChips(-nBet);
                    return in;
                }
                else if (in == 'j') { // if player gets the blackjack.
                        showHands(); // Show cards.
                        if (dealerHand.blackjack()) {
                            cout << "It is tied, and the bet is ";
                            cout << "returned." << endl;
                        }
                        else {
                            cout << "You won, and gained ";
                            cout << nBet << " chips." << endl;
                            takeChips(nBet);
                        };
                }
                else { 
                    while (dealerHand.getValue() < 17) 
                            dealerHand.addCard(myDecks.deal());
                    showHands(); // Show cards.

                    int dealerValue = dealerHand.getValue();
                    // If the dealer gets busted.
                    if (dealerValue > 21) { 
                        cout << "Dealer got busted, and you ";
                        cout << "gained " << nBet;
                        cout << " chips." << endl;
                        takeChips(nBet);
                    }
                    // Dealer wins.
                    else if (dealerValue > playerValue) {
                        if (dealerHand.blackjack()) {
                            cout << "Dealer got the ";
                            cout << "blackjack!\n";
                        }
                        cout << "Dealer won, and you lost ";
                        cout << nBet << " chips." << endl;
                        takeChips(-nBet);
                    } 
                    // Player wins.
                    else if (dealerValue < playerValue) {
                        cout << "You won, and you gained ";
                        cout << nBet << " chips." << endl;
                        takeChips(nBet);
                    }
                    // A tie.
                    else { 
                        // If the dealer has the blackjack and
                        // the score is tied, the dealer wins.
                        if (dealerHand.blackjack()) {
                            cout << "Dealer got the ";
                            cout << "blackjack! ";
                            cout << "You lost " << nBet;
                            cout << " chips." << endl;
                            takeChips(-nBet);
                        }
                        // the score is tied, and the dealer
                        // doesn't have blackjack.
                        else {	
                            cout << "It is tied, and the ";
                            cout << "bet is returned.";
                            cout << endl;
                        }
                    }

                }
                cout << "\n* End of the Round (your chips: ";
                cout << nPlayerChip << ")." << endl;
                cout << "===================================" << endl;

                // If all chips are used up, the game ends.
                if (nPlayerChip == 0 || nDealerChip == 0) return 'q'; 

                char input; // character for the user input.
                do { // (will be repeated until a right input is given).
                    cout << endl<< "Type n for a new round, r for rules";
                    cout << ", q to quit [n/r/q] (default: n): ";
                    cin.getline(temp, MAX_CHARACTER);
                    // extract the first char.
                    input = firstChar(temp, MAX_CHARACTER);
                    // Default is n(new round).
                    if (input == '0') input = 'n'; 
                    if (input == 'r') displayRules(); // Rules.
                } while (input != 'n' && input != 'q');
                return input;
        }

        void endGame() {
            cout << "\nYour remaining chips: " << nPlayerChip;
            cout << " (you ";
            int diff = nPlayerChip - PLAYER_CHIP;
            if (diff > 0) cout << "gained " << diff << " chips).";
            else if (diff < 0) 
                    cout << "lost " << -diff << " chips).";
            else cout << "have the same number of chips as started).";
            cout << endl << endl;
            cout << "---------------------------" << endl;
            cout << "     End of the Game     |" << endl;
            cout << "---------------------------" << endl;
        }

        // Display rules.
        void displayRules() {
            cout << endl;
            cout << "===================================" << endl;
            cout << "Bet a min of 1 chip and a max of 5 chips, win up to twice your "<<
                    "original bet.\nGet 21 to win, or get a higher card value than " <<
                    "the dealer.\nYou can only lose 100% of your original bet, but " <<
                    "you can up to 200% your original bet."<< endl;
            cout << "===================================" << endl;
            cout << endl;
        }

        // A function that extracts a first character from a c-string 
        // (returns '0' if none exists).
        // temp: character array, size: size of temp.
        static char firstChar(char *temp, int size) {
            int i=0;
            while (i < size && temp[i] != '\0' && (temp[i] == ' '\
                    || temp[i] == '\t' || temp[i] == '\n'))
                    i++;
            // If there is no character in temp, return '0'.
            if (temp[i] == '\0' || i >= size) return '0';
            else return temp[i];
        }

        // Show hands of both players 
        // (if hideFirst==true, the first card will not be shown).
        void showHands(bool hideFirst = false) {
            cout << "\nDealer has: \t";
            dealerHand.print(hideFirst);
            cout << "You have:\t ";
            playerHand.print(); 
            cout << endl;
        }

        // Chips changing hands.
        // (amount > 0: dealer to player; < 0, player to dealer).
        void takeChips(int amount){
            nPlayerChip += amount;
            nDealerChip -= amount;
        }
        
        friend class Dev; // example of friend class
        
};

//these three classes were mainly used for output
// an example of how to use inheritance in order to get the public data
class Extra: public Game{
    protected: // didnt use the protected thing in these
               // three extra functions
    public:
        void disclamer(){
            cout << endl;
            cout << endl;
            cout << "*** Extra Stuff ***" << endl;
            cout << "An Example of how Inheritance works." << endl;
            cout << "This example is using the Main game as an example" << endl;
            cout << "The information below is the cards of the player and dealer shown again." << endl;
            cout << "_______________________________________________" << endl;
        }
};

// each of the Extra classes were created in order to show off 
// a basic form of inheritance
class ExtraTwo: public Hand{
    protected:
    public:    
        void disclamer(){
            cout << endl;
            cout << endl;
            cout << "*** Extra Stuff Two***" << endl;
            cout << "This is another example of how inheritance works." << endl;
            cout << "This example is using the Hand class as an example" << endl;
            cout << "The information below is showing the cards the player " << endl;
            cout << "has at that given time that the function was called." << endl;
            cout << "_______________________________________________" << endl;
        }

};

// this class has the same reason for being created at the top class
class ExtraThree: public Decks{
    protected:
    public:    
        void disclamer(){
            cout << endl;
            cout << endl;
            cout << "*** Extra Stuff Three***" << endl;
            cout << "This is the last example of how inheritance works" << endl;
            cout << "This example is using the Deck class as an example" << endl;
            cout << "The information below is all the cards in the deck being printed " << endl;
            cout << "has at that given time that the function was called." << endl;
            cout << "_______________________________________________" << endl;
        }
        
};

//created to show polymorphism
class FakePlayerInfo{
    protected:
        string name;
        int age;
    public:
        void setInfo(string n, int a){
            name =n;;
            age =a;
        }
};
//data class created to store the data of a fake person
class Data:public FakePlayerInfo{
    public:
        int dataAge(){
            cout << "The players age is: ";
            return age;
        }
        string dataName(){
            cout << "The players name is: ";
            return name;
        }
};


// this class was created in order to show abstract classes
class FakeGuyTwo{
    protected:
        string name;
        int age;
        string address;
    public:
        virtual int dataAgeTwo()= 0;     
        virtual string dataNameTwo()= 0;
        virtual string dataAdrsTwo()= 0;
            
        void setAge(int a){
            age = a;
        }
        
        void setName(string n){
            name = n;
        }
        
        void setAdres(string adrs){
            address = adrs;
        }
};

//another class of a fake persons information
class DataTwo:public FakeGuyTwo{
    public:
        int dataAgeTwo(){
            cout << "The players age is: ";
            return age;
        }
        
        string dataNameTwo(){
            cout << "The players name is: ";
            return name;
        }
        
        string dataAdrsTwo(){
            cout << "The players address is: ";
            return address;
        }
        
        
};

//Friend of all classes
class Dev{
    public:
        void displayGame(Game &g){
            cout << "This is the bet amount in the Game class." << endl;
            cout << "Bet amount was: " << g.nBet;
        }
        
        void displayDecks(Decks &d){
            cout << "This is the pointer for the current card in the Decks class." << endl;
            cout << "Current card is: " << d.current;
        }
        
        void displayCard(Card &c){
            cout << "This is the variable for the suit in the Card class." << endl;
            cout << "Current suit is: " << c.suit << endl;
            cout << endl;
            cout << "This is the variable for the number of the suit in the Card class." << endl;
            cout << "Current number is: " << c.num;
        }
};

//example of inline usage
class Inlne{
    private:
        char choice;
    public:
        void get();
        void output();
};

//usage of the inline function for my final goodbye function
inline void Inlne::get(){
    choice = 'x';
}

inline void Inlne::output(){
    if(choice == 'x'){
        cout << "______________________________________" << endl;
        cout << "|**Thank you for running my Program**|" << endl;
        cout << "______________________________________" << endl;
    }
}

//aggregation example
class Aggr{
    public:
        string name;
        int age;
        
        Aggr(string name,int age){
            this->name =name;
            this->age=age;
        }
};

//created in parallel with the class above this in order to show
//aggregation
class AggrDet{
    private:
        Aggr *ag; // array of the peoples information that can be stored
        int balance;
        long accNum;
    public:
        AggrDet(int balance, int accNum, Aggr *ag){
            this->accNum =accNum;
            this->ag =ag;
            this->balance=balance;
        }
        
        void detail(){
            cout << "**Player Details**" << endl;
            cout << "Name: " << ag->name << endl;
            cout << "Age: " << ag->age << endl;
            cout << "Account Number: #" << accNum << endl;
            cout << "Balance: $" << balance << endl;
        }
};

#endif /* BLACKJACK_H */

