#include "stdc++.h"
#include<time.h> 
using namespace std;

//Graph class
class Graph{
    public:
    int n;
    unordered_set<int> *adj;
    Graph(int n){
        this->n=n;
        this->adj=new unordered_set<int>[n+1];
    }
    void add_edge(int u, int v){
        adj[u].insert(v);
    }
    void print(){
        for(int i=1;i<=n;i++){
            cout<<i<<"->";
            for(auto x:adj[i]){
                cout<<x<<","<<" ";
            }
            cout<<endl;
        }
    }
    int num_edges(){
        int m=0;
        for(int i=1;i<=n;i++){
            for(auto x:adj[i]){
                m++;
            }
            
        }
        return m;
    }
};


//update class
class update{
    public:
    int node;
    int levels;
    int entry_time;
    int process_time;
    update(){
        node=0;
        levels=0;
        entry_time=0;
        process_time=0;
    }
    update(int n,int l,int e, int p){
        node=n;
        levels=l;
        entry_time=e;
        process_time=p;
    }
    
    bool operator< (update u){
        if(entry_time!=u.entry_time)
        return entry_time<u.entry_time;
        if(process_time!=u.process_time)
        return process_time<u.process_time;
        if(levels!=u.levels)
        return levels<u.levels;
        return node<u.node;
    }
    
};



//compare function for two updates. Used for sorting updates by entry_time and to break ties.
bool compare(const update& u1, const update& u2){
        if(u1.entry_time!=u2.entry_time)
        return u1.entry_time<u2.entry_time;
        if(u1.process_time!=u2.process_time)
        return u1.process_time<u2.process_time;
        if(u1.levels!=u2.levels)
        return u1.levels<u2.levels;
        return u1.node<u2.node;
}

//Generating a random DAG
Graph generate_DAG(int n, int m){

    int avg=m/(n-1);
    int edges=0;
    srand(0); 
    unordered_set<int>* uset=new unordered_set<int>[n+1];

    for(int i=2;i<=n;i++){
        int deg=min(avg,i-1);
        deg=max(deg,1);
        bool b=0;
        while(deg--){
            int random=rand()%(i-1)+1;
            int prob=rand()%100;
            if(prob>50){
                uset[random].insert(i);
                edges++;
                b=1;
            }
        }
        if(b==0){
            int random=rand()%(i-1)+1;
            uset[random].insert(i);
            edges++;
        }
    }
    while(edges<m){
        int u1=rand()%n+1;
        int u2=rand()%n+1;
        if(u1==u2)
        continue;
        else{
            if(u1>u2){
                int temp=u1;
                u1=u2;
                u2=temp;
            }
            if(uset[u1].find(u2)==uset[u1].end()){
                uset[u1].insert(u2);
                edges++;
            }
        }
    }
    
    Graph g(n);
    for(int i=1;i<=n;i++){
        for(auto x:uset[i]){
            g.add_edge(i,x);
        }
    }
  
    return g;
}

//Generate random updates
update* generate_updates(int num_nodes,int num_updates,int levels_low=1,int levels_high=5,int entry_low=0,int entry_high=100,int process_low=1,int process_high=10){
    update* updates=new update[num_updates+1];
    srand(0);
    for(int i=1;i<=num_updates;i++){
        updates[i].node=rand()%(num_nodes)+1;
        updates[i].levels=rand()%(levels_high-levels_low+1)+levels_low;
        updates[i].entry_time=rand()%(entry_high-entry_low+1)+entry_low;
        updates[i].process_time=rand()%(process_high-process_low+1)+process_low;
    }
    sort(updates+1,updates+num_updates+1,compare);
    return updates;
}


void print_updates(update* updates,int n){
    for(int i=1;i<=n;i++){
        update x=updates[i];
        cout<<x.node<<" "<<x.levels<<" "<<x.entry_time<<" "<<x.process_time<<endl;
    }
    return;
}



struct element{
    int id;
    update* updates;
    int time;
    element(int id,update* updates,int time){
        this->id=id;
        this->updates=updates;
        this->time=time;
    }
    bool operator<(const element& e)const{
        if(this->time!=e.time)
        return this->time<e.time;
        if(updates[id].process_time!=updates[e.id].process_time)
        return updates[id].process_time<updates[e.id].process_time;
        if(updates[id].levels!=updates[e.id].levels)
        return updates[id].levels<updates[e.id].levels;
        return updates[id].node<updates[e.id].node;
    }
};

int compute_total_time(Graph* update_graph,update* updates,int num_updates){
    int total_time=0;
    set<element> st;
    int* time=new int[num_updates+1];
    for(int i=1;i<=num_updates;i++){
        time[i]=updates[i].entry_time;
        st.insert(element(i,updates,time[i]));
    }
    while(!st.empty()){
        
        if(st.size()%10000==0)
        cout<<"num of pending updates="<<st.size()<<endl;
        set<element>::iterator p=st.begin();
        int  uid=p->id;
        
        st.erase(st.begin());
        for(auto x:update_graph->adj[uid]){
            if(time[x]>=time[uid] && time[x]<time[uid]+updates[uid].process_time){
                st.erase(element(x,updates,time[x]));
                time[x]=time[uid]+updates[uid].process_time;
                st.insert(element(x,updates,time[x]));

            }

        }
        total_time=max(total_time,time[uid]+updates[uid].process_time);

    }
    return total_time;
}

void addedges(Graph* g1,Graph* g,update * updates,int id){
    int start_node=updates[id].node;
    int levels=updates[id].levels;
   
    unordered_set<int> explored;
    queue<pair<int,int> > q;
    q.push(make_pair(start_node,1));
    explored.insert(start_node);
    g1->add_edge(start_node,id);
   
    while(!q.empty()){
        if(q.front().second>=levels)
        break;
        for(auto x:g->adj[q.front().first]){
            if(explored.find(x)==explored.end()){
                explored.insert(x);
                
                q.push(make_pair(x,q.front().second+1));
                g1->add_edge(x,id);

            } 
           
        }
         q.pop();

    }
   
    return;
}
void build_update_graph(Graph* g,update* updates, int num_updates, Graph* update_graph ){
    int num_nodes=g->n;
    Graph g1(num_nodes);
    for(int i=1;i<=num_updates;i++){
        if(i%100000==0)
        cout<<"i="<<i<<endl;        
        addedges(&g1,g,updates,i);
    }
    cout<<"built g1"<<endl;
    for(int i=1;i<=num_nodes;i++){
        if(i%10000==0)
        cout<<"i="<<i<<endl;
        unordered_set<int>::iterator itr1,itr2;
        for(itr1=g1.adj[i].begin();itr1!=g1.adj[i].end();itr1++){
        
            for(itr2=g1.adj[i].begin();itr2!=g1.adj[i].end();itr2++){
                if((*itr1) != (*itr2))
                update_graph->add_edge(*itr1,*itr2);
                
            }
        }
    }
    return;
}

int main(){
    time_t mytime1 = time(NULL);
    char * time_str1 = ctime(&mytime1);
    time_str1[strlen(time_str1)-1] = '\0';
    cout<<time_str1<<endl;

    clock_t t=clock();
    int n=1000000;
    int m=3000000;
    int u=100000;
    Graph g= generate_DAG(n,m);
    cout<<"generated DAG"<<endl;
    //g.print();
    update* updates=generate_updates(n,u);
    cout<<"generated updates"<<endl;
    //print_updates(updates,u);
    Graph update_graph(u);
    cout<<"building update graph"<<endl;
    build_update_graph(&g,updates,u,&update_graph);
    cout<<"built update graph"<<endl;
    //update_graph.print();
    //cout<<update_graph.num_edges()<<endl;
    cout<<"Total time to process all updates="<<compute_total_time(&update_graph,updates,u)<<endl;
    // t=clock()-t;
    // cout<<t<<endl;
    time_t mytime2 = time(NULL);
    char * time_str2 = ctime(&mytime2);
    time_str2[strlen(time_str2)-1] = '\0';
    cout<<time_str2<<endl;
    cout << "Total time taken by program = "<< difftime(mytime2, mytime1) << " seconds " << endl; 
    
}