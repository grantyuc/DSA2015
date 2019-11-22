#include "DecisionTree.h"
#include <cstdlib>
//#include <cstdio>
#include <cstring>
#include <fstream>

using namespace std;

using std::string;

typedef std::vector<double*>::iterator Iterator;

int main(int argc,char** argv) {
    int dataTotal = 0;
    int featuresTotal = 0;
    const double EPSILON = atof(argv[2]);

    //int label[MAX_TRAINING_DATA];

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
        //label[dataTotal] = atoi(tmp);
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
    
    /*
    for(Iterator it=examples.begin(); it!=examples.end(); ++it){
        cout << int(**it) << " ";
        for(int i = 1; i<=featuresTotal; ++i){
            cout << (*it)[i] << " ";
        }
        cout << endl;
    }
    */
    
    DecisionTree* t = new DecisionTree(examples, featuresTotal, EPSILON);
    t->makeDecisionTree(t->getRoot());
    cout << *t << endl;
    //printTreePredict(*t, t->getRoot());

    delete t;

    for(int i = 0; i<MAX_TRAINING_DATA; ++i){
        delete[] trainings[i];
    }
    delete[] trainings;
}
