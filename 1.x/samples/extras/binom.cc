#include <iostream>
#include <cmath>
// Generate pseudo-random outcomes of tossing a biased coin 100 times.
// Compile with, e.g.
//
//    g++ -o binom binom.cc && ./binom > binom.dat

const int trials=100000; // amount of data to collect

int flip(double wt=0.5) // 0 < wt < 1
{
  if (rand() < (1-wt)*RAND_MAX)
    return 0;
  else
    return 1;
}

void toss(unsigned int tosses, double wt=0.5)
{
  int count=0;
  for (int i=0; i < tosses; ++i)
    count += flip(wt);

  std::cout << count << "\t";
}

int main()
{
  for (int j=0; j < trials; ++j)
    {
      toss(100, 0.05);
      toss(100, 0.1);
      toss(100, 0.3);
      toss(100);
      std::cout << "\n";
    }
}
