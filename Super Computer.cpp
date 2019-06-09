#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <tuple>

using namespace std;

int main()
{
    int N;
    vector<tuple<int,int>> Jobs;
    cin >> N; cin.ignore();
    tuple<int,int> range;
    
    for (int i = 0; i < N; i++) {
        int J;
        int D;
        cin >> J >> D; cin.ignore();
        range = make_tuple(J+D, J);
        Jobs.push_back(range);
    }
    
    //sort the intervals by their finishing time
    sort(Jobs.begin(), Jobs.end());
    
    //get the interval that has the earliest finishing time
    tuple<int,int> first = Jobs[0]; 
    int cut = 1; // How many prunes you do == the number of intervals in the solution
    
    for (auto const& job: Jobs){
        // If start time of the candidates is greater than the finishing time of the
        // of the first added interval, you've passed all intersecting candidates
        // and the next candidate is added and repeat
        if (get<1>(job) >= get<0>(first)){ 
            cut +=1;
            first = job;
        }
    }
    
    cout << cut << endl;
}