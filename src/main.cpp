#include <SFML/Graphics.hpp>
#include "game/grid.hpp"
#include "game/layout.hpp"
#include "game/math.hpp"

static sf::Color color_for(char s){
    switch(s){
        case 'R': return sf::Color::Red;
        case 'G': return sf::Color::Green;
        case 'B': return sf::Color::Blue;
        case 'Y': return sf::Color::Yellow;
        case 'X': return sf::Color::Black;
        case 'T': return sf::Color(255,165,0);
        case 'P': return sf::Color(165,42,42);
        default:  return sf::Color::White;
    }
}

int main() {
    // (Optional) one-time calibration in Debug builds
    Paytable pt;                  // default values
    //calibrate_to_target(pt, 0.98);  // enable when you want to pre-tune

    sf::RenderWindow win(sf::VideoMode(900, 700), "Pirots3 Slot");
    win.setVerticalSyncEnabled(true);

    sf::Font font;
    if(!font.loadFromFile("assets/arial.ttf"))
        font.loadFromFile("C:/Windows/Fonts/arial.ttf");

    Grid grid; generate_grid(grid);
    Layout layout = recompute_layout(win.getSize());

    sf::RectangleShape btn; btn.setFillColor({50,50,50});
    auto layout_button = [&](){
        btn.setSize({layout.spinRect.width, layout.spinRect.height});
        btn.setPosition({layout.spinRect.left, layout.spinRect.top});
    };
    layout_button();

    sf::Text btnText("SPIN", font, layout.textPx);
    auto center_btn_text = [&](){
        btnText.setCharacterSize((unsigned)std::clamp(layout.spinRect.height * 0.5f, 14.f, 32.f));
        auto b = btnText.getLocalBounds();
        btnText.setPosition(layout.spinRect.left + (layout.spinRect.width - b.width)*0.5f - b.left,
                            layout.spinRect.top  + (layout.spinRect.height- b.height)*0.5f - b.top);
    };
    center_btn_text();

    int64_t credits = 10000;
    const int64_t bet = 100;

    while (win.isOpen()) {
        sf::Event e{};
        while (win.pollEvent(e)) {
            if (e.type == sf::Event::Closed) win.close();
            if (e.type == sf::Event::Resized) {
                on_resize(win, e.size.width, e.size.height);
                layout = recompute_layout(win.getSize());
                layout_button();
                center_btn_text();
            }
            if (e.type == sf::Event::MouseButtonPressed) {
                sf::Vector2f p((float)e.mouseButton.x, (float)e.mouseButton.y);
                if (layout.spinRect.contains(p)) {
                    credits -= bet;
                    generate_grid(grid);
                    int64_t win_coins = evaluate_payout(grid, pt);
                    credits += win_coins;
                }
            }
        }

        win.clear(sf::Color::Black);

        // draw grid
        for (int r=0; r<ROWS; ++r) {
            for (int c=0; c<COLS; ++c) {
                sf::RectangleShape cell({layout.cell-4.f, layout.cell-4.f});
                cell.setPosition(layout.gridLeft + c*layout.cell + 2.f,
                                 layout.gridTop  + r*layout.cell + 2.f);
                cell.setFillColor(color_for(grid[r][c]));
                win.draw(cell);

                sf::Text t(std::string(1, grid[r][c]), font,
                           (unsigned)std::clamp(layout.cell * 0.35f, 10.f, 48.f));
                t.setFillColor(sf::Color::White);
                auto b = t.getLocalBounds();
                t.setPosition(layout.gridLeft + c*layout.cell + (layout.cell - b.width )*0.5f - b.left,
                              layout.gridTop  + r*layout.cell + (layout.cell - b.height)*0.5f - b.top);
                win.draw(t);
            }
        }

        // UI
        win.draw(btn);
        win.draw(btnText);

        // credits (top-left)
        sf::Text cr("Credits: " + std::to_string(credits), font, 18);
        cr.setPosition(8.f, 8.f);
        win.draw(cr);

        win.display();
    }
    return 0;
}
