#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <ctime>

using namespace std;

// Карта
struct Card {
    string suit;  // масть: ♠ ♥ ♦ ♣
    string rank;  // ранг: 2-10, J, Q, K, A
    int value;    // номинал
};

// Колода
class Deck {
private:
    vector<Card> cards;
    void createDeck() {
        string suits[4] = {"♠", "♥", "♦", "♣"};
        string ranks[13] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"};
        int values[13] = {2,3,4,5,6,7,8,9,10,10,10,10,11};

        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 13; ++j) {
                cards.push_back({suits[i], ranks[j], values[j]});
            }
        }
    }

public:
    Deck() {
        createDeck();
    }

    void shuffle() {
        random_device rd;
        mt19937 g(rd());
        std::shuffle(cards.begin(), cards.end(), g);
    }

    Card deal() {
        if (cards.empty()) {
            createDeck();
            shuffle();
        }
        Card card = cards.back();
        cards.pop_back();
        return card;
    }
};

// Подсчёт очков (учитывая тузы)
int getHandValue(const vector<Card>& hand) {
    int value = 0;
    int aces = 0;

    for (const auto& card : hand) {
        value += card.value;
        if (card.rank == "A") aces++;
    }

    while (value > 21 && aces > 0) {
        value -= 10;
        aces--;
    }
    return value;
}

// Вывод руки
void printHand(const vector<Card>& hand, bool hideFirst = false) {
    for (size_t i = 0; i < hand.size(); ++i) {
        if (hideFirst && i == 0) {
            cout << "?? ";
        } else {
            cout << hand[i].suit << hand[i].rank << " ";
        }
    }
    cout << endl;
}

int main() {
    cout << "=== БЛЭКДЖЕК ===\n\n";
    srand(time(0)); // для старых компиляторов

    Deck deck;
    deck.shuffle();

    char playAgain = 'y';

    while (playAgain == 'y' || playAgain == 'Y') {
        vector<Card> playerHand;
        vector<Card> dealerHand;

        // Раздача начальных карт
        playerHand.push_back(deck.deal());
        dealerHand.push_back(deck.deal());
        playerHand.push_back(deck.deal());
        dealerHand.push_back(deck.deal());

        cout << "Ваша рука: ";
        printHand(playerHand);
        cout << "Очки: " << getHandValue(playerHand) << endl;

        cout << "Рука дилера: ";
        printHand(dealerHand, true); // первая карта скрыта

        // Ход игрока
        bool playerBust = false;
        while (true) {
            int playerValue = getHandValue(playerHand);
            if (playerValue >= 21) break;

            cout << "\nВзять карту (h) или остановиться (s)? ";
            char choice;
            cin >> choice;

            if (choice == 'h' || choice == 'H') {
                playerHand.push_back(deck.deal());
                cout << "Вы взяли: " << playerHand.back().suit << playerHand.back().rank << endl;
                cout << "Ваша рука: ";
                printHand(playerHand);
                cout << "Очки: " << getHandValue(playerHand) << endl;
            } else {
                break;
            }
        }

        int playerValue = getHandValue(playerHand);
        if (playerValue > 21) {
            playerBust = true;
            cout << "\nВы перебрали! Вы проиграли.\n";
        }

        // Ход дилера
        if (!playerBust) {
            cout << "\nРука дилера: ";
            printHand(dealerHand);
            cout << "Очки дилера: " << getHandValue(dealerHand) << endl;

            while (getHandValue(dealerHand) < 17) {
                dealerHand.push_back(deck.deal());
                cout << "Дилер взял: " << dealerHand.back().suit << dealerHand.back().rank << endl;
                cout << "Рука дилера: ";
                printHand(dealerHand);
                cout << "Очки: " << getHandValue(dealerHand) << endl;
            }

            int dealerValue = getHandValue(dealerHand);

            if (dealerValue > 21) {
                cout << "\nДилер перебрал! Вы выиграли!\n";
            } else if (playerValue > dealerValue) {
                cout << "\nВы выиграли!\n";
            } else if (dealerValue > playerValue) {
                cout << "\nДилер выиграл!\n";
            } else {
                cout << "\nНичья!\n";
            }
        }

        cout << "\nСыграть ещё раз? (y/n): ";
        cin >> playAgain;
        
        // Очистка для новой игры
        if (playAgain == 'y' || playAgain == 'Y') {
            deck.shuffle(); // можно пересобрать колоду при необходимости
        }
    }

    cout << "\nСпасибо за игру! До встречи!\n";
    return 0;
}