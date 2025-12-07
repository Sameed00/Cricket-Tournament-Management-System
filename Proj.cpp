#include <iostream>
#include <string>
using namespace std;

class player {
public:
    string name;
    int runs, wickets;
    player *next;
    
    player(string n = "") : name(n), runs(0), wickets(0), next(NULL) {}
};

class team {
public:
    string name;
    player *plist;
    int played, won, lost, tied, points, pcount;
    team *next;
    
    team(string n = "") : name(n), plist(NULL), played(0), won(0),lost(0), tied(0), points(0), pcount(0), next(NULL) {}
    
    void addplayer(string pname) {
        player *p = new player(pname);
        if(!plist) plist = p;
        else {
            player *temp = plist;
            while(temp->next) temp = temp->next;
            temp->next = p;
        }
        pcount++;
    }
    
    void removelastplayer() {
        if(!plist) return;
        if(!plist->next) { delete plist; plist = NULL; pcount--; return; }
        player *temp = plist;
        while(temp->next->next) temp = temp->next;
        delete temp->next;
        temp->next = NULL;
        pcount--;
    }
};

class match {
public:
    string team1, team2, winner;
    int score1, score2;
    bool done;
    match *next;
    
    match(string t1, string t2) : team1(t1), team2(t2), winner(""),score1(0), score2(0), done(false), next(NULL) {}
};

class undoaction {
public:
    string type, d1, d2;
    int v1, v2;
    undoaction *next;
    
    undoaction(string t, string a="", string b="", int x=0, int y=0)
        : type(t), d1(a), d2(b), v1(x), v2(y), next(NULL) {}
};

class sorter {
public:
    static team* mergesorted(team *l, team *r) {
        if(!l) return r;
        if(!r) return l;
        if(l->points > r->points || (l->points==r->points && l->won > r->won)) {
            l->next = mergesorted(l->next, r);
            return l;
        }
        r->next = mergesorted(l, r->next);
        return r;
    }
    
    static team* mergesort(team *head) {
        if(!head || !head->next) return head;
        team *slow = head, *fast = head->next;
        while(fast && fast->next) { slow=slow->next; fast=fast->next->next; }
        team *mid = slow->next;
        slow->next = NULL;
        return mergesorted(mergesort(head), mergesort(mid));
    }
    
    static player* getlast(player *h) {
        while(h && h->next) h=h->next;
        return h;
    }
    
    static player* partition(player *head, player *end, player **newHead, player **newEnd) {
        player *pivot = end, *prev=NULL, *cur=head, *tail=pivot;
        
        while(cur != pivot) {
            if(cur->runs > pivot->runs) {
                if(!*newHead) *newHead = cur;
                prev = cur;
                cur = cur->next;
            } else {
                if(prev) prev->next = cur->next;
                player *temp = cur->next;
                cur->next = NULL;
                tail->next = cur;
                tail = cur;
                cur = temp;
            }
        }
        
        if(!*newHead) *newHead = pivot;
        *newEnd = tail;
        return pivot;
    }
    
    static player* quicksortrec(player *head, player *end) {
        if(!head || head==end) return head;
        player *newHead=NULL, *newEnd=NULL;
        player *pivot = partition(head, end, &newHead, &newEnd);
        
        if(newHead != pivot) {
            player *temp = newHead;
            while(temp->next != pivot) temp=temp->next;
            temp->next=NULL;
            newHead = quicksortrec(newHead, temp);
            temp = getlast(newHead);
            temp->next = pivot;
        }
        
        pivot->next = quicksortrec(pivot->next, newEnd);
        return newHead;
    }
    
    static player* quicksort(player *head) {
        return head ? quicksortrec(head, getlast(head)) : NULL;
    }
};

class tournament {
private:
    team *tlist;
    match *mlist;
    undoaction *ustack;
    int tcount, mcount;
    
    void pause() { cout << "\nPress Enter..."; cin.ignore(); cin.get(); }
    
    team* findteam(string nm) {
        team *t = tlist;
        while(t && t->name != nm) t=t->next;
        return t;
    }
    
    team* teambyindex(int idx) {
        team *t = tlist;
        for(int i=1;i<idx && t;i++) t=t->next;
        return t;
    }
    
    match* matchbyindex(int idx) {
        match *m = mlist;
        for(int i=1;i<idx && m;i++) m=m->next;
        return m;
    }
    
    void pushundo(string type, string a="", string b="", int x=0, int y=0) {
        undoaction *u = new undoaction(type, a, b, x, y);
        u->next = ustack;
        ustack = u;
    }
    
public:
    tournament() : tlist(NULL), mlist(NULL), ustack(NULL), tcount(0), mcount(0) {}
    
    void createteam() {
        cout << "\n=== Create Team ===\n";
        string nm;
        cout << "Team name: ";
        cin.ignore();
        getline(cin, nm);
        if(nm.empty()) { cout<<"Error: Empty name!\n"; pause(); return; }
        
        team *t = new team(nm);
        if(!tlist) tlist = t;
        else {
            team *temp = tlist;
            while(temp->next) temp=temp->next;
            temp->next = t;
        }
        
        tcount++;
        pushundo("team", nm);
        cout<<"Team created successfully!\n";
        pause();
    }
    
    void addplayer() {
        cout << "\n=== Add Player ===\n";
        if(!tlist) { cout<<"No teams available!\n"; pause(); return; }
        
        cout<<"Teams:\n";
        team *t = tlist;
        int idx=1;
        while(t) { cout<<idx++<<". "<<t->name<<"\n"; t=t->next; }
        
        int ch;
        cout<<"Select team: ";
        cin>>ch;
        t = teambyindex(ch);
        if(!t) { cout<<"Invalid selection!\n"; pause(); return; }
        
        string nm;
        cout<<"Player name: ";
        cin.ignore();
        getline(cin, nm);
        if(nm.empty()) { cout<<"Error: Empty name!\n"; pause(); return; }
        
        t->addplayer(nm);
        pushundo("player", t->name, nm);
        cout<<"Player added successfully!\n";
        pause();
    }
    
    void addfixture() {
        cout << "\n=== Add Fixture ===\n";
        if(tcount < 2) { cout<<"Need at least 2 teams!\n"; pause(); return; }
        
        cout<<"Teams:\n";
        team *t = tlist;
        int idx=1;
        while(t) { cout<<idx++<<". "<<t->name<<"\n"; t=t->next; }
        
        int a,b;
        cout<<"Team 1: "; cin>>a;
        cout<<"Team 2: "; cin>>b;
        
        team *t1 = teambyindex(a);
        team *t2 = teambyindex(b);
        if(!t1 || !t2 || t1==t2) { cout<<"Invalid selection!\n"; pause(); return; }
        
        match *m = new match(t1->name, t2->name);
        if(!mlist) mlist = m;
        else {
            match *temp = mlist;
            while(temp->next) temp=temp->next;
            temp->next=m;
        }
        
        mcount++;
        pushundo("fixture", t1->name, t2->name);
        cout<<"Fixture added successfully!\n";
        pause();
    }
    
    void enterresult() {
        cout << "\n=== Enter Result ===\n";
        if(!mlist) { cout<<"No fixtures available!\n"; pause(); return; }
        
        cout<<"Pending matches:\n";
        match *m = mlist;
        int idx=1;
        while(m) {
            if(!m->done) cout<<idx<<". "<<m->team1<<" vs "<<m->team2<<"\n";
            idx++;
            m=m->next;
        }
        
        int ch;
        cout<<"Select match: ";
        cin>>ch;
        m = matchbyindex(ch);
        
        if(!m || m->done) { cout<<"Invalid selection!\n"; pause(); return; }
        
        cout<<m->team1<<" score: "; cin>>m->score1;
        cout<<m->team2<<" score: "; cin>>m->score2;
        
        team *t1 = findteam(m->team1);
        team *t2 = findteam(m->team2);
        
        if(m->score1 > m->score2) {
            m->winner = m->team1;
            t1->won++; t1->points+=2; t2->lost++;
        } else if(m->score2 > m->score1) {
            m->winner = m->team2;
            t2->won++; t2->points+=2; t1->lost++;
        } else {
            t1->tied++; t2->tied++;
            t1->points++; t2->points++;
        }
        
        t1->played++; t2->played++;
        m->done = true;
        
        pushundo("result", m->team1, m->team2, m->score1, m->score2);
        cout<<"Result saved successfully!\n";
        pause();
    }
    
    void viewpoints() {
        cout<<"\n=== Points Table ===\n\n";
        if(!tlist) { cout<<"No teams available!\n"; pause(); return; }
        
        team *sorted = sorter::mergesort(tlist);
        
        cout<<"Pos  Team                 P   W   L   T   Pts\n";
        cout<<"----------------------------------------------\n";
        
        int pos=1;
        while(sorted) {
            cout<<pos++<<".   "<<sorted->name;
            for(int i=sorted->name.length(); i<20; i++) cout<<" ";
            cout<<sorted->played<<"   "<<sorted->won<<"   "<<sorted->lost<<"   "<<sorted->tied<<"   "<<sorted->points<<"\n";
            sorted = sorted->next;
        }
        pause();
    }
    
    void viewteams() {
        cout<<"\n=== All Teams ===\n\n";
        if(!tlist) { cout<<"No teams available!\n"; pause(); return; }
        
        team *t = tlist;
        int idx=1;
        while(t) {
            cout<<idx++<<". "<<t->name<<" (Players: "<<t->pcount<<", Record: "<<t->won<<"W-"<<t->lost<<"L-"<<t->tied<<"T)\n";
            if(t->plist) {
                cout<<"   Squad: ";
                player *p = t->plist;
                while(p) { cout<<p->name; if(p->next) cout<<", "; p=p->next; }
                cout<<"\n";
            }
            t=t->next;
        }
        pause();
    }
    
    void viewfixtures() {
        cout<<"\n=== All Fixtures ===\n\n";
        if(!mlist) { cout<<"No fixtures available!\n"; pause(); return; }
        
        match *m = mlist;
        int idx=1;
        
        while(m) {
            cout<<idx++<<". "<<m->team1<<" vs "<<m->team2;
            if(m->done) {
                cout<<" - Completed ("<<m->team1<<" "<<m->score1<<"-"<<m->score2<<" "<<m->team2<<")";
                if(m->winner != "") cout<<" Winner: "<<m->winner;
                else cout<<" Tie";
            } else cout<<" - Pending";
            cout<<"\n";
            m=m->next;
        }
        
        pause();
    }
    
    void viewtopscorers() {
        cout<<"\n=== Top Scorers ===\n\n";
        if(!tlist) { cout<<"No teams available!\n"; pause(); return; }
        
        player *all=NULL;
        team *t = tlist;
        
        while(t) {
            player *p = t->plist;
            while(p) {
                player *copy = new player(p->name);
                copy->runs = p->runs;
                copy->wickets = p->wickets;
                copy->next = all;
                all = copy;
                p=p->next;
            }
            t=t->next;
        }
        
        if(!all) { cout<<"No players available!\n"; pause(); return; }
        
        all = sorter::quicksort(all);
        
        cout<<"Rank  Player               Runs  Wickets\n";
        cout<<"------------------------------------------\n";
        
        int r=1;
        while(all) {
            cout<<r++<<".    "<<all->name;
            for(int i=all->name.length(); i<20; i++) cout<<" ";
            cout<<all->runs<<"    "<<all->wickets<<"\n";
            player *del = all;
            all = all->next;
            delete del;
        }
        pause();
    }
    
    void undo() {
        cout<<"\n=== Undo Last Action ===\n";
        if(!ustack) { cout<<"Nothing to undo!\n"; pause(); return; }
        
        undoaction *u = ustack;
        ustack = ustack->next;
        
        cout<<"Undoing: "<<u->type<<"\n";
        
        if(u->type == "team") {
            if(tlist && !tlist->next) { delete tlist; tlist=NULL; }
            else if(tlist) {
                team *t = tlist;
                while(t->next->next) t=t->next;
                delete t->next;
                t->next=NULL;
            }
            tcount--;
        }
        else if(u->type == "player") {
            team *t = findteam(u->d1);
            if(t) t->removelastplayer();
        }
        else if(u->type == "fixture") {
            if(mlist && !mlist->next) { delete mlist; mlist=NULL; }
            else if(mlist) {
                match *m = mlist;
                while(m->next->next) m=m->next;
                delete m->next;
                m->next=NULL;
            }
            mcount--;
        }
        
        delete u;
        cout<<"Action undone successfully!\n";
        pause();
    }
    
    void menu() {
        int ch;
        do {
            cout<<"\n========================================\n";
            cout<<"    Cricket Tournament Management\n";
            cout<<"========================================\n\n";
            cout<<"1.Create Team\n";
            cout<<"2.Add Player to Team\n";
            cout<<"3.Add Fixture\n";
            cout<<"4.Enter Match Result\n";
            cout<<"5.View Points Table\n";
            cout<<"6.View All Teams\n";
            cout<<"7.View All Fixtures\n";
            cout<<"8.View Top Scorers\n";
            cout<<"9.Undo Last Action\n";
            cout<<"10.Exit\n";
            cout<<"\nEnter your choice: ";
            cin>>ch;
            
            switch(ch) {
                case 1: createteam(); break;
                case 2: addplayer(); break;
                case 3: addfixture(); break;
                case 4: enterresult(); break;
                case 5: viewpoints(); break;
                case 6: viewteams(); break;
                case 7: viewfixtures(); break;
                case 8: viewtopscorers(); break;
                case 9: undo(); break;
                case 10: cout<<"\nThank you for using the system!\n"; break;
                default: cout<<"\nInvalid choice! Please try again.\n"; pause();
            }
        } while(ch != 10);
    }
    
    ~tournament() {
        while(tlist) { 
			team *t=tlist; 
			tlist=tlist->next; 
			delete t; }
			
        while(mlist) { 
			match *m=mlist; 
			mlist=mlist->next; 
			delete m; }
			
        while(ustack) { 
			undoaction *u=ustack; 
			ustack=ustack->next; 
			delete u; }
    }
};

int main() {
    tournament t;
    t.menu();
    return 0;
}
