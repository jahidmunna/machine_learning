#include<bits/stdc++.h>
using namespace std;
#define MAX_NODE 50

int setIndexForTheCurrentCity(string cityName,vector<string> &cityIndex)
{
    int lim = cityIndex.size();
    int i=0;

    for(i=0;i<lim;i++){
        if(cityIndex[i]==cityName){
            return i;
        }
    }
    cityIndex.push_back(cityName);
    return i;

}

void fileReadingAndDataStoring(string fileName, int graph[MAX_NODE][MAX_NODE],string originCity,string destinationCity,
                               int &originCityIndex, int &destinationCityIndex, int &totalCityNumber,vector <string> &cityIndex)
{
    ifstream inFile;
    inFile.open(fileName.c_str());

    string testData;
    while(getline(inFile,testData))
    {
        if(testData=="END OF INPUT")
            break;
        stringstream ss;
        ss<<testData;
        string source,destination;
        int distance;
        ss>>source;
        ss>>destination;
        ss>>distance;

        int sourceIndex = setIndexForTheCurrentCity(source,cityIndex);
        int destinationIndex = setIndexForTheCurrentCity(destination,cityIndex);

        if(originCity==source && originCityIndex<0)
            originCityIndex = sourceIndex;
        else if (originCity==destination && originCityIndex<0)
            originCityIndex = destinationIndex;

        if(destinationCity==source && destinationCityIndex<0)
            destinationCityIndex = sourceIndex;
        else if (destinationCity== destination && destinationCityIndex<0)
            destinationCityIndex = destinationIndex;

        graph[sourceIndex][sourceIndex] = 0;
        graph[destinationIndex][destinationIndex] = 0;
        graph[sourceIndex][destinationIndex] = distance;
        graph[destinationIndex][sourceIndex] = distance;

    }
    inFile.close();
    totalCityNumber = cityIndex.size();
}


void dijkstra(int sourceIndex,int destinationIndex,int adjMatrix[MAX_NODE][MAX_NODE],int totalCity,vector<string> &cityIndex){
    int source = sourceIndex;
    int destination = destinationIndex;
    int numOfVertices = totalCity ;
    bool mark[numOfVertices];
    int dis[numOfVertices],origin[numOfVertices],dijkstraPath[numOfVertices];

    for(int i=0;i<numOfVertices;i++) {
        mark[i] = false;
        origin[i] = -1;
        dis[i] = 100000000;
    }
    dis[source]= 0;
    int count = 0;

    while(count < numOfVertices) {
        int minDistance = 100000000;
        int closestUnmarkedNode;
        for(int i=0;i<numOfVertices;i++) {
            if((!mark[i]) && ( minDistance >= dis[i])) {
                minDistance = dis[i];
                closestUnmarkedNode = i;
            }
        }

        mark[closestUnmarkedNode] = true;
        for(int i=0;i<numOfVertices;i++) {
            if((!mark[i]) && (adjMatrix[closestUnmarkedNode][i]>0) ) {
                if(dis[i] > dis[closestUnmarkedNode]+adjMatrix[closestUnmarkedNode][i]) {
                    dis[i] = dis[closestUnmarkedNode]+adjMatrix[closestUnmarkedNode][i];
                    origin[i] = closestUnmarkedNode;
                }
            }
        }
        count++;
    }

    if(dis[destinationIndex]>10000000){
        cout<<"distance: infinity\nroute:\nnone"<<endl;
    }
    else
    {
        //cout<<"The path is exist"<<endl;
        int key =0;
        int node = destination;
        dijkstraPath[key] = destination;

        while(node != source){
            key++;
            dijkstraPath[key] = origin[node];
            node = origin[node];
        }

        cout<<"distance: "<<dis[destination]<<" km"<<endl;
        cout<<"route:"<<endl;

        for(int i=key;i>0;i--){
            cout<<cityIndex[dijkstraPath[i]]<<" to "
            <<cityIndex[dijkstraPath[i-1]]<<", "<<
            adjMatrix[dijkstraPath[i]][dijkstraPath[i-1]]<<" km"<<endl;
        }
    }

}


int main(int argc, char *argv[])
{

    string input_filename,origin_city,destination_city;
   // cin>>input_filename>>origin_city>>destination_city;

    if(argc>1 ){
        input_filename = argv[1];
        origin_city = argv[2];
        destination_city = argv[3];
    }


    int adjMatrix[MAX_NODE][MAX_NODE];
    memset(adjMatrix, -1,sizeof(adjMatrix)); //initialize adj matrix with -1

    int sourceIndex=-1, destinationIndex=-1;
    int totalCity = 0;
    vector <string> cityIndex;

    fileReadingAndDataStoring(input_filename,adjMatrix,origin_city,destination_city,sourceIndex,destinationIndex,totalCity,cityIndex);
    dijkstra(sourceIndex,destinationIndex,adjMatrix,totalCity,cityIndex);
}
