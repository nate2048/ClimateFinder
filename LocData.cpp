#include "LocData.h"

LocData::LocData() {
    winter = false;
}

LocData::LocData(pair<float, float> coordinates) {
    this->coordinates = coordinates;
    winter = false;
}

void LocData::setSummer(float TAvg, float EMinT, float EMaxT, int DX90, float Prcp, float Snow){
    this->TAvg[0] = TAvg;
    this->EMinT[0] = EMinT;
    this->EMaxT[0] = EMaxT;
    this->DX90 = DX90;
    this->Prcp[0] = Prcp;
    this->Snow[0] = Snow;
}

void LocData::setWinter(float TAvg, float EMinT, float EMaxT, int DX32, float Prcp, float Snow){
    this->TAvg[1] = TAvg;
    this->EMinT[1] = EMinT;
    this->EMaxT[1] = EMaxT;
    this->DX32 = DX32;
    this->Prcp[1] = Prcp;
    this->Snow[1] = Snow;
}

void LocData::setDiff(int season, float TAvgDiff, int DX32Diff, int DX90Diff, float PrcpDiff, float SnowDiff){
    this->TAvgDiff[season] = TAvgDiff;
    if(season == 0)
        this->DX90Diff = DX90Diff;
    if(season == 1)
        this->DX32Diff = DX32Diff;
    this->PrcpDiff[season] = PrcpDiff;
    this->SnowDiff[season] = SnowDiff;
}
vector<float> LocData::getDiff(){
    return {DX32Diff, DX90Diff, PrcpDiff[0], PrcpDiff[1], SnowDiff[0], SnowDiff[1], TAvgDiff[0], TAvgDiff[1]};
}
void LocData::setRank(float rank){
    this->rank = rank;
}


