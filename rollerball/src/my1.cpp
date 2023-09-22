#include <algorithm>
#include<queue>
#include <random>
#include <iostream>
#include <set>
using namespace std;;

#include "board.hpp"
// #include "board.cpp"
#include "engine.hpp"


int MaxVal(Board*b,int alpha,int beta,int depth);
int MinVal(Board*b,int alpha,int beta,int depth);

#define kingval 1000

#define rookval 100
#define bishopval 70
#define pawnval 50
#define checkval 20
#define depthlevel 3
#define promote 3
#define dist 10

PlayerColor playertoplay;

int manhatten(U8 pos,int a,int b){
    int x=getx(pos),y=gety(pos);
    return abs(x-a)+abs(y-b);
}



int checkpawn(Board* b, U8 pos){
    U8 pawn=b->data.board_0[pos];
    if(pawn & PAWN) return pawnval;
    if(pawn & PAWN_BISHOP) return bishopval;
    if(pawn & PAWN_ROOK) return rookval;
    return 0;
}


int evaluate(Board* b){
    int val1=0;
    int val2=0;
    int val=0;
    if(b->data.w_king!=DEAD)val1+=kingval;
    if(b->data.w_bishop!=DEAD)val1+=bishopval;
    if(b->data.w_rook_bs!=DEAD)val1+=rookval;
    if(b->data.w_rook_ws!=DEAD)val1+=rookval;
    if(b->data.w_pawn_bs!=DEAD){
        U8 pos=b->data.w_pawn_bs;
        U8 pawn=b->data.board_0[pos];
        val1+=checkpawn(b,pos);
        int val=0;
        if(pawn&PAWN){
            int x=getx(pos),y=gety(pos);
            val+=min(manhatten(pos,4,5),manhatten(pos,4,6));
            if(x==2 && y==0)val+=2;
        }
        val1+=abs(dist-val)*promote; 
    }
    if(b->data.w_pawn_ws!=DEAD){
        U8 pos=b->data.w_pawn_ws;
        U8 pawn=b->data.board_0[pos];
        val1+=checkpawn(b,pos);
        int val=0;
        if(pawn&PAWN){
            int x=getx(pos),y=gety(pos);
            val+=min(manhatten(pos,4,5),manhatten(pos,4,6));
            if(x==2 && y==1)val+=2;
        }
        val1+=abs(dist-val)*promote; 
    }

    if(b->data.b_king!=DEAD)val2+=kingval;
    if(b->data.b_bishop!=DEAD)val2+=bishopval;
    if(b->data.b_rook_bs!=DEAD)val2+=rookval;
    if(b->data.b_rook_ws!=DEAD)val2+=rookval;
    if(b->data.b_pawn_bs!=DEAD){
        U8 pos=b->data.b_pawn_bs;
        U8 pawn=b->data.board_0[pos];
        val2+=checkpawn(b,pos);
        int val=0;
        if(pawn&PAWN){
            int x=getx(pos),y=gety(pos);
            val+=min(manhatten(pos,2,1),manhatten(pos,2,0));
            if(x==4 && y==6)val+=2;
        }
        val2+=abs(dist-val)*promote; 
    }
    if(b->data.b_pawn_ws!=DEAD){
        U8 pos=b->data.b_pawn_ws;
        U8 pawn=b->data.board_0[pos];
        val2+=checkpawn(b,pos);
        int val=0;
        if(pawn&PAWN){
            int x=getx(pos),y=gety(pos);
            val+=min(manhatten(pos,2,1),manhatten(pos,2,0));
            if(x==4 && y==5)val+=2;
        }
        val2+=abs(dist-val)*promote; 
    }

    if(b->data.player_to_play==playertoplay){
        if(b->in_check())val-=checkval;
    }else{
        if(b->in_check())val+=checkval;
    }
    if(playertoplay==WHITE)val+=(val1-val2);
    else val+=(val2-val1);
    return val;
}


int MinVal(Board* b,int alpha,int beta,int depth){

    if(depth==depthlevel){
        return evaluate(b);
    }
    auto moveset=b->get_legal_moves();
    int val=INT_MAX;
    for(auto move: moveset){
        auto copyb=b->copy();
        copyb->do_move(move);
        val=min(val,MaxVal(copyb,alpha,beta,depth+1));
        beta=min(beta,val);
        delete copyb;
        if(alpha>=beta)return val;
    }
    return val;
}

int MaxVal(Board* b,int alpha,int beta,int depth){
    if(depth==depthlevel){
        return evaluate(b);
    }
    auto moveset=b->get_legal_moves();
    int val=INT_MIN;
    for(auto move : moveset){
        auto copyb=b->copy();
        copyb->do_move(move);
        val=max(val,MinVal(copyb,alpha,beta,depth+1));
        alpha=max(alpha,val);
        delete copyb;
        if(alpha>=beta) return val;
    }
    return val;
}

U16 BestMove(Board* b){
    auto moveset =b->get_legal_moves();
    U16 best_move=*moveset.begin();
    int mx=INT_MIN;
    playertoplay=b->data.player_to_play;
    for(auto move : moveset){
        Board* c = b->copy();
        c->do_move(move);
        int val=MinVal(c,INT_MIN,INT_MAX,1);
        if(val>mx){
            best_move=move;
            mx=val;
        }
        delete c;
    }
    b->do_move(best_move);
    return best_move;
}

void Engine::find_best_move(const Board& b) {
    auto moveset = b.get_legal_moves();
    if (moveset.size() == 0) {
        this->best_move = 0;
        return;
    }
    else {
        Board* c=b.copy();
        U16 best_move=BestMove(c);
        delete c;
        this->best_move=best_move;
    }
}