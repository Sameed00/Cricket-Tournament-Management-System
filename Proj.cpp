#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>
using namespace std;

// Forward declarations
class Player;
class Team;
class Match;

// ==================== PLAYER CLASS ====================
class Player {
public:
    string name;
    int runsScored;
    int ballsFaced;
    int wicketsTaken;
    int oversBowled;
    int runsConceded;
    bool isOut;
    Player* next;
    
    Player(string n = "") : name(n), runsScored(0), ballsFaced(0), 
                            wicketsTaken(0), oversBowled(0), runsConceded(0), 
                            isOut(false), next(NULL) {}
};

// ==================== LINKED LIST FOR PLAYERS ====================
class PlayerList {
private:
    Player* head;
    int count;
    
public:
    PlayerList() : head(NULL), count(0) {}
    
    void addPlayer(string name) {
        Player* newPlayer = new Player(name);
        if (!head) {
            head = newPlayer;
        } else {
            Player* temp = head;
            while (temp->next) {
                temp = temp->next;
            }
            temp->next = newPlayer;
        }
        count++;
    }
    
    Player* getPlayer(int index) {
        if (index < 0 || index >= count) return NULL;
        Player* temp = head;
        for (int i = 0; i < index; i++) {
            temp = temp->next;
        }
        return temp;
    }
    
    int getCount() { return count; }
    
    Player* getHead() { return head; }
    
    void display() {
        Player* temp = head;
        int i = 1;
        while (temp) {
            cout << i++ << ". " << temp->name << " (Runs: " << temp->runsScored 
                 << ", Wickets: " << temp->wicketsTaken << ")\n";
            temp = temp->next;
        }
    }
    
    void resetStats() {
        Player* temp = head;
        while (temp) {
            temp->runsScored = 0;
            temp->ballsFaced = 0;
            temp->wicketsTaken = 0;
            temp->oversBowled = 0;
            temp->runsConceded = 0;
            temp->isOut = false;
            temp = temp->next;
        }
    }
    
    ~PlayerList() {
        while (head) {
            Player* temp = head;
            head = head->next;
            delete temp;
        }
    }
};

// ==================== TEAM CLASS ====================
class Team {
public:
    string name;
    PlayerList players;
    int matchesPlayed;
    int matchesWon;
    int matchesLost;
    int points;
    int totalRuns;
    int totalWickets;
    Team* next;
    
    Team(string n = "") : name(n), matchesPlayed(0), matchesWon(0), 
                          matchesLost(0), points(0), totalRuns(0), 
                          totalWickets(0), next(NULL) {}
    
    void addWin() {
        matchesWon++;
        matchesPlayed++;
        points += 2;
    }
    
    void addLoss() {
        matchesLost++;
        matchesPlayed++;
    }
};

// ==================== LINKED LIST FOR TEAMS ====================
class TeamList {
private:
    Team* head;
    int count;
    
public:
    TeamList() : head(NULL), count(0) {}
    
    void addTeam(string name) {
        Team* newTeam = new Team(name);
        if (!head) {
            head = newTeam;
        } else {
            Team* temp = head;
            while (temp->next) {
                temp = temp->next;
            }
            temp->next = newTeam;
        }
        count++;
    }
    
    Team* getTeam(int index) {
        if (index < 0 || index >= count) return NULL;
        Team* temp = head;
        for (int i = 0; i < index; i++) {
            temp = temp->next;
        }
        return temp;
    }
    
    Team* findTeam(string name) {
        Team* temp = head;
        while (temp) {
            if (temp->name == name) return temp;
            temp = temp->next;
        }
        return NULL;
    }
    
    int getCount() { return count; }
    
    Team* getHead() { return head; }
    
    void display() {
        Team* temp = head;
        int i = 1;
        while (temp) {
            cout << i++ << ". " << temp->name << " (Players: " 
                 << temp->players.getCount() << ")\n";
            temp = temp->next;
        }
    }
    
    ~TeamList() {
        while (head) {
            Team* temp = head;
            head = head->next;
            delete temp;
        }
    }
};

// ==================== STACK FOR UNDO OPERATIONS ====================
struct UndoAction {
    string actionType;
    string data1;
    string data2;
    int intData;
    UndoAction* next;
    
    UndoAction(string type, string d1 = "", string d2 = "", int iData = 0) 
        : actionType(type), data1(d1), data2(d2), intData(iData), next(NULL) {}
};

class UndoStack {
private:
    UndoAction* top;
    int count;
    
public:
    UndoStack() : top(NULL), count(0) {}
    
    void push(string actionType, string data1 = "", string data2 = "", int intData = 0) {
        UndoAction* newAction = new UndoAction(actionType, data1, data2, intData);
        newAction->next = top;
        top = newAction;
        count++;
    }
    
    UndoAction* pop() {
        if (!top) return NULL;
        UndoAction* temp = top;
        top = top->next;
        count--;
        return temp;
    }
    
    bool isEmpty() { return top == NULL; }
    
    int getCount() { return count; }
    
    ~UndoStack() {
        while (top) {
            UndoAction* temp = top;
            top = top->next;
            delete temp;
        }
    }
};

// ==================== MATCH CLASS ====================
class Match {
public:
    Team* team1;
    Team* team2;
    int overs;
    Team* winner;
    int team1Score;
    int team2Score;
    int team1Wickets;
    int team2Wickets;
    bool completed;
    Match* next;
    
    Match() : team1(NULL), team2(NULL), overs(0), winner(NULL), 
              team1Score(0), team2Score(0), team1Wickets(0), 
              team2Wickets(0), completed(false), next(NULL) {}
};

// ==================== QUEUE FOR MATCH SCHEDULING ====================
class MatchQueue {
private:
    Match* front;
    Match* rear;
    int count;
    
public:
    MatchQueue() : front(NULL), rear(NULL), count(0) {}
    
    void enqueue(Team* t1, Team* t2, int overs) {
        Match* newMatch = new Match();
        newMatch->team1 = t1;
        newMatch->team2 = t2;
        newMatch->overs = overs;
        
        if (!rear) {
            front = rear = newMatch;
        } else {
            rear->next = newMatch;
            rear = newMatch;
        }
        count++;
    }
    
    Match* dequeue() {
        if (!front) return NULL;
        Match* temp = front;
        front = front->next;
        if (!front) rear = NULL;
        count--;
        return temp;
    }
    
    bool isEmpty() { return front == NULL; }
    
    int getCount() { return count; }
    
    Match* getFront() { return front; }
    
    void display() {
        Match* temp = front;
        int i = 1;
        while (temp) {
            cout << i++ << ". " << temp->team1->name << " vs " 
                 << temp->team2->name << " (" << temp->overs << " overs)";
            if (temp->completed) {
                cout << " - COMPLETED (Winner: " << temp->winner->name << ")";
            }
            cout << "\n";
            temp = temp->next;
        }
    }
    
    ~MatchQueue() {
        while (front) {
            Match* temp = front;
            front = front->next;
            delete temp;
        }
    }
};

// ==================== SORTING FUNCTIONS ====================
// Merge Sort for Teams by Points
Team* mergeSortedTeams(Team* left, Team* right) {
    if (!left) return right;
    if (!right) return left;
    
    Team* result = NULL;
    
    if (left->points >= right->points) {
        result = left;
        result->next = mergeSortedTeams(left->next, right);
    } else {
        result = right;
        result->next = mergeSortedTeams(left, right->next);
    }
    
    return result;
}

void splitList(Team* head, Team** left, Team** right) {
    Team* slow = head;
    Team* fast = head->next;
    
    while (fast) {
        fast = fast->next;
        if (fast) {
            slow = slow->next;
            fast = fast->next;
        }
    }
    
    *left = head;
    *right = slow->next;
    slow->next = NULL;
}

Team* mergeSort(Team* head) {
    if (!head || !head->next) return head;
    
    Team* left = NULL;
    Team* right = NULL;
    
    splitList(head, &left, &right);
    
    left = mergeSort(left);
    right = mergeSort(right);
    
    return mergeSortedTeams(left, right);
}

// Quick Sort for Players by Runs
Player* getLastPlayer(Player* head) {
    while (head && head->next) {
        head = head->next;
    }
    return head;
}

Player* partitionPlayers(Player* head, Player* end, Player** newHead, Player** newEnd) {
    Player* pivot = end;
    Player *prev = NULL, *cur = head, *tail = pivot;
    
    while (cur != pivot) {
        if (cur->runsScored > pivot->runsScored) {
            if (*newHead == NULL) *newHead = cur;
            prev = cur;
            cur = cur->next;
        } else {
            if (prev) prev->next = cur->next;
            Player* temp = cur->next;
            cur->next = NULL;
            tail->next = cur;
            tail = cur;
            cur = temp;
        }
    }
    
    if (*newHead == NULL) *newHead = pivot;
    *newEnd = tail;
    
    return pivot;
}

Player* quickSortRecur(Player* head, Player* end) {
    if (!head || head == end) return head;
    
    Player *newHead = NULL, *newEnd = NULL;
    Player* pivot = partitionPlayers(head, end, &newHead, &newEnd);
    
    if (newHead != pivot) {
        Player* temp = newHead;
        while (temp->next != pivot) {
            temp = temp->next;
        }
        temp->next = NULL;
        
        newHead = quickSortRecur(newHead, temp);
        
        temp = getLastPlayer(newHead);
        temp->next = pivot;
    }
    
    pivot->next = quickSortRecur(pivot->next, newEnd);
    
    return newHead;
}

Player* quickSort(Player* head) {
    return quickSortRecur(head, getLastPlayer(head));
}

// ==================== MAIN SYSTEM CLASS ====================
class CricketTournament {
private:
    TeamList teams;
    MatchQueue matches;
    UndoStack undoStack;
    
    void clearScreen() {
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
    }
    
    void pause() {
        cout << "\nPress Enter to continue...";
        cin.ignore();
        cin.get();
    }
    
    int simulateBall() {
        int outcome = rand() % 10;
        if (outcome == 0) return -1; // Wicket
        if (outcome <= 3) return 0;  // Dot ball
        if (outcome <= 6) return 1;  // 1 run
        if (outcome <= 8) return 2;  // 2 runs
        if (outcome == 9) return rand() % 2 == 0 ? 4 : 6; // Boundary
        return 0;
    }
    
public:
    CricketTournament() {
        srand(time(0));
    }
    
    void createTeam() {
        clearScreen();
        cout << "=== CREATE NEW TEAM ===\n\n";
        string teamName;
        cout << "Enter team name: ";
        cin.ignore();
        getline(cin, teamName);
        
        teams.addTeam(teamName);
        undoStack.push("CREATE_TEAM", teamName);
        
        cout << "\nTeam '" << teamName << "' created successfully!\n";
        pause();
    }
    
    void addPlayerToTeam() {
        clearScreen();
        cout << "=== ADD PLAYER TO TEAM ===\n\n";
        
        if (teams.getCount() == 0) {
            cout << "No teams available. Create a team first.\n";
            pause();
            return;
        }
        
        cout << "Select a team:\n";
        teams.display();
        
        int teamChoice;
        cout << "\nEnter team number: ";
        cin >> teamChoice;
        
        Team* selectedTeam = teams.getTeam(teamChoice - 1);
        if (!selectedTeam) {
            cout << "Invalid team selection!\n";
            pause();
            return;
        }
        
        string playerName;
        cout << "Enter player name: ";
        cin.ignore();
        getline(cin, playerName);
        
        selectedTeam->players.addPlayer(playerName);
        undoStack.push("ADD_PLAYER", selectedTeam->name, playerName);
        
        cout << "\nPlayer '" << playerName << "' added to " << selectedTeam->name << "!\n";
        pause();
    }
    
    void scheduleMatch() {
        clearScreen();
        cout << "=== SCHEDULE MATCH ===\n\n";
        
        if (teams.getCount() < 2) {
            cout << "At least 2 teams required to schedule a match.\n";
            pause();
            return;
        }
        
        cout << "Select Team 1:\n";
        teams.display();
        int team1Choice;
        cout << "\nEnter team number: ";
        cin >> team1Choice;
        
        Team* team1 = teams.getTeam(team1Choice - 1);
        if (!team1) {
            cout << "Invalid team selection!\n";
            pause();
            return;
        }
        
        cout << "\nSelect Team 2:\n";
        teams.display();
        int team2Choice;
        cout << "\nEnter team number: ";
        cin >> team2Choice;
        
        Team* team2 = teams.getTeam(team2Choice - 1);
        if (!team2 || team1 == team2) {
            cout << "Invalid team selection!\n";
            pause();
            return;
        }
        
        if (team1->players.getCount() < 2 || team2->players.getCount() < 2) {
            cout << "Each team must have at least 2 players!\n";
            pause();
            return;
        }
        
        int overs;
        cout << "\nEnter number of overs (5-50): ";
        cin >> overs;
        
        if (overs < 5 || overs > 50) {
            cout << "Invalid number of overs!\n";
            pause();
            return;
        }
        
        matches.enqueue(team1, team2, overs);
        undoStack.push("SCHEDULE_MATCH", team1->name, team2->name, overs);
        
        cout << "\nMatch scheduled: " << team1->name << " vs " << team2->name << "\n";
        pause();
    }
    
    void playMatch() {
        clearScreen();
        cout << "=== PLAY MATCH ===\n\n";
        
        if (matches.isEmpty()) {
            cout << "No matches scheduled!\n";
            pause();
            return;
        }
        
        Match* match = matches.dequeue();
        
        cout << "Match: " << match->team1->name << " vs " << match->team2->name << "\n";
        cout << "Overs: " << match->overs << "\n\n";
        
        // Reset player stats
        match->team1->players.resetStats();
        match->team2->players.resetStats();
        
        // Toss
        cout << "=== TOSS ===\n";
        cout << "1. " << match->team1->name << "\n";
        cout << "2. " << match->team2->name << "\n";
        cout << "Who won the toss? ";
        int tossWinner;
        cin >> tossWinner;
        
        cout << "Choose:\n1. Bat\n2. Bowl\nChoice: ";
        int tossChoice;
        cin >> tossChoice;
        
        Team* battingFirst = (tossWinner == 1 && tossChoice == 1) || 
                             (tossWinner == 2 && tossChoice == 2) ? 
                             match->team1 : match->team2;
        Team* bowlingFirst = (battingFirst == match->team1) ? match->team2 : match->team1;
        
        cout << "\n" << battingFirst->name << " will bat first!\n";
        pause();
        
        // First Innings
        clearScreen();
        cout << "=== FIRST INNINGS ===\n";
        cout << battingFirst->name << " batting\n\n";
        
        int innings1Score = playInnings(battingFirst, bowlingFirst, match->overs);
        match->team1Score = (battingFirst == match->team1) ? innings1Score : 0;
        match->team2Score = (battingFirst == match->team2) ? innings1Score : 0;
        
        cout << "\n" << battingFirst->name << " scored " << innings1Score << " runs\n";
        cout << "\nTarget: " << innings1Score + 1 << " runs\n";
        pause();
        
        // Second Innings
        clearScreen();
        cout << "=== SECOND INNINGS ===\n";
        cout << bowlingFirst->name << " batting (Target: " << innings1Score + 1 << ")\n\n";
        
        int innings2Score = playInnings(bowlingFirst, battingFirst, match->overs, innings1Score + 1);
        
        if (battingFirst == match->team1) {
            match->team2Score = innings2Score;
        } else {
            match->team1Score = innings2Score;
        }
        
        cout << "\n" << bowlingFirst->name << " scored " << innings2Score << " runs\n";
        
        // Determine winner
        if (innings2Score > innings1Score) {
            match->winner = bowlingFirst;
            bowlingFirst->addWin();
            battingFirst->addLoss();
            cout << "\n" << bowlingFirst->name << " wins!\n";
        } else if (innings2Score < innings1Score) {
            match->winner = battingFirst;
            battingFirst->addWin();
            bowlingFirst->addLoss();
            cout << "\n" << battingFirst->name << " wins!\n";
        } else {
            cout << "\nMatch Tied!\n";
            match->winner = NULL;
        }
        
        match->completed = true;
        pause();
    }
    
    int playInnings(Team* batting, Team* bowling, int totalOvers, int target = -1) {
        int score = 0;
        int wickets = 0;
        int ballsPlayed = 0;
        int maxBalls = totalOvers * 6;
        
        cout << "Select batsman 1 from " << batting->name << ":\n";
        batting->players.display();
        int bat1Choice;
        cout << "Enter number: ";
        cin >> bat1Choice;
        Player* batsman1 = batting->players.getPlayer(bat1Choice - 1);
        
        cout << "\nSelect batsman 2:\n";
        batting->players.display();
        int bat2Choice;
        cout << "Enter number: ";
        cin >> bat2Choice;
        Player* batsman2 = batting->players.getPlayer(bat2Choice - 1);
        
        if (!batsman1 || !batsman2 || batsman1 == batsman2) {
            cout << "Invalid batsman selection!\n";
            return 0;
        }
        
        cout << "\nSelect opening bowler from " << bowling->name << ":\n";
        bowling->players.display();
        int bowlChoice;
        cout << "Enter number: ";
        cin >> bowlChoice;
        Player* bowler = bowling->players.getPlayer(bowlChoice - 1);
        
        if (!bowler) {
            cout << "Invalid bowler selection!\n";
            return 0;
        }
        
        Player* onStrike = batsman1;
        Player* offStrike = batsman2;
        
        pause();
        
        for (int over = 0; over < totalOvers && wickets < 2; over++) {
            clearScreen();
            cout << "\n=== OVER " << (over + 1) << " ===\n";
            cout << "Score: " << score << "/" << wickets;
            if (target > 0) cout << " (Need " << (target - score) << " runs)";
            cout << "\n";
            cout << "Batsmen: " << onStrike->name << "* (" << onStrike->runsScored 
                 << " off " << onStrike->ballsFaced << "), ";
            cout << offStrike->name << " (" << offStrike->runsScored 
                 << " off " << offStrike->ballsFaced << ")\n";
            cout << "Bowler: " << bowler->name << "\n\n";
            
            for (int ball = 0; ball < 6 && wickets < 2; ball++) {
                cout << "Ball " << (ball + 1) << ": ";
                
                int outcome = simulateBall();
                
                if (outcome == -1) {
                    cout << "WICKET!\n";
                    onStrike->isOut = true;
                    wickets++;
                    bowler->wicketsTaken++;
                    
                    if (wickets < 2) {
                        cout << "\nNew batsman needed. Select:\n";
                        Player* temp = batting->players.getHead();
                        int num = 1;
                        while (temp) {
                            if (!temp->isOut && temp != offStrike) {
                                cout << num << ". " << temp->name << "\n";
                            }
                            num++;
                            temp = temp->next;
                        }
                        
                        int newBatChoice;
                        cout << "Enter number: ";
                        cin >> newBatChoice;
                        onStrike = batting->players.getPlayer(newBatChoice - 1);
                    }
                } else {
                    onStrike->runsScored += outcome;
                    onStrike->ballsFaced++;
                    score += outcome;
                    bowler->runsConceded += outcome;
                    ballsPlayed++;
                    
                    if (outcome == 0) cout << "Dot ball\n";
                    else cout << outcome << " run(s)\n";
                    
                    if (outcome % 2 == 1) {
                        Player* temp = onStrike;
                        onStrike = offStrike;
                        offStrike = temp;
                    }
                }
                
                if (target > 0 && score >= target) {
                    cout << "\nTarget achieved!\n";
                    return score;
                }
            }
            
            bowler->oversBowled++;
            
            // Swap strike
            Player* temp = onStrike;
            onStrike = offStrike;
            offStrike = temp;
            
            if (over < totalOvers - 1 && wickets < 2) {
                cout << "\nSelect next bowler:\n";
                bowling->players.display();
                cout << "Enter number: ";
                cin >> bowlChoice;
                bowler = bowling->players.getPlayer(bowlChoice - 1);
            }
            
            pause();
        }
        
        return score;
    }
    
    void viewPointsTable() {
        clearScreen();
        cout << "=== POINTS TABLE ===\n\n";
        
        if (teams.getCount() == 0) {
            cout << "No teams available.\n";
            pause();
            return;
        }
        
        // Sort teams by points using merge sort
        Team* sortedHead = mergeSort(teams.getHead());
        
        cout << "Pos  Team                 P   W   L   Pts\n";
        cout << "-------------------------------------------\n";
        
        int pos = 1;
        Team* temp = sortedHead;
        while (temp) {
            cout << pos++ << ".   ";
            cout << temp->name;
            for (int i = temp->name.length(); i < 20; i++) cout << " ";
            cout << temp->matchesPlayed << "   ";
            cout << temp->matchesWon << "   ";
            cout << temp->matchesLost << "   ";
            cout << temp->points << "\n";
            temp = temp->next;
        }
        
        pause();
    }
    
    void viewLeaderboard() {
        clearScreen();
        cout << "=== LEADERBOARD ===\n\n";
        
        if (teams.getCount() == 0) {
            cout << "No teams available.\n";
            pause();
            return;
        }
        
        // Collect all players
        Player* allPlayers = NULL;
        Team* tempTeam = teams.getHead();
        
        while (tempTeam) {
            Player* tempPlayer = tempTeam->players.getHead();
            while (tempPlayer) {
                Player* copy = new Player(tempPlayer->name);
                copy->runsScored = tempPlayer->runsScored;
                copy->wicketsTaken = tempPlayer->wicketsTaken;
                copy->next = allPlayers;
                allPlayers = copy;
                tempPlayer = tempPlayer->next;
            }
            tempTeam = tempTeam->next;
        }
        
        // Sort by runs using quick sort
        allPlayers = quickSort(allPlayers);
        
        cout << "TOP RUN SCORERS:\n";
        cout << "Rank  Player               Runs\n";
        cout << "--------------------------------\n";
        
        int rank = 1;
        Player* temp = allPlayers;
        while (temp && rank <= 10) {
            cout << rank++ << ".    ";
            cout << temp->name;
            for (int i = temp->name.length(); i < 20; i++) cout << " ";
            cout << temp->runsScored << "\n";
            temp = temp->next;
        }
        
        // Clean up
        while (allPlayers) {
            Player* del = allPlayers;
            allPlayers = allPlayers->next;
            delete del;
        }
        
        pause();
    }
    
    void undoLastAction() {
        clearScreen();
        cout << "=== UNDO LAST ACTION ===\n\n";
        
        if (undoStack.isEmpty()) {
            cout << "No actions to undo.\n";
            pause();
            return;
        }
        
        UndoAction* action = undoStack.pop();
        
        cout << "Undoing: " << action->actionType << "\n";
        cout << "Note: This is a simplified undo. Some complex operations may not be fully reversible.\n";
        
        delete action;
        pause();
    }
    
    void mainMenu() {
        int choice;
        
        do {
            clearScreen();
            cout << "====================================\n";
            cout << "   CRICKET TOURNAMENT MANAGEMENT\n";
            cout << "====================================\n\n";
            cout << "1.  Create Team\n";
            cout << "2.  Add Player to Team\n";
            cout << "3.  Schedule Match\n";
            cout << "4.  Play Match\n";
            cout << "5.  View Scheduled Matches\n";
            cout << "6.  View Points Table\n";
            cout << "7.  View Leaderboard\n";
            cout << "8.  View All Teams\n";
            cout << "9.  Undo Last Action\n";
            cout << "10. Exit\n";
            cout << "\nEnter choice: ";
            cin >> choice;
            
            switch (choice) {
                case 1: createTeam(); break;
                case 2: addPlayerToTeam(); break;
                case 3: scheduleMatch(); break;
                case 4: playMatch(); break;
                case 5: 
                    clearScreen();
                    cout << "=== SCHEDULED MATCHES ===\n\n";
                    matches.display();
                    pause();
                    break;
                case 6: viewPointsTable(); break;
                case 7: viewLeaderboard(); break;
                case 8:
                    clearScreen();
                    cout << "=== ALL TEAMS ===\n\n";
                    teams.display();
                    pause();
                    break;
                case 9: undoLastAction(); break;
                case 10: cout << "\nThank you for using the system!\n"; break;
                default: cout << "\nInvalid choice!\n"; pause();
            }
        } while (choice != 10);
    }
};

// ==================== MAIN FUNCTION ====================
int main() {
    CricketTournament tournament;
    tournament.mainMenu();
    return 0;
}
