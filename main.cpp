#include <SFML/Graphics.hpp>
#include <array>
#include <random>
#include <algorithm>

constexpr int ROWS = 7;
constexpr int COLS = 6;

// Minimum margins and UI height
constexpr float SIDE_MARGIN = 8.f;
constexpr float TOP_MARGIN  = 8.f;
constexpr float UI_HEIGHT   = 80.f;   // space reserved at bottom for the SPIN button

const std::array<char, 7> SYMBOLS = {'R','G','B','Y','X','T','P'};
const std::array<int, 7>  WEIGHTS = { 20, 20, 20, 20,  5, 10,  5};

char randomSymbol() {
    static std::mt19937 rng{std::random_device{}()};
    int total = 0; for (int w : WEIGHTS) total += w;
    std::uniform_int_distribution<int> d(0, total - 1);
    int r = d(rng);
    for (size_t i = 0; i < WEIGHTS.size(); ++i) {
        if (r < WEIGHTS[i]) return SYMBOLS[i];
        r -= WEIGHTS[i];
    }
    return 'R';
}

sf::Color symbolColor(char s) {
    switch (s) {
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

using Grid = std::array<std::array<char, COLS>, ROWS>;
void generate(Grid& g) { for (auto& r : g) for (auto& c : r) c = randomSymbol(); }

// ---------- layout state ----------
struct Layout {
    float cell;                 // square cell size in pixels
    float gridLeft, gridTop;    // top-left of grid
    sf::FloatRect spinRect;     // button rect
    unsigned textPx;            // character size
};

Layout recompute_layout(const sf::Vector2u& winSz) {
    Layout L{};

    // available area for grid (leave margins and UI space)
    float W = static_cast<float>(winSz.x);
    float H = static_cast<float>(winSz.y);

    float availW = std::max(0.f, W - 2*SIDE_MARGIN);
    float availH = std::max(0.f, H - (TOP_MARGIN + UI_HEIGHT));

    // choose the largest square cell that fits
    float cellByW = availW / COLS;
    float cellByH = availH / ROWS;
    L.cell = std::max(8.f, std::floor(std::min(cellByW, cellByH))); // keep >=8px

    // center the grid horizontally; stick to top margin vertically
    float gridW = L.cell * COLS;
    float gridH = L.cell * ROWS;
    L.gridLeft = std::floor((W - gridW) * 0.5f);
    L.gridTop  = TOP_MARGIN + std::floor((availH - gridH) * 0.5f);

    // button centered at bottom area
    const sf::Vector2f btnSize(120.f, 40.f);
    sf::Vector2f btnPos(std::floor((W - btnSize.x) * 0.5f),
                        std::floor(H - UI_HEIGHT * 0.5f - btnSize.y * 0.5f));
    L.spinRect = {btnPos.x, btnPos.y, btnSize.x, btnSize.y};

    // text size proportional to cell
    L.textPx = static_cast<unsigned>(std::clamp(L.cell * 0.35f, 10.f, 48.f));
    return L;
}

void on_resize(sf::RenderWindow& win, unsigned w, unsigned h) {
    // update the view so SFML doesn’t stretch the scene
    sf::FloatRect visible(0.f, 0.f, static_cast<float>(w), static_cast<float>(h));
    win.setView(sf::View(visible));
}

int main() {
    sf::RenderWindow win(sf::VideoMode(900, 700), "Pirots3 Slot (resizable)");
    win.setVerticalSyncEnabled(true);
    win.setFramerateLimit(120);

    sf::Font font;
    font.loadFromFile("C:/Windows/Fonts/arial.ttf");

    Grid grid; generate(grid);
    Layout layout = recompute_layout(win.getSize());

    sf::RectangleShape btn; // SPIN button visual
    btn.setSize({layout.spinRect.width, layout.spinRect.height});
    btn.setPosition({layout.spinRect.left, layout.spinRect.top});
    btn.setFillColor(sf::Color(50,50,50));

    sf::Text btnText("SPIN", font, layout.textPx);
    btnText.setFillColor(sf::Color::White);

    auto update_button_text_pos = [&](){
        // center text inside button
        btnText.setCharacterSize(static_cast<unsigned>(std::clamp(layout.spinRect.height * 0.5f, 14.f, 32.f)));
        btnText.setPosition(
            layout.spinRect.left + (layout.spinRect.width  - btnText.getLocalBounds().width ) * 0.5f - btnText.getLocalBounds().left,
            layout.spinRect.top  + (layout.spinRect.height - btnText.getLocalBounds().height) * 0.5f - btnText.getLocalBounds().top
        );
    };
    update_button_text_pos();

    while (win.isOpen()) {
        sf::Event e;
        while (win.pollEvent(e)) {
            if (e.type == sf::Event::Closed) win.close();

            if (e.type == sf::Event::Resized) {
                on_resize(win, e.size.width, e.size.height);
                layout = recompute_layout(win.getSize());
                btn.setPosition({layout.spinRect.left, layout.spinRect.top});
                btn.setSize({layout.spinRect.width, layout.spinRect.height});
                update_button_text_pos();
            }

            if (e.type == sf::Event::MouseButtonPressed) {
                sf::Vector2f p((float)e.mouseButton.x, (float)e.mouseButton.y);
                if (layout.spinRect.contains(p)) {
                    generate(grid);
                }
            }
        }

        win.clear(sf::Color::Black);

        // draw grid
        for (int r = 0; r < ROWS; ++r) {
            for (int c = 0; c < COLS; ++c) {
                sf::RectangleShape cell({layout.cell - 4.f, layout.cell - 4.f});
                cell.setPosition(layout.gridLeft + c*layout.cell + 2.f,
                                 layout.gridTop  + r*layout.cell + 2.f);
                cell.setFillColor(symbolColor(grid[r][c]));
                win.draw(cell);

                sf::Text t(std::string(1, grid[r][c]), font,
                           static_cast<unsigned>(std::clamp(layout.cell * 0.35f, 10.f, 48.f)));
                t.setFillColor(sf::Color::White);
                // center symbol inside the cell
                auto bounds = t.getLocalBounds();
                float tx = layout.gridLeft + c*layout.cell + (layout.cell - bounds.width) * 0.5f - bounds.left;
                float ty = layout.gridTop  + r*layout.cell + (layout.cell - bounds.height) * 0.5f - bounds.top;
                t.setPosition(tx, ty);
                win.draw(t);
            }
        }

        // draw button
        win.draw(btn);
        win.draw(btnText);

        win.display();
    }
    return 0;
}
