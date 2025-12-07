#include <iostream>
#include <string>
using namespace std;

// ==================== CLASSES ====================
class Player {
public:
    string name;
    int runs, wickets;
    Player* next;
    Player(string n = "") : name(n), runs(0), wickets(0), next(NULL) {}
};

class Team {
public:
    string name;
    Player* players;
    int played, won, lost, tied, points, playerCount;
    Team* next;
    
    Team(string n = "") : name(n), players(NULL), played(0), won(0), 
                          lost(0), tied(0), points(0), playerCount(0), next(NULL) {}
    
    void addPlayer(string pName) {
        Player* p = new Player(pName);
        if (!players) players = p;
        else {
            Player* temp = players;
            while (temp->next) temp = temp->next;
            temp->next = p;
        }
        playerCount++;
    }
    
    void removeLastPlayer() {
        if (!players) return;
        if (!players->next) { delete players; players = NULL; playerCount--; return; }
        Player* temp = players;
        while (temp->next->next) temp = temp->next;
        delete temp->next;
        temp->next = NULL;
        playerCount--;
    }
};

class Match {
public:
    string team1, team2, winner;
    int score1, score2;
    bool completed;
    Match* next;
    
    Match(string t1, string t2) : team1(t1), team2(t2), winner(""), 
                                  score1(0), score2(0), completed(false), next(NULL) {}
};

class UndoAction {
public:
    string type, data1, data2;
    int int1, int2;
    UndoAction* next;
    
    UndoAction(string t, string d1="", string d2="", int i1=0, int i2=0) 
        : type(t), data1(d1), data2(d2), int1(i1), int2(i2), next(NULL) {}
};

// ==================== SORTING ALGORITHMS ====================
class Sorter {
public:
    // Merge Sort for Teams (Recursive)
    static Team* mergeSorted(Team* l, Team* r) {
        if (!l) return r;
        if (!r) return l;
        if (l->points > r->points || (l->points == r->points && l->won > r->won)) {
            l->next = mergeSorted(l->next, r);
            return l;
        }
        r->next = mergeSorted(l, r->next);
        return r;
    }
    
    static Team* mergeSort(Team* head) {
        if (!head || !head->next) return head;
        Team *slow = head, *fast = head->next;
        while (fast && fast->next) { slow = slow->next; fast = fast->next->next; }
        Team* mid = slow->next;
        slow->next = NULL;
        return mergeSorted(mergeSort(head), mergeSort(mid));
    }
    
    // Quick Sort for Players (Recursive)
    static Player* getLast(Player* h) {
        while (h && h->next) h = h->next;
        return h;
    }
    
    static Player* partition(Player* head, Player* end, Player** newHead, Player** newEnd) {
        Player *pivot = end, *prev = NULL, *cur = head, *tail = pivot;
        while (cur != pivot) {
            if (cur->runs > pivot->runs) {
                if (!*newHead) *newHead = cur;
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
        if (!*newHead) *newHead = pivot;
        *newEnd = tail;
        return pivot;
    }
    
    static Player* quickSortRec(Player* head, Player* end) {
        if (!head || head == end) return head;
        Player *newHead = NULL, *newEnd = NULL;
        Player* pivot = partition(head, end, &newHead, &newEnd);
        if (newHead != pivot) {
            Player* temp = newHead;
            while (temp->next != pivot) temp = temp->next;
            temp->next = NULL;
            newHead = quickSortRec(newHead, temp);
            temp = getLast(newHead);
            temp->next = pivot;
        }
        pivot->next = quickSortRec(pivot->next, newEnd);
        return newHead;
    }
    
    static Player* quickSort(Player* head) {
        return head ? quickSortRec(head, getLast(head)) : NULL;
    }
};

// ==================== TOURNAMENT MANAGEMENT ====================
class Tournament {
private:
    Team* teams;
    Match* matches;
    UndoAction* undoStack;
    int teamCount, matchCount;
    
    void pause() { cout << "\nPress Enter..."; cin.ignore(); cin.get(); }
    
    Team* findTeam(string name) {
        Team* t = teams;
        while (t && t->name != name) t = t->next;
        return t;
    }
    
    Team* getTeamByIndex(int index) {
        Team* t = teams;
        for (int i = 1; i < index && t; i++) t = t->next;
        return t;
    }
    
    Match* getMatchByIndex(int index) {
        Match* m = matches;
        for (int i = 1; i < index && m; i++) m = m->next;
        return m;
    }
    
    void pushUndo(string type, string d1="", string d2="", int i1=0, int i2=0) {
        UndoAction* u = new UndoAction(type, d1, d2, i1, i2);
        u->next = undoStack;
        undoStack = u;
    }
    
public:
    Tournament() : teams(NULL), matches(NULL), undoStack(NULL), teamCount(0), matchCount(0) {}
    
    void createTeam() {
        cout << "\n=== CREATE TEAM ===\n";
        string name;
        cout << "Team name: ";
        cin.ignore();
        getline(cin, name);
        if (name.empty()) { cout << "Error: Empty name!\n"; pause(); return; }
        
        Team* t = new Team(name);
        if (!teams) teams = t;
        else {
            Team* temp = teams;
            while (temp->next) temp = temp->next;
            temp->next = t;
        }
        teamCount++;
        pushUndo("TEAM", name);
        cout << "? Team created!\n";
        pause();
    }
    
    void addPlayer() {
        cout << "\n=== ADD PLAYER ===\n";
        if (!teams) { cout << "No teams!\n"; pause(); return; }
        
        cout << "Teams:\n";
        Team* t = teams;
        int i = 1;
        while (t) { cout << i++ << ". " << t->name << "\n"; t = t->next; }
        
        int choice;
        cout << "Select team: "; cin >> choice;
        t = getTeamByIndex(choice);
        if (!t) { cout << "Invalid!\n"; pause(); return; }
        
        string name;
        cout << "Player name: ";
        cin.ignore();
        getline(cin, name);
        if (name.empty()) { cout << "Error: Empty name!\n"; pause(); return; }
        
        t->addPlayer(name);
        pushUndo("PLAYER", t->name, name);
        cout << "? Player added!\n";
        pause();
    }
    
    void addFixture() {
        cout << "\n=== ADD FIXTURE ===\n";
        if (teamCount < 2) { cout << "Need 2+ teams!\n"; pause(); return; }
        
        cout << "Teams:\n";
        Team* t = teams;
        int i = 1;
        while (t) { cout << i++ << ". " << t->name << "\n"; t = t->next; }
        
        int c1, c2;
        cout << "Team 1: "; cin >> c1;
        cout << "Team 2: "; cin >> c2;
        
        Team *t1 = getTeamByIndex(c1), *t2 = getTeamByIndex(c2);
        if (!t1 || !t2 || t1 == t2) { cout << "Invalid!\n"; pause(); return; }
        
        Match* m = new Match(t1->name, t2->name);
        if (!matches) matches = m;
        else {
            Match* temp = matches;
            while (temp->next) temp = temp->next;
            temp->next = m;
        }
        matchCount++;
        pushUndo("FIXTURE", t1->name, t2->name);
        cout << "? Fixture added!\n";
        pause();
    }
    
    void enterResult() {
        cout << "\n=== ENTER RESULT ===\n";
        if (!matches) { cout << "No fixtures!\n"; pause(); return; }
        
        cout << "Pending matches:\n";
        Match* m = matches;
        int i = 1;
        while (m) {
            if (!m->completed) cout << i << ". " << m->team1 << " vs " << m->team2 << "\n";
            i++;
            m = m->next;
        }
        
        int num;
        cout << "Select: "; cin >> num;
        m = getMatchByIndex(num);
        if (!m || m->completed) { cout << "Invalid!\n"; pause(); return; }
        
        cout << m->team1 << " score: "; cin >> m->score1;
        cout << m->team2 << " score: "; cin >> m->score2;
        
        Team* t1 = findTeam(m->team1);
        Team* t2 = findTeam(m->team2);
        
        if (m->score1 > m->score2) {
            m->winner = m->team1;
            t1->won++; t1->points += 2; t2->lost++;
        } else if (m->score2 > m->score1) {
            m->winner = m->team2;
            t2->won++; t2->points += 2; t1->lost++;
        } else {
            t1->tied++; t2->tied++;
            t1->points++; t2->points++;
        }
        
        t1->played++; t2->played++;
        m->completed = true;
        pushUndo("RESULT", m->team1, m->team2, m->score1, m->score2);
        cout << "? Result recorded!\n";
        pause();
    }
    
    void viewPoints() {
        cout << "\n=== POINTS TABLE ===\n\n";
        if (!teams) { cout << "No teams!\n"; pause(); return; }
        
        Team* sorted = Sorter::mergeSort(teams);
        cout << "Pos Team                 P   W   L   T  Pts\n";
        cout << "--------------------------------------------\n";
        
        int pos = 1;
        while (sorted) {
            cout << pos++ << ".  " << sorted->name;
            for (int i = sorted->name.length(); i < 20; i++) cout << " ";
            cout << sorted->played << "   " << sorted->won << "   " 
                 << sorted->lost << "   " << sorted->tied << "  " << sorted->points << "\n";
            sorted = sorted->next;
        }
        pause();
    }
    
    void viewTeams() {
        cout << "\n=== ALL TEAMS ===\n\n";
        if (!teams) { cout << "No teams!\n"; pause(); return; }
        
        Team* t = teams;
        int i = 1;
        while (t) {
            cout << i++ << ". " << t->name << " (Players: " << t->playerCount 
                 << ", " << t->won << "W-" << t->lost << "L-" << t->tied << "T)\n";
            if (t->players) {
                cout << "   Squad: ";
                Player* p = t->players;
                while (p) { cout << p->name; if (p->next) cout << ", "; p = p->next; }
                cout << "\n";
            }
            t = t->next;
        }
        pause();
    }
    
    void viewFixtures() {
        cout << "\n=== ALL FIXTURES ===\n\n";
        if (!matches) { cout << "No fixtures!\n"; pause(); return; }
        
        Match* m = matches;
        int i = 1;
        while (m) {
            cout << i++ << ". " << m->team1 << " vs " << m->team2;
            if (m->completed) {
                cout << " - DONE (" << m->team1 << " " << m->score1 
                     << "-" << m->score2 << " " << m->team2 << ")";
                if (m->winner != "") cout << " Winner: " << m->winner;
                else cout << " TIE";
            } else cout << " - PENDING";
            cout << "\n";
            m = m->next;
        }
        pause();
    }
    
    void viewTopScorers() {
        cout << "\n=== TOP SCORERS ===\n\n";
        if (!teams) { cout << "No teams!\n"; pause(); return; }
        
        Player* all = NULL;
        Team* t = teams;
        while (t) {
            Player* p = t->players;
            while (p) {
                Player* copy = new Player(p->name);
                copy->runs = p->runs;
                copy->wickets = p->wickets;
                copy->next = all;
                all = copy;
                p = p->next;
            }
            t = t->next;
        }
        
        if (!all) { cout << "No players!\n"; pause(); return; }
        
        all = Sorter::quickSort(all);
        cout << "Rank Player              Runs Wickets\n";
        cout << "--------------------------------------\n";
        
        int rank = 1;
        while (all) {
            cout << rank++ << ".   " << all->name;
            for (int i = all->name.length(); i < 20; i++) cout << " ";
            cout << all->runs << "   " << all->wickets << "\n";
            Player* del = all;
            all = all->next;
            delete del;
        }
        pause();
    }
    
    void undo() {
        cout << "\n=== UNDO ===\n";
        if (!undoStack) { cout << "Nothing to undo!\n"; pause(); return; }
        
        UndoAction* u = undoStack;
        undoStack = undoStack->next;
        cout << "Undoing: " << u->type << "\n";
        
        if (u->type == "TEAM") {
            if (teams && !teams->next) { delete teams; teams = NULL; }
            else if (teams) {
                Team* t = teams;
                while (t->next->next) t = t->next;
                delete t->next;
                t->next = NULL;
            }
            teamCount--;
        } else if (u->type == "PLAYER") {
            Team* t = findTeam(u->data1);
            if (t) t->removeLastPlayer();
        } else if (u->type == "FIXTURE") {
            if (matches && !matches->next) { delete matches; matches = NULL; }
            else if (matches) {
                Match* m = matches;
                while (m->next->next) m = m->next;
                delete m->next;
                m->next = NULL;
            }
            matchCount--;
        }
        
        delete u;
        cout << "? Undone!\n";
        pause();
    }
    
    void menu() {
        int choice;
        do {
            cout << "\n======================================\n";
            cout << "  CRICKET TOURNAMENT MANAGEMENT\n";
            cout << "======================================\n";
            cout << "1. Create Team\n";
            cout << "2. Add Player\n";
            cout << "3. Add Fixture\n";
            cout << "4. Enter Result\n";
            cout << "5. Points Table\n";
			cout << "6. View Teams\n";
			cout << "7. View Fixtures\n";
			cout << "8. Top Scorers\n";
			cout << "9. Undo\n";
			cout << "10. Exit\n";
            cout << "Choice: ";
            cin >> choice;
            
            switch (choice) {
                case 1: createTeam(); break;
                case 2: addPlayer(); break;
                case 3: addFixture(); break;
                case 4: enterResult(); break;
                case 5: viewPoints(); break;
                case 6: viewTeams(); break;
                case 7: viewFixtures(); break;
                case 8: viewTopScorers(); break;
                case 9: undo(); break;
                case 10: cout << "\nGoodbye!\n"; break;
                default: cout << "\nInvalid!\n"; pause();
            }
        } while (choice != 10);
    }
    
    ~Tournament() {
        while (teams) { Team* t = teams; teams = teams->next; delete t; }
        while (matches) { Match* m = matches; matches = matches->next; delete m; }
        while (undoStack) { UndoAction* u = undoStack; undoStack = undoStack->next; delete u; }
    }
};

int main() {
    Tournament t;
    t.menu();
    return 0;
}
