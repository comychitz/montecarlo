#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <iomanip>
#include <string>
#include <sstream>
#include <iostream>
#include <locale>

using namespace std;

/**
 * Simple Monte Carlo simulation for testing expectancy of a system over a
 * certain number of trades.  This simulation is structured in days, where one
 * or more trades are executed each day. Risk per day is divided by the number
 * of trades per day if you want to test distributing the risk equally among X
 * number of trades each day.
 */
const int STARTING_CAPITAL = 100000;
const float PERCENT_OF_CAPITAL_RISKED_PER_TRADE = 4;
const int NUM_TRADES_PER_DAY = 1;
const int TOTAL_NUM_DAYS = 250;
const long TOTAL_RUNS = 100000;

const float PERCENT_WIN = 55.0;
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


int main() {
  srand(time(NULL));

  long numRuns = 0;
  long totalCapital = 0;
  long numAccountsBlownOut = 0;
  long maxResultingCapital = 0;
  long minResultingCapital = STARTING_CAPITAL;
  long numAccountsLessThan50PercentOfStartingCapital = 0;

  while (numRuns++ < TOTAL_RUNS)
  {
    int numTrades = 0;
    float currentCapital = STARTING_CAPITAL;

    while (numTrades++ < TOTAL_NUM_DAYS)
    {
      float risk = (currentCapital * (PERCENT_OF_CAPITAL_RISKED_PER_TRADE / 100)) / NUM_TRADES_PER_DAY;

      for (int i = 0; i < NUM_TRADES_PER_DAY; i++)
      {
        bool win = rand() % 100 < PERCENT_WIN;
        if (win)
        {
          currentCapital += risk * WIN_AMOUNT;
        }
        else
        {
          currentCapital -= risk * LOSS_AMOUNT;
        }
      }

      if (currentCapital <= 0)
      {
        numAccountsBlownOut++;
        break;
      }
    }
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

  cout << "============== INPUT PARAMS ================" << endl;
  cout << "Starting capital: $" << formatWithCommas(STARTING_CAPITAL) << endl;
  cout << "Fixed fractional risk percentage: " << PERCENT_OF_CAPITAL_RISKED_PER_TRADE << "\%" << endl;
  cout << "Win percentage: " << PERCENT_WIN << "\%" << endl;
  cout << "Avg win (in terms of R): " << WIN_AMOUNT << endl;
  cout << "Avg loss (in terms of R): " << LOSS_AMOUNT << endl;
  cout << "Number of days: " << TOTAL_NUM_DAYS << endl;
  cout << "Number of trades per day: " << NUM_TRADES_PER_DAY << endl;
  cout << "Total number of simulations: " << formatWithCommas(TOTAL_RUNS) << endl;
  cout << "================= RESULTS ===================" << endl;
  cout << "Average resulting capital: $" << formatWithCommas(totalCapital/TOTAL_RUNS) << endl;
  cout << "Max resulting capital: $" << formatWithCommas(maxResultingCapital) << endl;
  cout << "Min resulting capital: $" << formatWithCommas(minResultingCapital) << endl;
  cout << "\% accounts <50\% of starting capital: " << float(numAccountsLessThan50PercentOfStartingCapital)/TOTAL_RUNS*100 << "\%" << endl;
  cout << "Number of accounts blown out: " << numAccountsBlownOut << endl;
  cout << "============================================" << endl;
  return 0;
}
