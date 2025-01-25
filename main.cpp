#include <iomanip>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>

template <typename T>
double getAverage(std::vector<T> const& v)
{
  if(v.empty()) 
  {
    return 0;
  }

  return std::reduce(v.begin(), v.end(), 0.0) / v.size();
}
 
void simulate(int maxHit, double accuracy, int atkspd, int attempts)
{
  std::vector<int> simulation;

  std::random_device rd;
  std::mt19937 accgen(rd());
  
  std::uniform_real_distribution<double> accdis(0.0, 1.0);

  for (size_t i = 0; i < attempts; i++)
  {
    int attack;

    std::random_device rd;
    std::mt19937 hitgen(rd());

    if(accuracy >= accdis(accgen)) 
    { // Hit first
      std::uniform_int_distribution<int> hitdis((int)(maxHit * 0.5), maxHit - 1);
      int hit = hitdis(hitgen);
      attack = hit + (int)(hit * 0.5) + (int)(hit * 0.25) + ((int)(hit * 0.25) + 1);
      std::cout << std::setw(2) << std::left << attack << " : " << std::setw(2) << std::right << hit << " + " << std::setw(2) << std::right << (int)(hit * 0.5) << " + " << std::setw(2) << std::right << (int)(hit * 0.25) << " + " << std::setw(2) << std::right << ((int)(hit * 0.25) + 1) << "\n";
    }
    else if (accuracy >= accdis(accgen))
    { // Hit second
      std::uniform_int_distribution<int> hitdis((int)(maxHit * 0.375), (int)(maxHit * 0.875));
      int hit = hitdis(hitgen);
      attack = 0 + hit + (int)(hit * 0.5) + ((int)(hit * 0.5) + 1);
      std::cout << std::setw(2) << std::left << attack << " : " << std::setw(2) << std::right << 0 << " + " << std::setw(2) << std::right << hit << " + " << std::setw(2) << std::right << (int)(hit * 0.5) << " + " << std::setw(2) << std::right << ((int)(hit * 0.5) + 1) << "\n";
    }
    else if (accuracy >= accdis(accgen))
    { // Hit third
      std::uniform_int_distribution<int> hitdis((int)(maxHit * 0.25), (int)(maxHit * 0.75));
      int hit = hitdis(hitgen);
      attack = 0 + 0 + hit + (hit + 1);
      std::cout << std::setw(2) << std::left << attack << " : " << std::setw(2) << std::right << 0 << " + " << std::setw(2) << std::right << 0 << " + " << std::setw(2) << std::right << hit << " + " << std::setw(2) << std::right << (hit + 1) << "\n";
    }
    else if (accuracy >= accdis(accgen))
    { // Hit fourth
      std::uniform_int_distribution<int> hitdis((int)(maxHit * 0.25), (int)(maxHit * 1.25));
      int hit = hitdis(hitgen);
      attack = 0 + 0 + 0 + hit;
      std::cout << std::setw(2) << std::left << attack << " : " << std::setw(2) << std::right << 0 << " + " << std::setw(2) << std::right << 0 << " + " << std::setw(2) << std::right << 0 << " + " << std::setw(2) << std::right << hit << "\n";
    }
    else
    { // Miss
      std::uniform_int_distribution<int> hitdis(0, 2);
      if(hitdis(hitgen) < 2)
      {
        attack = 0 + 0 + 1 + 1;
        std::cout << std::setw(2) << std::left << attack << " : " << std::setw(2) << std::right << 0 << " + " << std::setw(2) << std::right << 0 << " + " << std::setw(2) << std::right << 1 << " + " << std::setw(2) << std::right << 1 << "\n";
      }
      else
      {
        attack = 0 + 0 + 0 + 0;
        std::cout << std::setw(2) << std::left << attack << " : " << std::setw(2) << std::right << 0 << " + " << std::setw(2) << std::right << 0 << " + " << std::setw(2) << std::right << 0 << " + " << std::setw(2) << std::right << 0 << "\n";
      }
    }

    simulation.push_back(attack);
  }

  double damageAverage = getAverage(simulation);
  double damagePerSecond = (damageAverage / atkspd) / 0.6;

  std::cout << "\n";
  std::cout << "Simulated " << simulation.size() << " attacks\n";
  std::cout << "Average Hit: " << damageAverage << "\n";
  std::cout << "Dps: " << damagePerSecond << "\n";

  return;
}

int main()
{
  int maxhit = 33;
  double accuracy = 0.4191;
  int atkspd = 4;
  int attempts = 100000;

  simulate(maxhit, accuracy, atkspd, attempts);

  return 0;
}