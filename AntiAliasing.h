#pragma once

#include "render.h"

struct UniformSSAAx4 : public PostRenderer {
public:
    explicit UniformSSAAx4(Renderer  * renderer) : PostRenderer(renderer) {
        output.reserve(renderer->height * renderer->width / 4);
    };

private:
    static Color Reduce(Color c1, Color c2, Color c3, Color c4) {
        return (c1 + c2 + c3 + c4) / 4;
    }

public:
    void Apply() override {
        const auto& img = renderer->GetBuffer();
        const size_t size = img.size();

        for (
                auto row = img.begin(), row1 = img.begin() + renderer->width ;
                row < img.end();
                row += renderer->width + renderer->width, row1 += renderer->width + renderer->width
                )
        {
            for ( auto i = row, i1 = row1; i < row1; i += 2, i1 += 2 ) {
                output.push_back(
                        Reduce( *i, *(i+1), *i1, *(i1+1) )
                );
            }
        }
    }

    size_t GetWidth() override {
        return renderer->width / 2;
    }

    size_t GetHeight() override {
        return renderer->height / 2;
    }
};


struct AverageFilter : public PostRenderer {
private:
    int window;

public:
    explicit AverageFilter(Renderer  * renderer, int window) : PostRenderer(renderer), window(window) {
        if (window%2 == 0) throw std::runtime_error("AverageFilter: cannot use filter with an even window size (no central element), sorry");
        output.reserve(renderer->height * renderer->width );
    };

private:
    static Color Reduce(const std::vector<Color>& colors) {
        Color total;
        for (const Color& c : colors ) {
            total += c;
        }
        return total / colors.size();
    }

public:
    void Apply() override {
        const auto& img = renderer->GetBuffer();
        const size_t size = img.size();

        const auto w = renderer->width;
        const auto h = renderer->height;
        const int halfwin = window / 2;

        size_t rowStart = 0;

        std::vector<std::vector<Color>::const_iterator> lefts;
        for (int i=0; i<window; i++) {
            lefts.push_back(img.begin() + w*i);
        }

        for (size_t i = halfwin; i < h - halfwin; i++ ) {
            for (size_t j = halfwin; j < w - halfwin; j++) {
                std::vector<Color> colors;
                colors.reserve(window*window);
                for (auto& left : lefts) {
                    for (auto slider=left; slider < left+window; slider++ ) {
                        colors.push_back(*slider);
                    }
                    left++;
                }
                output.push_back(Reduce(colors));
            }
            for (auto& left : lefts) {
                left += window - 1;
            }
        }
    }

    size_t GetWidth() override {
        return renderer->width - window + 1;
    }

    size_t GetHeight() override {
        return renderer->height - window + 1;
    }
};
