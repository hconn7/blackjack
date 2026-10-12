#include "Random.h"
#include <algorithm>
#include <array>
#include <iostream>
#include <ostream>

static constexpr int MAX_DECK{52};

namespace Settings {
constexpr int BUST{22};
constexpr int STOP_DEALING{17};

} // namespace Settings

struct Player {
  int score;
};

struct Card {

  enum Rank {
    rank_ace,
    rank_2,
    rank_3,
    rank_4,
    rank_5,
    rank_6,
    rank_7,
    rank_8,
    rank_9,
    rank_10,
    rank_jack,
    rank_queen,
    rank_king,

    max_ranks
  };

  enum Suit {
    suit_club,
    suit_diamond,
    suit_heart,
    suit_spade,

    max_suits
  };

  Rank rank{};
  Suit suit{};

  static constexpr std::array allRanks{
      rank_ace, rank_2, rank_3,  rank_4,    rank_5,     rank_6,   rank_7,
      rank_8,   rank_9, rank_10, rank_jack, rank_queen, rank_king};

  static constexpr std::array allSuits{suit_club, suit_diamond, suit_heart,
                                       suit_spade};

  friend std::ostream& operator<<(std::ostream& out, const Card& card) {
    static constexpr std::array<char, max_ranks> ranks{
        'A', '2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K'};

    static constexpr std::array<char, max_suits> suits{'C', 'D', 'H', 'S'};

    char rank{ranks[card.rank]};
    char suit{suits[card.suit]};

    out << rank << suit;
    return out;
  }
  int value() const {
    static constexpr std::array rankValues{11, 2, 3,  4,  5,  6, 7,
                                           8,  9, 10, 10, 10, 10};
    return rankValues[rank];
  }
};

class Deck {
public:
  std::array<Card, MAX_DECK> initDeck() {
    int idx{0};
    for (int i{0}; i < Card::max_suits; ++i) {
      for (int j{0}; j < Card::max_ranks; ++j) {
        m_deck[idx] = Card{Card::allRanks[j], Card::allSuits[i]};
        ++idx;
      }
    }
    return m_deck;
  }

  Card dealCard() {
    static int j{0};
    while (j < m_deck.size()) {
      return m_deck[j++];
    }
    return Card{};
  }

  friend std::ostream& operator<<(std::ostream& out, const Deck& deck) {
    for (auto card : deck.m_deck) {
      out << card;
    }
    return out;
  }
  void shuffle() { std::shuffle(m_deck.begin(), m_deck.end(), Random::mt); }

private:
  std::array<Card, MAX_DECK> m_deck{initDeck()};
};

bool playerTurn(Deck& d, Player& p) {

  while (p.score < Settings::BUST) {
    std::cout << "(h) to hit, or (s) to stand: ";
    char choice{};
    std::cin >> choice;
    if (choice == 'h') {
      int cardVal{d.dealCard().value()};

      p.score += cardVal;
      std::cout << "You were deal " << cardVal << "  You now have: " << p.score
                << '\n';
      return true;
      if (p.score >= Settings::BUST) {
        std::cout << "You busted! \n";
        return true;
      }
    }
    if (choice == 's') {
      return true;
    }
  }
  return false;
}
bool dealerTurn(Deck& deck, Player& player) {

  while (player.score < Settings::STOP_DEALING) {

    Card newCard{deck.dealCard()};
    std::cout << "The dealer flips a " << newCard
              << "  They now have: " << player.score + newCard.value() << '\n';
    player.score += newCard.value();
  }

  if (player.score > Settings::BUST) {
    std::cout << "The dealer went bust! \n";
    return true;
  }
  return false;
}

bool playBlackJack() {
  Deck d{};
  Player de{};
  d.shuffle();
  Player p{};

  int dealerCard{d.dealCard().value()};
  int playCardOne{d.dealCard().value()};
  int playerCardTwo{d.dealCard().value()};

  std::cout << "The dealer is showing: " << dealerCard << '\n';

  int totalValP = playCardOne + playerCardTwo;
  std::cout << "You have score: " << totalValP << '\n';
  p.score += totalValP;
  while (!playerTurn(d, p)) {
  }
  if (dealerTurn(d, de)) {
    return true;
  } else {
    return false;
  }
}
int main() {

  if (playBlackJack()) {
    std::cout << "You win!\n";
  } else {
    std::cout << "You lose!\n";
  }

  return 0;
}
