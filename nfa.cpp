#include <iostream>
#include <set>
#include <map>
#include <utility>
#include <string>
#include <iterator>
#include <queue>
using namespace std;

class NFA{
    private:
        int currentState;
        // utility functions

        // Building Alphabets
        void buildAlphabet(string RE){
            RE = compactRegexGeneration(RE);
            for(int i = 0; i < RE.size(); i++){
                if(RE[i] != '+' && RE[i] != '*' && RE[i] != '(' && RE[i] != ')' && RE[i] != ' ') characters.insert(RE[i]);
                characters.insert('E');
            }
        }

        // Compact form of REGEX
        string compactRegexGeneration(string Regex){
        	string compact="";
	        for(int i=0;i<Regex.size();i++){
		        if(Regex[i] == ' ') continue;
    		    compact+=Regex[i];
	        }
	        return compact;
        }
    
        int generateNFA(int subInitialState, string RE, int i){
            int closeState = 0;
            for(;i < RE.size(); i++){
                if(characters.find(RE[i]) != characters.end() && i < RE.size()-1){
                    if(RE[i+1] != '*') {
                        transitionFunction[make_pair(currentState,RE[i])].insert(stateNum+1);
                        currentState=++stateNum;
                        setOfStates.insert(currentState);
                    }
                    else{
                        transitionFunction[make_pair(currentState,RE[i])].insert(stateNum);
                        i++;
                    }
                }
                else if(characters.find(RE[i]) != characters.end()) {
                    transitionFunction[make_pair(stateNum,RE[i])].insert(stateNum++);
                    currentState = stateNum;
                    setOfStates.insert(currentState);
                }
                else if(RE[i] == '+'){

                    if(closeState == 0){
                        currentState = ++stateNum;
                        setOfStates.insert(currentState);
                        closeState = currentState;
                        currentState = subInitialState;
                        transitionFunction[make_pair(stateNum-1, 'E')].insert(closeState);
                    } else{
                        transitionFunction[make_pair(stateNum, 'E')].insert(closeState);
                        currentState = subInitialState;
                    }
                }
                else if(RE[i] == '('){
                    int _subInitialState = currentState;
                    i = generateNFA(_subInitialState, RE, i+1);
                }
                else if(RE[i] == ' ') continue;
                else{ // RE[i] = ')'
                    if(i < RE.size()-1 && RE[i+1] == '*'){
                        if(closeState == 0) {
                            transitionFunction[make_pair(stateNum,'E')].insert(subInitialState);
                        }
                        else{
                            transitionFunction[make_pair(currentState, 'E')].insert(closeState);
                            transitionFunction[make_pair(closeState,'E')].insert(subInitialState);
                        }
                        i += 1;
                        currentState = subInitialState;
                    }
                    else{
                        if(closeState != 0) {
                            transitionFunction[make_pair(currentState, 'E')].insert(closeState);
                            currentState = closeState;
                        }
                    }
                    return i;
                }
            }
        }

        set<int> isValidUtility(char w, int state){
            set<int> frontier;
            if(transitionFunction.find(make_pair(state, w)) != transitionFunction.end()){
                frontier.insert(transitionFunction[make_pair(state, w)].begin(), transitionFunction[make_pair(state, w)].end());
            }

            if(transitionFunction.find(make_pair(state, 'E')) != transitionFunction.end()){
                //set<int>::iterator itr = transitionFunction[make_pair(state, 'E')].begin();
                set<int> eclose = findEpsilonClosure(state);
                if(eclose.find(state) == eclose.end()){
                    frontier.insert(eclose.begin(), eclose.end());
                }

                /*while(itr!=transitionFunction[make_pair(state, 'E')].end()){
                    set<int> temp = isValidUtility(w,*itr);
                    frontier.insert(temp.begin(), temp.end());
                    itr++;
                }*/
            }
            cout << "cdvf";
            printSetOfStates(frontier);
            return frontier;
        }

        set<int> findEpsilonClosure(int state){
            set<int> eclose;
            set<int>::iterator itr;
            if(transitionFunction.find(make_pair(state, 'E')) != transitionFunction.end())
                eclose.insert(transitionFunction[make_pair(state, 'E')].begin(), transitionFunction[make_pair(state, 'E')].end());
            for(itr = eclose.begin(); itr != eclose.end(); itr++){
                if(transitionFunction.find(make_pair(*itr, 'E')) != transitionFunction.end())
                eclose.insert(transitionFunction[make_pair(*itr, 'E')].begin(), transitionFunction[make_pair(*itr, 'E')].end());
            }
            printSetOfStates(eclose);
            return eclose;
        }

        set<int> epsilonTransitionsOnState(int states){
            set<int> state;
            state.insert(states);
            if(transitionFunction.find(make_pair(states, 'E')) != transitionFunction.end()){
                set<int>::iterator itr = transitionFunction[make_pair(states, 'E')].begin();
                for(; itr != transitionFunction[make_pair(states, 'E')].end(); itr++){
                    state.insert(*itr);
                    set<int> temp = epsilonTransitionsOnState(*itr);
                    state.insert(temp.begin(), temp.end());
                }
            }
            return state;
        }

    public:
        // 5-tuple
        set<int> setOfStates;
        set<char> characters;
        set<int> acceptingStates;
        int startState;
        map<pair<int, char>, set<int> > transitionFunction;

        NFA(){

        }

        int stateNum;
        void buildNFA(string RE){
            buildAlphabet(RE);
            stateNum = 0;
            startState = 0; setOfStates.insert(0);
            currentState = 0;

            int initialState = 0; int closeState = 0;
            for(int i = 0; i < RE.size(); i++){
                if(characters.find(RE[i]) != characters.end() && i < RE.size()-1){
                    if(RE[i+1] != '*') {
                        transitionFunction[make_pair(currentState,RE[i])].insert(stateNum+1);
                        currentState=++stateNum;
                        setOfStates.insert(currentState);
                    }
                    else{
                        transitionFunction[make_pair(currentState,RE[i])].insert(stateNum);
                        i++;
                    }
                }
                else if(characters.find(RE[i]) != characters.end()) {
                    if(closeState != 0){
                        transitionFunction[make_pair(currentState,RE[i])].insert(stateNum+1);
                        currentState = ++stateNum;
                        setOfStates.insert(currentState);
                        if(i == RE.size()-1){
                            transitionFunction[make_pair(currentState, 'E')].insert(closeState);
                            currentState = closeState;
                        }
                    }
                    else{
                        transitionFunction[make_pair(currentState,RE[i])].insert(stateNum+1);
                        currentState = ++stateNum;
                        setOfStates.insert(currentState);
                    }
                }
                else if(RE[i] == '+'){

                    if(closeState == 0){
                        currentState = ++stateNum;
                        setOfStates.insert(currentState);
                        closeState = currentState;
                        currentState = initialState;
                        transitionFunction[make_pair(stateNum-1, 'E')].insert(closeState);
                        acceptingStates.insert(stateNum);
                    }
                    else{
                        transitionFunction[make_pair(stateNum, 'E')].insert(closeState);
                        currentState = initialState;
                    }
                }
                else if(RE[i] == '('){
                    int subInitialState = currentState;

                    i = generateNFA(subInitialState, RE, i+1);
                    if((RE[i] == '*' && i == RE.size()-1) || (RE[i] == '*' && i < RE.size()-1 && characters.find(RE[i+1]) == characters.end()))
                        acceptingStates.insert(currentState);
                    closeState = 0;
                }
            }
            acceptingStates.insert(currentState);
        }

        bool isValid(string w){
            for(int i = 0; i < w.size(); i++){
                if(characters.find(w[i]) == characters.end()) return false;
            }

            set<int> finalStates = isValidUtility(w[0], 0);
            int i = 1;
            while(!finalStates.empty() && i < w.size()){
                set<int> tempFinal;
                set<int>::iterator itr = finalStates.begin();
                while(itr != finalStates.end()){
                    set<int> temp = isValidUtility(w[i],*itr);
                    tempFinal.insert(temp.begin(), temp.end());
                    itr++;
                }
                finalStates.swap(tempFinal);
                i++;
            }

            set<int> temporary;
            set<int>::iterator finalCheck = finalStates.begin();
            for(;finalCheck != finalStates.end(); finalCheck++){
                set<int> temps = epsilonTransitionsOnState(*finalCheck);
                temporary.insert(temps.begin(), temps.end());
            }
            temporary.swap(finalStates);
            finalCheck = finalStates.begin();
            for(; finalCheck != finalStates.end(); finalCheck++){
                if(acceptingStates.find(*finalCheck) != acceptingStates.end()) return true;
            }
            return false;
        }

        string Dijkstra(int beginState){
            vector<string> strings;
            for(int i = 0; i < setOfStates.size(); i++) strings.push_back("+");
            strings[beginState] = "";
            queue<pair<int, string> > Q;
            Q.push(make_pair(beginState, strings[beginState]));
            while(!Q.empty()){
                pair<int, string> x = Q.front();
                Q.pop();

                set<char>::iterator itr;
                for(itr = characters.begin(); itr != characters.end(); itr++){
                    if(transitionFunction.find(make_pair(x.first, *itr)) != transitionFunction.end()){
                        // relax for all out-neighbors
                        set<int> outNeighbours = transitionFunction[make_pair(x.first, *itr)];
                        set<int>::iterator ii;
                        for(ii = outNeighbours.begin(); ii != outNeighbours.end(); ii++){
                            string y = strings[x.first] + (*itr);
                            if(*itr == 'E') y = strings[x.first];
                            string z = strings[*ii];
                            if(z == "+"){
                                strings[*ii] = y;
                                Q.push(make_pair(*ii, strings[*ii]));
                            }
                            else{
                                if(strings[*ii].size() > y.size()) strings[*ii] = y;
                            }
                        }
                    }
                }
            }
            set<int>::iterator it = acceptingStates.begin();
            string shortest = strings[*it];
            for(; it != acceptingStates.end(); it++){
                if(shortest.size() > strings[*it].length()) shortest = strings[*it];
            }
            return shortest;
        }

        void printTransitionFunction(){
            map<pair<int, char>, set<int> >::iterator itr;
            set<int>::iterator delta;
            for(itr = transitionFunction.begin(); itr != transitionFunction.end(); itr++){
                cout << "(" << itr->first.first << ", " << itr->first.second << "): ";
                for(delta = itr->second.begin(); delta != itr->second.end(); delta++){
                    cout << *delta << " ";
                }
                cout << endl;
            }
            cout << endl;
        }

        void printSetOfStates(set<int> s){
            set<int>::iterator it = s.begin();
            for(; it != s.end(); it++) cout << *it << " ";
            cout << endl;
        }
};

int main(){
    NFA N;
    //N.buildNFA("aaab+ba(ab)*");
    N.buildNFA("(a+(ab)*)*");
    N.printTransitionFunction();
    cout << "Accepting State(s): ";
    N.printSetOfStates(N.acceptingStates);
    cout << endl;

    string w;
    cout << "Enter the string: ";
    cin >> w;
    if(N.isValid(w)) cout << w << " is accepted by the NFA\n";
    else cout << w << " is not accepted by the NFA\n";

    string shortest = N.Dijkstra(0);
    shortest = shortest=="+"||shortest=="" ? "the empty string" : shortest;
    cout << "The shortest string accepted by the NFA/Regex is: " << shortest << endl;
}
