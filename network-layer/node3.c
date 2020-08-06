#include <stdio.h>
#include <string.h>

extern struct rtpkt {
  int sourceid;       /* id of sending router sending this pkt */
  int destid;         /* id of router to which pkt being sent
                         (must be an immediate neighbor) */
  int mincost[4];    /* min cost to node 0 ... 3 */
  };

extern int TRACE;
extern int YES;
extern int NO;

static int inf = 999;

typedef struct distance_table
{
  int costs[4][4];
} dt3;

static int identity = 3;
static dt3 dtable;
void printdt3(struct distance_table *dtptr);
/* students to write the following two routines, and maybe some others */
static void transmit(struct distance_table dt){
  struct rtpkt vector;
  for(int i = 0; i < 4; i++){
    vector.mincost[i] = dt.costs[i][identity];
  }
  vector.sourceid = identity;
  for(int i = 0; i < 4; i++){
    if( i == identity){
      continue;
    }
    vector.destid = i;
    tolayer2(vector);
  }
}

void rtinit3()
{
  int tmp[4][4] = {
    {7,   inf,  inf,  7},
    {inf, inf,  inf,  inf},
    {inf, inf,  2,    2},
    {inf, inf,  inf,  0}
  };

  memcpy(dtable.costs, tmp, sizeof(tmp));
  printdt3(&dtable);
  transmit(dtable);
}

extern int min(int a, int b);

void rtupdate3(rcvdpkt)
  struct rtpkt *rcvdpkt;

{
  int anyChange = 0, min_dst;
  for(int i = 0;i < 4; i++){
    dtable.costs[i][rcvdpkt->sourceid] = rcvdpkt->mincost[i] + dtable.costs[rcvdpkt->sourceid][identity];
  }
  for(int dst = 0;dst  < 4; dst++){
    min_dst = inf;
    for(int via = 0; via < 4; via++){
      if(via == identity){
        continue;
      }
      min_dst = min(min_dst, dtable.costs[dst][via] + dtable.costs[via][identity]);
    }
    if(min_dst != dtable.costs[dst][identity]){
      dtable.costs[dst][identity] = min_dst;
      anyChange = 1;
    }
  }
  if(anyChange){
    transmit(dtable);
    printdt3(&dtable);
  }

}


void printdt3(dtptr)
  struct distance_table *dtptr;

{
  printf("             via     \n");
  printf("   D3 |    0     2 \n");
  printf("  ----|-----------\n");
  printf("     0|  %3d   %3d\n",dtptr->costs[0][0], dtptr->costs[0][2]);
  printf("dest 1|  %3d   %3d\n",dtptr->costs[1][0], dtptr->costs[1][2]);
  printf("     2|  %3d   %3d\n",dtptr->costs[2][0], dtptr->costs[2][2]);

}







