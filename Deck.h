/********************************************************************************
 
 Deck of cards. 
 
********************************************************************************/

using namespace std;

class Deck{
    
public:
    
    Deck(){
        cards={258, 259, 261, 263, 267, 269, 273, 275, 279, 285, 287, 293, 297, 514, 515, 517, 519, 523, 525, 529, 531, 535, 541, 543, 549, 553, 1026, 1027, 1029, 1031, 1035, 1037, 1041, 1043, 1047, 1053, 1055, 1061, 1065, 2050, 2051, 2053, 2055, 2059, 2061, 2065, 2067, 2071, 2077, 2079, 2085, 2089};
        order={0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51};
        pointer=0;
        reset();
    }
    
    // Shuffle the "order" array.
    void Shuffle(){
        unsigned seed =chrono::system_clock::now().time_since_epoch().count();
        shuffle(order.begin(), order.end(), default_random_engine(seed));
    }
    
    // Set the pointer to zero and shuffle.
    void reset(){
        pointer=0;
        Shuffle();
    }
    
    // Pick a number from the "order" array to which the "pointer"
    // points and increment the pointer by one.
    int deal_card(){
        int a=order[pointer++];
        int c=cards[a];
        return c;
    }
    
    // Interface to the private variables:
    
    vector<int> get_cards(){
        return cards;
    }
    array<int,52> get_order(){
        return order;
    }
    int get_pointer(){
        return pointer;
    }
    
private:
    
    vector<int> cards;
    array<int,52> order;
    int pointer;
    
};
