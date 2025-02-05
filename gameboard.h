#pragma once

#include <QAbstractListModel>
#include <vector>

//#include <QTime>
#include <QElapsedTimer>

#include "sqlitemanager.h"
class GameBoard : public QAbstractListModel {
  Q_OBJECT
  // Property for last hidden element on the board
  Q_PROPERTY(int hiddenElementValue READ hiddenElementValue CONSTANT)
  // Property for counting tiles moves
  Q_PROPERTY(int stepsCount READ stepsCount NOTIFY stepsCountChanged)
  // Property for board state, completed or not. Every object in qml depend on
  // this state
  Q_PROPERTY(
      bool bBoardState READ BoardState WRITE setBoardState NOTIFY stateChanged)
public:
  static constexpr size_t DEFAULT_DIMENSION{4};

  GameBoard(QObject *parent = nullptr,
            size_t board_dimension = DEFAULT_DIMENSION);

  struct Tile {
    size_t value{};
    Tile &operator=(const size_t new_value) {
      value = new_value;
      return *this;
    }
    bool operator==(const size_t other) { return other == value; }
    friend inline bool operator<(const GameBoard::Tile &lhs,
                                 const GameBoard::Tile &rhs);
    friend inline bool operator>(const GameBoard::Tile &lhs,
                                 const GameBoard::Tile &rhs);
    friend inline bool operator<=(const GameBoard::Tile &lhs,
                                  const GameBoard::Tile &rhs);
    friend inline bool operator>=(const GameBoard::Tile &lhs,
                                  const GameBoard::Tile &rhs);
  };

  Q_INVOKABLE void shuffle();
  Q_INVOKABLE void finishBoard();

  int rowCount(const QModelIndex &parent = QModelIndex()) const override;

  QVariant data(const QModelIndex &index,
                int role = Qt::DisplayRole) const override;

  size_t hiddenElementValue() const;

  Q_INVOKABLE bool move(int index);

  using Position = std::pair<size_t, size_t>;
  using iterator = std::vector<Tile>::iterator;

  int stepsCount();
  void setStepsCounter(int stepsCounter);

  bool BoardState() const;
  void setBoardState(bool value);

signals:
  void stepsCountChanged(int steps);
  void completed();
  void stateChanged(bool state);

private:
  std::vector<Tile> m_raw_board;

  const size_t m_dimension;
  const size_t m_boardsize;

  const size_t m_hiddenElementValue;

  int m_stepsCounter{0};

  bool bBoardState{false};

  QElapsedTimer gameTimer;
  size_t m_gameTime{};

  bool isBoardValid() const;
  bool isBoardFinished();
  bool isPositionValid(const size_t position) const;

  Position getRowCol(size_t index) const;

  sqlitemanager database;
};

inline bool operator<(const GameBoard::Tile &lhs, const GameBoard::Tile &rhs) {
  return lhs.value < rhs.value;
}
inline bool operator>(const GameBoard::Tile &lhs, const GameBoard::Tile &rhs) {
  return rhs < lhs;
}
inline bool operator<=(const GameBoard::Tile &lhs, const GameBoard::Tile &rhs) {
  return !(lhs > rhs);
}
inline bool operator>=(const GameBoard::Tile &lhs, const GameBoard::Tile &rhs) {
  return !(lhs < rhs);
}
