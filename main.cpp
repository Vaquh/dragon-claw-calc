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
  int attackSpeedTicks {};
  double accuracy {};
  int maxHit {};
};

struct Attack
{
  int hitsplatOne {};
  int hitsplatTwo {};
  int hitsplatThree {};
  int hitsplatFour {};

  int totalDamage() const
  {
    return hitsplatOne + hitsplatTwo + hitsplatThree + hitsplatFour;
  }
};

class Simulation
{
public:
  Simulation(size_t numAttempts)
  {
    std::random_device rd {};
    std::array<std::random_device::result_type, std::mt19937::state_size> seedData {};
    std::generate(seedData.begin(), seedData.end(), std::ref(rd));
    std::seed_seq seedSeq { seedData.begin(), seedData.end() };
    this->randomGenerator.seed(seedSeq);

    this->numAttempts = numAttempts;
    this->attemptsPadding = static_cast<int>(std::to_string(this->numAttempts).length());
  }

  void setNumAttempts(size_t numAttempts)
  {
    if (numAttempts <= attacks.size())
    {
      numAttempts = attacks.size();
    }

    this->numAttempts = numAttempts;
    attemptsPadding = static_cast<int>(std::to_string(this->numAttempts).length());
    return;
  }

  size_t getNumAttempts() const
  {
    return numAttempts;
  }

  size_t size() const
  {
    return attacks.size();
  }

  void generateAttacks(Weapon weapon)
  {
    for (size_t i { attacks.size() }; i < numAttempts; i++)
    {
      Attack currentAttack;

      if (weapon.accuracy >= accuracyDistribution(randomGenerator))
      {
        currentAttack.hitsplatOne = randomDamage(static_cast<int>(weapon.maxHit * 0.5), weapon.maxHit - 1);
        currentAttack.hitsplatTwo = static_cast<int>(currentAttack.hitsplatOne * 0.5);
        currentAttack.hitsplatThree = static_cast<int>(currentAttack.hitsplatTwo * 0.5);
        currentAttack.hitsplatFour = currentAttack.hitsplatThree + 1;
      }
      else if (weapon.accuracy >= accuracyDistribution(randomGenerator))
      {
        currentAttack.hitsplatOne = 0;
        currentAttack.hitsplatTwo = randomDamage(static_cast<int>(weapon.maxHit * 0.375), static_cast<int>(weapon.maxHit * 0.875));
        currentAttack.hitsplatThree = static_cast<int>(currentAttack.hitsplatTwo * 0.5);
        currentAttack.hitsplatFour = currentAttack.hitsplatThree + 1;
      }
      else if (weapon.accuracy >= accuracyDistribution(randomGenerator))
      {
        currentAttack.hitsplatOne = 0;
        currentAttack.hitsplatTwo = 0;
        currentAttack.hitsplatThree = randomDamage(static_cast<int>(weapon.maxHit * 0.25), static_cast<int>(weapon.maxHit * 0.75));
        currentAttack.hitsplatFour = currentAttack.hitsplatThree + 1;
      }
      else if (weapon.accuracy >= accuracyDistribution(randomGenerator))
      {
        currentAttack.hitsplatOne = 0;
        currentAttack.hitsplatTwo = 0;
        currentAttack.hitsplatThree = 0;
        currentAttack.hitsplatFour = randomDamage(static_cast<int>(weapon.maxHit * 0.25), static_cast<int>(weapon.maxHit * 1.25));
      }
      else
      {
        if (randomDamage(0, 2) < 2)
        {
          currentAttack.hitsplatOne = 0;
          currentAttack.hitsplatTwo = 0;
          currentAttack.hitsplatThree = 1;
          currentAttack.hitsplatFour = 1;
        }
        else
        {
          currentAttack.hitsplatOne = 0;
          currentAttack.hitsplatTwo = 0;
          currentAttack.hitsplatThree = 0;
          currentAttack.hitsplatFour = 0;
        }
      }

      attacks.push_back(currentAttack);
    }

    return;
  }

  double getAttacksMeanAverage() const
  {
    if (attacks.empty())
    {
      return 0;
    }

    std::vector<int> combinedHitsplats;
    for (const Attack& attack : attacks)
    {
      combinedHitsplats.push_back(attack.totalDamage());
    }

    return static_cast<double>(std::reduce(combinedHitsplats.begin(), combinedHitsplats.end())) / static_cast<double>(combinedHitsplats.size());
  }

  void printAttacks() const
  {
    if (attacks.empty())
    {
      std::cout << "There are no attacks saved in this simulation." << '\n';
      return;
    }

    for (size_t i { 0 }; Attack attack : attacks)
    {
      i++;
      printAttack(i, attack);
    }

    return;
  }

  void clear()
  {
    attacks.clear();
    numAttempts = 0;
    attemptsPadding = 0;
    return;
  }

private:
  size_t numAttempts {};
  int attemptsPadding {};

  std::vector<Attack> attacks {};

  std::mt19937 randomGenerator {};
  std::uniform_real_distribution<double> accuracyDistribution { 0.0, 1.0 };

  int randomDamage(int minHit, int maxHit)
  {
    std::uniform_int_distribution<int> randomDamageDistribution(minHit, maxHit);
    return randomDamageDistribution(randomGenerator);
  }

  void printAttack(size_t currentAttempt, Attack attack) const
  {
    std::cout << std::setw(attemptsPadding) << std::right << currentAttempt << " : "
      << std::setw(2) << std::right << attack.totalDamage() << " = "
      << std::setw(2) << std::right << attack.hitsplatOne << " + "
      << std::setw(2) << std::right << attack.hitsplatTwo << " + "
      << std::setw(2) << std::right << attack.hitsplatThree << " + "
      << std::setw(2) << std::right << attack.hitsplatFour << "\n";
    return;
  }
};

int main()
{
#if defined(NDEBUG)
  const bool verbose = false;
#else
  const bool verbose = true;
#endif

  const int numberOfAttempts { 10 };
  Simulation simulation(numberOfAttempts);

  const int attackSpeedTicks { 4 };
  const double accuracy { 0.4191 };
  const int maxhit { 33 };
  Weapon dragonClaws { attackSpeedTicks, accuracy, maxhit };

  simulation.generateAttacks(dragonClaws);

  if (verbose)
  {
    simulation.printAttacks();
  }

  double damageAverage = simulation.getAttacksMeanAverage();
  double damagePerTick = damageAverage / dragonClaws.attackSpeedTicks;
  double damagePerSecond = damagePerTick / 0.6;

  std::cout << "Simulated " << simulation.size() << " attacks\n"
    << "Average Hit: " << damageAverage << "\n"
    << "Dps: " << damagePerSecond << "\n";

  return 0;
}
