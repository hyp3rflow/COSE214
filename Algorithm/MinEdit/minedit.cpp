#include <cstdio>
#include <algorithm>
#include <iostream>

using namespace std;

const int MAX = 1e5;
char M[MAX], N[MAX];
int dp[MAX][MAX];

int main(){
    scanf("%[^\n]%*c", M);
    scanf("%[^\n]%*c", N);

    int cntM = strlen(M);
    int cntN = strlen(N);

    for(int i=0; i<=cntM; i++) dp[i][0] = i;
    for(int i=0; i<=cntN; i++) dp[0][i] = i;

    for(int i=1; i<=cntM; i++){
        for(int j=1; j<=cntN; j++){
            int tmp1 = dp[i-1][j] + 1;
            int tmp2 = dp[i][j-1] + 1;
            
            int tmp3 = dp[i-1][j-1];
            if(M[i-1] != N[j-1]) tmp3 += 2;

            dp[i][j] = min(min(tmp1, tmp2), tmp3);
        }
    }

    printf("%d", dp[cntM][cntN]);
}