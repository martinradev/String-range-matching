#include "ExperimentGenerator.hpp"
#include <fstream>
#include <cstdlib>
#include <ctime>
using namespace std;

ExperimentGenerator::ExperimentGenerator() {
    srand(time(NULL));
};

void ExperimentGenerator::generateExperiment(const string & experimentFolder, size_t experimentsCount, size_t substringAtLeast) {
    ifstream in(experimentFolder + "text.in");
    if (!in.good()) {
        cerr << "Couldn't open file: " << experimentFolder << "text.in" << endl;
        exit(1);
    }
    string s;
    cout << "Start reading file for experiment " << experimentFolder << endl;
    in.seekg(0, std::ios::end);
    s.reserve(in.tellg());
    in.seekg(0, std::ios::beg);
    s.assign((std::istreambuf_iterator<char>(in)),
            std::istreambuf_iterator<char>());
    cout << "Done reading file for experiment " << experimentFolder << endl;
    cout << "Start building suffix array for experiment " << experimentFolder << endl;
    SuffixArray<char> arr = SuffixArray<char>(s);
    cout << "Done building suffix array for experiment " << experimentFolder << endl;
    cout << "Start processing substrings for experiment " << experimentFolder << endl;
    map<size_t, pair<size_t, size_t> > mp;
    for (int i = 1; i <= experimentsCount; ++i) {
        pair<size_t, size_t> pp = getRandomSubstring(substringAtLeast, s.length());
        string ran = s.substr(pp.first,pp.second);
        size_t l = arr.lowerBound(ran);
        ran.back()++;
        size_t r = arr.upperBound(ran);
        ran.back()--;
        mp.insert(make_pair(r-l-1, pp));
        cout << "Processed: \b" << i << "/" << experimentsCount << endl;
    }
    cout << "Done processing substrings for experiment " << experimentFolder << endl;
    cout << "Start saving data for experiment " << experimentFolder << endl;
    saveMap(mp, experimentFolder);
    cout << "Done saving data for experiment " << experimentFolder << endl;
}

void ExperimentGenerator::saveMap(const map<size_t, pair<size_t, size_t> > & experimentData, const string & experimentDirectory) {
    ofstream out(experimentDirectory + "queries.in");
    out << experimentData.size() << endl;
    typedef pair<size_t, pair<size_t, size_t> > ExperimentRow;
    for (ExperimentRow row : experimentData) {
        out << row.first << " " << row.second.first << " " << row.second.second << endl;
    }
    out.close();
}

pair<size_t, size_t> ExperimentGenerator::getRandomSubstring(size_t atLeast, size_t length) {
    size_t i = rand()%(length-atLeast);
    size_t j = 0;
    while (j == 0 || i+j >= length) {
        j = rand()%20 + atLeast;
    }
    return make_pair(i,j);
}
