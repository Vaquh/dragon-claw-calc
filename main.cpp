#include <iomanip>
#include <iostream>
#include <numeric>
#include <random>
#include <string>
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

int randDamage(int minHit, int maxHit)
{
  std::random_device rd;
  std::mt19937_64 dmgGenerator(rd());
  std::uniform_int_distribution<int> dmgDistribution(minHit, maxHit);
  
  return dmgDistribution(dmgGenerator);
}

int main()
{
  int maxhit = 33;
  double accuracy = 0.4191;
  int atkspd = 4;
  int attempts = 100000;
  
  int attemptsWidth = std::to_string(attempts).length();

  std::vector<int> simulation;

  std::random_device rd;
  std::mt19937_64 accGenerator(rd());
  std::uniform_real_distribution<double> accDistribution(0.0, 1.0);

  for (size_t i = 0; i < attempts; i++)
  {
    int attack;

    if(accuracy >= accDistribution(accGenerator)) 
    { // Hit first
      int dmg = randDamage(static_cast<int>(maxhit * 0.5), maxhit - 1);
      attack = dmg + static_cast<int>(dmg * 0.5) + static_cast<int>(dmg * 0.25) + (static_cast<int>(dmg * 0.25) + 1);
      std::cout << std::setw(attemptsWidth) << std::right << i + 1                            << " : "
                << std::setw(2)             << std::right << attack                           << " - "
                << std::setw(2)             << std::right << dmg                              << " + "
                << std::setw(2)             << std::right << static_cast<int>(dmg * 0.5)      << " + "
                << std::setw(2)             << std::right << static_cast<int>(dmg * 0.25)     << " + "
                << std::setw(2)             << std::right << static_cast<int>(dmg * 0.25) + 1 << "\n";
    }
    else if (accuracy >= accDistribution(accGenerator))
    { // Hit second
      int dmg = randDamage(static_cast<int>(maxhit * 0.375), static_cast<int>(maxhit * 0.875));
      attack = 0 + dmg + static_cast<int>(dmg * 0.5) + (static_cast<int>(dmg * 0.5) + 1);
      std::cout << std::setw(attemptsWidth) << std::right << i + 1                           << " : "
                << std::setw(2)             << std::right << attack                          << " : "
                << std::setw(2)             << std::right << 0                               << " + "
                << std::setw(2)             << std::right << dmg                             << " + "
                << std::setw(2)             << std::right << static_cast<int>(dmg * 0.5)     << " + "
                << std::setw(2)             << std::right << static_cast<int>(dmg * 0.5) + 1 << "\n";
    }
    else if (accuracy >= accDistribution(accGenerator))
    { // Hit third
      int dmg = randDamage(static_cast<int>(maxhit * 0.25), static_cast<int>(maxhit * 0.75));
      attack = 0 + 0 + dmg + (dmg + 1);
      std::cout << std::setw(attemptsWidth) << std::right << i + 1   << " : "
                << std::setw(2)             << std::right << attack  << " : "
                << std::setw(2)             << std::right << 0       << " + "
                << std::setw(2)             << std::right << 0       << " + "
                << std::setw(2)             << std::right << dmg     << " + "
                << std::setw(2)             << std::right << dmg + 1 << "\n";
    }
    else if (accuracy >= accDistribution(accGenerator))
    { // Hit fourth
      int dmg = randDamage(static_cast<int>(maxhit * 0.25), static_cast<int>(maxhit * 1.25));
      attack = 0 + 0 + 0 + dmg;
      std::cout << std::setw(attemptsWidth) << std::right << i + 1  << " : "
                << std::setw(2)             << std::right << attack << " : "
                << std::setw(2)             << std::right << 0      << " + "
                << std::setw(2)             << std::right << 0      << " + "
                << std::setw(2)             << std::right << 0      << " + "
                << std::setw(2)             << std::right << dmg    << "\n";
    }
    else
    { // Miss
      if(randDamage(0, 2) < 2)
      {
        attack = 0 + 0 + 1 + 1;
        std::cout << std::setw(attemptsWidth) << std::right << i + 1  << " : "
                  << std::setw(2)             << std::right << attack << " : "
                  << std::setw(2)             << std::right << 0      << " + "
                  << std::setw(2)             << std::right << 0      << " + "
                  << std::setw(2)             << std::right << 1      << " + "
                  << std::setw(2)             << std::right << 1      << "\n";
      }
      else
      {
        attack = 0 + 0 + 0 + 0;
        std::cout << std::setw(attemptsWidth) << std::right << i + 1  << " : "
                  << std::setw(2)             << std::right << attack << " : "
                  << std::setw(2)             << std::right << 0      << " + "
                  << std::setw(2)             << std::right << 0      << " + "
                  << std::setw(2)             << std::right << 0      << " + "
                  << std::setw(2)             << std::right << 0      << "\n";
      }
    }

    simulation.push_back(attack);
  }

  double damageAverage = getAverage(simulation);
  double damagePerSecond = (damageAverage / atkspd) / 0.6;

  std::cout << "\n"
            << "Simulated "    << simulation.size() << " attacks\n"
            << "Average Hit: " << damageAverage     << "\n"
            << "Dps: "         << damagePerSecond   << "\n";

  return 0;
}