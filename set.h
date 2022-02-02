


class Set
{
public:
    Set();
    Set(Set &set);
    ~Set();

    void incrementScore1(); //limited to 99 in toString
    void incrementScore2();
    void decrementScore1();
    void decrementScore2();
    void resetScore();

private:
    int setNumber;
    int score1;
    int score2;


};