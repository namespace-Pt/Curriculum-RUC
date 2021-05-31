#include<iostream>
#include<algorithm>
#include<set>
#include<vector>
#include<deque>
#include<string>
#include<map>
#include<set>
#include"parser_LR/symbol.cpp"
using namespace std;
int main(){
    // set<char> a;
    // a.insert('a');
    // a.insert('a');
    // a.insert('b');
    // set<char> b;
    // b.insert('a');
    // b.insert('c');

    // set<char> c;

    // set_intersection(a.begin(),a.end(),b.begin(),b.end(),inserter(c,c.begin()));

    // for(auto i = c.begin();i!=c.end();i++){
    //     cout<<*i;
    // }
    // int a = -2;
    // cout<<a<<endl<<!a;
    // string a = "123";
    // cout<<a.find("23");
    // // cout<<a.find("45");
    // if(a.find("45") == string::npos){
    //     cout<<"fuck";
    // }
    // deque<int> a;
    // a.push_front(1);
    // a.push_front(2);
    // a.push_front(3);

    // while(!a.empty()){
    //     cout<<a.back();
    //     a.pop_back();
    // }

    // map<int,vector<int>> a;
    // vector<int> &i = a[1];
    // // a[1].push_back(1);
    // // a[1].push_back(1);
    // // a[1].push_back(1);
    // i.push_back(1);
    // i.push_back(1);
    // for(auto it = a.cbegin();it!=a.cend();it++){
    //     for(int j = 0;j < it->second.size();j++){
    //         cout<<it->second[j];
    //     }
    // }

    // set<Symbol*> a;
    // a.insert(new Symbol());

    // auto it = a.find(nullptr);
    // if(it == a.end()){
    //     cout<<"yes";
    // }
    // else{
    //     cout<<"fuck";
    // }
    // if(!(*it)){
    //     cout<<"yes";
    // }
    // else{
    //     cout<<"fuck"<<*it;
    // }

    map<int,Symbol*> a;
    cout<<a[0];
    if(!a[0]){
        cout<<"nice";
    }
    return 0;
}