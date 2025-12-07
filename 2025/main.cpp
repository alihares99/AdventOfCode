#include "useful.h"
#include "day01.h"
#include "day02.h"
#include "day03.h"
#include "day04.h"
#include "day05.h"
#include "day06.h"
#include "day07.h"
/*
#include "day08.h"
#include "day09.h"
#include "day10.h"
#include "day11.h"
#include "day12.h"
*/

unique_ptr<SolutionPair> get_solution(int day_number)
{
    SolutionPair *p = nullptr;
    switch (day_number)
    {
    case 1: p = new Day01;break;
    case 2: p = new Day02; break;
    case 3: p = new Day03; break;
    case 4: p = new Day04; break;
    case 5: p = new Day05; break;
    case 6: p = new Day06; break;
    case 7: p = new Day07; break;
    /*
    case 8: p = new Day08; break;
    case 9: p = new Day09; break;
    case 10: p = new Day10; break;
    case 11: p = new Day11; break;
    case 12: p = new Day12; break;
    */
    default:
        cout << "Solution " << day_number << " not implemented" << endl;
    }
    return unique_ptr<SolutionPair>(p);
}

int main(int argc, const char *argv[])
{
    if (argc == 1)
    {
        int i = 7;
        // for (int i = 1; i <= 22; i++)
        {
            string day = to_string(i);
            const char *argv_test[] = {"", day.c_str()};
            main(2, argv_test);
            cout << endl;
        }
        return 1;
    }

    if (argc != 2)
    {
        cout << "Only 1 input parameter is needed." << endl;
        return 1;
    }

    int day_number;
    try
    {
        day_number = stoi(argv[1]);
    }
    catch (...)
    {
        cout << argv[1] << " is not a number" << endl;
        return 1;
    }

    string input_file = "../inputs/input";
    input_file += (day_number < 10 ? "0" : "") + to_string(day_number) + ".txt";
    if (!filesystem::exists(input_file))
    {
        cout << "input file not found. expected file: " << input_file << endl;
        return 1;
    }

    unique_ptr<SolutionPair> solution = get_solution(day_number);
    if (solution)
    {
        auto result = solution->solve(input_file);
        cout << "day " << day_number << " part1 is: " << result.first << endl;
        cout << "day " << day_number << " part2 is: " << result.second << endl;
    }

    return 0;
}