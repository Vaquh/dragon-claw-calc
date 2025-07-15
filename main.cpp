#include <algorithm>
#include <array>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <random>
#include <string>
#include <vector>

struct Weapon
{
  int attackSpeedTicks{};
  double accuracy{};
  int maxHit{};
};

struct Attack
{
  int firstHitsplat{};
  int secondHitsplat{};
  int thirdHitsplat{};
  int fourthHitsplat{};

  int totalDamage()
  {
    return firstHitsplat + secondHitsplat + thirdHitsplat + fourthHitsplat;
  }
};

class Simulation
{
public:
  std::vector<Attack> attacks{};
  int attemptsTotal{};

  Simulation(int attemptsTotal)
  {
    this->attemptsTotal = attemptsTotal;
  }

  int attemptsPadding()
  {
    return std::to_string(attemptsTotal).length();
  }

  double attacksMeanAverage()
  {
    if (attacks.empty())
    {
      return 0;
    }

    std::vector<int> combinedHitsplats;
    for (Attack attack : attacks)
    {
      combinedHitsplats.push_back(attack.totalDamage());
    }

    return static_cast<double>(std::reduce(combinedHitsplats.begin(), combinedHitsplats.end())) / combinedHitsplats.size();
  }
};

int randomDamage(int minHit, int maxHit, std::mt19937 &randomGenerator)
{
  std::uniform_int_distribution<int> randomDamageDistribution(minHit, maxHit);
  return randomDamageDistribution(randomGenerator);
}

void printDamageRoll(int attemptsTotal, int currentAttempt, Attack attack)
{
  int attemptPadding = std::to_string(attemptsTotal).length();
  std::cout << std::setw(attemptPadding) << std::right << currentAttempt << " : "
            << std::setw(2) << std::right << attack.totalDamage() << " = "
            << std::setw(2) << std::right << attack.firstHitsplat << " + "
            << std::setw(2) << std::right << attack.secondHitsplat << " + "
            << std::setw(2) << std::right << attack.thirdHitsplat << " + "
            << std::setw(2) << std::right << attack.fourthHitsplat << "\n";
}

int main()
{
  const bool verbose = false;

  const int numberOfAttempts{10000000};
  Simulation simulation(numberOfAttempts);

  const int attackSpeedTicks{4};
  const double accuracy{0.4191};
  const int maxhit{33};
  Weapon dragonClaws{attackSpeedTicks, accuracy, maxhit};

  std::random_device rd{};
  std::array<std::random_device::result_type, std::mt19937::state_size> seedData{};
  std::generate(seedData.begin(), seedData.end(), std::ref(rd));
  std::seed_seq ss{seedData.begin(), seedData.end()};
  std::mt19937 randomGenerator{ss};

  std::uniform_real_distribution<double> accuracyDistribution(0.0, 1.0);

  for (int i = 0; i < simulation.attemptsTotal; i++)
  {
    int currentAttempt = i + 1;
    Attack currentAttack;

    if (dragonClaws.accuracy >= accuracyDistribution(randomGenerator))
    {
      currentAttack.firstHitsplat = randomDamage(static_cast<int>(dragonClaws.maxHit * 0.5), dragonClaws.maxHit - 1, randomGenerator);
      currentAttack.secondHitsplat = static_cast<int>(currentAttack.firstHitsplat * 0.5);
      currentAttack.thirdHitsplat = static_cast<int>(currentAttack.secondHitsplat * 0.5);
      currentAttack.fourthHitsplat = currentAttack.thirdHitsplat + 1;
      if (verbose)
      {
        printDamageRoll(simulation.attemptsTotal, currentAttempt, currentAttack);
      }
    }
    else if (dragonClaws.accuracy >= accuracyDistribution(randomGenerator))
    {
      currentAttack.firstHitsplat = 0;
      currentAttack.secondHitsplat = randomDamage(static_cast<int>(dragonClaws.maxHit * 0.375), static_cast<int>(dragonClaws.maxHit * 0.875), randomGenerator);
      currentAttack.thirdHitsplat = static_cast<int>(currentAttack.secondHitsplat * 0.5);
      currentAttack.fourthHitsplat = currentAttack.thirdHitsplat + 1;
      if (verbose)
      {
        printDamageRoll(simulation.attemptsTotal, currentAttempt, currentAttack);
      }
    }
    else if (dragonClaws.accuracy >= accuracyDistribution(randomGenerator))
    {
      currentAttack.firstHitsplat = 0;
      currentAttack.secondHitsplat = 0;
      currentAttack.thirdHitsplat = randomDamage(static_cast<int>(dragonClaws.maxHit * 0.25), static_cast<int>(dragonClaws.maxHit * 0.75), randomGenerator);
      currentAttack.fourthHitsplat = currentAttack.thirdHitsplat + 1;
      if (verbose)
      {
        printDamageRoll(simulation.attemptsTotal, currentAttempt, currentAttack);
      }
    }
    else if (dragonClaws.accuracy >= accuracyDistribution(randomGenerator))
    {
      currentAttack.firstHitsplat = 0;
      currentAttack.secondHitsplat = 0;
      currentAttack.thirdHitsplat = 0;
      currentAttack.fourthHitsplat = randomDamage(static_cast<int>(dragonClaws.maxHit * 0.25), static_cast<int>(dragonClaws.maxHit * 1.25), randomGenerator);
      if (verbose)
      {
        printDamageRoll(simulation.attemptsTotal, currentAttempt, currentAttack);
      }
    }
    else
    {
      if (randomDamage(0, 2, randomGenerator) < 2)
      {
        currentAttack.firstHitsplat = 0;
        currentAttack.secondHitsplat = 0;
        currentAttack.thirdHitsplat = 1;
        currentAttack.fourthHitsplat = 1;
        if (verbose)
        {
          printDamageRoll(simulation.attemptsTotal, currentAttempt, currentAttack);
        }
      }
      else
      {
        currentAttack.firstHitsplat = 0;
        currentAttack.secondHitsplat = 0;
        currentAttack.thirdHitsplat = 0;
        currentAttack.fourthHitsplat = 0;
        if (verbose)
        {
          printDamageRoll(simulation.attemptsTotal, currentAttempt, currentAttack);
        }
      }
    }

    simulation.attacks.push_back(currentAttack);
  }

  double damageAverage = simulation.attacksMeanAverage();
  double damagePerTick = damageAverage / dragonClaws.attackSpeedTicks;
  double damagePerSecond = damagePerTick / 0.6; // Ticks are roughly 0.6 the length of a real-time second

  std::cout << "Simulated " << simulation.attacks.size() << " attacks\n"
            << "Average Hit: " << damageAverage << "\n"
            << "Dps: " << damagePerSecond << "\n";

  return 0;
}