class Piece {
 private:
  int x_;
  int y_;
};

class Pawn : public Piece {
 private:
  bool double_;
};

class Knight : public Piece {};

class Bishop : public Piece {};

class Rook : public Piece {
 private:
  bool moved_;
};

class Queen : public Piece {};

class King : public Piece {
 private:
  bool moved_;
};

class Board {
 private:
  std::unique_ptr<Piece> board_[8][8];
};

int main() { return 0; }
