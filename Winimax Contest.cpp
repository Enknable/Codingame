#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <tuple>
#include <map>
#include <ctime>

using namespace std;
void printTuple(tuple<auto,auto> tup){
    cerr << get<0>(tup) << " " << get<1>(tup) << endl;
}
struct Course {
    map<tuple<int,int>,int> balls;
    vector<tuple<int,int>> hazards;
    map<tuple<int,int>,bool> holes;
    vector<string> layout;
    
};

void recurseBall(int lastDirection, tuple<int,int,int> ball, vector<string> layout, vector<tuple<int,int>> ballString, vector<vector<tuple<int,int>>>& finalBalls){
    int x = get<0>(ball);
    int y = get<1>(ball);
    int strokes = get<2>(ball);
    int height = layout.size();
    int width = layout[0].size();
    
    vector<string> Layout = layout;
    
    if (Layout[y][x] == 'H'){
        ballString.push_back(make_tuple(x,y));
        finalBalls.push_back(ballString);
        return;
    }
    
    if (strokes == 0)
        return;
    // lastDirection 0 = R, 1 = L, 2 = D, 3 = U
    if (x + strokes < width && lastDirection != 1)
        if (Layout[y][x+strokes] == '.' || Layout[y][x+strokes] == 'H'){ //right
            tuple<int,int,int> tempBall = make_tuple(x+strokes, y, strokes-1);
            vector<tuple<int,int>> tempBallString = ballString;
            tempBallString.push_back(make_tuple(x,y));
            recurseBall(0, tempBall, Layout, tempBallString, finalBalls);
        }
    if (x - strokes >= 0 && lastDirection != 0)
        if (Layout[y][x-strokes] == '.' || Layout[y][x-strokes] == 'H'){ //left
            tuple<int,int,int> tempBall = make_tuple(x-strokes, y, strokes-1);
            vector<tuple<int,int>> tempBallString = ballString;
            tempBallString.push_back(make_tuple(x,y));
            recurseBall(1, tempBall, Layout, tempBallString, finalBalls);
            
        }
    if (strokes + y < height && lastDirection != 3)
        if (Layout[y+strokes][x] == '.' || Layout[y+strokes][x] == 'H'){ //down
            tuple<int,int,int> tempBall = make_tuple(x, y+strokes, strokes-1);
            vector<tuple<int,int>> tempBallString = ballString;
            tempBallString.push_back(make_tuple(x,y));
            recurseBall(2, tempBall, Layout, tempBallString, finalBalls);
            
        }
    if (y - strokes >= 0 && lastDirection != 2)
        if (Layout[y-strokes][x] == '.' || Layout[y-strokes][x] == 'H'){ //up
            tuple<int,int,int> tempBall = make_tuple(x, y-strokes, strokes-1);
            vector<tuple<int,int>> tempBallString = ballString;
            tempBallString.push_back(make_tuple(x,y));
            recurseBall(3, tempBall, Layout, tempBallString, finalBalls);
            
        }
        
    return;
}
map<tuple<int,int>, vector<vector<tuple<int,int>>>> allPaths(vector<tuple<int,int,int>> ballContainer, Course course){
    map<tuple<int,int>, vector<vector<tuple<int,int>>>> finalPaths;
    
    for (auto const& balls: ballContainer){
        vector<vector<tuple<int,int>>> finalBalls;
        vector<tuple<int,int>> ballString;
        
        int x = get<0>(balls);
        int y = get<1>(balls);
        finalPaths[make_tuple(x,y)];
        
        recurseBall(-1, balls, course.layout, ballString, finalBalls);
        
        
        for (auto const& ball: finalBalls){
            vector<tuple<int,int>> tempBalls;
            for (auto path = ball.begin()+1; path != ball.end(); ++path){
                tempBalls.push_back(*path);
            }
            finalPaths[make_tuple(x,y)].push_back(tempBalls);
        }
                       
    }
    return finalPaths;
}
void printFinalPaths(map<tuple<int,int>, vector<vector<tuple<int,int>>>> finalPaths){
    for (auto const& ball : finalPaths){
        cerr << "Ball: ";
        printTuple(ball.first);
        cerr << endl;
        for (auto const& paths: ball.second){
            // if size == 1 it's a lock
            tuple<int,int> holeBall = paths.back();
            cerr << "Hole: ";
            printTuple(holeBall);
            for (auto const& hits: paths){
                printTuple(hits);
                continue;
            }
        }
        cerr << endl;
    }
}

vector<tuple<int,int>> expandTwoPoints(tuple<int,int> point1, tuple<int,int> point2){
    int x1,y1,x2,y2;
    vector<tuple<int,int>> expandedPoints;
    
    x1 = get<0>(point1);
    y1 = get<1>(point1);
    x2 = get<0>(point2);
    y2 = get<1>(point2);
    
    if (x1 == x2)
        if (y1 > y2)
            for (int i = y1; i > y2; i--)
                expandedPoints.push_back(make_tuple(x1,i));
        else
            for (int i = y1; i < y2; i++)
                expandedPoints.push_back(make_tuple(x1,i));
    else
        if (x1 > x2)
            for (int i = x1; i > x2; i--)
                expandedPoints.push_back(make_tuple(i,y1));
        else
            for (int i = x1; i < x2; i++)
                expandedPoints.push_back(make_tuple(i,y1));
                
    return expandedPoints;
    
}
vector<tuple<int,int>> expandFullPath(tuple<int,int> firstStop, vector<tuple<int,int>> fullPath){
    vector<tuple<int,int>> expandPoints;
    
    expandPoints = expandTwoPoints(firstStop, fullPath[0]);
    
    for (int i = 1; i < fullPath.size(); i++){
        vector<tuple<int,int>> expandTempPoints;
        expandTempPoints = expandTwoPoints(fullPath[i-1], fullPath[i]);
        expandPoints.insert(expandPoints.end(), expandTempPoints.begin(), expandTempPoints.end());
    }
    expandPoints.push_back(fullPath.back());
    return expandPoints;
}

void recurseHole(map<tuple<int,int>, vector<vector<tuple<int,int>>>> finalPaths, vector<tuple<int,int>> holderTemp, vector<vector<tuple<int,int>>>& holderFinal){
    
    if (finalPaths.empty()){
        holderFinal.push_back(holderTemp);
        return;
    }

    tuple<int,int> firstBall = finalPaths.begin()->first;
    vector<vector<tuple<int,int>>> firstBallPaths = finalPaths.begin()->second;
    
    if (count(holderTemp.begin(), holderTemp.end(), firstBall) > 0)
        return;
        
    for (auto path: firstBallPaths){
        vector<tuple<int,int>> tempHolderTemp = holderTemp;
        //expand the path
        vector<tuple<int,int>> expandedPath = expandFullPath(firstBall, path);
        for(auto const& tup: expandedPath){
            if (find(holderTemp.begin(), holderTemp.end(), tup) != holderTemp.end()){
                break;
            }
            if (tup == expandedPath.back()){
                //holderTemp.insert(expandFullPath(path));
                holderTemp.insert(holderTemp.end(), expandedPath.begin(), expandedPath.end());
                holderTemp.push_back(make_tuple(-1,-1));
                
                map<tuple<int,int>, vector<vector<tuple<int,int>>>> tempFinalPaths = finalPaths;
                
                tempFinalPaths.erase(tempFinalPaths.begin());
                recurseHole(tempFinalPaths, holderTemp, holderFinal);
            }
        }
        holderTemp = tempHolderTemp;
    }
    
    return;
    
}
void preProcessOnes(map<tuple<int,int>, vector<vector<tuple<int,int>>>>& finalPaths, map<tuple<int,int>, vector<tuple<int,int>>>& lockedIn, vector<string> layout){
    bool noOnes = true;
    while (noOnes){
        vector<tuple<int,int>> tempHolder;
        for (auto const& balls: finalPaths){
            if (balls.second.size() == 1){
                noOnes = true;
                lockedIn[balls.first] = balls.second[0];
                vector<tuple<int,int>> lockedFullPaths;
                lockedFullPaths = expandFullPath(balls.first, balls.second[0]);
                
                tempHolder.insert(tempHolder.end(), lockedFullPaths.begin(), lockedFullPaths.end());
                finalPaths.erase(balls.first);
                break;
                
            }
            if (finalPaths.empty()){
                noOnes = false;
                break;
            }
        }
        
        if (tempHolder.size() == 0){
            noOnes = false;
            break;
        }

        for (auto it = finalPaths.begin(); it != finalPaths.end(); ++it){
            tuple<int,int> trailBall = (*it).first;
            for (auto st = (*it).second.begin(); st != (*it).second.end();){
                if ((*st).size() == 0)
                    continue;
                vector<tuple<int,int>> lockedFullPaths;
                lockedFullPaths = expandFullPath(trailBall, (*st));
                
                for (auto const& tups: lockedFullPaths){
                    int x, y;
                    x = get<0>(tups);
                    y = get<1>(tups);
                    
                    if (find(tempHolder.begin(), tempHolder.end(), tups) != tempHolder.end()){
                        (*it).second.erase(st--);
                        break;
                    }
                }
                ++st;
            }
        }
    }
}
void printFinalSet(vector<vector<tuple<int,int>>> finalizedSet){
    for (auto const& set: finalizedSet){
        for (auto const& tup: set)
            printTuple(tup);
        cerr << endl;    
    }
}

vector<vector<tuple<int,int>>> convertLockedIn(map<tuple<int,int>, vector<tuple<int,int>>> lockedIn){
    vector<vector<tuple<int,int>>> finalizedSet;
    for (auto const& final: lockedIn){
        vector<tuple<int,int>> lockedFullPaths;
        lockedFullPaths = expandFullPath(final.first, final.second);
        
        vector<tuple<int,int>> subFinalizedSet;
        for (auto const& tup: lockedFullPaths)
            subFinalizedSet.push_back(tup);
            
        finalizedSet.push_back(subFinalizedSet);
    }
    return finalizedSet;
}
void printLockedIn(map<tuple<int,int>, vector<tuple<int,int>>> lockedIn){
    for (auto const& final: lockedIn){
        vector<tuple<int,int>> lockedFullPaths;
        lockedFullPaths = expandFullPath(final.first, final.second);
        
        for (auto const& tup: lockedFullPaths)
            printTuple(tup);
        cerr << endl;
    }
}

vector<vector<vector<tuple<int,int>>>> convertHolderFinal(vector<vector<tuple<int,int>>> holderFinal){
    vector<vector<vector<tuple<int,int>>>> possibleFinalizedSets;
    
    for (auto const& finalVec: holderFinal){
        vector<vector<tuple<int,int>>> onePossible;
        vector<tuple<int,int>> subFinal;
        for (auto const& final: finalVec){
            if (get<0>(final) != -1)
                subFinal.push_back(final);
            else if (!subFinal.empty()){
                onePossible.push_back(subFinal);
                subFinal.clear();
            }
        }
        if (!onePossible.empty())
            possibleFinalizedSets.push_back(onePossible);
    }
    return possibleFinalizedSets;
}
void printPossFinalSets(vector<vector<vector<tuple<int,int>>>> possibleFinalizedSets){
    for (auto const& possible: possibleFinalizedSets){
        for (auto const& onePossible: possible){
            for (auto const& tup: onePossible)
                printTuple(tup);
            cerr << endl;
        }
        cerr << endl;
    }
}

char chooseArrow(tuple<int,int> firstPoint, tuple<int,int> secondPoint){
    int x1, y1, x2, y2;
    
    x1 = get<0>(firstPoint);
    y1 = get<1>(firstPoint);
    x2 = get<0>(secondPoint);
    y2 = get<1>(secondPoint);
    
    if (x1 == x2)
        if (y1 > y2)
            return '^';
        else
            return 'v';
    else
        if (x1 > x2)
            return '<';
        else
            return '>';
}
void finalizeLayout(vector<vector<tuple<int,int>>> finalizedSet, vector<vector<vector<tuple<int,int>>>> possibleFinalizedSets, vector<string> Layout){
    
    vector<string> finalLayout = Layout;
    
    int height = 0;
    for (auto const& line: finalLayout){
        for (int i = 0; i < line.size(); i++){
            if (line[i] == 'X')
                finalLayout[height][i] = '.';
        }
        height++;
    }
    if (!finalizedSet.empty())
        for (auto oneDirection: finalizedSet){
            for (vector<tuple<int,int>>::size_type i = 0; i < oneDirection.size() - 1; i++){
                int x1,y1,x2,y2;
                
                char arrow = chooseArrow(oneDirection[i], oneDirection[i+1]);
                
                x1 = get<0>(oneDirection[i]);
                y1 = get<1>(oneDirection[i]);
                
                x2 = get<0>(oneDirection[i+1]);
                y2 = get<1>(oneDirection[i+1]);
                
                if (Layout[y1][x1] == 'H')
                    finalLayout[y1][x1] = '.';
                else
                    finalLayout[y1][x1] = arrow;
            }
            int x = get<0>(oneDirection.back());
            int y = get<1>(oneDirection.back());
            finalLayout[y][x] = '.';
            
            
        }
    
    if (!possibleFinalizedSets.empty())
        for (auto oneDirection: possibleFinalizedSets[0]){
            for (vector<tuple<int,int>>::size_type i = 0; i < oneDirection.size() - 1; i++){
                int x1,y1,x2,y2;
                
                char arrow = chooseArrow(oneDirection[i], oneDirection[i+1]);
                
                x1 = get<0>(oneDirection[i]);
                y1 = get<1>(oneDirection[i]);
                
                x2 = get<0>(oneDirection[i+1]);
                y2 = get<1>(oneDirection[i+1]);
                
                if (Layout[y1][x1] == 'H')
                    finalLayout[y1][x1] = '.';
                else
                    finalLayout[y1][x1] = arrow;
            }
            int x = get<0>(oneDirection.back());
            int y = get<1>(oneDirection.back());
            finalLayout[y][x] = '.';
        }
        
    
    for (auto const& line: finalLayout)
        cout << line << endl;
}

int main()
{
    Course course;
    int width;
    int height;
    cin >> width >> height; cin.ignore();
    
    vector<tuple<int,int,int>> ballContainer;
    map<tuple<int,int>, vector<vector<tuple<int,int>>>> finalPaths;
    
    for (int i = 0; i < height; i++) {
        string row;
        cin >> row; cin.ignore();
        course.layout.push_back(row);
        for (int j = 0; j < width; j++){
            tuple<int,int> pos = make_tuple(j,i);
            if (isdigit(row[j])){
                course.balls[pos] = (int)row[j] - 48;
                ballContainer.push_back(make_tuple(j, i, (int)row[j] - 48));
            }
            if (row[j] == 'X')
                course.hazards.push_back(pos);
            if (row[j] == 'H')
                course.holes[pos] = true;
        }
    }
    finalPaths = allPaths(ballContainer, course); // recursiving find every path each ball can take based only on landing spots

    map<tuple<int,int>, vector<tuple<int,int>>> lockedIn; //lockedIn are paths that are going to be used for sure
    preProcessOnes(finalPaths, lockedIn, course.layout); // loop through and save balls that only have one path option (guaranteed) then delete every path (expanded) that has any commonalities with that path (expanded) and loop until only multi path balls are left 
    //printFinalPaths(finalPaths); // finalPaths hold balls that have multiple options
    
    vector<vector<tuple<int,int>>> finalizedSet;
    finalizedSet = convertLockedIn(lockedIn); // convert to easy to use format
    //printFinalSet(finalizedSet);

    vector<tuple<int,int>> holderTemp;
    vector<vector<tuple<int,int>>> holderFinal; // holderFinal holds vectors of paths that have no intersections
    recurseHole(finalPaths, holderTemp, holderFinal); // recursively searches all expanded paths to find non overlapping paths (they're separated by (-1,-1) for ease within a complicated recursive function) and split later
    //printFinalSet(holderFinal);
    
    tuple<int,int> sepTup = make_tuple(-1,-1);
    
    vector<vector<vector<tuple<int,int>>>> possibleFinalizedSets;
    possibleFinalizedSets = convertHolderFinal(holderFinal); // convert to easy to use format (remove (-1,-1) splitter)
    //printPossFinalSets(possibleFinalizedSets);
    
    finalizeLayout(finalizedSet, possibleFinalizedSets, course.layout); // vector<tuples> to fill in the layout with arrow decision built in

    
}