// Project UID db1f506d06d84ab787baf250c265e24e
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <cmath>
#include <vector>
#include <map>
#include <cassert>
#include <set>
#include "csvstream.h"
using namespace std;

class Classifier {
public
    //constructor
    Classifier(string training_in, string test_in):
    training(training_in), test(test_in) {
    }

    void debug(){
        Do_debug = true;
    }
    void trainer() {
        csvstream fin (training);
        try{
            csvstream fin (training);
        }
        catch (const csvstream_exception &e){
            cout << "Error opening file: " << training << endl;
        }
        std::vector<std::pair<std::string, std::string>> row;
        int i = 0;
        int j = 0;
        int tag = -1;
        int content = -1;
        while (fin >> row) {            //read in row
            int a = 0;
            for (auto mhm : row) {
                if (mhm.first == "tag") {
                    tag = a;
                }
                if (mhm.first == "content") {
                    content = a;
                }
                a++;
            }
            data_2[row[tag].second]++; //update data_2
            istringstream source(row[content].second);
            set<string> words = {istream_iterator<string>{source},
                istream_iterator<string>{}};  //extract unique words from content column
            for (auto i : words) {
                data_1[row[tag].second][i]++;
            } //update data_1
            vocab.insert(words.begin(), words.end());
            posts++;
            if (Do_debug == true){
                if(i == 0){
                  std::cout << "training data:" << std::endl;
                  ++i;
                }
                 std::cout << "  label = " << row[tag].second
                 <<", content = " << row[content].second << endl;
                 ++j;
            }
            else {
                j++;
            }
        }
        cout << "trained on "<< j << " examples" << endl;
        if (Do_debug == true){
                
                cout << "vocabulary size = " << vocab.size() << endl << endl;
                cout << "classes:" << endl;
                for (auto a : data_2){
                    cout << "  " << a.first << ", "<< a.second << " examples, "
                    << "log-prior = " << log(a.second / posts) << endl;
                }
                cout << "classifier parameters:" << endl;
                for (auto a : data_2){
                    double x = 0;
                    double z = 0;
                    for (auto c : vocab){
                        for (auto b : data_1) {
                            if (b.first == a.first) {
                            if (b.second.find(c) != b.second.end()) {
                                z += b.second[c];
                                }
                            }
                        if (z != 0) {
                            x = log(z / a.second);
                        }
                        else {
                            //calculate how many times word i showed up in total
                            double d = 0;
                            for (auto g : data_1) {
                                if (g.second.find(c) != g.second.end()) {
                                    d += g.second[c];
                                }
                            }
                            if (d != 0) {
                                x = log(d / posts);
                            }
                            else {
                                x = log(1 / posts);
                            }
                        }
                        if(z != 0){
                            cout << "  "<< b.first << ":" << c << ","
                            << " count = " << b.second[c] << ", log-likelihood = "
                            << x << endl;
                            z = 0;
                        }
                    } 
                }
            }
            std::cout << endl;
        }
    }
    
    //function which reads in test data, use code from spec, updates map of maps
    void labeller() {
        csvstream fin2(test);

        try{
            csvstream fin2 (test);
        }
        catch (const csvstream_exception &e){
            cout << "Error opening file: " << test << endl;
        }
        std::vector<std::pair<std::string, std::string>> row;
        int j = 0; //number of posts tested
        int h = 0; //number of labels correctly guessed
        std::cout << "test data:" << std::endl;
        while (fin2 >> row) {
            int tag = -1;
            int content = -1;
            int aa = 0;
            for (auto mhm : row) {
                if (mhm.first == "tag") {
                    tag = aa;
                }
                if (mhm.first == "content") {
                    content = aa;
                }
                aa++;
            }
            j++;
            std::cout << "  correct = " << row[tag].second << ", predicted = ";
            istringstream source(row[content].second);
            set<string> words = {istream_iterator<string>{source},
                istream_iterator<string>{}};
            double a = -INFINITY; // sum of logs for this post, add to in for loop
            std::string max_label = "";
            for (auto l : data_2) {
                double c = 0;
                c += log(l.second / posts);
                std::string temp_label = l.first;
                for (auto i : words) {
                //calculate how many times word i showed up under label row[2].second
                double b = 0;
                if (data_1[l.first].find(i) != data_1[l.first].end()) {
                    b += data_1[l.first][i];
                    }
              /*  for (auto k : data_1) {
                    if (k.first == l.first) {
                        if (k.second.find(i) != k.second.end()) {
                            b += k.second[i];
                            }
                        }
                    } */
                    if (b != 0) {
                        c += log(b / l.second);
                    }
                    else {
                         //calculate how many times word i showed up in total
                        double d = 0;
                        for (auto h : words) {
                            if (data_1[h].find(i) != data_1[h].end()) {
                            d += 0.64 * data_1[h][i];
                            }
                        }
                    /*    for (auto g : data_1) {
                            if (g.second.find(i) != g.second.end()) {
                                d += g.second[i];
                            }
                        } */
                        if (d != 0) {
                            c += log(d / posts);
                        }
                        else {
                            c += log(1 / posts);
                        }
                    }
                }
                if (c > a) {
                    a = c;
                    max_label = temp_label;
                }
                if (c == a && temp_label[0] < max_label[0]) {
                    a = c;
                    max_label = temp_label;
                }
            //if c = a, do a = c only if temp_label is alphabetically before max_label
            }
            std::cout << max_label << ", log-probability score = " << a << std::endl;
            std::cout << "  content = " << row[content].second << std::endl << std::endl;
            if (max_label == row[tag].second) {
                h++;
            }
        }
        std::cout << "performance: " << h << " / " << j << " posts predicted correctly"
            << std::endl;
    }
    
    ~Classifier() {
        data_1.clear();
        data_2.clear();
    }
    //function which, given test post, ouputs likely label
private:
    //map stores each label (paired with the number of posts with that label), keyed to
    //map of all words
    //that show up under that label w/ number of times they show up
    std::map<std::string, std::map<std::string, int>> data_1;
    std::map<std::string, int> data_2;
   // bool debug;
    double posts;
    std::set<string> vocab;
    const std::string training;
    const std::string test;
    bool Do_debug = false;
};

int main(int argc, char *argv[]) {
    cout.precision(3);
    if (argc != 3 && argc != 4) {
        std::cout << "Usage: main.exe TRAIN_FILE TEST_FILE [--debug]" << std::endl;
        return -1;
    }
    const string debug = "--debug";
    if (argc == 4 && *(argv+3) != debug) {
        std::cout << "Usage: main.exe TRAIN_FILE TEST_FILE [--debug]" << std::endl;
        return -1;
    }
    Classifier machine = *new Classifier(*(argv + 1), *(argv + 2));
    if(argc == 4 && *(argv+3) == debug){
        machine.debug();
    }
    machine.trainer();
    machine.labeller();
    //creates classifier object with file names, debug
    //run read in/training function
    //run labelling function on test posts
}

//error opening file message??
//cout 'trained on x examples' in the beginning
//fix csvstream (format may be two columns not four)

