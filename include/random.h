
#ifndef RANDOM_H
#define RANDOM_H
#include "format.h"
#include <cstddef>
#include <random>
#include <vector>

namespace hwj
{
class RandomInt
{
  public:
    std::mt19937 engine;
    std::uniform_int_distribution<> distrib;
    RandomInt(int min, int max) : engine(std::random_device()()), distrib(min, max)
    {
    }
    int get()
    {
        return distrib(engine);
    }
    std::vector<int> get(size_t n)
    {
        std::vector<int> res(n);
        for (size_t i = 0; i < n; i++)
        {
            res[0] = distrib(engine);
        }
        return res;
    }
};
} // namespace hwj
#endif
