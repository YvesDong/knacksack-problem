// an algorithmic technique for solving a problem by recursively breaking it down into simpler subproblems,
// and using the fact that the optimal solution to the overall problem depends upon the optimal solution to it's individual subproblems.

// DP algorithm solves each subproblem just once and then remembers its answer,
// thereby avoiding re-computation of the answer for similar subproblem every time.

// It also gives us a life lesson - Make life less complex. 

// The knapsack problem is a problem in combinatorial optimization: 
// Given a set of items, each with a weight and a value, 
// determine the number of each item to include in a collection so that the total weight is less than or equal to a given limit and the total value is as large as possible. 

#include <iostream>
#include <iterator>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
using namespace std;  
 
#define N 20 // no. of drivers
#define S 120 // salary upper bound
#define L 5 // max no. of drivers in a team
#define V 10*S

struct drivers_input
{
	std::vector<std::string> name = {"None"};
	std::vector<int> prize_ten = {0}; // multiplied by 10 for convenience
	std::vector<int> point = {0}; // the point that each driver earns
};

drivers_input read_csv();

int main()  
{  
	// user interface
	int s_usr;
	do 
	{
		cout << "Please input the budge of your team (in million, between 0 and 120): " << endl;
		cin >> s_usr;
	} while (s_usr < 0 || s_usr > 120);
	int v = 10 * s_usr;
	
	// load drivers info from csv
	drivers_input input = read_csv();
	std::vector<std::string> name = input.name;
	std::vector<int> prize_ten = input.prize_ten;
	std::vector<int> point = input.point;
	
	int f[N + 1][V + 1][L + 1] = {}; // f[i][j][k] means the max sum of points that the first i drivers earn, when the salary limit is j and k drivers per team
	int g[N + 1][V + 1][L + 1] = {}; // buffer of recording current best drivers 
	int l[N + 1][V + 1][L + 1] = {}; // current number of drivers

	for (int i = 1; i <= N; i++)
	{
		for (int j = 1; j <= V; j++)
		{
			for (int k = 1; k <= L; k++)
			{
				if (j < prize_ten[i])
				{
					f[i][j][k] = f[i - 1][j][k];
					g[i][j][k] = g[i - 1][j][k]; 
					l[i][j][k] = l[i - 1][j][k];
				}
				else
				{
					int cons_f = f[i - 1][j][k];
					int aggr_f = f[i - 1][j - prize_ten[i]][k - 1] + point[i];
					int cons_no = l[i - 1][j][k];
					int aggr_no = l[i - 1][j - prize_ten[i]][k - 1] + 1;
					std::string aggr_buff = std::to_string(g[i - 1][j - prize_ten[i]][k - 1]);
					std::string po = i < 10 ? ("0" + std::to_string(i)) : std::to_string(i);
                    
					// either go aggressively or conservatively. 
					// aggression means picking the current driver i and previous drivers aggr_buff,
					// when (1) the number of total drivers increases or (2) the number ties but the total points increase
					if (cons_no < aggr_no || (cons_no == aggr_no && cons_f < aggr_f))
					{
						f[i][j][k] = aggr_f;
						g[i][j][k] = std::stoi(aggr_buff + po); // leads to missing 0 at the start of g()
						l[i][j][k] = aggr_no;
					}
					// go conservatively and pick the driver combination g[i - 1][j][k] in the last iteration
					else
					{
						f[i][j][k] = cons_f;
						g[i][j][k] = g[i - 1][j][k];
						l[i][j][k] = cons_no;
					}
				}
                
				// if (j == V) // for debug
				// {
				//     cout << i << "\t" << j/10 << "\t" << k << "\t" << f[i][j][k] << "\t" << l[i][j][k] << "\t" << g[i][j][k] << endl;
				// }
			}
		}
	}

	// print out the optimal solutions
	if (l[N][v][L] < L)
		cout << s_usr << " million is not enough to pay five drivers!" << endl;
	else
	{
		cout << "The total points earned by the optimal drivers: " << f[N][v][L] << endl;
		cout << "The drivers are: " << endl;
		int no[L] = {};
		string allstr = std::to_string(g[N][v][L]);
		string headstr = allstr;
		int hdsize = allstr.size();
		int h = 0;
		while (headstr.size() >= 2)
		{
			string tailstr = headstr.substr(headstr.size() - 2);
			headstr = allstr.substr(0, hdsize - 2 * h - 2);
			no[h] = std::stoi(tailstr);
			cout << "\t" << name[no[h]] << ": " << point[no[h]] << " points" << endl;
			h++;
		}
		if (no[L-1] == 0)
		{
			no[L-1] = std::stoi(headstr);
			cout << "\t" << name[no[L-1]] << ": " << point[no[L-1]] << " points" << endl;
		}
	}

	return 0;  
}

drivers_input read_csv()
{
	drivers_input input;

	std::ifstream ifile("data.csv");

	std::string line; 
	while (std::getline(ifile, line))
	{
		std::istringstream iss{line};
		std::vector<std::string> tokens;
		std::string token;
		while (std::getline(iss, token, ','))
			tokens.push_back(token);

		std::string name = tokens[0];
		int prize_ten = std::stof(tokens[1])*10;
		int point = std::stoi(tokens[2]);
        
		input.name.push_back(name);
		input.prize_ten.push_back(prize_ten);
		input.point.push_back(point);
	}
	    
	return input;
}
