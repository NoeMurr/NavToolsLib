//
// Created by Noè Murr on 06/09/2017.
//

#ifndef NAVTOOLS_STRINGUTILS_HPP
#define NAVTOOLS_STRINGUTILS_HPP


#include <string>
#include <sstream>
#include <vector>
#include <iterator>

namespace ntl {
    namespace utils {
        template<typename Out>
        void split(
            const std::string &s, char delim, Out result,
            bool skipEmpty = false
        ) {
            std::stringstream ss;
            ss.str(s);
            std::string item;
            while (std::getline(ss, item, delim)) {
                if (skipEmpty and item.empty()) continue;
                *(result++) = item;
            }
        }

        std::vector<std::string> split(
            const std::string &s, char delim, bool skipEmpty = false
        ) {
            using namespace std;
            vector<string> elems;
            split(s, delim, back_inserter(elems), skipEmpty);
            return std::move(elems);
        }
    }
}


#endif //NAVTOOLS_STRINGUTILS_HPP
