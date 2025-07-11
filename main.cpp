#include <iomanip>
#include <iostream>
#include <numeric>
#include <random>
#include <string>
#include <vector>

struct Weapon 
{
  int attackSpeedTicks;
  double accuracy;
  int maxHit;
};

struct Attack 
{
  int firstHit;
  int secondHit;
  int thirdHit;
  int fourthHit;

  int totalDamage ()
  {
    return firstHit + secondHit + thirdHit + fourthHit;
  }
};

int randomDamage (int minHit, int maxHit, std::mt19937_64& randomDamageGenerator)
{
  std::uniform_int_distribution<int> randomDamageDistribution(minHit, maxHit);
  return randomDamageDistribution(randomDamageGenerator);
}

void printDamageRoll (int attemptWidth, int attempt, int totalDamage, int firstHit, int secondHit, int thirdHit, int fourthHit)
{
  std::cout << std::setw(attemptWidth) << std::right << attempt     << " : "
            << std::setw(2)            << std::right << totalDamage << " = "
            << std::setw(2)            << std::right << firstHit    << " + "
            << std::setw(2)            << std::right << secondHit   << " + "
            << std::setw(2)            << std::right << thirdHit    << " + "
            << std::setw(2)            << std::right << fourthHit   << "\n";
}

template <typename T, typename A>
double vectorMeanAverage (std::vector<T,A> const& vector)
{
   if (vector.empty()) { return 0; }
   return static_cast<double>(std::reduce(vector.begin(), vector.end())) / vector.size();
}

int main()
{
  const bool verbose = false;

  Weapon dragonClaws;
  dragonClaws.attackSpeedTicks = 4;
  dragonClaws.accuracy = 0.4191;
  dragonClaws.maxHit = 33;

  std::vector<int> simulation;
  const int simulationAttempts = 1000000;
  const int attemptsWidth = std::to_string(simulationAttempts).length();

  std::random_device rd1;
  std::mt19937_64 accuracyGenerator(rd1());
  std::uniform_real_distribution<double> accuracyDistribution(0.0, 1.0);

  std::random_device rd2;
  std::mt19937_64 randomDamageGenerator(rd2());

  for (size_t i = 0; i < simulationAttempts; i++)
  {
    int currentAttempt = i + 1;
    Attack currentAttack;

    if (dragonClaws.accuracy >= accuracyDistribution(accuracyGenerator)) 
    { // Pass first accuracy check
      currentAttack.firstHit = randomDamage(static_cast<int>(dragonClaws.maxHit * 0.5), dragonClaws.maxHit - 1, randomDamageGenerator);
      currentAttack.secondHit = static_cast<int>(currentAttack.firstHit * 0.5);
      currentAttack.thirdHit = static_cast<int>(currentAttack.secondHit * 0.5);
      currentAttack.fourthHit = currentAttack.thirdHit + 1;
      if (verbose) {
        printDamageRoll(attemptsWidth, currentAttempt, currentAttack.totalDamage(), currentAttack.firstHit, currentAttack.secondHit, currentAttack.thirdHit, currentAttack.fourthHit);
      }
    }
    else if (dragonClaws.accuracy >= accuracyDistribution(accuracyGenerator))
    { // Pass second accuracy check
      currentAttack.firstHit = 0;
      currentAttack.secondHit = randomDamage(static_cast<int>(dragonClaws.maxHit * 0.375), static_cast<int>(dragonClaws.maxHit * 0.875), randomDamageGenerator);
      currentAttack.thirdHit = static_cast<int>(currentAttack.secondHit * 0.5);
      currentAttack.fourthHit = currentAttack.thirdHit + 1;
      if (verbose) {
        printDamageRoll(attemptsWidth, currentAttempt, currentAttack.totalDamage(), currentAttack.firstHit, currentAttack.secondHit, currentAttack.thirdHit, currentAttack.fourthHit);
      }
    }
    else if (dragonClaws.accuracy >= accuracyDistribution(accuracyGenerator))
    { // Pass third accuracy check
      currentAttack.firstHit = 0;
      currentAttack.secondHit = 0;
      currentAttack.thirdHit = randomDamage(static_cast<int>(dragonClaws.maxHit * 0.25), static_cast<int>(dragonClaws.maxHit * 0.75), randomDamageGenerator);
      currentAttack.fourthHit = currentAttack.thirdHit + 1;
      if (verbose) {
        printDamageRoll(attemptsWidth, currentAttempt, currentAttack.totalDamage(), currentAttack.firstHit, currentAttack.secondHit, currentAttack.thirdHit, currentAttack.fourthHit);
      }
    }
    else if (dragonClaws.accuracy >= accuracyDistribution(accuracyGenerator))
    { // Pass fourth accuracy check
      currentAttack.firstHit = 0;
      currentAttack.secondHit = 0;
      currentAttack.thirdHit = 0;
      currentAttack.fourthHit = randomDamage(static_cast<int>(dragonClaws.maxHit * 0.25), static_cast<int>(dragonClaws.maxHit * 1.25), randomDamageGenerator);
      if (verbose) {
        printDamageRoll(attemptsWidth, currentAttempt, currentAttack.totalDamage(), currentAttack.firstHit, currentAttack.secondHit, currentAttack.thirdHit, currentAttack.fourthHit);
      }
    }
    else
    { // Fail all accuracy checks
      if (randomDamage(0, 2, randomDamageGenerator) < 2)
      {
        currentAttack.firstHit = 0;
        currentAttack.secondHit = 0;
        currentAttack.thirdHit = 1;
        currentAttack.fourthHit = 1;
        if (verbose) {
          printDamageRoll(attemptsWidth, currentAttempt, currentAttack.totalDamage(), currentAttack.firstHit, currentAttack.secondHit, currentAttack.thirdHit, currentAttack.fourthHit);
        }
      }
      else
      {
        currentAttack.firstHit = 0;
        currentAttack.secondHit = 0;
        currentAttack.thirdHit = 0;
        currentAttack.fourthHit = 0;
        if (verbose) {
          printDamageRoll(attemptsWidth, currentAttempt, currentAttack.totalDamage(), currentAttack.firstHit, currentAttack.secondHit, currentAttack.thirdHit, currentAttack.fourthHit);
        } 
      }
    }

    simulation.push_back(currentAttack.totalDamage());
  }

  double damageAverage = vectorMeanAverage(simulation);
  double damagePerTick = damageAverage / dragonClaws.attackSpeedTicks;
  double damagePerSecond = damagePerTick / 0.6;

  std::cout << "\n"
            << "Simulated "    << simulation.size() << " attacks\n"
            << "Average Hit: " << damageAverage     << "\n"
            << "Dps: "         << damagePerSecond   << "\n";

  return 0;
}