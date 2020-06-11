/**
 * Author: Alemdar Salmoor
 * Date: November 5, 2019
**/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>

int NuctoDec(char c){
    if (c == 'A'){ return 0; }
    else if(c == 'C'){ return 1; }
    else if(c == 'G'){ return 2; }
    else { return 3; }
}

int max(int A, int B){
    if(A > B){
        return A;
    }
    else
    {
        return B;
    }
    
}

void computeGlobal(char ** seq, int * seqSize, int (* sMatrix)[4], int gapopen, char *** seqOutReverse, int * seqOutLen, int * score){
    
    int ** sGrid = malloc((seqSize[0]+1) * sizeof(int *));
    char ** dGrid = malloc((seqSize[0]+1) * sizeof(char *));
    int first, second, third;

    int curScore;
    char curDir;

    char letter;

    *seqOutReverse = malloc(2 * sizeof(char *));
    int seqOutCapacity = max(seqSize[0], seqSize[1]);
    int seqOutSize = 0;
    (*seqOutReverse)[0] = malloc(seqOutCapacity * sizeof(char));
    (*seqOutReverse)[1] = malloc(seqOutCapacity * sizeof(char));

    //initialize
    int i = 0, j;
    while (i < seqSize[0] + 1)
    {
        sGrid[i] = malloc((seqSize[1]+1) * sizeof(int));
        dGrid[i] = malloc((seqSize[1]+1) * sizeof(char));

        j = 0;
        while (j < seqSize[1] + 1)
        {
            if(i == 0 && j == 0){ sGrid[i][j] = 0; dGrid[i][j] = 't';}

            if(i == 0 && j != 0){
                sGrid[i][j] = sGrid[i][j-1] + gapopen;
                dGrid[i][j] = 'l';
            }

            if(j == 0 && i != 0){
                sGrid[i][j] = sGrid[i-1][j] + gapopen;
                dGrid[i][j] = 'u';
            }
            
            j++;
        }
        
        i++;
    }


    //fill in
    i = 1;
    while (i < seqSize[0] + 1)
    {

        j = 1;
        while (j < seqSize[1] + 1)
        {
            first = sGrid[i-1][j-1] + sMatrix[NuctoDec(seq[0][i-1])][NuctoDec(seq[1][j-1])];
            second = sGrid[i-1][j] + gapopen;
            third = sGrid[i][j-1] + gapopen;

            if(first >= second){
                curScore = first;
                curDir = 'd';

                if(first < third){
                    curScore = third;
                    curDir = 'l';
                }
            }
            else
            {
                curScore = second;
                curDir = 'u';

                if(second < third){
                    curScore = third;
                    curDir = 'l';
                }
            }

            sGrid[i][j] = curScore;
            dGrid[i][j] = curDir;

            j++;
        }
        
        i++;
    }


    //find
    letter = dGrid[seqSize[0]][seqSize[1]];

    i = seqSize[0];
    j = seqSize[1];

    while (letter != 't')
    {
        if(seqOutSize >= seqOutCapacity)
        {
            (*seqOutReverse)[0] = realloc((*seqOutReverse)[0], seqOutCapacity * 2);
            (*seqOutReverse)[1] = realloc((*seqOutReverse)[1], seqOutCapacity * 2);
            seqOutCapacity = seqOutCapacity * 2;
        }

        if(letter == 'd'){
            (*seqOutReverse)[0][seqOutSize] = seq[0][i-1];
            (*seqOutReverse)[1][seqOutSize] = seq[1][j-1];

            i--;
            j--;
        }
        else if(letter == 'l')
        {
            (*seqOutReverse)[0][seqOutSize] = '-';
            (*seqOutReverse)[1][seqOutSize] = seq[1][j-1];
            j--;
        }
        else
        {
            (*seqOutReverse)[0][seqOutSize] = seq[0][i-1];
            (*seqOutReverse)[1][seqOutSize] = '-';
            i--;
        }

        seqOutSize++;

        letter = dGrid[i][j];
        
    }    

    *score = sGrid[seqSize[0]][seqSize[1]];
    *seqOutLen = seqOutSize;

    i = 0;
    while (i < seqSize[0]+1)
    {
        free(dGrid[i]);
        free(sGrid[i]);
        i++;
    }
    
    free(sGrid);
    free(dGrid);

}

void computeLocal(char ** seq, int * seqSize, int (* sMatrix)[4], int gapopen, char *** seqOutReverse, int * seqOutLen, int * score){

    int ** sGrid = malloc((seqSize[0]+1) * sizeof(int *));
    char ** dGrid = malloc((seqSize[0]+1) * sizeof(char *));
    int first, second, third, zero = 0;

    int curScore;
    char curDir;

    char letter;
    int max = 0;
    int maxi = 0;
    int maxj = 0;

    *seqOutReverse = malloc(2 * sizeof(char *));
    int seqOutCapacity = 1;
    int seqOutSize = 0;
    (*seqOutReverse)[0] = malloc(seqOutCapacity * sizeof(char));
    (*seqOutReverse)[1] = malloc(seqOutCapacity * sizeof(char));

    int i = 0, j;
    while (i < seqSize[0] + 1)
    {
        sGrid[i] = malloc((seqSize[1]+1) * sizeof(int));
        dGrid[i] = malloc((seqSize[1]+1) * sizeof(char));

        j = 0;
        while (j < seqSize[1] + 1)
        {
            if(i == 0 && j == 0){ sGrid[i][j] = 0; dGrid[i][j] = 't';}

            if(i == 0 && j != 0){
                sGrid[i][j] = zero;
                dGrid[i][j] = 't';
            }

            if(j == 0 && i != 0){
                sGrid[i][j] = zero;
                dGrid[i][j] = 't';
            }
            
            j++;
        }
        
        i++;
    }


    //fill in
    i = 1;
    while (i < seqSize[0] + 1)
    {

        j = 1;
        while (j < seqSize[1] + 1)
        {
            first = sGrid[i-1][j-1] + sMatrix[NuctoDec(seq[0][i-1])][NuctoDec(seq[1][j-1])];
            second = sGrid[i-1][j] + gapopen;
            third = sGrid[i][j-1] + gapopen;

            if(zero >= first && zero >= second && zero >= third){ curScore = zero; curDir = 't';}
            else if(first >= zero && first >= second && first >= third){ curScore = first; curDir = 'd';}
            else if(second >= first && second >= zero && second >= third){ curScore = second; curDir = 'u';}
            else{ curScore = third; curDir = 'l';}
            

            sGrid[i][j] = curScore;
            dGrid[i][j] = curDir;

            if(curScore >= max){ max = curScore; maxi = i; maxj = j;}

            j++;
        }
        
        i++;
    }


    //find
    letter = dGrid[maxi][maxj];

    i = maxi;
    j = maxj;

    while (letter != 't')
    {
        if(seqOutSize >= seqOutCapacity)
        {
            (*seqOutReverse)[0] = realloc((*seqOutReverse)[0], seqOutCapacity * 2);
            (*seqOutReverse)[1] = realloc((*seqOutReverse)[1], seqOutCapacity * 2);
            seqOutCapacity = seqOutCapacity * 2;
        }

        if(letter == 'd'){
            (*seqOutReverse)[0][seqOutSize] = seq[0][i-1];
            (*seqOutReverse)[1][seqOutSize] = seq[1][j-1];

            i--;
            j--;
        }
        else if(letter == 'l')
        {
            (*seqOutReverse)[0][seqOutSize] = '-';
            (*seqOutReverse)[1][seqOutSize] = seq[1][j-1];
            j--;
        }
        else
        {
            (*seqOutReverse)[0][seqOutSize] = seq[0][i-1];
            (*seqOutReverse)[1][seqOutSize] = '-';
            i--;
        }

        seqOutSize++;

        letter = dGrid[i][j];
        
    }    

    *score = sGrid[maxi][maxj];
    *seqOutLen = seqOutSize;

    i = 0;
    while (i < seqSize[0]+1)
    {
        free(dGrid[i]);
        free(sGrid[i]);
        i++;
    }
    
    free(sGrid);
    free(dGrid);


}

void computeAGlobal(char ** seq, int * seqSize, int (* sMatrix)[4], int gapopen, int gapext, char *** seqOutReverse, int * seqOutLen, int * score){

    int ** V = malloc((seqSize[0]+1) * sizeof(int *));
    char ** P = malloc((seqSize[0]+1) * sizeof(char *));
    int ** G = malloc((seqSize[0]+1) * sizeof(int *));
    int first, second, third;


    char letter;

    *seqOutReverse = malloc(2 * sizeof(char *));
    int seqOutCapacity = max(seqSize[0], seqSize[1]);
    int seqOutSize = 0;
    (*seqOutReverse)[0] = malloc(seqOutCapacity * sizeof(char));
    (*seqOutReverse)[1] = malloc(seqOutCapacity * sizeof(char));

    //sup grid
    int i = 0, j;
    int ** F = malloc((seqSize[0] + 1) * sizeof(int *));
    int ** E = malloc((seqSize[0] + 1) * sizeof(int *));


    //initialize
    i = 0;
    while (i < seqSize[0] + 1)
    {
        V[i] = malloc((seqSize[1]+1) * sizeof(int));
        P[i] = malloc((seqSize[1]+1) * sizeof(char));
        E[i] = malloc((seqSize[1]+1) * sizeof(int));
        F[i] = malloc((seqSize[1]+1) * sizeof(int));
        G[i] = malloc((seqSize[1]+1) * sizeof(int));
        

        j = 0;
        while (j < seqSize[1] + 1)
        {
            if(i == 0 && j == 0){
                
                E[i][j] = INT_MIN/2;
                F[i][j] = INT_MIN/2;
                G[i][j] = INT_MIN/2;
                V[i][j] = 0;
                P[i][j] = 't';
            }

            if(i == 0 && j != 0){
                
                E[i][j] = 0;
                F[i][j] = gapopen + j * gapext;
                G[i][j] = 0;
                V[i][j] = gapopen + j * gapext;
                P[i][j] = 'l';

            }

            if(j == 0 && i != 0){

                E[i][j] = gapopen + i * gapext;
                F[i][j] = 0;
                G[i][j] = 0;
                V[i][j] = gapopen + i * gapext;
                P[i][j] = 'u';

            }
            
            j++;
        }
        
        i++;
    }

    //fill in
    i = 1;
    while (i < seqSize[0] + 1)
    {

        j = 1;
        while (j < seqSize[1] + 1)
        {

            E[i][j] = max(E[i][j-1] + gapext, (V[i][j-1] + gapopen + gapext));
            F[i][j] = max(F[i-1][j] + gapext, (V[i-1][j] + gapopen + gapext));
            G[i][j] = V[i-1][j-1] + sMatrix[NuctoDec(seq[0][i-1])][NuctoDec(seq[1][j-1])];

        
            if(F[i][j] >= G[i][j] && F[i][j] >= E[i][j]){ V[i][j] = F[i][j]; P[i][j] = 'u';}
            else if(E[i][j] >= G[i][j] && E[i][j] >= F[i][j]){ V[i][j] = E[i][j]; P[i][j] = 'l'; }
            else{ V[i][j] = G[i][j]; P[i][j] = 'd'; }

            j++;
        }
        
        i++;
    }

    //find
    letter = P[seqSize[0]][seqSize[1]];

    i = seqSize[0];
    j = seqSize[1];

    while (letter != 't')
    {
        if(seqOutSize >= seqOutCapacity)
        {
            (*seqOutReverse)[0] = realloc((*seqOutReverse)[0], seqOutCapacity * 2);
            (*seqOutReverse)[1] = realloc((*seqOutReverse)[1], seqOutCapacity * 2);
            seqOutCapacity = seqOutCapacity * 2;
        }

        if(letter == 'd'){
            (*seqOutReverse)[0][seqOutSize] = seq[0][i-1];
            (*seqOutReverse)[1][seqOutSize] = seq[1][j-1];

            i--;
            j--;
        }
        else if(letter == 'l')
        {
            (*seqOutReverse)[0][seqOutSize] = '-';
            (*seqOutReverse)[1][seqOutSize] = seq[1][j-1];
            j--;
        }
        else
        {
            (*seqOutReverse)[0][seqOutSize] = seq[0][i-1];
            (*seqOutReverse)[1][seqOutSize] = '-';
            i--;
        }

        seqOutSize++;

        letter = P[i][j];
        
    }
    

    *score = V[seqSize[0]][seqSize[1]];
    *seqOutLen = seqOutSize;

    i = 0;
    while (i < seqSize[0]+1)
    {
        free(P[i]);
        free(V[i]);
        free(E[i]);
        free(F[i]);
        free(G[i]);
        i++;
    }
    
    free(V);
    free(P);
    free(E);
    free(F);
    free(G);

}

void computeALocal(char ** seq, int * seqSize, int (* sMatrix)[4], int gapopen, int gapext, char *** seqOutReverse, int * seqOutLen, int * score){

    int ** V = malloc((seqSize[0]+1) * sizeof(int *));
    char ** P = malloc((seqSize[0]+1) * sizeof(char *));
    int ** G = malloc((seqSize[0]+1) * sizeof(int *));
    int first, second, third, zero = 0;


    char letter;
    int maxSoFar = 0;
    int maxi = 0;
    int maxj = 0;

    *seqOutReverse = malloc(2 * sizeof(char *));
    int seqOutCapacity = max(seqSize[0], seqSize[1]);
    int seqOutSize = 0;
    (*seqOutReverse)[0] = malloc(seqOutCapacity * sizeof(char));
    (*seqOutReverse)[1] = malloc(seqOutCapacity * sizeof(char));

    //sup grid
    int i = 0, j;
    int ** F = malloc((seqSize[0] + 1) * sizeof(int *));
    int ** E = malloc((seqSize[0] + 1) * sizeof(int *));


    //initialize
    i = 0;
    while (i < seqSize[0] + 1)
    {
        V[i] = malloc((seqSize[1]+1) * sizeof(int));
        P[i] = malloc((seqSize[1]+1) * sizeof(char));
        E[i] = malloc((seqSize[1]+1) * sizeof(int));
        F[i] = malloc((seqSize[1]+1) * sizeof(int));
        G[i] = malloc((seqSize[1]+1) * sizeof(int));
        

        j = 0;
        while (j < seqSize[1] + 1)
        {
            if(i == 0 && j == 0){
                
                E[i][j] = 0;
                F[i][j] = 0;
                G[i][j] = 0;
                V[i][j] = 0;
                P[i][j] = 't';
            }

            if(i == 0 && j != 0){
                
                E[i][j] = 0;
                F[i][j] = 0;
                G[i][j] = 0;
                V[i][j] = 0;
                P[i][j] = 't';

            }

            if(j == 0 && i != 0){

                E[i][j] = 0;
                F[i][j] = 0;
                G[i][j] = 0;
                V[i][j] = 0;
                P[i][j] = 't';

            }
            
            j++;
        }
        
        i++;
    }

    //fill in
    i = 1;
    while (i < seqSize[0] + 1)
    {

        j = 1;
        while (j < seqSize[1] + 1)
        {

            E[i][j] = max(E[i][j-1] + gapext, (V[i][j-1] + gapopen + gapext));
            F[i][j] = max(F[i-1][j] + gapext, (V[i-1][j] + gapopen + gapext));
            G[i][j] = V[i-1][j-1] + sMatrix[NuctoDec(seq[0][i-1])][NuctoDec(seq[1][j-1])];

        
            if(zero >= G[i][j] && zero >= F[i][j] && zero >= E[i][j]){ V[i][j] = zero; P[i][j] = 't';}
            else if(F[i][j] >= G[i][j] && F[i][j] >= E[i][j]){ V[i][j] = F[i][j]; P[i][j] = 'u';}
            else if(E[i][j] >= G[i][j] && E[i][j] >= F[i][j]){ V[i][j] = E[i][j]; P[i][j] = 'l'; }
            else{ V[i][j] = G[i][j]; P[i][j] = 'd'; }

            if(V[i][j] >= maxSoFar){ maxSoFar = V[i][j]; maxi = i; maxj = j;}

            j++;
        }
        
        i++;
    }

    //find
    letter = P[maxi][maxj];

    i = maxi;
    j = maxj;

    while (letter != 't')
    {
        if(seqOutSize >= seqOutCapacity)
        {
            (*seqOutReverse)[0] = realloc((*seqOutReverse)[0], seqOutCapacity * 2);
            (*seqOutReverse)[1] = realloc((*seqOutReverse)[1], seqOutCapacity * 2);
            seqOutCapacity = seqOutCapacity * 2;
        }

        if(letter == 'd'){
            (*seqOutReverse)[0][seqOutSize] = seq[0][i-1];
            (*seqOutReverse)[1][seqOutSize] = seq[1][j-1];

            i--;
            j--;
        }
        else if(letter == 'l')
        {
            (*seqOutReverse)[0][seqOutSize] = '-';
            (*seqOutReverse)[1][seqOutSize] = seq[1][j-1];
            j--;
        }
        else
        {
            (*seqOutReverse)[0][seqOutSize] = seq[0][i-1];
            (*seqOutReverse)[1][seqOutSize] = '-';
            i--;
        }

        seqOutSize++;

        letter = P[i][j];
        
    }
    

    *score = V[maxi][maxj];
    *seqOutLen = seqOutSize;

    i = 0;
    while (i < seqSize[0]+1)
    {
        free(P[i]);
        free(V[i]);
        free(E[i]);
        free(F[i]);
        free(G[i]);
        i++;
    }
    
    free(V);
    free(P);
    free(E);
    free(F);
    free(G);

}

int main(int argc, char** argv){

    char * mode = argv[2];
    char * input = argv[4];
    int gapopen = atoi(argv[6]);
    int gapext;

    // printf("mode: %s, input: %s, gapopen: %d\n", mode, input, gapopen); 

    if(strcmp(mode, "aglobal") == 0 || strcmp(mode, "alocal") == 0){ gapext = atoi(argv[8]); }

    char letter = '0';

    int seqCount = -1;

    int seqSize[2] = {0, 0};
    int seqCapacity[2] = {1,1};
    char * seq[2];
    seq[0] = malloc(sizeof(char));
    seq[1] = malloc(sizeof(char));


    int seqNameSize[2] = {0, 0};
    int seqNameCapacity[2] = {1, 1};
    char * seqName[2];
    seqName[0] = malloc(sizeof(char));
    seqName[1] = malloc(sizeof(char));

    int score;
    FILE * finput = fopen(input, "r");
    
    while (fscanf(finput, "%c", &letter) > 0)
    {
        if (letter == '>'){
            seqCount++;
            while (fscanf(finput, "%c", &letter) > 0){
                if (letter == '\n'){
                    break;
                }
                if(seqNameSize[seqCount] >= seqNameCapacity[seqCount])
                {
                    seqName[seqCount] = realloc(seqName[seqCount], seqNameCapacity[seqCount] * 2);
                    seqNameCapacity[seqCount] = seqNameCapacity[seqCount] * 2;
                }
                seqName[seqCount][seqNameSize[seqCount]] = letter;
                seqNameSize[seqCount]++;
            }
        }

        if(letter != '\n'){
            if (seqSize[seqCount] >= seqCapacity[seqCount])
            {
                seq[seqCount] = realloc(seq[seqCount], seqCapacity[seqCount] * 2);
                seqCapacity[seqCount] = seqCapacity[seqCount] * 2;
            }
            seq[seqCount][seqSize[seqCount]] = letter;
            seqSize[seqCount]++;
        }

    }


    int sMatrix[4][4];

    int i = 0, j;

    while (i < 4)
    {
        j = 0;
        while (j < 4)
        {
            if(i == j){ sMatrix[i][j] = 2; }
            else{ sMatrix[i][j] = -3; }
            
            j++;
            
        }
        i++;
    }



    char ** seqOutReverse;
    int seqOutLen;

    if(strcmp(mode, "global") == 0){ computeGlobal(seq, seqSize, sMatrix, gapopen, &seqOutReverse, &seqOutLen, &score); }
    else if(strcmp(mode, "local") == 0){ computeLocal(seq, seqSize, sMatrix, gapopen, &seqOutReverse, &seqOutLen, &score); }
    else if(strcmp(mode, "aglobal") == 0){ computeAGlobal(seq, seqSize, sMatrix, gapopen, gapext, &seqOutReverse, &seqOutLen, &score); }
    else { computeALocal(seq, seqSize, sMatrix, gapopen, gapext, &seqOutReverse, &seqOutLen, &score); }
    

    //Equalize nameLength
    if(seqNameSize[0] < seqNameSize[1]){

        while (seqNameSize[0] < seqNameSize[1])
        {
            if(seqNameSize[0] >= seqNameCapacity[0])
            {
                seqName[0] = realloc(seqName[0], seqNameCapacity[0] * 2);
                seqNameCapacity[0] = seqNameCapacity[0] * 2;
            }
            seqName[0][seqNameSize[0]] = ' ';
            seqNameSize[0]++;
        }
    }
    else{

        while (seqNameSize[1] < seqNameSize[0])
        {
            if(seqNameSize[1] >= seqNameCapacity[1])
            {
                seqName[1] = realloc(seqName[1], seqNameCapacity[1] * 2);
                seqNameCapacity[1] = seqNameCapacity[1] * 2;
            }
            seqName[1][seqNameSize[1]] = ' ';
            seqNameSize[1]++;
        }
    }

    //Create output file for sequences

    char * outFileName;

    if(strcmp(mode, "global") == 0){ outFileName = "global-naiveGap.aln"; }
    else if(strcmp(mode, "local") == 0){ outFileName = "local-naiveGap.aln"; }
    else if(strcmp(mode, "aglobal") == 0){ outFileName = "global-affineGap.aln"; }
    else{ outFileName = "local-affineGap.aln"; }


    FILE *ouf = fopen(outFileName, "w+");
    fprintf(ouf, "Score = %d\n", score);
    fprintf(ouf, "\n");

    int cnt = 0;
    int index0 = seqOutLen - 1;
    int index1 = seqOutLen - 1;

    while (true)
    {
        cnt = 0;
        fprintf(ouf, "%s\t", seqName[0]);
        while (cnt < 60 && index0 >= 0)
        {
            fprintf(ouf, "%c", seqOutReverse[0][index0]);
            index0--;
            cnt++;
        }
        fprintf(ouf, "\n");


        cnt = 0;
        fprintf(ouf, "%s\t", seqName[1]);
        while (cnt < 60 && index1 >= 0)
        {
            fprintf(ouf, "%c", seqOutReverse[1][index1]);
            index1--;
            cnt++;
        }
        fprintf(ouf, "\n");

        fprintf(ouf, "\n");

        if(index0 < 0){
            break;
        }
        
    }

    fclose(ouf);

    free(seq[0]);
    free(seq[1]);
    free(seqName[0]);
    free(seqName[1]);

    free(seqOutReverse[0]);
    free(seqOutReverse[1]);
    
    return 0;

}