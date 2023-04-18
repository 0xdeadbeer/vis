#include <stdio.h>
#include <iostream>
#include <ftxui/dom/elements.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/event.hpp> 

using namespace std; 
using namespace ftxui; 

int main() {

    auto screen = ScreenInteractive::FitComponent();

    auto component = Renderer([&screen] {
        auto make_box = [](int x, int y, int i) {
            string title = to_string(i);
            return window(text(""), text(title) | hcenter | size(WIDTH, EQUAL, x) | size(HEIGHT, EQUAL, y));
        };

        Element calendar = hflow({
            make_box(12,7,0),
            make_box(7,7,1),
            make_box(7,7,2),
            make_box(7,7,3),
            make_box(7,7,4),
            make_box(7,7,5),
            make_box(7,7,6),
            make_box(7,7,7)
        }); 
         
        return window(text("Vi Scheduler"), vbox(calendar));
    });
   

    screen.Loop(component);

    return 0; 
}
