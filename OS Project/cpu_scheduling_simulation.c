#include <stdio.h>

typedef struct{
    int pid, at, bt, pr;
    int ct, tat, wt, rt, done;
} P;

/* ---------- Utility ---------- */
void avgPrint(P p[], int n){
    float awt=0, atat=0;
    for(int i=0;i<n;i++){
        awt+=p[i].wt;
        atat+=p[i].tat;
    }
    printf("\nAverage Waiting Time = %.2f", awt/n);
    printf("\nAverage Turnaround Time = %.2f\n", atat/n);
}

void printGantt(int gp[], int gt[], int g){
    printf("\nGantt Chart:\n|");
    for(int i=0;i<g;i++)
        printf(" P%d |", gp[i]);
    printf("\n");
    for(int i=0;i<=g;i++)
        printf("%-5d", gt[i]);
    printf("\n");
}

void printTable(P p[], int n){
    printf("\nPID  AT   BT   CT   TAT  WT\n");
    for(int i=0;i<n;i++)
        printf("%-4d %-4d %-4d %-4d %-4d %-4d\n",
               p[i].pid,p[i].at,p[i].bt,p[i].ct,p[i].tat,p[i].wt);
}

/* ---------- FCFS ---------- */
void fcfs(P p[], int n){
    int time,g=0;
    int gt[50],gp[50];

    for(int i=0;i<n-1;i++)
        for(int j=i+1;j<n;j++)
            if(p[i].at>p[j].at){
                P t=p[i]; p[i]=p[j]; p[j]=t;
            }

    time=p[0].at;   // start from first arrival

    for(int i=0;i<n;i++){
        gp[g]=p[i].pid; gt[g++]=time;
        time+=p[i].bt;
        p[i].ct=time;
    }
    gt[g]=time;

    printf("\n--- FCFS ---");
    printGantt(gp,gt,g);

    for(int i=0;i<n;i++){
        p[i].tat=p[i].ct-p[i].at;
        p[i].wt=p[i].tat-p[i].bt;
    }
    printTable(p,n);
    avgPrint(p,n);
}

/* ---------- SJF (Non-Preemptive) ---------- */
void sjf(P p[], int n){
    int time,done=0,g=0;
    int gt[50],gp[50];

    for(int i=0;i<n;i++) p[i].done=0;

    // sort by AT
    for(int i=0;i<n-1;i++)
        for(int j=i+1;j<n;j++)
            if(p[i].at>p[j].at){
                P t=p[i]; p[i]=p[j]; p[j]=t;
            }

    time=p[0].at;

    while(done<n){
        int idx=-1,min=9999;
        for(int i=0;i<n;i++)
            if(!p[i].done && p[i].at<=time && p[i].bt<min){
                min=p[i].bt; idx=i;
            }
        if(idx==-1){ time++; continue; }

        gp[g]=p[idx].pid; gt[g++]=time;
        time+=p[idx].bt;
        p[idx].ct=time;
        p[idx].done=1;
        done++;
    }
    gt[g]=time;

    printf("\n--- SJF ---");
    printGantt(gp,gt,g);

    for(int i=0;i<n;i++){
        p[i].tat=p[i].ct-p[i].at;
        p[i].wt=p[i].tat-p[i].bt;
    }
    printTable(p,n);
    avgPrint(p,n);
}

/* ---------- Priority (Non-Preemptive) ---------- */
void priority(P p[], int n){
    int time,done=0,g=0;
    int gt[50],gp[50];

    for(int i=0;i<n;i++) p[i].done=0;

    for(int i=0;i<n-1;i++)
        for(int j=i+1;j<n;j++)
            if(p[i].at>p[j].at){
                P t=p[i]; p[i]=p[j]; p[j]=t;
            }

    time=p[0].at;

    while(done<n){
        int idx=-1,high=9999;
        for(int i=0;i<n;i++)
            if(!p[i].done && p[i].at<=time && p[i].pr<high){
                high=p[i].pr; idx=i;
            }
        if(idx==-1){ time++; continue; }

        gp[g]=p[idx].pid; gt[g++]=time;
        time+=p[idx].bt;
        p[idx].ct=time;
        p[idx].done=1;
        done++;
    }
    gt[g]=time;

    printf("\n--- Priority ---");
    printGantt(gp,gt,g);

    printf("\nPID  AT   BT   PR   CT   TAT  WT\n");
    for(int i=0;i<n;i++){
        p[i].tat=p[i].ct-p[i].at;
        p[i].wt=p[i].tat-p[i].bt;
        printf("%-4d %-4d %-4d %-4d %-4d %-4d %-4d\n",
               p[i].pid,p[i].at,p[i].bt,p[i].pr,p[i].ct,p[i].tat,p[i].wt);
    }
    avgPrint(p,n);
}

/* ---------- Round Robin ---------- */
void rr(P p[], int n, int tq){
    int time,done=0,g=0;
    int gt[50],gp[50];

    for(int i=0;i<n-1;i++)
        for(int j=i+1;j<n;j++)
            if(p[i].at>p[j].at){
                P t=p[i]; p[i]=p[j]; p[j]=t;
            }

    time=p[0].at;

    while(done<n){
        for(int i=0;i<n;i++){
            if(p[i].rt>0 && p[i].at<=time){
                gp[g]=p[i].pid; gt[g++]=time;

                if(p[i].rt>tq){
                    time+=tq;
                    p[i].rt-=tq;
                }else{
                    time+=p[i].rt;
                    p[i].rt=0;
                    p[i].ct=time;
                    done++;
                }
            }
        }
    }
    gt[g]=time;

    printf("\n--- Round Robin ---");
    printGantt(gp,gt,g);

    for(int i=0;i<n;i++){
        p[i].tat=p[i].ct-p[i].at;
        p[i].wt=p[i].tat-p[i].bt;
    }
    printTable(p,n);
    avgPrint(p,n);
}

/* ---------- MAIN ---------- */
int main(){
    int n,ch,tq;
    P p[10],c[10];

    printf("Enter number of processes: ");
    scanf("%d",&n);

    for(int i=0;i<n;i++){
        p[i].pid=i+1;
        printf("Enter AT & BT of P%d: ",i+1);
        scanf("%d%d",&p[i].at,&p[i].bt);
        p[i].rt=p[i].bt;
        p[i].pr=0;
    }

    printf("\n1.FCFS\n2.SJF\n3.Priority\n4.Round Robin\nChoose: ");
    scanf("%d",&ch);

    for(int i=0;i<n;i++) c[i]=p[i];

    if(ch==1) fcfs(c,n);
    else if(ch==2) sjf(c,n);
    else if(ch==3){
        for(int i=0;i<n;i++){
            printf("Enter priority of P%d: ",i+1);
            scanf("%d",&c[i].pr);
        }
        priority(c,n);
    }
    else if(ch==4){
        printf("Enter Time Quantum: ");
        scanf("%d",&tq);
        rr(c,n,tq);
    }

    return 0;
}