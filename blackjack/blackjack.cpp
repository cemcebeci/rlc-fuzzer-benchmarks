
#include <algorithm>
#include <iostream>
#include <random>
#include <string>
#include <vector>
#include <cassert>

#ifndef SUIT_COUNT
    #define SUIT_COUNT 4
#endif


class Blackjack 
{
    public:
        Blackjack(int player_count): 
            player_count(player_count),
            player_hands(player_count, std::vector<int>()),
            has_passed(player_count, false),
            hand_is_over(false),
            dealt(false)
        {
            for (int i = 1; i <= 13; i++)
            {
                for(int j = 0; j < SUIT_COUNT; j++)
                    deck.push_back(i);
            }
        }
        
        void shuffle_deck()
        {
            std::random_shuffle(deck.begin(), deck.end());
        }

        void deal()
        {
            require(!dealt);

            for(int player = 0; player < player_count; player++ )
            {
                for (int count = 0; count < 2; count ++) // deal two cards to each player.
                {    
                    int card = deck.back();
                    deck.pop_back();
                    player_hands.at(player).push_back(card);   
                }
            }

            dealt = true;
        }

        void take_card(int player)
        {
            require(!hand_is_over);
            require(dealt);
            require(player >= 0 && player < player_count);
            require(!has_passed.at(player)); // a player that has passed can't draw anymore.


            assert(deck.size() > 0); // We are expecting the fuzzer to find this.

            int card = deck.back();
            deck.pop_back();
            player_hands.at(player).push_back(card);
            
        }

        void pass(int player)
        {
            require(!hand_is_over);
            require(dealt);
            require(player >= 0 && player < player_count);
            require(!has_passed.at(player)); // a player that has passed can't pass again.

            has_passed.at(player) = true;
        }

        int calculate_winner()
        {
            require(hand_is_over);
            int max_points = -1;
            int winner = -1;
            for (int i = 0; i < player_count; i++)
            {
                if(calculate_points(i) > max_points)
                {
                    winner = i;
                    max_points = calculate_points(i);
                }
            }

            return winner; 
        }

        void dump_state()
        {
            std::cout << "--------------------------------\n";
            std::cout << "Deck: \n\t";
            for (int i : deck) std::cout << i << ", ";
            std::cout << "\n";

            std::cout << "Hands:\n";
            for (int i = 0 ; i < player_count; i++)
            {
                std::cout << "\tPlayer " << i << " (" << calculate_points(i) << " points): ";
                for (int j : player_hands.at(i)) std::cout << j << ", ";
                std::cout << "\n";
            }

            std::cout << "Has passed: ";
            for (bool b : has_passed) std::cout << b << ", ";
            std::cout << "\n";
        }

        int calculate_points(int player)
        {
            require(player >= 0 && player < player_count);

            int points = 0;
            int num_ones = 0; 

            for (int card : player_hands.at(player))
            {
                points += card;
                if(card == 1)
                    num_ones ++;
            }

            while (num_ones > 0 && points + 10 <= 21)
            {
                num_ones --;
                points += 10;
            }
            return points <=  21 ? points : -1;
        }

    class BlackjackException : public std::exception {};

    private:
        int player_count;
        std::vector<int> deck;
        std::vector<std::vector<int>> player_hands;
        std::vector<bool> has_passed;
        bool hand_is_over;
        bool dealt;

        void require(bool expr)
        {
            if(!expr) throw BlackjackException();
        }
};