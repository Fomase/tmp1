#include <vector>
#include <map>
#include <string>
#include <string_view>
#include <algorithm>
#include <cassert>

using namespace std;

class Translator {
public:
    Translator() = default;
    
    void Add(string_view source, string_view target) {
        words.push_back({source.begin(), source.end()});
        words.push_back({target.begin(), target.end()});
        auto& sproxy = *find(words.begin(), words.end(), source);
        auto& tproxy = *find(words.begin(), words.end(), target);
        sources[sproxy] = tproxy;
        targets[tproxy] = sproxy;
    }
    
    string_view TranslateForward(string_view source) const {
        return sources.at(source);
    }
    
    string_view TranslateBackward(string_view target) const {
        return targets.at(target);
    }

private:
    std::map<string_view, string_view> sources;
    std::map<string_view, string_view> targets;
    std::vector<std::string> words;
};

void TestSimple() {
    Translator translator;
    translator.Add(string("okno"s), string("window"s));
    translator.Add(string("stol"s), string("table"s));

    assert(translator.TranslateForward("okno"s) == "window"s);
    assert(translator.TranslateBackward("table"s) == "stol"s);
    assert(translator.TranslateForward("table"s) == ""s);
}

int main() {
    TestSimple();
    return 0;
}
