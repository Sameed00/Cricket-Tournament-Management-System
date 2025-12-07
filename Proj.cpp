#include <iostream>
#include <string>
#include <time.h>
using namespace std;

// ====================== CRICKET CLASSES (Declared first) ======================

class Player {
private:
    int id;
    string name;
    string type;
    int runs;
    int wickets;
public:
    Player(int i, string n, string t) : id(i), name(n), type(t), runs(0), wickets(0) {}
    
    void addStats(int r, int w) {
        runs += r;
        wickets += w;
    }
    
    void display() {
        cout << "[" << id << "] " << name << " (" << type << ") - " 
             << runs << " runs, " << wickets << " wickets" << endl;
    }
    
    int getRuns() { return runs; }
    int getId() { return id; }
    string getName() { return name; }
    int getWickets() { return wickets; }
};

class Team {
private:
    int id;
    string name;
    Player* players[15];
    int playerCount;
    int points;
    int wins;
public:
    Team(int i, string n) : id(i), name(n), playerCount(0), points(0), wins(0) {}
    
    void addPlayer(Player* p) {
        if (playerCount < 15) players[playerCount++] = p;
    }
    
    void updatePoints(bool won) {
        if (won) {
            points += 2;
            wins++;
        }
    }
    
    void display() {
        cout << "\nTeam " << id << ": " << name 
             << " | Points: " << points << " | Wins: " << wins << endl;
        for (int i = 0; i < playerCount; i++) {
            cout << "  "; players[i]->display();
        }
    }
    
    int getPoints() { return points; }
    string getName() { return name; }
    int getId() { return id; }
    int getPlayerCount() { return playerCount; }
    Player* getPlayer(int i) { return (i < playerCount) ? players[i] : nullptr; }
};

class Match {
private:
    int id;
    Team* team1;
    Team* team2;
    string result;
public:
    Match(int i, Team* t1, Team* t2) : id(i), team1(t1), team2(t2), result("Pending") {}
    
    void simulate() {
        int score1 = rand() % 200 + 100;
        int score2 = rand() % 200 + 100;
        
        if (score1 > score2) {
            result = team1->getName() + " won by " + to_string(score1 - score2) + " runs";
            team1->updatePoints(true);
        } else {
            result = team2->getName() + " won by " + to_string(score2 - score1) + " runs";
            team2->updatePoints(true);
        }
    }
    
    void display() {
        cout << "Match " << id << ": " << team1->getName() << " vs " << team2->getName() 
             << " - " << result << endl;
    }
};

// ====================== SIMPLIFIED DATA STRUCTURES ======================

template <typename T>
class Node {
public:
    T data;
    Node* next;
    Node(T val) : data(val), next(nullptr) {}
};

template <typename T>
class List {
private:
    Node<T>* head;
public:
    List() : head(nullptr) {}
    
    void add(T val) {
        Node<T>* newNode = new Node<T>(val);
        if (!head) head = newNode;
        else {
            Node<T>* temp = head;
            while (temp->next) temp = temp->next;
            temp->next = newNode;
        }
    }
    
    Node<T>* getHead() { return head; }
    
    void display() {
        Node<T>* curr = head;
        while (curr) {
            cout << curr->data << " ";
            curr = curr->next;
        }
        cout << endl;
    }
};

class Stack {
private:
    string* arr;
    int top;
    int capacity;
public:
    Stack(int size = 100) : top(-1), capacity(size) {
        arr = new string[size];
    }
    
    ~Stack() {
        delete[] arr;
    }
    
    void push(string action) {
        if (top < capacity - 1) arr[++top] = action;
    }
    
    string pop() {
        if (top >= 0) return arr[top--];
        return "";
    }
};

class Queue {
private:
    string* arr;
    int front, rear, size, capacity;
public:
    Queue(int size = 100) : front(0), rear(-1), size(0), capacity(size) {
        arr = new string[size];
    }
    
    ~Queue() {
        delete[] arr;
    }
    
    void enqueue(string match) {
        if (size < capacity) {
            rear = (rear + 1) % capacity;
            arr[rear] = match;
            size++;
        }
    }
    
    string dequeue() {
        if (size > 0) {
            string match = arr[front];
            front = (front + 1) % capacity;
            size--;
            return match;
        }
        return "";
    }
};

// ====================== ADVANCED SORTING ALGORITHMS ======================

// Merge Sort for Players by Runs
void mergePlayersByRuns(Player* arr[], int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    
    Player** L = new Player*[n1];
    Player** R = new Player*[n2];
    
    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];
    
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i]->getRuns() >= R[j]->getRuns()) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
    
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
    
    delete[] L;
    delete[] R;
}

void mergeSortPlayersByRuns(Player* arr[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSortPlayersByRuns(arr, left, mid);
        mergeSortPlayersByRuns(arr, mid + 1, right);
        mergePlayersByRuns(arr, left, mid, right);
    }
}

// Merge Sort for Teams by Points
void mergeTeamsByPoints(Team* arr[], int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    
    Team** L = new Team*[n1];
    Team** R = new Team*[n2];
    
    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];
    
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i]->getPoints() >= R[j]->getPoints()) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
    
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
    
    delete[] L;
    delete[] R;
}

void mergeSortTeamsByPoints(Team* arr[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSortTeamsByPoints(arr, left, mid);
        mergeSortTeamsByPoints(arr, mid + 1, right);
        mergeTeamsByPoints(arr, left, mid, right);
    }
}


int partition(Team* arr[], int low, int high) {
    string pivot = arr[high]->getName();
    int i = low - 1;
    
    for (int j = low; j < high; j++) {
        if (arr[j]->getName() <= pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return i + 1;
}

void quickSortTeamsByName(Team* arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSortTeamsByName(arr, low, pi - 1);
        quickSortTeamsByName(arr, pi + 1, high);
    }
}


Player* binarySearchPlayers(Player* arr[], int left, int right, int id) {
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (arr[mid]->getId() == id) return arr[mid];
        if (arr[mid]->getId() < id) left = mid + 1;
        else right = mid - 1;
    }
    return nullptr;
}

// ====================== TOURNAMENT SYSTEM ======================

class CricketTournament {
private:
    Team* teams[20];
    Player* players[100];
    int teamCount;
    int playerCount;
    int matchCount;
    Stack undoStack;
    Queue matchQueue;
    
    
    void sortPlayersById(Player* arr[], int n) {
        for (int i = 1; i < n; i++) {
            Player* key = arr[i];
            int j = i - 1;
            while (j >= 0 && arr[j]->getId() > key->getId()) {
                arr[j + 1] = arr[j];
                j--;
            }
            arr[j + 1] = key;
        }
    }
    
    
    void displayBracket(int level, int matchNo) {
        if (level == 0) return;
        
        string spaces(level * 2, ' ');
        cout << spaces << "Match " << matchNo << " (Round " << level << ")" << endl;
        
        displayBracket(level - 1, matchNo * 2);
        displayBracket(level - 1, matchNo * 2 + 1);
    }
    
    
    void generateFixtures(int round, int n, string current[], int& count) {
        if (round >= n * (n - 1) / 2) return;
        
        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                current[count++] = "Match " + to_string(count + 1) + ": " + 
                                  teams[i]->getName() + " vs " + teams[j]->getName();
                round++;
                if (round >= 5) return; // Limit to 5 fixtures for simplicity
            }
        }
    }
    
public:
    CricketTournament() : teamCount(0), playerCount(0), matchCount(0) {}
    
    ~CricketTournament() {
        
        for (int i = 0; i < playerCount; i++) delete players[i];
        for (int i = 0; i < teamCount; i++) delete teams[i];
    }
    
    void addTeam(string name) {
        teams[teamCount++] = new Team(teamCount, name);
        undoStack.push("Added team: " + name);
        cout << "Team '" << name << "' added with ID: " << teamCount << endl;
    }
    
    void addPlayer(string name, string type, int teamId) {
        if (teamId > 0 && teamId <= teamCount) {
            players[playerCount] = new Player(playerCount + 1, name, type);
            teams[teamId - 1]->addPlayer(players[playerCount]);
            
            
            players[playerCount]->addStats(rand() % 500, rand() % 20);
            
            playerCount++;
            undoStack.push("Added player: " + name);
            cout << "Player '" << name << "' added to team " << teamId << endl;
        } else {
            cout << "Invalid team ID!" << endl;
        }
    }
    
    void scheduleMatch(int team1Id, int team2Id) {
        if (team1Id > 0 && team2Id > 0 && team1Id <= teamCount && team2Id <= teamCount && team1Id != team2Id) {
            string matchDesc = "Match " + to_string(matchCount + 1) + ": " + 
                              teams[team1Id - 1]->getName() + " vs " + teams[team2Id - 1]->getName();
            matchQueue.enqueue(matchDesc);
            matchCount++;
            cout << "Scheduled: " << matchDesc << endl;
        } else {
            cout << "Invalid teams or same team selected!" << endl;
        }
    }
    
    void playMatch() {
        string next = matchQueue.dequeue();
        if (!next.empty()) {
            cout << "\nPlaying: " << next << endl;
            
            int winner = rand() % 2;
            if (winner == 0) {
                cout << teams[0]->getName() << " wins!" << endl;
                teams[0]->updatePoints(true);
            } else {
                cout << teams[1]->getName() << " wins!" << endl;
                teams[1]->updatePoints(true);
            }
            undoStack.push("Played: " + next);
        } else {
            cout << "No matches scheduled!" << endl;
        }
    }
    
    void undo() {
        string last = undoStack.pop();
        if (!last.empty()) {
            cout << "Undid: " << last << endl;
        } else {
            cout << "Nothing to undo!" << endl;
        }
    }
    
    void showPointsTable() {
        cout << "\n=== POINTS TABLE (Sorted by Merge Sort) ===" << endl;
        if (teamCount == 0) {
            cout << "No teams available!" << endl;
            return;
        }
        
        Team* sortedTeams[20];
        for (int i = 0; i < teamCount; i++) sortedTeams[i] = teams[i];
        
        mergeSortTeamsByPoints(sortedTeams, 0, teamCount - 1);
        
        cout << "Pos | Team | Points | Wins" << endl;
        cout << "----------------------------" << endl;
        for (int i = 0; i < teamCount; i++) {
            cout << i + 1 << ".  " << sortedTeams[i]->getName() 
                 << " - " << sortedTeams[i]->getPoints() 
                 << " points (" << sortedTeams[i]->getId() << " wins)" << endl;
        }
    }
    
    void showTopPlayers() {
        cout << "\n=== TOP PLAYERS (Sorted by Merge Sort) ===" << endl;
        if (playerCount == 0) {
            cout << "No players available!" << endl;
            return;
        }
        
        Player* allPlayers[100];
        int total = 0;
        
        for (int i = 0; i < teamCount; i++) {
            for (int j = 0; j < teams[i]->getPlayerCount(); j++) {
                allPlayers[total++] = teams[i]->getPlayer(j);
            }
        }
        
        if (total == 0) {
            cout << "No players to display!" << endl;
            return;
        }
        
        mergeSortPlayersByRuns(allPlayers, 0, total - 1);
        
        int limit = (total < 5) ? total : 5;
        for (int i = 0; i < limit; i++) {
            cout << i + 1 << ". "; allPlayers[i]->display();
        }
    }
    
    void searchPlayer() {
        int id;
        cout << "Enter player ID to search: ";
        cin >> id;
        
        Player* allPlayers[100];
        int total = 0;
        
        for (int i = 0; i < teamCount; i++) {
            for (int j = 0; j < teams[i]->getPlayerCount(); j++) {
                allPlayers[total++] = teams[i]->getPlayer(j);
            }
        }
        
        if (total == 0) {
            cout << "No players available!" << endl;
            return;
        }
        
        
        sortPlayersById(allPlayers, total);
        
        Player* found = binarySearchPlayers(allPlayers, 0, total - 1, id);
        if (found) {
            cout << "\nPlayer found (using Binary Search):" << endl;
            found->display();
        } else {
            cout << "Player not found!" << endl;
        }
    }
    
    void searchTeam() {
        string name;
        cout << "Enter team name to search: ";
        cin.ignore();
        getline(cin, name);
        
        
        for (int i = 0; i < teamCount; i++) {
            if (teams[i]->getName() == name) {
                cout << "\nTeam found:" << endl;
                teams[i]->display();
                return;
            }
        }
        cout << "Team not found!" << endl;
    }
    
    void showTournamentTree() {
        cout << "\n=== TOURNAMENT BRACKET (Recursive Display) ===" << endl;
        displayBracket(4, 1);
    }
    
    void generateAllFixtures() {
        cout << "\n=== FIXTURES (Backtracking Algorithm) ===" << endl;
        if (teamCount < 2) {
            cout << "Need at least 2 teams!" << endl;
            return;
        }
        
        string fixtures[100];
        int count = 0;
        
        generateFixtures(0, teamCount, fixtures, count);
        
        for (int i = 0; i < count && i < 10; i++) {
            cout << fixtures[i] << endl;
        }
    }
    
    void showAllTeams() {
        cout << "\n=== ALL TEAMS ===" << endl;
        if (teamCount == 0) {
            cout << "No teams available!" << endl;
            return;
        }
        
        Team* sortedTeams[20];
        for (int i = 0; i < teamCount; i++) sortedTeams[i] = teams[i];
        
        quickSortTeamsByName(sortedTeams, 0, teamCount - 1);
        
        for (int i = 0; i < teamCount; i++) {
            cout << i + 1 << ". ";
            sortedTeams[i]->display();
        }
    }
    
    void showMenu() {
        cout << "\n==== CRICKET TOURNAMENT MANAGEMENT ====" << endl;
        cout << "1. Add Team" << endl;
        cout << "2. Add Player to Team" << endl;
        cout << "3. Schedule Match" << endl;
        cout << "4. Play Next Match" << endl;
        cout << "5. Undo Last Action" << endl;
        cout << "6. Show Points Table (Merge Sort)" << endl;
        cout << "7. Show Top Players (Merge Sort)" << endl;
        cout << "8. Search Player (Binary Search)" << endl;
        cout << "9. Search Team" << endl;
        cout << "10. Show Tournament Tree (Recursion)" << endl;
        cout << "11. Generate Fixtures (Backtracking)" << endl;
        cout << "12. Show All Teams (Quick Sort by Name)" << endl;
        cout << "13. Exit" << endl;
        cout << "Choose option: ";
    }
};

// ====================== MAIN ======================

int main() {
    CricketTournament tournament;
    int choice;
    
    srand(time(0)); 
    
    
    tournament.addTeam("India");
    tournament.addTeam("Australia");
    tournament.addTeam("England");
    
    tournament.addPlayer("Virat Kohli", "Batsman", 1);
    tournament.addPlayer("Rohit Sharma", "Batsman", 1);
    tournament.addPlayer("Steve Smith", "Batsman", 2);
    tournament.addPlayer("Pat Cummins", "Bowler", 2);
    tournament.addPlayer("Joe Root", "Batsman", 3);
    
    tournament.scheduleMatch(1, 2);
    tournament.scheduleMatch(1, 3);
    tournament.scheduleMatch(2, 3);
    
    cout << "\nWelcome to Cricket Tournament Management System!" << endl;
    cout << "Sample data loaded: 3 teams, 5 players, 3 matches scheduled." << endl;
    
    do {
        tournament.showMenu();
        cin >> choice;
        
        switch(choice) {
            case 1: {
                string name;
                cout << "Enter team name: ";
                cin.ignore();
                getline(cin, name);
                tournament.addTeam(name);
                break;
            }
            case 2: {
                string name, type;
                int teamId;
                cout << "Enter player name: ";
                cin.ignore();
                getline(cin, name);
                cout << "Enter player type (Batsman/Bowler): ";
                getline(cin, type);
                cout << "Enter team ID: ";
                cin >> teamId;
                tournament.addPlayer(name, type, teamId);
                break;
            }
            case 3: {
                int t1, t2;
                cout << "Enter team 1 ID: ";
                cin >> t1;
                cout << "Enter team 2 ID: ";
                cin >> t2;
                tournament.scheduleMatch(t1, t2);
                break;
            }
            case 4:
                tournament.playMatch();
                break;
            case 5:
                tournament.undo();
                break;
            case 6:
                tournament.showPointsTable();
                break;
            case 7:
                tournament.showTopPlayers();
                break;
            case 8:
                tournament.searchPlayer();
                break;
            case 9:
                tournament.searchTeam();
                break;
            case 10:
                tournament.showTournamentTree();
                break;
            case 11:
                tournament.generateAllFixtures();
                break;
            case 12:
                tournament.showAllTeams();
                break;
            case 13:
                cout << "\nThank you for using Cricket Tournament Management System!" << endl;
                break;
            default:
                cout << "Invalid choice! Please try again." << endl;
        }
        
        cout << endl;
        
    } while (choice != 13);
    
    return 0;
}
