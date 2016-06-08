#include<iostream>
#include <fstream>
#include <string.h>

using namespace std;

ofstream outfile;

class pipes
{
public:
    string start;
    string end;
    int state[24],length;

    pipes()
    {
        for(int i=0;i<24;i++)
            state[i]=1;
    }

    void starting(string s)
    {
        start=s;
    }
    void ending(string s)
    {
        end=s;
    }
    void len(int l)
    {
        length=l;
    }
    void turn_off(int i)
    {
        state[i]=0;
    }

};

class node
{
    public:
    string name;
    int depth;
    node * next;
};

class ucs
{
    node * start, * dest, * explored, * frontier, * child, * node_p;
    pipes * p;
    int start_time,no_pipes;
    bool found;
    public:
    ucs(string s,node * d, pipes * pipe, int t,int n)
    {
        start=new node;
        start->name=s;
        start->next=NULL;
        start_time=t;
        no_pipes=n;
        dest=d;
        found=false;
        p=pipe;
        frontier=start;
        node_p=frontier;
        frontier->depth=t;
        explored=NULL;
    }

    void begin_search()
    {
        //Write code to check whether start node is destination node
        node * temp;
        while(frontier!=NULL)
        {
            add_children(frontier->name,frontier->depth);
            if(check_list(frontier->name,dest))
                {
                    int x=(frontier->depth)%24;
                    outfile<<frontier->name<<" "<<x<<endl;
                    found=true;
                    break;
                }
            temp=frontier->next;
            if(explored==NULL)
                frontier->next=NULL;
            else
                frontier->next=explored;
            explored=frontier;
            frontier=temp;
        }
        if(!found)
            {
                outfile<<"None"<<endl;
            }
    }

    void add_children(string str,int d)
    {
        int x;
        child=NULL;
        for(int i=0;i<no_pipes;i++)
        {
            x=d%24;
            if(p[i].start==str&&!check_list(p[i].end,explored)&&p[i].state[x]!=0&&!check_list(p[i].end,frontier,d+p[i].length))
            {
                delete_node(frontier,p[i].end);
                child = add_node(child,p[i].end,d+p[i].length);
            }
        }

        //adding child to frontier
        if(child!=NULL)
        {
            node * prev=NULL, * b;
            while(child!=NULL)
            {
                b=frontier;
                while(b!=NULL&&b->depth<=child->depth)
                {
                    if(b->depth==child->depth&&b->name.compare(child->name)>0)
                        break;
                    prev=b;
                    b=b->next;
                }
                b=child;
                child=child->next;
                b->next=prev->next;
                prev->next=b;
            }
        }
    }

    bool check_list(string str,node * l,int d=0)
    {
        bool check=false;
        while(l!=NULL)
        {
            if(l->name==str&&(l->depth<d||d==0))
                {
                    check=true;
                    return true;
                    break;

                }
            l=l->next;
        }
        return check;

    }

    void delete_node(node * l, string str)
    {
        node * prev=NULL;
        while(l!=NULL)
        {

            if(l->name==str)
            {
                prev->next=l->next;
                delete l;
                break;
            }
            prev=l;
            l=l->next;
        }
    }

    node * add_node(node * l,string str,int d)
    {
        node * n = new node;
        n->name=str;
        n->depth=d;
        n->next=NULL;
        if(l==NULL)
            l=n;
        else
        {
            n->next=l;
            l=n;
        }
        return l;
    }
};

class bfs_dfs
{
    node * start, * dest, * explored, * frontier, * child, * end_f, * node_p;
    pipes * p;
    int start_time,no_pipes,type;//type=0 for BFS 1 for DFS
    bool found;
    public:
    bfs_dfs(string s,node * d, pipes * pipe, int t,int n,int m)
    {
        start=new node;
        start->name=s;
        start->next=NULL;
        start_time=t;
        no_pipes=n;
        dest=d;
        type=m;
        found=false;
        p=pipe;
        frontier=start;
        node_p=frontier;
        frontier->depth=0;
        explored=NULL;
    }

    void begin_search()
    {
        //Write code to check whether start node is destination node
        node * temp;
        while(frontier!=NULL)
        {
            add_children(frontier->name,frontier->depth);
            if(check_list(frontier->name,dest))
                {
                    int x=(frontier->depth+start_time)%24;
                    outfile<<frontier->name<<" "<<x<<endl;
                    found=true;
                    break;
                }
            temp=frontier->next;
            if(explored==NULL)
                frontier->next=NULL;
            else
                frontier->next=explored;
            explored=frontier;
            frontier=temp;
        }
        if(frontier==NULL&&!found)
        {
            outfile<<"None"<<endl;
        }
    }

    void add_children(string str,int d)
    {
        child=NULL;
        for(int i=0;i<no_pipes;i++)
        {
            if(p[i].start==str&&!check_list(p[i].end,explored)) //&&!check_list(p[i].end,frontier)
            {
                child = add_node(child,p[i].end,d);
            }
        }

        //adding child to frontier
        if(child!=NULL)
        {
            end_f=child;
            while(end_f!=NULL)
            {
                if(end_f->next==NULL||found)
                    break;
                end_f=end_f->next;
            }

            if(type==0)
            {
                node_p->next=child;
                node_p=end_f;
            }
            else
            {
                end_f->next=frontier->next;
                frontier->next=child;
            }
        }
    }

    bool check_list(string str,node * l)
    {
        bool check=false;
        while(l!=NULL)
        {
            if(l->name==str)
                {
                    check=true;
                    break;
                }
            l=l->next;
        }
        return check;

    }

    node * add_node(node * l,string str,int d)
    {
        node * n = new node;
        node * m = l;
        node * prev=NULL;
        n->name=str;
        n->depth=d+1;
        n->next=NULL;
        if(l==NULL)
            l=n;
        else
        {
            while(m!=NULL&&m->name.compare(str)<0)
            {
                prev=m;
                m=m->next;
            }
            if(prev==NULL)
            {
                n->next=l;
                l=n;
            }
            else
            {
                n->next=prev->next;
                prev->next=n;
            }
        }
        return l;
    }
};

int main(int argc, char* argv[])
{

    char a;
    string data,start_node,type;
    node * dest_node, * nodep, * mid_node;;
    int n,d,i,j,m,k,p,o,start_time;
    ifstream infile;
    if(argc>=2)
        infile.open(argv[2]);
    else
        infile.open("input.txt");
    outfile.open("output.txt");
    infile >> n;
    for(i=1;i<=n;i++)
    {
        infile>>type;
        infile>>start_node;
        dest_node=new node;
        dest_node->next=NULL;
        infile>>data;
        dest_node->name=data;
        infile.get(a);
        d=1;
        while(a!='\n')
        {
            d++;
            nodep=new node;
            nodep->next=dest_node;
            infile>>data;
            nodep->name=data;
            dest_node=nodep;
            infile.get(a);
        }
        nodep=dest_node;
        for(j=0;j<d;j++)
        {
            nodep=nodep->next;
        }

        //Allocating middle nodes
        infile.get(a);
        if(a=='\n')
            ;
        else
        {
            k=0;
            mid_node=new node;
            mid_node->next=NULL;
            while(a!='\n'&&a!=' ')
            {
               mid_node->name+=a;
                infile.get(a);
                k++;
            }
            m=1;

            while(a!='\n')
            {
                m++;
                nodep=new node;
                nodep->next=mid_node;
                infile>>data;
                nodep->name=data;
                mid_node=nodep;
                infile.get(a);
            }
            nodep=mid_node;
            for(j=0;j<m;j++)
            {
                nodep=nodep->next;
            }
        }

        //No of pipes
        infile>>p;
        pipes *pipe = new pipes[p];
        for(j=0;j<p;j++)
        {
            infile>>data;
            pipe[j].starting(data);
            infile>>data;
            pipe[j].ending(data);
            infile>>k;//length
            pipe[j].len(k);
            infile>>o;//no of off-periods
            for(k=0;k<o;k++)
            {
                infile>>data;
                int p1=0,p2=0,l=0;
                while(data[l]!='-')
                    p1=p1*10+data[l++]-48;
                l++;
                while(data[l]!='\0')
                    p2=p2*10+data[l++]-48;
                for(;p1<=p2;p1++)
                    pipe[j].turn_off(p1);
            }


        }
        infile>>start_time;

        if(type=="BFS")
        {
            bfs_dfs * case1 = new bfs_dfs(start_node,dest_node,pipe,start_time,p,0);
            case1->begin_search();
            delete case1;
        }
        else if(type=="DFS")
        {
            bfs_dfs * case1 = new bfs_dfs(start_node,dest_node,pipe,start_time,p,1);
            case1->begin_search();
            delete case1;
        }
        else if(type=="UCS")
        {
            ucs * case1 = new ucs(start_node,dest_node,pipe,start_time,p);
            case1->begin_search();
            delete case1;
        }

        delete[] pipe;
        delete dest_node;
        if(mid_node)
        {
            delete mid_node;
            mid_node=NULL;
        }
    }


    infile.close();

    return 0;
}
