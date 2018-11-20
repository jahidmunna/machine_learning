#include<bits/stdc++.h>
using namespace std;

void takingDataFromFile(string fileName, vector< vector<double> > &storingArray,vector<int> &classLabels,int *row,int *col,vector<double> &sumPerAttribute)
{
    ifstream inFile;
    inFile.open(fileName.c_str());
    string testData;

    while(getline(inFile,testData))
    {
        istringstream iss(testData);
        double x;
        vector<double> temp;
        int columInput =0;
        while(iss>>x){
            temp.push_back(x);
            if(*row==0){
                sumPerAttribute.push_back(x);
                ++*col;
            }
            else{
                double sum = sumPerAttribute[columInput]+x;
                sumPerAttribute[columInput] = sum;
            }

            columInput++;
        }

        columInput =0;

        int tempClass = (int)temp[temp.size()-1];
        classLabels.push_back(tempClass);
        temp.erase(temp.begin()+temp.size()-1);
        sumPerAttribute.erase(sumPerAttribute.begin()+sumPerAttribute.size()-1);// sum only attribute data
        storingArray.push_back(temp); //takes only attributes data in examples array, not class data
        ++*row;
    }

    --*col;
    inFile.close();
}

void meanCalculator(vector<double> &storingVector, vector<double> &takingValueFromSumVector, int row,int col )
{
    for(int i=0;i<col;i++)
    {
        double temp = takingValueFromSumVector[i]/(row*1.0);
        //cout<<"Mean for "<<takingValueFromSumVector[i]<<" = "<<temp<<endl;
        storingVector.push_back(temp);
    }

}

double standardDaviationCalculator(vector< vector <double> > &examples, int attributeColumNumber, int mean,int row)
{
    double sum = 0;
    for(int i=0;i<row;i++)
    {
        double temValue = examples[i][attributeColumNumber];
        temValue = pow(temValue-mean,2);
        sum+=temValue;
    }

    return sqrt(sum/(row*1.0));
}

void normalizeData(vector< vector <double> > &examples, vector<double> &attributeMean, int row, int col)
{
    for(int i=0;i<col;i++)
    {
        double mean = attributeMean[i];
        double deviation = standardDaviationCalculator(examples,i,mean,row);
        for(int j=0;j<row;j++)
        {
            double tempValue = (examples[j][i] - mean) / deviation;
            examples[j][i] = tempValue;
        }

    }

}

void euclideanDistance(vector< vector <double> > &testExample, int testRowNumber,
              vector< vector <double> > &trainExample, int trainCol, int trainRow,
               multimap<double,int> & trackingIndex)
{
    for(int i=0;i<trainRow;i++)
    {
        double squareSumOfTheDistance = 0;
        for(int j=0;j<trainCol;j++)
        {
            double testValue = testExample[testRowNumber][j];
            double trainValue = trainExample[i][j];
            squareSumOfTheDistance += pow(testValue-trainValue,2);
        }

        double result = sqrt(squareSumOfTheDistance);
        trackingIndex.insert(pair<double,int>(result,i));
    }

}

int mostCommonLabel(map<int,int> &numberPerClassLabel)
{
    map<int,int>::iterator it;
    int maxValue= -10000000;
    int maxKey = -10000000;

     for(it = numberPerClassLabel.begin();it !=numberPerClassLabel.end();++it)
    {
        if(maxValue<it->second)
        {
            maxValue=it->second;
            maxKey = it->first;
        }
    }


    return maxKey;
}


int main(int argc, char *argv[])
{

    string trainFile;
    string testFile;
    string valueForK;
    if(argc>1 ){
        trainFile = argv[1];
        testFile = argv[2];
        valueForK = argv[4];
    }

    int k;

    trainFile+=".txt";
    testFile+=".txt";
    stringstream ss;
    ss<<valueForK;
    ss>>k;

    /*
    cout<<trainFile<<endl;
    cout<<testFile<<endl;
    cout<<k<<endl;
    */

    vector< vector<double> > examplesTrain,examplesTest;
    vector<int>classLabelsTrain,classLabelsTest;
    vector<double>attributeSumTrain,attributeSumTest;
    vector<double>attributeMeanTrain,attributeMeanTest;

    int trainRow=0,trainCol=0;
    int testRow=0,testCol=0;

    takingDataFromFile(trainFile,examplesTrain,classLabelsTrain,&trainRow,&trainCol,attributeSumTrain);
    takingDataFromFile(testFile,examplesTest,classLabelsTest,&testRow,&testCol,attributeSumTest);


    meanCalculator(attributeMeanTrain,attributeSumTrain,trainRow,trainCol); // for training data
    meanCalculator(attributeMeanTest,attributeSumTest,testRow,testCol); // for test data

    //nor normalize all data using standard deviation value 1 and mean value 0
    normalizeData(examplesTrain,attributeMeanTrain,trainRow,trainCol);
    normalizeData(examplesTest,attributeMeanTest,testRow,testCol);


    int totalAccurate = 0;

    for(int i=0;i<testRow;i++)
    {
        multimap<double,int> trackingIndex; // it holds distance in ascending order and train examples indexNumber
        euclideanDistance(examplesTest,i,examplesTrain,trainCol,trainRow,trackingIndex);

        multimap<double,int>::iterator it;

        int z=0;
        map<int,int>mp; //it holds the totalNumber that a class is appeared in neighbor objects
        for(it = trackingIndex.begin();it!=trackingIndex.end() && z<k;++it)
        {
         //   cout<<"distance = "<<it->first<<endl;
            int exampleObjectNumber = it->second;

            int neighborLabel = classLabelsTrain[exampleObjectNumber];
         //   cout<<"NeighborLabel = "<<neighborLabel<<endl;

            if(mp.find(neighborLabel)==mp.end())
            {
                mp[neighborLabel] = 1;
            }
            else
                ++mp[neighborLabel];

            ++z;

        }

        int predicted_class = mostCommonLabel(mp);
        int nn_index = -1;
        double distanceFromNeighbor=0;

        z=0;

        for(it = trackingIndex.begin();it!=trackingIndex.end() && z<k;++it)
        {


            int exampleObjectNumber = it->second;
            int holdinLabel = classLabelsTrain[exampleObjectNumber];

            if(predicted_class==holdinLabel){
                nn_index=exampleObjectNumber;
                distanceFromNeighbor = it->first;
              //  cout<<"distance = "<<distanceFromNeighbor<<endl;
                break;
            }
            z++;
        }

        int object_id = i;
        int true_class = classLabelsTest[i];
        double accuracy=0;
        if(true_class==predicted_class)
        {
         //   cout<<"Same"<<endl;
            accuracy =1.0;
            totalAccurate++;
        }

        printf("ID=%5d, predicted=%3d, true=%3d, nn=%5d, ",object_id, predicted_class, true_class, nn_index);
        cout<<"distance="<<distanceFromNeighbor<<"\taccuracy=    "<<accuracy<<endl;


    }

    double classification_accuracy =(totalAccurate/(testRow*1.0));
    printf("classification accuracy= ");
    cout<<classification_accuracy<<endl;

}
