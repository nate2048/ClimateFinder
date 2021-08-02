#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <sstream>
#include <cmath>
#include <vector>
#include "LocData.h"

struct Param{
    //User preferences or exclusion factors
    //Preference: find location with attribute that comes close to this value
    //Exclusion factor (EF): if above/below this value, ignore location
    //Boolean reveals EF or preference: EF == false and preference == true

    //strictly a preference and required
    float TAvg[2] = {0}; //Average temp for season

    //strictly EF
    float EMinT[2] = {0}; //extreme min temp for season
    float EMaxT[2] = {0}; //extreme max temp for season

    //can be a preference or an EF
    int DX32 = -1; //num days with max temp <= 32F only winter
    int DX90 = -1; //num days with max temp >= 90F only summer
    float Prcp[2] = {-1}; //total (avg) precipitation in inches for season
    float Snow[2] = {-1}; //total (avg) snowfall in inches for season

    //first boolean indicates if param used or not
    //second boolean reveals EF or preference: EF == false and preference == true
    pair<bool, bool> selected[6] = {make_pair(false,false)};
};

void ShowMenu();
void SpecialParam(int option, Param& UserPref);
vector<float> LoadFile(string path, map<pair<float, float>, LocData>& locations, Param& pref);
void calcRank(map<pair<float, float>, LocData>& locations, vector<float> winterMax, vector<float> summerMax, Param& pref);

/*
vector<LocData> sortData(map<pair<float, float>, LocData>& locations, char choice);

void quickSort(vector<LocData>& locations, int low, int high);
int partition(vector<LocData>& locations, int low, int high);
void swap(LocData* a, LocData* b);

void mergeSort(vector<LocData>& locations, int const begin, int const end);
void merge(vector<LocData>& locations, int const left, int const mid, int const right);
*/

int main() {

    Param UserPref;
    float summer, winter;

    cout << "\n-------------------Welcome to Climate Finder-------------------\n" << endl;
    cout << "Climate Finder is a tool that compares your climate preferences" << endl;
    cout << "to over 100,000 locations within the United States and matches" << endl;
    cout << "you to your perfect travel destination or future residency." << endl;
    cout << "---------------------------------------------------------\n" << endl;
    cout << "This first set of preferences are required and the" << endl;
    cout << "following climate parameters are optional." << endl;
    cout << "\n-------Please enter all preferences as integers--------\n" << endl;
    cout << "Preferred avg temperature (in degrees Fahrenheit) for -" << endl;
    cout << "Summer: ";
    cin >> summer;
    cout << "Winter: ";
    cin >> winter;

    UserPref.TAvg[0] = summer;
    UserPref.TAvg[1] = winter;

    cout << "\n---------------------------------------------------------\n" << endl;
    string cont;
    cout << "The following climate preferences are optional, you can" << endl;
    cout << "view your results now or continue by adding more preferences.\n" << endl;
    cout << R"(Would you like to continue? (enter "yes" or "no") )";
    cin >> cont;
    if(cont == "yes"){
        int option = 0;
        while(option != 7){
            ShowMenu();
            cout << "Selection (enter a number between 1 and 7): ";
            cin >> option;
            cout << endl;
            if(option == 7)
                break;
            else if(0 < option && option < 7)
                SpecialParam(option, UserPref);
            else{
                cout << "invalid input..." << endl;
                ShowMenu();
                continue;
            }

        }
    }

    //load location nodes that fall within users preferences
    //function returns vector with data specifying the largest deviant from the users preferences
    //these values are used to calculate how closely a location matches a users preferences
    map<pair<float, float>, LocData> locations;
    vector<float> maxDiffSum = LoadFile("../SummerClimate.csv", locations, UserPref);
    vector<float> maxDiffWinter = LoadFile("../WinterClimate.csv", locations, UserPref);


    //if location passes for summer climate but not for winter, node is removed from map
    vector<pair<float,float>> deleteNodes;
    for(auto iter = locations.begin(); iter != locations.end(); ++iter){
        if(!iter->second.winter)
            deleteNodes.push_back(iter->first);
    }
    for(int i = 0; i < deleteNodes.size(); i++)
        locations.erase(deleteNodes[i]);

    calcRank(locations, maxDiffSum, maxDiffWinter, UserPref);
    /*
    vector<LocData> quickSorted = sortData(locations, 'q');
    vector<LocData> mergeSorted = sortData(locations, 'm');
    */

}

void ShowMenu(){
    cout << "\n---------------------------------------------------------\n" << endl;
    cout << "Available options:" << endl;
    cout << "1. Set limit on minimum temperature" << endl;
    cout << "2. Set limit on maximum temperature" << endl;
    cout << "3. Set limit or preference for num days temp is below 32F" << endl;
    cout << "4. Set limit or preference for num days temp is above 90F" << endl;
    cout << "5. Set limit or preference for average monthly precipitation" << endl;
    cout << "6. Set limit or preference for average monthly snowfall" << endl;
    cout << "7. View results\n" << endl;
}

void SpecialParam(int option, Param& UserPref){

    float summer = 0, winter = 0;
    int prefOrLimit;
    bool prefOrLimitB;

    if (option == 1)
        cout << "Smallest acceptable minimum temperature in the" << endl;
    else if (option == 2)
        cout << "Largest acceptable maximum temperature in the" << endl;
    else{
        cout << "Would you like to set a -" << endl;
        cout << "1. limit" << endl;
        cout << "2. preference\n" << endl;
        cout << "Selection (enter 1 or 2): ";
        cin >> prefOrLimit;
        cout << endl;
        if (option == 3 && prefOrLimit == 1)
            cout << "Greatest number of days where temp is below 32F in the" << endl;
        if (option == 3 && prefOrLimit == 2)
            cout << "Ideal number of days where temp is below 32F in the" << endl;
        if (option == 4 && prefOrLimit == 1)
            cout << "Greatest number of days where temp is above 90F in the" << endl;
        if (option == 4 && prefOrLimit == 2)
            cout << "Ideal number of days where temp is above 90F in the" << endl;
        if (option == 5 && prefOrLimit == 1)
            cout << "Max precipitation in inches in the" << endl;
        if (option == 5 && prefOrLimit == 2)
            cout << "Ideal level of precipitation in inches in the" << endl;
        if (option == 6 && prefOrLimit == 1)
            cout << "Max snowfall in inches in the" << endl;
        if (option == 6 && prefOrLimit == 2)
            cout << "Ideal level of snowfall in inches in the" << endl;

        prefOrLimitB = (prefOrLimit != 1);
    }
    if(option != 3){
        cout << "Summer: ";
        cin >> summer;
    }
    if(option != 4){
        cout << "Winter: ";
        cin >> winter;
    }

    if (option == 1){
        UserPref.EMinT[0] = summer;
        UserPref.EMinT[1] = winter;
        UserPref.selected[0] = make_pair(true, false);
    }
    else if (option == 2){
        UserPref.EMaxT[0] = summer;
        UserPref.EMaxT[1] = winter;
        UserPref.selected[1] = make_pair(true, false);
    }
    else if (option == 3){
        UserPref.DX32 = (int)winter;
        UserPref.selected[2] = make_pair(true, prefOrLimitB);
    }
    else if (option == 4){
        UserPref.DX90 = (int)summer;
        UserPref.selected[3] = make_pair(true, prefOrLimitB);
    }
    else if (option == 5){
        UserPref.Prcp[0] = summer;
        UserPref.Prcp[1] = winter;
        UserPref.selected[4] = make_pair(true, prefOrLimitB);
    }
    else{
        UserPref.Snow[0] = summer;
        UserPref.Snow[1] = winter;
        UserPref.selected[5] = make_pair(true, prefOrLimitB);
    }

    cout << "Preference Successfully entered!" << endl;
}

vector<float> LoadFile(string path, map<pair<float,float>, LocData>& locations, Param& pref){

    string season = (path == "../SummerClimate.csv") ? "Summer" : "Winter";
    int index = (path == "../SummerClimate.csv") ? 0 : 1;

    //values store max differences between a users preferences and the climate for all nodes
    //are only stored for locations that aren't ruled out
    int maxDiff32 = 0, maxDiff90 = 0;
    float maxDiffPrcp = 0, maxDiffSnow = 0;

    ifstream readFile;
    readFile.open(path);

    if(readFile.is_open()){

        string lineFile;
        getline(readFile, lineFile);

        //Columns of CSV files
        //TAvg  EMinT  EMaxT  DX32  DX90  Prcp  Snow  Y  X

        while(getline(readFile, lineFile)){

            istringstream stream(lineFile);

            string temp;
            float TAvg, EMinT, EMaxT, Prcp, Snow, latitude, longitude;
            int DX32, DX90;

            float TAvgDiff, prcpDiff = 0, snowDiff = 0;
            int DX32Diff = 0, DX90Diff = 0;

            //TAvg: average temp
            getline (stream, temp, ',');
            TAvgDiff = abs(stof(temp) - pref.TAvg[index]);
            if (TAvgDiff <= 3)
                TAvg = stof(temp);
            else
                continue;

            //EMinT and EMaxT are used exclusively to reduce size of qualifying set (EF)
            //EMinT: extreme minimum temperature
            getline (stream, temp, ',');
            EMinT = stof(temp);
            if(pref.selected[0].first){
                if(EMinT < pref.EMinT[index])
                    continue;
            }

            getline (stream, temp, ',');
            EMaxT = stof(temp);
            if(pref.selected[1].first){
                if(EMaxT > pref.EMaxT[index])
                    continue;
            }

            getline (stream, temp, ',');
            DX32 = stoi(temp);
            if(pref.selected[2].first && season == "Winter"){
                //used to help assign ranking
                if(pref.selected[2].second)
                    DX32Diff = abs(DX32 - pref.DX32);
                //used to exclude certain location nodes
                else
                    if(DX32 > pref.DX32)
                        continue;
            }

            getline (stream, temp, ',');
            DX90 = stoi(temp);
            if(pref.selected[3].first && season == "Summer"){
                if(pref.selected[3].second)
                    DX90Diff = abs(DX90 - pref.DX90);
                else
                    if(DX90 > pref.DX90)
                        continue;
            }

            getline (stream, temp, ',');
            Prcp = stof(temp);
            if(pref.selected[4].first){
                if(pref.selected[4].second)
                    prcpDiff = abs(Prcp - pref.Prcp[index]);
                else
                    if(Prcp > pref.Prcp[index])
                        continue;
            }

            getline (stream, temp, ',');
            Snow = stof(temp);
            if(pref.selected[5].first){
                if(pref.selected[5].second)
                    snowDiff = abs(Snow - pref.Snow[index]);
                else
                    if(Snow > pref.Snow[index])
                        continue;
            }

            getline(stream, temp, ',');
            latitude = stof(temp);
            getline(stream, temp, ',');
            longitude = stof(temp);

            pair<float, float> coordinates = make_pair(latitude, longitude);

            //since summer file is read first and location must meet specification for both summer and winter
            //location node can only be created in when reading through summer file
            if(season == "Summer"){
                LocData newLocation(coordinates);
                newLocation.setSummer(TAvg, EMinT, EMaxT, DX90, Prcp, Snow);
                locations[coordinates] = newLocation;
            }
            if(season == "Winter"){
                //didn't match summer parameters so node was never made
                if(locations.find(coordinates) == locations.end())
                    continue;
                else{
                    locations[coordinates].setWinter(TAvg, EMinT, EMaxT, DX32, Prcp, Snow);
                    //needed so it is simple to determine if specifications were met for summer but not winter
                    locations[coordinates].winter = true;
                }
            }
            //for each location, the magnitude of the climate factors deviation from the users preferences is stored
            locations[coordinates].setDiff(index, TAvgDiff, DX32Diff, DX90Diff, prcpDiff, snowDiff);

            maxDiff32 = (DX32Diff > maxDiff32) ? DX32Diff : maxDiff32;
            maxDiff90 = (DX90Diff > maxDiff90) ? DX90Diff : maxDiff90;
            maxDiffPrcp = (prcpDiff > maxDiffPrcp) ? prcpDiff : maxDiffPrcp;
            maxDiffSnow = (snowDiff > maxDiffSnow) ? snowDiff : maxDiffSnow;
        }
        vector<float> v = {(float)maxDiff32, (float)maxDiff90, maxDiffPrcp, maxDiffSnow};
        return v;
    }
    cout << "file not opened" << endl;
    return {};
}

void calcRank(map<pair<float, float>, LocData>& locations, vector<float> winterMax, vector<float> summerMax,Param& pref){
    float rank;
    for(auto iter = locations.begin(); iter != locations.end(); iter++){
        rank = 0;
        vector<float> currLoc = iter->second.getDiff();
        if(pref.selected[2].second)
            rank += currLoc[0] / winterMax[0];
        if(pref.selected[3].second)
            rank += currLoc[1] / summerMax[1];
        if(pref.selected[4].second)
            rank += (currLoc[2] / summerMax[2]) + (currLoc[3] / winterMax[2]);
        if(pref.selected[5].second)
            rank += (currLoc[4] / summerMax[3]) + (currLoc[5] / winterMax[3]);
        rank += (currLoc[6] + currLoc[7]) / 6;

        iter->second.setRank(rank);
    }
}

/*
vector<LocData> sortData(map<pair<float, float>, LocData>& locations, char choice)
{
    vector<LocData> result;
    
    for(auto it = locations.begin(); it != locations.end(); it++)
        result.push_back(it->second);
    
    if(choice == 'q')
        quickSort(result, 0, result.size() - 1);
    if(choice == 'm')
        mergeSort(result, 0, result.size() - 1);
        
    return result;
}

void quickSort(vector<LocData>& locations, int low, int high)
{
    if(low < high)
    {
        int partIndex = partition(locations, low, high);
        
        quickSort(locations, low, partIndex - 1);
        quickSort(locations, partIndex + 1, high);
    }
}

int partition(vector<LocData>& locations, int low, int high)
{
    int pivot = locations[high].rank;
    int index = low - 1;
    
    for(int i = low; i <= high - 1; i++)
    {
        if(locations[i].rank <= pivot)
        {
            index++;
            swap(&locations[index], &locations[i]);
        }
    }
    
    swap(&locations[index + 1], &locations[high]);
    return index + 1;
}

void swap(LocData* a, LocData* b)
{
    LocData temp = *a;
    *a = *b;
    *b = temp;
}

void mergeSort(vector<LocData>& locations, int const begin, int const end)
{
    if(begin >= end)
        return;
        
    auto mid = begin + (end - begin) / 2;
    mergeSort(locations, begin, mid);
    mergeSort(locations, mid + 1, end);
    merge(locations, begin, mid, end);
}

void merge(vector<LocData>& locations, int const left, int const mid, int const right)
{
    int const leftHalf = mid - left + 1;
    int const rightHalf = right - mid;
    
    vector<LocData> leftArray = vector<LocData>(leftHalf);
    vector<LocData> rightArray = vector<LocData>(rightHalf);
    
    for(int i = 0; i < leftHalf; i++)
        leftArray[i] = locations[left + i];
        
    for(int i = 0; i < rightHalf; i++)
        rightArray[i] = locations[mid + 1 + i];
        
        
    auto indexLeft = 0, indexRight = 0;
    int indexOfMerge = left;
    
    while(indexLeft < leftHalf && indexRight < rightHalf)
    {
        if(leftArray[indexLeft].rank <= rightArray[indexRight].rank)
        {
            locations[indexOfMerge] = leftArray[indexLeft];
            indexLeft++;
        }
        else
        {
            locations[indexOfMerge] = rightArray[indexRight];
            indexRight++;
        }
        indexOfMerge++;
    }
    
    while(indexLeft < leftHalf)
    {
        locations[indexOfMerge] = leftArray[indexLeft];
        indexLeft++;
        indexOfMerge++;
    }
    
    while(indexRight < rightHalf)
    {
        locations[indexOfMerge] = rightArray[indexRight];
        indexRight++;
        indexOfMerge++;
    }
}
*/
