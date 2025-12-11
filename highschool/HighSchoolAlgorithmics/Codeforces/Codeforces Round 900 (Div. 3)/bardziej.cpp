#include <iostream>
#include <locale> // to work with locales
#include <string>
using namespace std;

// Defining a structure that inherits from ctype<char>
struct changeDelimiter : ctype<char> {

    // Constructor for changeDelimiter
    changeDelimiter()
        : ctype<char>(
            createTable()) // Initializing the base class
                           // ctype<char> with a table
    {
    }
    // Static function to create a table with custom
    // settings
    static mask const* createTable()
    {
        static mask
            rc[table_size]; // Creating a table with the
                            // size of the character set
        rc['\n']
            = ctype_base::space; // Set the newline
                                 // character to be treated
                                 // as whitespace
        return rc; // Return the modified table
    }
};

int main()
{
    // Creating a custom locale with the changeDelimiter
    // facet
    cin.imbue(locale(cin.getloc(), new changeDelimiter));
    cout << "Hej, czy jestes moze moja dziewczyna?(T/N)" << '\n';
    string odp;
    cin >> odp;
    while(odp != "T" && odp != "N"){
        cout << "T lub N";
        cin >> odp;
    }
    if(odp == "N"){
        cout << "Aha, to nie gadam z toba." << '\n';
        cin >> odp;
        return 0;
    }
    cout << "A jak bardzo mnie kochasz??" << '\n';
    cin >> odp;
    if(odp == "wcale" || odp == "wgl" || odp == "ani trochę" || odp == "wogóle" || odp == "w ogóle" || odp == "nic a nic"){
        cout << "Aha to super. Ja ciebie bardzo." << '\n';
        cin >> odp;
        return 0;
    }
    string my_ans = "";
    bool is_number = false;
    for(int i = 0; i < odp.size(); i++){
        string number = "";
        if(int('0') <= int(odp[i]) && int('9') >= int(odp[i])){
            is_number = true;
            while(int('0') <= int(odp[i]) && int('9') >= int(odp[i])){
                number += odp[i];
                i++;
            }
        }
        string number2 = "";
        if(number.size() > 0){
            int num = stoi(number)+1;
            number2 = to_string(num);
        }
        my_ans += number2+odp[i];
    }
    if(!is_number)
        cout << "A ja ciebie " << odp << " + 1." << '\n';
    else
        cout << "A ja ciebie " << my_ans << '.' << '\n';
    cin >> my_ans;
};
