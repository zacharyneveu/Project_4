// Project 4: Solving knapsack using greedy algorithms
// p4.cpp
// Bobby Ky & Zach Neveu

#include <iostream>
#include <limits.h>
#include <list>
#include <fstream>
#include <queue>
#include <vector>
#include <time.h>
#include <stack>
#include <cmath>

using namespace std;

#include "d_except.h"
#include "d_matrix.h"
#include "knapsack.h"

knapsack bnb(knapsack &start);

int main()
{
	char x;
	ifstream fin;
	stack <int> moves;
	string fileName;

	// Read the name of the file from the keyboard or
	// hard code it here for testing.

	//fileName = "instances/knapsack12.input";

	cout << "Enter filename" << endl;
	cin >> fileName;

	fin.open(fileName.c_str());
	if (!fin)
	{
		cerr << "Cannot open " << fileName << endl;
		exit(1);
	}

	try
	{
		cout << "Reading knapsack instance" << endl;
		knapsack k(fin);

		knapsack best = bnb(k);

		//k.printSolution();
		cout << endl << "Output file created." << endl;


	}    

	catch (indexRangeError &ex) 
	{ 
		cout << ex.what() << endl; exit(1);
	}
	catch (rangeError &ex)
	{
		cout << ex.what() << endl; exit(1);
	}

}

knapsack bnb(knapsack &start)
{
	//cout <<	"Starting bound: " << start.bound() << endl;
	deque<knapsack> d;
	d.push_back(start);
	knapsack best = start;
	bool backtracking = false;
	clock_t startt = clock();

	while(!d.empty())
	{
		// clock logic to quit after 10 minutes
		auto diff_seconds = (float)(clock() - startt) / CLOCKS_PER_SEC;
		if(diff_seconds > 600)
		{
			cout << "Reached 600 seconds time limit" << endl;
			best.printSolution();
			return best;
		}


		// Each level pushes an item into stack
		auto itnum = d.size()==1 ?  0 : d.size()-2;
		auto vec = start.getKnapsackVector(itnum);
		auto idx = vec[0];

		// If all items have been fixed
		if(itnum == start.getNumObjects()-1 && !backtracking)
		{
			if(d.back().getValue() > best.getValue())
			{
				best = d.back();
			}
			//cout << "best value: " << best.getValue() << endl;
			backtracking = true;
		}
		else //if not all items have been fixed yet
		{
			// Select next item
			if(!backtracking)
			{
				//cout << "not backtracking..." << endl;
				knapsack last = d.back();
				// If it would break the cost limit
				if(last.getCost() + last.getCost(idx) > last.getCostLimit())
				{
					// push current knapsack (next item set to 0)
					d.push_back(last);
				}
				else
				{
					last.select(idx);
					//cout << "Setting item "<<itnum << " and pushing to deque" << endl;
					d.push_back(last);
				}
			}
			else // if backtracking
			{
				if(itnum <= 0)
					break;
				//cout << "Backtracking" << endl;
				//cout << "itnum: " <<itnum << endl;

				knapsack last = d.back();
				auto lastvec = start.getKnapsackVector(itnum-1);
				auto lastidx = lastvec[0];
				if(last.isSelected(lastidx))
				{
					//cout << "Unselecting "<< itnum-1 << endl;
					last.unSelect(lastidx);
					d.pop_back();
					d.push_back(last);

					// Explore if possibly good
					if(last.bound() > best.getValue())
					{
						//cout << "Exploring branch from itnum: "<<itnum<<"with bound: " << last.bound() << endl;
						backtracking = false;
					}
					else
					{
						//cout << "bound of "<<last.bound() << " <= "<< best.getValue() << endl;
					}
				}
				else
				{
					d.pop_back();
				}
			}
		}
	}
	cout << "Time (s): "<<(float)(clock() - startt) / CLOCKS_PER_SEC<<endl;
	best.printSolution();
	for(int i; i < best.getNumObjects(); ++i)
	{
		cout << "item " << i << ": " << best.isSelected(i) << endl;
	}
	return(best);
}
