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
    if (c == '-')
        return scoringCharacters.length();

    for (int i = 0; i < scoringCharacters.length(); i++)
    {
        if (scoringCharacters[i] == c)
        {
            return i;
        }
    }
    return -1;
}

double computeAlignmentScoreFromStrings(string A, string B)
{
    double cost = 0;
    for (int i = 0; i < A.length(); i++)
    {
        cost += scoringMatrix[findIndexOfScoringCharacter(A[i])][findIndexOfScoringCharacter(B[i])];
    }
    // cout << "Cost from alignment = " << cost << endl;
    return cost;
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

vd nwScore(string X, string Y)
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
            score[1][j] = max(scoreSub, max(scoreDel, scoreIns));

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

pair<pss, pii> findLocalAlignment()
{
    double maxScore = -MAXFLOAT;
    pss bestAlignment;
    pii startIndices;

    for (int i = 0; i < firstString.length(); i++)
    {
        for (int j = 0; j < secondString.length(); j++)
        {
            for (int p = i; p < firstString.length(); p++)
            {
                for (int q = j; q < secondString.length(); q++)
                {
                    pss result = hirschberg(firstString.substr(i, p - i + 1), secondString.substr(j, q - j + 1));
                    double score = computeAlignmentScoreFromStrings(result.first, result.second);
                    if (score > maxScore) {
                        maxScore = score;
                        bestAlignment = result;
                        startIndices.first = i;
                        startIndices.second = j;
                    }
                }
            }
        }
    }

    return pair<pss, pii>(bestAlignment, startIndices);
}

int main()
{
    init();

    // string x = "ATA--GCA", y = "ATATTGC-";

    // double score = computeAlignmentScoreFromStrings(x, y);

    // cout << score << endl;

    pss globalAlignment = hirschberg(firstString, secondString);

    cout << "Global Alignment:\n";

    cout << (swapped ? globalAlignment.second: globalAlignment.first) << endl;
    cout << (swapped ? globalAlignment.first : globalAlignment.second) << endl;

    pair<pss, pii> result = findLocalAlignment();

    pss alignedStrings = result.first;
    pii startIndices = result.second;

    cout << "\nLocal Alignment:\n";

    cout << (swapped ? alignedStrings.second : alignedStrings.first) << endl;
    cout << (swapped ? alignedStrings.first : alignedStrings.second) << endl;

    cout << "Alignment starts in first string at position (1-indexed) = " << (startIndices.first + 1) << endl;
    cout << "Alignment starts in second string at position (1-indexed) = " << (startIndices.second + 1) << endl;

    return 0;
}