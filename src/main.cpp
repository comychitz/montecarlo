#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <iomanip>
#include <string>
#include <sstream>
#include <iostream>
#include <locale>
#include <vector>
#include <cmath>

using namespace std;

/**
 * Simple Monte Carlo simulation for testing expectancy of a system over a
 * certain number of trades.  
 */
const int STARTING_CAPITAL = 100000;
const float PERCENT_OF_CAPITAL_RISKED_PER_TRADE = 2;
const int TOTAL_NUM_TRADES = 250;
const long TOTAL_RUNS = 100000;

const float PERCENT_WIN = 50.0;
const float WIN_AMOUNT = 1.2;
const float LOSS_AMOUNT = 1;

// courtesy of: https://stackoverflow.com/questions/7276826/format-number-with-commas-in-c
template<class T>
string formatWithCommas(T value)
{
    stringstream ss;
    ss.imbue(locale(""));
    ss << fixed << value;
    return ss.str();
}

long calcVariance(long avg, vector<long> nums) 
{
  long variance = 0;
  for (long i = 0; i < nums.size(); i++) {
    variance += pow((avg - nums.at(i)), 2);
  }
  return variance / nums.size();
}


int main() {

  long numRuns = 0;
  long totalCapital = 0;
  vector<long> resultingCapitals;
  long numAccountsBlownOut = 0;
  long maxResultingCapital = 0;
  long minResultingCapital = STARTING_CAPITAL;
  long numAccountsLessThan50PercentOfStartingCapital = 0;

  while (numRuns++ < TOTAL_RUNS)
  {
    srand(numRuns);
    int numTrades = 0;
    float currentCapital = STARTING_CAPITAL;

    while (numTrades++ < TOTAL_NUM_TRADES)
    {
      float risk = (currentCapital * (PERCENT_OF_CAPITAL_RISKED_PER_TRADE / 100));
      bool win = rand() % 100 < PERCENT_WIN;
      if (win)
      {
        currentCapital += risk * WIN_AMOUNT;
      }
      else
      {
        currentCapital -= risk * LOSS_AMOUNT;
      }

      if (currentCapital <= 0)
      {
        numAccountsBlownOut++;
        break;
      }
    }

    resultingCapitals.push_back(currentCapital);

    if (currentCapital < STARTING_CAPITAL / 2) {
      numAccountsLessThan50PercentOfStartingCapital++;
    }
    if (currentCapital > maxResultingCapital) {
      maxResultingCapital = currentCapital;
    }
    if (currentCapital < minResultingCapital) {
      minResultingCapital = currentCapital;
    }
    totalCapital += currentCapital;
  }

  long avgResultingCapital = totalCapital/TOTAL_RUNS;
  long variance = calcVariance(avgResultingCapital, resultingCapitals);
  long standardDev = (long) sqrt(variance);

  cout << "============== INPUT PARAMS ================" << endl;
  cout << "Starting capital: $" << formatWithCommas(STARTING_CAPITAL) << endl;
  cout << "Fixed fractional risk percentage: " << PERCENT_OF_CAPITAL_RISKED_PER_TRADE << "\%" << endl;
  cout << "Win percentage: " << PERCENT_WIN << "\%" << endl;
  cout << "Avg win (in terms of R): " << WIN_AMOUNT << endl;
  cout << "Avg loss (in terms of R): " << LOSS_AMOUNT << endl;
  cout << "Total number of trades: " << TOTAL_NUM_TRADES << endl;
  cout << "Total number of simulations: " << formatWithCommas(TOTAL_RUNS) << endl;
  cout << "================= RESULTS ===================" << endl;
  cout << "Average resulting capital: $" << formatWithCommas(avgResultingCapital) << endl;
  cout << "Max resulting capital: $" << formatWithCommas(maxResultingCapital) << endl;
  cout << "Min resulting capital: $" << formatWithCommas(minResultingCapital) << endl;
  cout << "Standard Deviation: " << formatWithCommas(standardDev) << endl;
  cout << "Coefficient of variation: " << setprecision(2) << ((float) standardDev)/avgResultingCapital << endl;
  cout << "\% accounts <50\% of starting capital: " << float(numAccountsLessThan50PercentOfStartingCapital)/TOTAL_RUNS*100 << "\%" << endl;
  cout << "Number of accounts blown out: " << numAccountsBlownOut << endl;
  cout << "============================================" << endl;
  return 0;
}
