#include <bits/stdc++.h>

#define vd vector<double>
#define vvd vector<vd>
#define pii pair<int, int>
#define pss pair<string, string>

using namespace std;

int scoringMatrixSize;
vvd scoringMatrix;
string firstString, secondString, scoringCharacters;
string movesForFirstString, movesForSecondString;
int firstStringStartIndex = 0, secondStringStartIndex = 0;
bool swapped = false;

void printvvd(vvd v)
{
    for (int i = 0; i < v.size(); i++)
    {
        for (int j = 0; j < v[i].size(); j++)
        {
            cout << v[i][j] << '\t';
        }
        cout << endl;
    }
    cout << endl;
}

int findIndexOfScoringCharacter(char c)
{
    for (int i = 0; i < scoringCharacters.length(); i++)
    {
        if (scoringCharacters[i] == c)
        {
            return i;
        }
    }
    return -1;
}

void init()
{
    ifstream fileInputStream("input.txt");

    fileInputStream >> firstString >> secondString >> scoringCharacters;

    if (firstString.length() < secondString.length())
    {
        string temp = firstString;
        firstString = secondString;
        secondString = temp;
        swapped = true;
    }

    scoringMatrixSize = scoringCharacters.length() + 1;

    scoringMatrix = vvd(scoringMatrixSize);

    for (int i = 0; i < scoringMatrixSize; i++)
    {
        for (int j = 0; j < scoringMatrixSize; j++)
        {
            double d;
            fileInputStream >> d;
            scoringMatrix[i].push_back(d);
        }
    }
    fileInputStream.close();
}

int argMax(vd scoreL, vd scoreR)
{
    reverse(scoreR.begin(), scoreR.end());
    int ymid = -1;
    double score = -MAXFLOAT;

    for (int i = 0; i < scoreR.size(); i++)
    {
        if (scoreL[i] + scoreR[i] > score)
        {
            score = scoreL[i] + scoreR[i];
            ymid = i;
        }
    }
    return ymid;
}

vd nwScore(string X, string Y, bool isLocalAlignment)
{
    int ylen = Y.length();
    int xlen = X.length();

    vvd score = vvd(2);
    score[0] = vd(ylen + 1);
    score[1] = vd(ylen + 1);

    score[0][0] = 0;

    for (int j = 1; j < ylen + 1; j++)
    {
        score[0][j] = score[0][j - 1] + scoringMatrix[scoringMatrixSize - 1][findIndexOfScoringCharacter(Y[j - 1])];
    }

    for (int i = 1; i < xlen + 1; i++)
    {
        score[1][0] = score[0][0] + scoringMatrix[findIndexOfScoringCharacter(X[i - 1])][scoringMatrixSize - 1];

        for (int j = 1; j < ylen + 1; j++)
        {
            double scoreSub = score[0][j - 1] + scoringMatrix[findIndexOfScoringCharacter(X[i - 1])][findIndexOfScoringCharacter(Y[j - 1])];
            double scoreDel = score[0][j] + scoringMatrix[findIndexOfScoringCharacter(X[i - 1])][scoringMatrixSize - 1];
            double scoreIns = score[1][j - 1] + scoringMatrix[scoringMatrixSize - 1][findIndexOfScoringCharacter(Y[j - 1])];

            // score[1][j] = isLocalAlignment ? max(max(scoreSub, max(scoreDel, scoreIns)), 0.0) : max(scoreSub, max(scoreDel, scoreIns));
            score[i][j] = max(scoreSub, max(scoreDel, scoreIns));

            score[0] = score[1];
        }
    }

    return score[1];
}

pss needlemanWunsch(string X, string Y)
{
    bool flag = false;

    if (Y.length() == 1)
    {
        flag = true;
        string temp = X;
        X = Y;
        Y = temp;
    }

    double score = -MAXFLOAT;
    int idx = -1;

    for (int i = 0; i < Y.length(); i++)
    {
        int tempScore = scoringMatrix[findIndexOfScoringCharacter(X[0])][findIndexOfScoringCharacter(Y[i])];
        if (tempScore > score)
        {
            score = tempScore;
            idx = i;
        }
    }

    if (scoringMatrix[findIndexOfScoringCharacter(X[0])][scoringMatrixSize - 1] > score)
    {
        string xString = X;
        string yString = "-" + Y;
        for (int i = 0; i < Y.length(); i++)
        {
            xString += "-";
        }
        return flag ? pss(yString, xString) : pss(xString, yString);
    }
    else
    {
        string xString = "";
        string yString = "";
        for (int i = 0; i < Y.length(); i++)
        {
            if (i == idx)
            {
                xString += X;
            }
            else
            {
                xString += "-";
            }
            yString += Y[i];
        }
        return flag ? pss(yString, xString) : pss(xString, yString);
    }
}

pss hirschberg(string X, string Y)
{
    string Z = "";
    string W = "";

    if (X.length() == 0)
    {
        // cout << "X IS ZERO\n";
        for (int i = 0; i < Y.length(); i++)
        {
            Z = Z + '-';
            W = W + Y[i];
        }
    }
    else if (Y.length() == 0)
    {
        // cout << "Y IS ZERO\n";
        for (int i = 0; i < X.length(); i++)
        {
            W = W + '-';
            Z = Z + X[i];
        }
    }
    else if (X.length() == 1 || Y.length() == 1)
    {
        // cout << "X OR Y IS ONE\n";
        pss returnedStrings = needlemanWunsch(X, Y);

        Z = returnedStrings.first;
        W = returnedStrings.second;
    }
    else
    {
        // cout << "NORMAL\n";
        int xlen = X.length();
        int xmid = X.length() / 2;
        int ylen = Y.length();

        string xCopy = X.substr(xmid, xlen - xmid);
        string yCopy = Y;
        reverse(xCopy.begin(), xCopy.end());
        reverse(yCopy.begin(), yCopy.end());

        vd scoreL = nwScore(X.substr(0, xmid), Y);
        vd scoreR = nwScore(xCopy, yCopy);

        int ymid = argMax(scoreL, scoreR);

        // cout << "xmid = " << xmid << endl;
        // cout << "X = " << X << ", XL = " << X.substr(0, xmid - 1) << ", XR = " << X.substr(xmid, xlen - xmid) << endl;

        // cout << "ymid = " << ymid << endl;
        // cout << "Y = " << Y << ", YL = " << Y.substr(0, ymid - 1) << ", YR = " << Y.substr(ymid, ylen - ymid) << endl;

        pss returnedStringsL = hirschberg(X.substr(0, xmid), Y.substr(0, ymid));
        pss returnedStringsR = hirschberg(X.substr(xmid, xlen - xmid), Y.substr(ymid, ylen - ymid));
        Z = returnedStringsL.first + returnedStringsR.first;
        W = returnedStringsL.second + returnedStringsR.second;
    }

    return pss(Z, W);
}

void findLocalAlignment()
{
    /*      
                         _ 0
                        |_ s[i-1][j-1] + del(v[i], w[j])
        s[i][j] =   max{|_ s[i-1][j] + del(v[i], -)
                        |_ s[i][j-1] + del(-, w[j])
    */
    int gridSize = firstString.length();

    double cornerCharacter, current;

    vector<int> scores = vector<int>(firstString.length());

    int firstStringIndex, secondStringIndex;

    for (int i = 0; i < firstString.length(); i++)
    {
        for (int j = 0; j < secondString.length(); j++)
        {
            char firstChar;
            char secondChar;
            double maxScore = INT_MIN;
            firstStringIndex = scoringCharacters.find(firstString[i]);
            secondStringIndex = scoringCharacters.find(secondString[j]);

            if (i == 0 && j == 0)
            {
                bool flag = false;

                if (scoringMatrix[firstStringIndex][scoringMatrixSize - 1] >= maxScore)
                {
                    maxScore = scoringMatrix[firstStringIndex][scoringMatrixSize - 1];
                    firstChar = firstString[i];
                    secondChar = '-';
                    j--;
                    flag = true;
                }

                if (scoringMatrix[scoringMatrixSize - 1][secondStringIndex] >= maxScore)
                {
                    maxScore = scoringMatrix[scoringMatrixSize - 1][secondStringIndex];
                    firstChar = '-';
                    secondChar = secondString[j];
                    i--;
                    flag = true;
                }

                if (scoringMatrix[firstStringIndex][secondStringIndex] >= maxScore)
                {
                    maxScore = scoringMatrix[firstStringIndex][secondStringIndex];
                    firstChar = firstString[i];
                    secondChar = secondString[j];
                    flag = true;
                }
            }
            else if (i == 0)
            {
                bool flag = false;

                if (scoringMatrix[firstStringIndex][scoringMatrixSize - 1] >= maxScore)
                {
                    maxScore = scoringMatrix[firstStringIndex][scoringMatrixSize - 1];
                    firstChar = firstString[i];
                    secondChar = '-';
                    j--;
                    flag = true;
                }

                if (scoringMatrix[scoringMatrixSize - 1][secondStringIndex] >= maxScore)
                {
                    maxScore = scoringMatrix[scoringMatrixSize - 1][secondStringIndex];
                    firstChar = '-';
                    secondChar = secondString[j];
                    i--;
                    flag = true;
                }

                if (scoringMatrix[firstStringIndex][secondStringIndex] >= maxScore)
                {
                    maxScore = scoringMatrix[firstStringIndex][secondStringIndex];
                    firstChar = firstString[i];
                    secondChar = secondString[j];
                    flag = true;
                }
            }
        }
    }
}

int main()
{
    init();

    // string x = "AG", y = "TACG";

    // vd scores = nwScore(x, y);

    // for (int i = 0; i < scores.size(); i++)
    // {
    //     cout << scores[i] << ' ';
    // }

    // cout << endl;

    pss result = hirschberg(firstString, secondString);

    cout << (swapped ? result.second : result.first) << endl;
    cout << (swapped ? result.first : result.second) << endl;

    return 0;
}