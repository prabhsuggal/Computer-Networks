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

static int connectcosts1[4] = { 1,  0,  1, 999};

typedef struct distance_table
{
  int costs[4][4];
} dt1;


static int identity = 1;
static dt1 dtable;
void printdt1(struct distance_table *dtptr);
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

void rtinit1()
{
  int tmp[4][4] = {
    {1, 1, inf, inf},
    {inf, 0, inf, inf},
    {inf, 1, 1, inf},
    {inf, inf, inf, inf}
  };

  memcpy(dtable.costs, tmp, sizeof(tmp));
  printdt1(&dtable);
  transmit(dtable);

}

extern int min(int a, int b);

void rtupdate1(rcvdpkt)
  struct rtpkt *rcvdpkt;
{
  int anyChange = 0, min_dst;
  for(int i = 0;i < 4; i++){
    dtable.costs[i][rcvdpkt->sourceid] = min(inf, rcvdpkt->mincost[i] + dtable.costs[rcvdpkt->sourceid][identity]);
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
    printdt1(&dtable);
  }
}


void printdt1(dtptr)
  struct distance_table *dtptr;

{
  printf("             via   \n");
  printf("   D1 |    0     2 \n");
  printf("  ----|-----------\n");
  printf("     0|  %3d   %3d\n",dtptr->costs[0][0], dtptr->costs[0][2]);
  printf("dest 2|  %3d   %3d\n",dtptr->costs[2][0], dtptr->costs[2][2]);
  printf("     3|  %3d   %3d\n",dtptr->costs[3][0], dtptr->costs[3][2]);

}



void linkhandler1(linkid, newcost)
int linkid, newcost;
/* called when cost from 1 to linkid changes from current value to newcost*/
/* You can leave this routine empty if you're an undergrad. If you want */
/* to use this routine, you'll need to change the value of the LINKCHANGE */
/* constant definition in prog3.c from 0 to 1 */

{
  printf("Cost of link b/w %d and %d Changes from %d to %d\n",
        identity, linkid, connectcosts1[linkid], newcost);
  int min_dst = inf, anyChange = 0;
  for(int dst = 0; dst < 4; dst++){
    dtable.costs[dst][linkid] = min(inf, dtable.costs[dst][linkid] - connectcosts1[linkid] + newcost);
    for(int via = 0; via<4; via++){
      if(via == identity){
        continue;
      }
      min_dst = min(min_dst, dtable.costs[dst][via]);
    }
    if (min_dst != dtable.costs[dst][identity])
    {
      dtable.costs[dst][identity] = min_dst;
      anyChange = 1;
    }

  }
  if(anyChange){
    transmit(dtable);
    printdt1(&dtable);
  }
  connectcosts1[linkid] = newcost;
  return;
}


