#include <cstdio>
#include <cstdlib>
#include <iostream>

using namespace std;

int main(int argc, char **argv) {
    if(argc != 4)
        return 1;

    int dimensions = atoi(argv[1]);
    int points = atoi(argv[2]);
    FILE *pIn =  freopen(argv[3], "r", stdin);
    FILE *pOut = freopen("changed.stc", "w", stdout);

    double data;
    cout << "set verbose info\n";
    cout << "set fitalg 1\n";
    cout << "set norma independent\n";
    cout << "set dimension " << dimensions << "\n";
    cout << "minsert " << points << "\n";

    for(int i = 0; i < points; i++) {
        for(int j = 0; j < dimensions; j++) {
            if(j == 0) {
                cin >> data;
            }

            cin >> data;
            cout << std::scientific << data << "\t";

            if(j == dimensions - 1) {
                cin >> data;
            }
        }
        cout << "\n";
    }

    cout << "close\n";

    fclose(pIn);
    fclose(pOut);
    return 0;
}

