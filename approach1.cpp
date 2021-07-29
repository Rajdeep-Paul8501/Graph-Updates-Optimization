#include "stdc++.h"
#include<time.h> 
using namespace std;

//Graph class
class Graph{
    public:
    int n;
    vector<int> *adj;
    Graph(int n){
        this->n=n;
        this->adj=new vector<int>[n+1];
    }
    void add_edge(int u, int v){
        adj[u].push_back(v);
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

// finds maximum time at which all nodes in a particular update finish processing
int max_finish_time(Graph *g,update* updates,int* nodes_finish_time, int x,unordered_set<int>* explored){
    //return 1;
    int start_node=updates[x].node;
    int levels=updates[x].levels;
    int finish_time=nodes_finish_time[start_node];
    
    queue<pair<int,int> > q;
    q.push(make_pair(start_node,1));
    explored->insert(start_node);
    while(!q.empty()){
        if(q.front().second >=levels){
            break;
        }
        for(auto x:g->adj[q.front().first]){
            if(explored->find(x)==explored->end()){
                explored->insert(x);
                q.push(make_pair(x,q.front().second+1));
                finish_time=max(finish_time,nodes_finish_time[x]);
            }
        }
        q.pop();

    }
    return finish_time;
}

//function to compute total time
int compute_total_time(Graph *g, update* updates, int num_updates){
    
    int total_time=0;
    int max_start_time=0;
    int num_nodes=g->n;
    int nodes_finish_time[num_nodes+1];
    int num_pending_updates=num_updates;
    for(int i=1;i<=num_nodes;i++)
    nodes_finish_time[i]=0;
    
    unordered_map<int,set<int> > time_update_map;
    
    
    
    for(int i=1;i<=num_updates;i++){
        max_start_time=max(max_start_time,updates[i].entry_time);
        
        
        time_update_map[updates[i].entry_time].insert(i);
       
    }
    clock_t time_count=0;
    clock_t time_else=0;
    clock_t time_bfs=0;
    clock_t time_if=0;
    clock_t time_overall=0;
    clock_t time_erase=0;
    int t=0;
    while(t<=max_start_time){
        
        if(t%10==0){
            if(t!=0)
            time_overall=clock()-time_overall;
            cout<<"time_step="<<t<<endl;
            cout<<"Number of pending updates="<<num_pending_updates<<endl;
            cout<<"time_else="<<time_else<<endl;
            cout<<"time_if="<<time_if<<endl;
            cout<<"time_bfs="<<time_bfs<<endl;
            cout<<"time_overall="<<time_overall<<endl;
            cout<<"time_erase="<<time_erase<<endl;
            time_else=0;
            time_if=0;
            time_bfs=0;
            time_erase=0;
            time_overall=clock();

        }
    
        if(time_update_map.find(t)!=time_update_map.end()){
            
            for(auto x:time_update_map[t]){
               
                time_count=clock();
                unordered_set<int> explored;
                int possible_execute_time=max_finish_time(g,updates,nodes_finish_time,x,&explored);
                time_count=clock()-time_count;
                time_bfs+=time_count;
                if(possible_execute_time<=t){
                    time_count=clock();
                    total_time=max(total_time,t+updates[x].process_time);
                    for(auto y:explored){
                        nodes_finish_time[y]=t+updates[x].process_time;
                    }
                    num_pending_updates--;
                    time_count=clock()-time_count;
                    time_if+=time_count;
                }
                else{
                    
                   time_count=clock();
                    time_update_map[possible_execute_time].insert(x);
                   
                    max_start_time=max(max_start_time,possible_execute_time);
                    time_count=clock()-time_count;
                    time_else+=time_count;
                }
            }
            time_count=clock();
            time_update_map.erase(t);
            time_count=clock()-time_count;
            time_erase+=time_count;

            
        }
        
        t++;
       

    }
    return total_time;

}

//main
int main(){
    time_t mytime1 = time(NULL);
    char * time_str1 = ctime(&mytime1);
    time_str1[strlen(time_str1)-1] = '\0';
    cout<<time_str1<<endl;
    clock_t program_time=clock();
    //n is number of nodes
    int n=1000000;   
    // m is number of edges                                 
    int m=3000000;  
    // u is number of updates                                            
    int u=100000;                                              
    clock_t time_generate_graph=clock();
    // g is a random DAG
    Graph g=generate_DAG(n,m);                                  
    time_generate_graph=clock()-time_generate_graph;
    cout<<"generated DAG"<<endl;  
    // printing DAG          
    //g.print();
    clock_t time_generate_updates =clock();
    // generating random updates
    update* updates=generate_updates(n,u);
    time_generate_updates=clock()-time_generate_updates;
    cout<<"generated updates"<<endl;
    // printing updates
    //print_updates(updates,u);
    clock_t time_compute_total_time=clock();
    // computing time to process all updates
    int total_time=compute_total_time(&g,updates,u);
    time_compute_total_time=clock()-time_compute_total_time;
    program_time=clock()-program_time;
    cout<<"Total time to process all updates="<<total_time<<endl;
    time_t mytime2 = time(NULL);
    char * time_str2 = ctime(&mytime2);
    time_str2[strlen(time_str2)-1] = '\0';
    cout<<time_str2<<endl;
    cout << "Total time taken by program = "<< difftime(mytime2, mytime1) << " seconds " << endl; 
}


