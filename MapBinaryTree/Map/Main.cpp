#include "Map.h"
#include <type_traits>
#include <cassert>
using namespace std;

int main()
{
    Map KDA;
    KDA.insert("Chovy", 5.65);
    Map KDA2;
    KDA2.insert("Efai", 3.46);
    KDA2.insert("Turtle", 8.73);
    KDA.swap(KDA2);
    assert(KDA.size() == 2 && KDA.contains("Efai") && KDA.contains("Turtle") &&
        KDA2.size() == 1 && KDA2.contains("Chovy"));
}
