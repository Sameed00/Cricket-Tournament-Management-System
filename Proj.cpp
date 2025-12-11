#include <iostream>
#include <string>
#include <cmath>
using namespace std;

// Simple random num generator
int randomInt(int min, int max) {
    static int seed = 12345;
    seed = (seed * 1103515245 + 12345) & 0x7fffffff;
    return min + (seed % (max - min + 1));
}

// player nodes ll
class PlayerNode {
public:
    string name;
    int runs, wickets, matches;
    PlayerNode* next;
    PlayerNode(string n) : name(n), runs(0), wickets(0), matches(0), next(nullptr) {}
};

// Team Class with Player ll
class Team {
public:
    string name;
    int played, won, lost, points, totalRuns, totalWickets;
    PlayerNode* players;
    Team* next;
    Team(string n) : name(n), played(0), won(0), lost(0), points(0),totalRuns(0), totalWickets(0), players(nullptr), next(nullptr) {}
    
    void addPlayer(string pName) {
        PlayerNode* newPlayer = new PlayerNode(pName);
        if (!players) {
            players = newPlayer;
        } else {
            PlayerNode* temp = players;
            while (temp->next) temp = temp->next;
            temp->next = newPlayer;
        }
    }
    
    void displayPlayers() {
        PlayerNode* temp = players;
        cout << "  Players: ";
        while (temp) {
            cout << temp->name;
            if (temp->next) cout << ", ";
            temp = temp->next;
        }
        cout << endl;
    }
    
    int getPlayerCount() {
        int count = 0;
        PlayerNode* temp = players;
        while (temp) {
            count++;
            temp = temp->next;
        }
        return count;
    }
};


class BracketNode {
public:
    string teamName;
    int score;
    BracketNode *left, *right;
    
    BracketNode(string name = "TBD") : teamName(name), score(0), left(nullptr), right(nullptr) {}
};

// Knockout Bracket Tree
class KnockoutBracket {
    BracketNode* root;
    
    void displayBracket(BracketNode* node, int depth = 0, string prefix = "") {
        if (!node) return;
        
        for (int i = 0; i < depth; i++) cout << "    ";
        cout << prefix << node->teamName;
        if (node->score > 0) cout << " (" << node->score << ")";
        cout << endl;
        
        if (node->left || node->right) {
            displayBracket(node->left, depth + 1, "L-- ");
            displayBracket(node->right, depth + 1, "R-- ");
        }
    }
    
public:
    KnockoutBracket() : root(nullptr) {}
    
    void createBracket(Team** teams, int count) {
        if (count < 2) return;
        
        root = new BracketNode("Final");
        root->left = new BracketNode("Semi-Final 1");
        root->right = new BracketNode("Semi-Final 2");
        
        if (count >= 2) {
            root->left->left = new BracketNode(teams[0]->name);
            root->left->right = new BracketNode(teams[1]->name);
        }
        if (count >= 4) {
            root->right->left = new BracketNode(teams[2]->name);
            root->right->right = new BracketNode(teams[3]->name);
        }
    }
    
    void display() {
        if (!root) {
            cout << "\nNo knockout bracket created!\n";
            return;
        }
        cout << "\n=== KNOCKOUT BRACKET (Binary Tree) ===\n";
        displayBracket(root);
    }
};

// AVL tree node (ranking of players)
class AVLNode {
public:
    PlayerNode* player;
    AVLNode *left, *right;
    int height;
    
    AVLNode(PlayerNode* p) : player(p), left(nullptr), right(nullptr), height(1) {}
};

// AVL Tree for Player Rankings
class PlayerRankingTree {
    AVLNode* root;
    
    int getHeight(AVLNode* node) {
        return node ? node->height : 0;
    }
    
    int getBalance(AVLNode* node) {
        return node ? getHeight(node->left) - getHeight(node->right) : 0;
    }
    
    AVLNode* rightRotate(AVLNode* y) {
        AVLNode* x = y->left;
        AVLNode* T2 = x->right;
        x->right = y;
        y->left = T2;
        y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
        x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
        return x;
    }
    
    AVLNode* leftRotate(AVLNode* x) {
        AVLNode* y = x->right;
        AVLNode* T2 = y->left;
        y->left = x;
        x->right = T2;
        x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
        y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
        return y;
    }
    
    AVLNode* insertNode(AVLNode* node, PlayerNode* player) {
        if (!node) return new AVLNode(player);
        
        if (player->runs < node->player->runs)
            node->left = insertNode(node->left, player);
        else
            node->right = insertNode(node->right, player);
        
        node->height = 1 + max(getHeight(node->left), getHeight(node->right));
        int balance = getBalance(node);
        
        if (balance > 1 && player->runs < node->left->player->runs)
            return rightRotate(node);
        if (balance < -1 && player->runs > node->right->player->runs)
            return leftRotate(node);
        if (balance > 1 && player->runs > node->left->player->runs) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }
        if (balance < -1 && player->runs < node->right->player->runs) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }
        
        return node;
    }
    
    void inorderTraversal(AVLNode* node, int& rank) {
        if (!node) return;
        inorderTraversal(node->right, rank);
        cout << "  " << rank++ << ". " << node->player->name 
             << " - " << node->player->runs << " runs, " 
             << node->player->wickets << " wickets\n";
        inorderTraversal(node->left, rank);
    }
    
public:
    PlayerRankingTree() : root(nullptr) {}
    
    void insert(PlayerNode* player) {
        root = insertNode(root, player);
    }
    
    void displayRankings() {
        if (!root) {
            cout << "\nNo players ranked!\n";
            return;
        }
        cout << "\n=== PLAYER RANKINGS (AVL Tree Traversal) ===\n";
        int rank = 1;
        inorderTraversal(root, rank);
    }
    
    void clear(AVLNode* node) {
        if (!node) return;
        clear(node->left);
        clear(node->right);
        delete node;
    }
    
    void rebuild(Team* teamHead) {
        clear(root);
        root = nullptr;
        Team* temp = teamHead;
        while (temp) {
            PlayerNode* player = temp->players;
            while (player) {
                insert(player);
                player = player->next;
            }
            temp = temp->next;
        }
    }
    
    ~PlayerRankingTree() { clear(root); }
};

// Stack for Undo Operations
class UndoStack {
    string* operations;
    int top, capacity;
public:
    UndoStack(int cap = 50) : top(-1), capacity(cap) {
        operations = new string[capacity];
    }
    void push(string op) {
        if (top < capacity - 1) operations[++top] = op;
    }
    string pop() {
        return (top >= 0) ? operations[top--] : "";
    }
    bool isEmpty() { return top == -1; }
    ~UndoStack() { delete[] operations; }
};

// Queue for Match Scheduling
class MatchNode {
public:
    string team1, team2;
    MatchNode* next;
    MatchNode(string t1, string t2) : team1(t1), team2(t2), next(nullptr) {}
};

class MatchQueue {
    MatchNode *front, *rear;
public:
    MatchQueue() : front(nullptr), rear(nullptr) {}
    
    void enqueue(string t1, string t2) {
        MatchNode* newMatch = new MatchNode(t1, t2);
        if (!rear) {
            front = rear = newMatch;
        } else {
            rear->next = newMatch;
            rear = newMatch;
        }
    }
    
    bool dequeue(string& t1, string& t2) {
        if (!front) return false;
        MatchNode* temp = front;
        t1 = temp->team1;
        t2 = temp->team2;
        front = front->next;
        if (!front) rear = nullptr;
        delete temp;
        return true;
    }
    
    bool isEmpty() { return front == nullptr; }
    
    int count() {
        int c = 0;
        MatchNode* temp = front;
        while (temp) {
            c++;
            temp = temp->next;
        }
        return c;
    }
};

// Hash Table for Quick Team Lookup
class HashTable {
    Team** table;
    int size;
    
    int hashFunction(string key) {
        int hash = 0;
        for (char c : key) hash = (hash * 31 + c) % size;
        return hash;
    }
    
public:
    HashTable(int s = 20) : size(s) {
        table = new Team*[size];
        for (int i = 0; i < size; i++) table[i] = nullptr;
    }
    
    void insert(Team* team) {
        int idx = hashFunction(team->name);
        if (!table[idx]) {
            table[idx] = team;
        } else {
            Team* temp = table[idx];
            while (temp->next) temp = temp->next;
            temp->next = team;
        }
    }
    
    Team* search(string name) {
        int idx = hashFunction(name);
        Team* temp = table[idx];
        while (temp) {
            if (temp->name == name) return temp;
            temp = temp->next;
        }
        return nullptr;
    }
};

// Priority Queue (Max Heap) for Top Scorers
class MaxHeap {
    PlayerNode** heap;
    int size, capacity;
    
    int parent(int i) { return (i - 1) / 2; }
    int left(int i) { return 2 * i + 1; }
    int right(int i) { return 2 * i + 2; }
    
    void heapifyUp(int i) {
        while (i > 0 && heap[parent(i)]->wickets < heap[i]->wickets) {
            PlayerNode* temp = heap[parent(i)];
            heap[parent(i)] = heap[i];
            heap[i] = temp;
            i = parent(i);
        }
    }
    
    void heapifyDown(int i) {
        int maxIdx = i;
        int l = left(i), r = right(i);
        if (l < size && heap[l]->wickets > heap[maxIdx]->wickets) maxIdx = l;
        if (r < size && heap[r]->wickets > heap[maxIdx]->wickets) maxIdx = r;
        if (i != maxIdx) {
            PlayerNode* temp = heap[i];
            heap[i] = heap[maxIdx];
            heap[maxIdx] = temp;
            heapifyDown(maxIdx);
        }
    }
    
public:
    MaxHeap(int cap = 100) : size(0), capacity(cap) {
        heap = new PlayerNode*[capacity];
    }
    
    void insert(PlayerNode* player) {
        if (size < capacity) {
            heap[size] = player;
            heapifyUp(size);
            size++;
        }
    }
    
    PlayerNode* extractMax() {
        if (size == 0) return nullptr;
        PlayerNode* max = heap[0];
        heap[0] = heap[--size];
        heapifyDown(0);
        return max;
    }
    
    bool isEmpty() { return size == 0; }
};

// Tournament Management System
class TournamentSystem {
    Team* teamHead;
    HashTable* teamHash;
    MatchQueue* matchQueue;
    UndoStack* undoStack;
    PlayerRankingTree* playerTree;
    KnockoutBracket* knockout;
    int teamCount;
    
    Team* getTeamAt(int index) {
        Team* temp = teamHead;
        for (int i = 0; i < index && temp; i++) temp = temp->next;
        return temp;
    }
    
    void generateRoundRobinFixtures() {
        if (teamCount < 2) {
            cout << "Need at least 2 teams!\n";
            return;
        }
        
        for (int i = 0; i < teamCount; i++) {
            for (int j = i + 1; j < teamCount; j++) {
                Team* t1 = getTeamAt(i);
                Team* t2 = getTeamAt(j);
                matchQueue->enqueue(t1->name, t2->name);
            }
        }
        cout << "\n*** " << matchQueue->count() << " Round-robin fixtures generated! ***\n";
    }
    
    void quickSortTeams(Team** arr, int low, int high) {
        if (low < high) {
            Team* pivot = arr[high];
            int i = low - 1;
            for (int j = low; j < high; j++) {
                if (arr[j]->points > pivot->points || 
                    (arr[j]->points == pivot->points && arr[j]->won > pivot->won)) {
                    i++;
                    Team* temp = arr[i];
                    arr[i] = arr[j];
                    arr[j] = temp;
                }
            }
            Team* temp = arr[i + 1];
            arr[i + 1] = arr[high];
            arr[high] = temp;
            int pi = i + 1;
            quickSortTeams(arr, low, pi - 1);
            quickSortTeams(arr, pi + 1, high);
        }
    }
    
public:
    TournamentSystem() : teamHead(nullptr), teamCount(0) {
        teamHash = new HashTable();
        matchQueue = new MatchQueue();
        undoStack = new UndoStack();
        playerTree = new PlayerRankingTree();
        knockout = new KnockoutBracket();
    }
    
    void addTeam() {
        string teamName;
        int numPlayers;
        cout << "\nEnter team name: ";
        cin.ignore();
        getline(cin, teamName);
        
        Team* newTeam = new Team(teamName);
        cout << "Enter number of players: ";
        cin >> numPlayers;
        cin.ignore();
        
        for (int i = 0; i < numPlayers; i++) {
            string playerName;
            cout << "  Player " << (i + 1) << " name: ";
            getline(cin, playerName);
            newTeam->addPlayer(playerName);
        }
        
        if (!teamHead) {
            teamHead = newTeam;
        } else {
            Team* temp = teamHead;
            while (temp->next) temp = temp->next;
            temp->next = newTeam;
        }
        
        teamHash->insert(newTeam);
        teamCount++;
        undoStack->push("ADD_TEAM:" + teamName);
        cout << "Team added successfully!\n";
    }
    
    void displayTeams() {
        if (!teamHead) {
            cout << "\nNo teams registered!\n";
            return;
        }
        cout << "\n=== REGISTERED TEAMS ===\n";
        Team* temp = teamHead;
        int count = 1;
        while (temp) {
            cout << count++ << ". " << temp->name << endl;
            temp->displayPlayers();
            temp = temp->next;
        }
    }
    
    void scheduleMatches() {
        generateRoundRobinFixtures();
    }
    
    void simulateAllMatches() {
        if (matchQueue->isEmpty()) {
            cout << "\nNo matches to simulate! Generate fixtures first.\n";
            return;
        }
        
        cout << "\n*** SIMULATING ALL MATCHES ***\n\n";
        int matchNo = 1;
        
        while (!matchQueue->isEmpty()) {
            string t1Name, t2Name;
            matchQueue->dequeue(t1Name, t2Name);
            
            Team* team1 = teamHash->search(t1Name);
            Team* team2 = teamHash->search(t2Name);
            
            if (!team1 || !team2) continue;
            
            int score1 = randomInt(120, 200);
            int score2 = randomInt(120, 200);
            
            cout << "Match " << matchNo++ << ": " << t1Name << " vs " << t2Name << endl;
            cout << "  " << t1Name << ": " << score1 << " runs" << endl;
            cout << "  " << t2Name << ": " << score2 << " runs" << endl;
            
            team1->played++;
            team2->played++;
            team1->totalRuns += score1;
            team2->totalRuns += score2;
            
            // Distribute runs to players
            int p1Count = team1->getPlayerCount();
            int p2Count = team2->getPlayerCount();
            
            PlayerNode* p = team1->players;
            for (int i = 0; i < p1Count && p; i++) {
                int playerRuns = randomInt(10, 50);
                p->runs += playerRuns;
                p->wickets += randomInt(0, 3);
                p->matches++;
                p = p->next;
            }
            
            p = team2->players;
            for (int i = 0; i < p2Count && p; i++) {
                int playerRuns = randomInt(10, 50);
                p->runs += playerRuns;
                p->wickets += randomInt(0, 3);
                p->matches++;
                p = p->next;
            }
            
            if (score1 > score2) {
                team1->won++;
                team1->points += 2;
                team2->lost++;
                cout << "  Winner: " << t1Name << endl;
            } else if (score2 > score1) {
                team2->won++;
                team2->points += 2;
                team1->lost++;
                cout << "  Winner: " << t2Name << endl;
            } else {
                team1->points++;
                team2->points++;
                cout << "  Result: TIE" << endl;
            }
            cout << endl;
        }
        
        playerTree->rebuild(teamHead);
        cout << "*** All matches completed! ***\n";
    }
    
    void displayPointsTable() {
        if (!teamHead) {
            cout << "\nNo teams to display!\n";
            return;
        }
        
        Team** arr = new Team*[teamCount];
        Team* temp = teamHead;
        for (int i = 0; i < teamCount; i++) {
            arr[i] = temp;
            temp = temp->next;
        }
        
        quickSortTeams(arr, 0, teamCount - 1);
        
        cout << "\n=== POINTS TABLE (Quick Sort) ===\n";
        cout << "Pos  Team                P    W    L    Pts   Runs\n";
        cout << "----------------------------------------------------\n";
        for (int i = 0; i < teamCount; i++) {
            cout << (i + 1) << ".   " << arr[i]->name;
            for (int j = arr[i]->name.length(); j < 20; j++) cout << " ";
            cout << arr[i]->played << "    " << arr[i]->won << "    " 
                 << arr[i]->lost << "    " << arr[i]->points 
                 << "     " << arr[i]->totalRuns << endl;
        }
        delete[] arr;
    }
    
    void displayPlayerRankings() {
        playerTree->rebuild(teamHead);
        playerTree->displayRankings();
    }
    
    void createKnockoutBracket() {
        Team** arr = new Team*[teamCount];
        Team* temp = teamHead;
        for (int i = 0; i < teamCount; i++) {
            arr[i] = temp;
            temp = temp->next;
        }
        
        quickSortTeams(arr, 0, teamCount - 1);
        knockout->createBracket(arr, teamCount);
        knockout->display();
        delete[] arr;
    }
    
    void searchTeam() {
        string name;
        cout << "\nEnter team name to search: ";
        cin.ignore();
        getline(cin, name);
        
        Team* team = teamHash->search(name);
        if (team) {
            cout << "\n*** Team Found (Hash Table) ***\n";
            cout << "Name: " << team->name << endl;
            cout << "Played: " << team->played << ", Won: " << team->won 
                 << ", Lost: " << team->lost << ", Points: " << team->points << endl;
            team->displayPlayers();
        } else {
            cout << "Team not found!\n";
        }
    }
    
    void topWicketTakers() {
        MaxHeap heap;
        Team* temp = teamHead;
        
        while (temp) {
            PlayerNode* player = temp->players;
            while (player) {
                heap.insert(player);
                player = player->next;
            }
            temp = temp->next;
        }
        
        cout << "\n=== TOP 5 WICKET TAKERS (Max Heap) ===\n";
        for (int i = 0; i < 5 && !heap.isEmpty(); i++) {
            PlayerNode* top = heap.extractMax();
            if (top) cout << (i + 1) << ". " << top->name << " - " << top->wickets << " wickets\n";
        }
    }
    
    void undo() {
        string lastOp = undoStack->pop();
        if (lastOp.empty()) {
            cout << "\nNothing to undo!\n";
        } else {
            cout << "\nUndone: " << lastOp << endl;
        }
    }
};

int main() {
    TournamentSystem tournament;
    int choice;
    
    cout << "\n*** CRICKET TOURNAMENT MANAGEMENT SYSTEM ***\n";
    
    while (true) {
        cout << "\n========== MAIN MENU ==========\n";
        cout << "1. Add Team\n";
        cout << "2. Display All Teams\n";
        cout << "3. Generate Fixtures (Round Robin)\n";
        cout << "4. Simulate All Matches (Automated)\n";
        cout << "5. Display Points Table (Quick Sort)\n";
        cout << "6. Display Player Rankings (AVL Tree)\n";
        cout << "7. Create Knockout Bracket (BST)\n";
        cout << "8. Search Team (Hash Table)\n";
        cout << "9. Top Wicket Takers (Max Heap)\n";
        cout << "10. Undo Last Operation\n";
        cout << "0. Exit\n";
        cout << "===============================\n";
        cout << "Enter choice: ";
        cin >> choice;
        
        switch (choice) {
            case 1: tournament.addTeam(); break;
            case 2: tournament.displayTeams(); break;
            case 3: tournament.scheduleMatches(); break;
            case 4: tournament.simulateAllMatches(); break;
            case 5: tournament.displayPointsTable(); break;
            case 6: tournament.displayPlayerRankings(); break;
            case 7: tournament.createKnockoutBracket(); break;
            case 8: tournament.searchTeam(); break;
            case 9: tournament.topWicketTakers(); break;
            case 10: tournament.undo(); break;
            case 0: cout << "\nThank you for using the system!\n"; return 0;
            default: cout << "\nInvalid choice! Try again.\n";
        }
    }
    
    return 0;
}
