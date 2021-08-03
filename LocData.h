#include <utility>
#include <vector>
#pragma once
using namespace std;

class LocData {
private:

    float TAvg[2], EMinT[2], EMaxT[2], Prcp[2], Snow[2];
    int DX32, DX90;

    float TAvgDiff[2], DX32Diff, DX90Diff, PrcpDiff[2], SnowDiff[2];

public:

    pair<float, float> coordinates;
    bool winter;
    float rank;

    LocData();
    LocData(pair<float, float> coordinates);
    void setSummer(float TAvg, float EMinT, float EMaxT, int DX90, float Prcp, float Snow);
    void setWinter(float TAvg, float EMinT, float EMaxT, int DX32, float Prcp, float Snow);
    void setDiff(int season, float TAvgDiff, int DX32Diff, int DX90Diff, float PrcpDiff, float SnowDiff);
    vector<float> getDiff();
    void setRank(float rank);
    vector<float> getData();

};


