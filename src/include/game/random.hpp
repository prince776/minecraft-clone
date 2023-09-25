#include <random>

inline int getRandomNumber(std::mt19937 rng, int l, int r) {
    std::uniform_int_distribution<int> dist(l, r);
    return dist(rng);
}
