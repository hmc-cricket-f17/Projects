#include "parser.hpp"
#include "parsers.hpp"
#include "widget.hpp"

#include <iostream>
#include <optional>
#include <string>
#include <stdexcept>
#include <ctype.h>

using namespace std;

int main(int argc, char **argv)
{
    auto parseFn = 
        [](const string& input) -> optional<pair<Widget, string>> {
            if (input.empty()) {
                return {};
            }

            return make_optional(make_pair(Widget::createWidget(input[0]), input.substr(1)));
        };

    int n = 6;
    auto parseIntFn = 
        [&n](const string& input) -> optional<pair<int, string>> {
            if (input.empty()) {
                return {};
            }

            return make_optional(make_pair(n, input.substr(1)));
        };
   
    // auto pGen{
    //     [&parseIntFn](const Widget&){ 
    //         return Parser<int>{parseIntFn}; 
    //     }
    // };

    // Parser<Widget> p1{parseFn};
    // Parser<int> p2 = p1.andThen(move(pGen));

    // try {
    //     int w = p2.parse(argv[1]);
    //     cout << w << endl;
    // } catch (invalid_argument& e) {
    //     cerr << e.what() << endl;
    // }

    Parser<Widget> p1{parseFn};
    Parser<int> p2{parseIntFn};
    Parser<pair<Widget, int>> p3 = p1.combine(p2);
    
    try {
        pair<Widget, int> widgets = p3.parse(argv[1]);
        cout << widgets.first << " " << widgets.second << endl;
    } catch (invalid_argument& e) {
        cerr << e.what() << endl;
    }

    // **************TODO: THIS SEGFAULTS**************
    // Parser<Widget> p1{parseFn};
    // Parser<int> p2{parseIntFn};
    // Parser<int> p3{parseIntFn};
    // Parser<pair<pair<Widget, int>, int>> p4 = p1.combine(p2).combine(p3);
    // cout << "h" << endl;
    
    // try {
    //     pair<pair<Widget, int>, int> widgets = p4.parse(argv[1]);
    //     cout << "h" << endl;
    //     cout << widgets.first.first << " " << widgets.first.second << " " << widgets.second << endl;
    // } catch (invalid_argument& e) {
    //     cerr << e.what() << endl;
    // }
    //**************

    // try {
    //     pair<char, char> chars = q.parse(argv[1]);
    //     cout << chars.first << " "  << chars.second << endl;
    // } catch (invalid_argument& e) {
    //     cerr << e.what() << endl;
    // }
}
