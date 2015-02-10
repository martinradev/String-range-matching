#ifndef EXPERIMENT_GENERATOR_HPP
#define EXPERIMENT_GENERATOR_HPP

#include "SuffixArray.hpp"
#include <map>
#include <string>

/*!
    Helps generating experiment data
*/
class ExperimentGenerator {
    public:
    /*!
        generates experiment data given some text
        it will randomly select substrings and find how many times they repeat.
        All of the data will be sorted and saved in a file called queries.in in the appropriate folder
        where the format of it will be
        N -> number of lines = \a experimentsCount
        N lines of 3 unsigned integers A B and C
        where
        TEXT[B..B+C) repeats itself A times
    */
    void generateExperiment(const std::string & experimentFolder, size_t experimentsCount, size_t substringAtLeast);
    /*!
        initializes the experiment generator
    */
    ExperimentGenerator();
    private:
    /*!
        returns a \a std::pair where the first element points to the starting index of the
        substring in the text and the second element is the length of the substring
        The second element will have a value of at least \a atLeast.
        \a length points to the length of the text
    */
    std::pair<size_t, size_t> getRandomSubstring(size_t atLeast, size_t length);
    /*!
        saves the experiment data map into
    */
    void saveMap(const std::map<size_t, std::pair<size_t, size_t> > & experimentData, const std::string & experimentDirectory);
};

#endif // EXPERIMENT_GENERATOR_HPP
