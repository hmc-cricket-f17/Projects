#ifndef CARD_HPP_INCLUDED
#define CARD_HPP_INCLUDED 1

#include <cstddef>
#include <iostream>
#include <string>

#define BOARD -2

enum Suit{
    HEARTS,
    DIAMONDS,
    CLUBS,
    SPADES
};

typedef unsigned char uchar;

class Card{

    public:
        Card() = default; // default constructor
        Card(uchar value, Suit suit); // parameterized constructor
        Card(uchar cardNum); // 0 = 2Hearts, 1 = 3Hearts, ... , 13 = 2Diamonds, ... , 26 = 2Clubs, ... , 50 = KSpades, 51 = ASpades
        Card(const Card& otherCard) = default; // default copy constructor
        ~Card() = default;
        void setPlayerPos(short pos);

        bool operator==(const Card& otherCard);
        friend inline std::ostream& operator<<(std::ostream& out, const Card& card);

    //private:
        uchar value_; // Holds Card's would-be index in Player value_arr
        Suit suit_;
        uchar encoding_;
        short player_pos_; // Keeps track of which player has this card, -1 for single player, -2 for multiple players' board
};

inline std::ostream& operator<<(std::ostream& out, const Card& card)
{
    // Restore real value from Player value_arr index
    uchar val = card.value_ == 12 ? 1 : card.value_ + 2;
    std::string s;
    switch(card.suit_){
        case 0:
            s = "HEARTS";
            break;
        case 1:
            s = "DIAMONDS";
            break;
        case 2:
            s = "CLUBS";
            break;
        default:
            s = "SPADES";
    }
    out << val << " " << s;
    return out;
}

#endif 