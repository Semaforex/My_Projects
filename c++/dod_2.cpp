#include <iostream>
using namespace std;

int t1[100002];
int t2[100002];
int w[100003];
string a, b;
bool az = true;
bool bz = true;

void zamiana(){
  for(int i = a.size()-1; i >=0; i--){
    t1[a.size()-i-1] = int(a[i]-'0');
  }
  for(int i = b.size()-1; i >= 0; i--){
    t2[b.size()-i-1] = int(b[i]-'0');
  }
}

void dodawanko(){
  for(int i = 0; i < 100001; i++){
    w[i+1] = (w[i]+t2[i]+t1[i])/10;
    w[i] = (w[i]+t2[i]+t1[i])%10;
  }
}

void odejmowanie1(){
    for(int i = 0; i < 100001; i++){
        if(t1[i] < t2[i]){
        t1[i+1] = t1[i+1]-1;
        t1[i] = t1[i]+10;
    }
    w[i] = t1[i]-t2[i];
    }
}

void odejmowanie2(){
    for(int i = 0; i < 100001; i++){
        if(t2[i] < t1[i]){
        t2[i+1] = t2[i+1]-1;
        t2[i] = t2[i]+10;
    }
    w[i] = t2[i]-t1[i];
    }
}

int wypisywanie(){
  for(int i = 100001; i >= 0; i--){
    if(w[i] != 0){
      for(int d = i; d >= 0; d--){
        cout << w[d];
      }
      return 0;
    }
    if(i == 0){
      cout << 0;
    }
  }
}

int sprawdzanie(){
    if(az==true&&bz==true){
        dodawanko();
        wypisywanie();
    }
    else if(az==true&&bz==false){
        if(a.size()>b.size()-1){
            odejmowanie1();
            wypisywanie();
            return 0;
        }
        else if(a.size()==b.size()-1){
        for(int i=a.size()-1;i>=0;i--){
            if(t1[i]>t2[i]){
                odejmowanie1();
                wypisywanie();
                return 0;
            }
            else if(t1[i]<t2[i]){
                odejmowanie2();
                cout<<'-';
                wypisywanie();
                return 0;
            }
        }
        cout<<0;
        }
        else if(a.size()<b.size()-1){
                odejmowanie2();
                cout<<'-';
                wypisywanie();
                return 0;
        }
    }
    else if(az==false&&bz==true){
        if(a.size()-1>b.size()){
            odejmowanie1();
            cout<<'-';
            wypisywanie();
            return 0;
        }
        else if(a.size()-1==b.size()){
            for(int i=a.size()-1;i>=0;i--){
                if(t1[i]>t2[i]){
                    odejmowanie1();
                    cout<<'-';
                    wypisywanie();
                    return 0;
                }
                else if(t1[i]<t2[i]){
                    odejmowanie2();
                    wypisywanie();
                    return 0;
                }
            }
            cout<<0;
        }
        else if(a.size()-1<b.size()){
                odejmowanie2();
                wypisywanie();
                return 0;
        }

    }
    else if(az==false&&bz==false){
        dodawanko();
        cout<<'-';
        wypisywanie();
    }
}

int main(){
    cin>>a>>b;
    if(a[0]=='-'){
        az=false;
        a[0]='0';
    }
    if(b[0]=='-'){
        bz=false;
        b[0]='0';
    }
    if(a=="0"&&b=="0"){
        cout<<0;
        return 0;
    }
    zamiana();
    sprawdzanie();
}
