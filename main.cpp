#include <iomanip>
#include <iostream>
#include <numeric>
#include <random>
#include <string>
#include <vector>

struct Weapon {
  int attackSpeedTicks;
  double accuracy;
  int maxHit;
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
    int firstHit;
    int secondHit;
    int thirdHit;
    int fourthHit;
    int totalDamage;

    if (dragonClaws.accuracy >= accuracyDistribution(accuracyGenerator)) 
    { // Pass first accuracy check
      firstHit = randomDamage(static_cast<int>(dragonClaws.maxHit * 0.5), dragonClaws.maxHit - 1, randomDamageGenerator);
      secondHit = static_cast<int>(firstHit * 0.5);
      thirdHit = static_cast<int>(secondHit * 0.5);
      fourthHit = thirdHit + 1;
      totalDamage = firstHit + secondHit + thirdHit + fourthHit;
      if (verbose) {
        printDamageRoll(attemptsWidth, currentAttempt, totalDamage, firstHit, secondHit, thirdHit, fourthHit);
      }
    }
    else if (dragonClaws.accuracy >= accuracyDistribution(accuracyGenerator))
    { // Pass second accuracy check
      firstHit = 0;
      secondHit = randomDamage(static_cast<int>(dragonClaws.maxHit * 0.375), static_cast<int>(dragonClaws.maxHit * 0.875), randomDamageGenerator);
      thirdHit = static_cast<int>(secondHit * 0.5);
      fourthHit = thirdHit + 1;
      totalDamage = firstHit + secondHit + thirdHit + fourthHit;
      if (verbose) {
        printDamageRoll(attemptsWidth, currentAttempt, totalDamage, firstHit, secondHit, thirdHit, fourthHit);
      }
    }
    else if (dragonClaws.accuracy >= accuracyDistribution(accuracyGenerator))
    { // Pass third accuracy check
      firstHit = 0;
      secondHit = 0;
      thirdHit = randomDamage(static_cast<int>(dragonClaws.maxHit * 0.25), static_cast<int>(dragonClaws.maxHit * 0.75), randomDamageGenerator);
      fourthHit = thirdHit + 1;
      totalDamage = firstHit + secondHit + thirdHit + fourthHit;
      if (verbose) {
        printDamageRoll(attemptsWidth, currentAttempt, totalDamage, firstHit, secondHit, thirdHit, fourthHit);
      }
    }
    else if (dragonClaws.accuracy >= accuracyDistribution(accuracyGenerator))
    { // Pass fourth accuracy check
      firstHit = 0;
      secondHit = 0;
      thirdHit = 0;
      fourthHit = randomDamage(static_cast<int>(dragonClaws.maxHit * 0.25), static_cast<int>(dragonClaws.maxHit * 1.25), randomDamageGenerator);
      totalDamage = firstHit + secondHit + thirdHit + fourthHit;
      if (verbose) {
        printDamageRoll(attemptsWidth, currentAttempt, totalDamage, firstHit, secondHit, thirdHit, fourthHit);
      }
    }
    else
    { // Fail all accuracy checks
      if (randomDamage(0, 2, randomDamageGenerator) < 2)
      {
        firstHit = 0;
        secondHit = 0;
        thirdHit = 1;
        fourthHit = 1;
        totalDamage = firstHit + secondHit + thirdHit + fourthHit;
        if (verbose) {
          printDamageRoll(attemptsWidth, currentAttempt, totalDamage, firstHit, secondHit, thirdHit, fourthHit);
        }
      }
      else
      {
        firstHit = 0;
        secondHit = 0;
        thirdHit = 0;
        fourthHit = 0;
        totalDamage = firstHit + secondHit + thirdHit + fourthHit;
        if (verbose) {
          printDamageRoll(attemptsWidth, currentAttempt, totalDamage, firstHit, secondHit, thirdHit, fourthHit);
        } 
      }
    }

    simulation.push_back(totalDamage);
  }

  double damageAverage = vectorMeanAverage(simulation);
  double damagePerSecond = (damageAverage / dragonClaws.attackSpeedTicks) / 0.6;

  std::cout << "\n"
            << "Simulated "    << simulation.size() << " attacks\n"
            << "Average Hit: " << damageAverage     << "\n"
            << "Dps: "         << damagePerSecond   << "\n";

  return 0;
}