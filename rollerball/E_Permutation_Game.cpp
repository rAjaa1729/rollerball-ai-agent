#include<bits/stdc++.h>
using namespace std;
void solve(){
    int n;cin>>n;
    vector<int> v(n+1);
    for(int i=1;i<=n;i++)cin>>v[i];
    vector<bool> check(n+1,false);
    int x=0,c=0,y=0;
    for(int i=1;i<=n;i++){
        if(v[i]==i)continue;
        check[i]=true;
        x++;
    }
    for(int i=1;i<=n;i++){
        if(v[i]==n-i+1)continue;
        if(check[i]){
            x--;c++;
        }else y++;
    }
    if(y+c<x)cout<<"Second";
    else if(y>=x+c)cout<<"First";
    else cout<<"Tie";
    cout<<endl;
}
int main(){
    int t;cin>>t;
    while(t--)solve();
}