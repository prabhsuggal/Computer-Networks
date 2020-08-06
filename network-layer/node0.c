#include <stdio.h>
#include <string.h>

static int inf = 999;
extern struct rtpkt {
  int sourceid;       /* id of sending router sending this pkt */
  int destid;         /* id of router to which pkt being sent
                         (must be an immediate neighbor) */
  int mincost[4];    /* min cost to node 0 ... 3 */
  };

extern int TRACE;
extern int YES;
extern int NO;

static int connectcosts0[4] = { 0,  1,  3, 7 };

typedef struct distance_table
{
  int costs[4][4];
} dt0;

static int identity = 0;
static dt0 dtable;
void printdt0(struct distance_table *dtptr);
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


void rtinit0()
{
  int tmp[4][4] = {
    {0 , inf, inf, inf},
    {1, 1, inf, inf},
    {3, inf, 3, inf},
    {7, inf, inf, 7}
  };

  memcpy(dtable.costs, tmp, sizeof(tmp));
  printdt0(&dtable);
  transmit(dtable);

}

extern int min(int a, int b);

void rtupdate0(rcvdpkt)
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
    printdt0(&dtable);
  }
}


void printdt0(dtptr)
  struct distance_table *dtptr;

{
  printf("                via     \n");
  printf("   D0 |    1     2    3 \n");
  printf("  ----|-----------------\n");
  printf("     1|  %3d   %3d   %3d\n",dtptr->costs[1][1],
	 dtptr->costs[1][2],dtptr->costs[1][3]);
  printf("dest 2|  %3d   %3d   %3d\n",dtptr->costs[2][1],
	 dtptr->costs[2][2],dtptr->costs[2][3]);
  printf("     3|  %3d   %3d   %3d\n",dtptr->costs[3][1],
	 dtptr->costs[3][2],dtptr->costs[3][3]);
   return;
}

void linkhandler0(linkid, newcost)
  int linkid, newcost;

/* called when cost from 0 to linkid changes from current value to newcost*/
/* You can leave this routine empty if you're an undergrad. If you want */
/* to use this routine, you'll need to change the value of the LINKCHANGE */
/* constant definition in prog3.c from 0 to 1 */

{
  printf("Cost of link b/w %d and %d Changes from %d to %d\n",
        identity, linkid, connectcosts0[linkid], newcost);
  int min_dst, anyChange = 0;
  for(int dst = 0; dst < 4; dst++){
    min_dst = inf;
    dtable.costs[dst][linkid] = min(inf, dtable.costs[dst][linkid] - connectcosts0[linkid] + newcost);
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
    printdt0(&dtable);
  }
  connectcosts0[linkid] = newcost;
  return;
}

