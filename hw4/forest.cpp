#include "DecisionTree.h"
#include <cstdlib>
//#include <cstdio>
#include <algorithm>
#include <cstring>
#include <fstream>
#include <ctime>
#include <random>
#include <chrono>

using namespace std;

using std::string;

typedef std::vector<double*>::iterator Iterator;

int main(int argc,char** argv) {
    int dataTotal = 0;
    int featuresTotal = 0;
    const int FOREST_SIZE = atoi(argv[2]);

    double** trainings = new double*[MAX_TRAINING_DATA];
    for(int i = 0; i<MAX_TRAINING_DATA; ++i){
        double* features = new double[MAX_FEATURE];
        trainings[i] = features;
    }

    std::ifstream fin;
    string istring;
    fin.open(argv[1]);

    while (std::getline(fin, istring)) {
        char *cstring, *tmp;
        memset(trainings[dataTotal], 0, sizeof(double) * MAX_FEATURE);

        cstring = new char[istring.size() + 1];
        strncpy(cstring, istring.c_str(), istring.size()+1);

        tmp = strtok(cstring, ": ");
        trainings[dataTotal][0] = atof(tmp);
        tmp = strtok(NULL, ": ");

        while(tmp != NULL) {
            int id = atoi(tmp);
            featuresTotal = id > featuresTotal ? id : featuresTotal;
            tmp = strtok(NULL, ": ");
            trainings[dataTotal][id] = atof(tmp);
            tmp = strtok(NULL, ": ");
        }

        delete[] cstring;

        ++dataTotal;
    }
    vector<double*> examples(trainings, trainings + dataTotal);
    
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();

    const int TREE_SIZE = dataTotal/3;
    DecisionTree** t = new DecisionTree*[FOREST_SIZE];

    for(int i = 0; i<FOREST_SIZE; ++i){
        shuffle(examples.begin(), examples.end(), default_random_engine(seed));
        t[i] = new DecisionTree(examples.begin(), examples.begin()+TREE_SIZE, featuresTotal, 0.);
        t[i]->makeDecisionTree(t[i]->getRoot());
    }

    printForestPredict(t, FOREST_SIZE);

    for(int i = 0; i<FOREST_SIZE; ++i){
        delete t[i];
    }
    delete[] t;

    for(int i = 0; i<MAX_TRAINING_DATA; ++i){
        delete[] trainings[i];
    }
    delete[] trainings;
}
