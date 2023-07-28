// MIT License
//
// Copyright (c) 2023 Oliver D
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

// File: Entry.cpp
// Purpose: Definition of main and includes

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>

#include "Entry.hpp"
#include "Serializer.hpp"

#include "cxxopts.hpp"
#include "json.hpp"

using Json = nlohmann::json;

struct Corner final {
  float radius = {};
  sf::Color color;
  std::string useTexture;
  std::size_t vertices;
};

void from_json(const Json &json, Corner &corner) {}

struct Input final {
  std::filesystem::path outputFile;
  sf::Vector2f size;
  std::filesystem::path useTexture;
  sf::Color color;
  std::map<std::string, Corner> corners;
};

void from_json(const Json &json, Input &input) {}

namespace Option {

namespace Program {
auto name() { return "curve-my-corners"; }

auto info() { return "Generates colored rectangles with curved corners"; }
} // namespace Program

namespace Help {
auto codes() { return "h,help"; }

auto code() { return "h"; }

auto info() { return "prints this message"; }

auto value() { return cxxopts::value<bool>()->default_value("false"); }
} // namespace Help

namespace Config {
auto codes() { return "c,config"; }

auto code() { return "c"; }

auto info() { return "specify configuration json file path"; }

auto value() {
  return cxxopts::value<std::string>()->default_value("input.json");
}
} // namespace Config

} // namespace Option

int main(const int argc, const char *const *const argv) {

  try {
    using OptionParser = cxxopts::Options;

    using namespace Option;
    OptionParser optionParser(Program::name(), Program::info());

    optionParser.add_options()(Help::codes(), Help::info(), Help::value())(
        Config::codes(), Config::info(), Config::value());

    auto options = optionParser.parse(argc, argv);

    if (options[Help::code()].as<bool>()) {
      std::cout << optionParser.help() << std::endl;
      return 0;
    }

    Curvy::Serializer reader{options[Config::code()].as<std::string>()};
    reader.execute();
    const InputData inputData =
        Json::parse(reader.getContents()).get<InputData>();

    sf::RenderTexture renderTexture;
    renderTexture.create(static_cast<unsigned int>(inputData.width),
                         static_cast<unsigned int>(inputData.height));

    // TOP LEFT CURVE
    const CornerData &topLeftData = inputData.corners.at("TopLeft");
    sf::VertexArray topLeftTriangle(sf::PrimitiveType::Triangles, 3);

    topLeftTriangle[0].position = {topLeftData.radius, 0.0F};
    topLeftTriangle[0].color = topLeftData.color;

    topLeftTriangle[1].position = {0.0F, 0.0F};
    topLeftTriangle[1].color = topLeftData.color;

    topLeftTriangle[2].position = {0.0F, topLeftData.radius};
    topLeftTriangle[2].color = topLeftData.color;

    sf::CircleShape topLeftCurve(topLeftData.radius, topLeftData.vertices);

    topLeftCurve.setFillColor(sf::Color::Blue);
    topLeftCurve.move(0.0F, 0.0F);

    // BOTTOM LEFT CURVE
    const CornerData &bottomLeftData = inputData.corners.at("BottomLeft");
    sf::VertexArray bottomLeftTriangle(sf::PrimitiveType::Triangles, 3);

    bottomLeftTriangle[0].position = {0.0F,
                                      inputData.height - bottomLeftData.radius};
    bottomLeftTriangle[0].color = bottomLeftData.color;

    bottomLeftTriangle[1].position = {0.0F, inputData.height};
    bottomLeftTriangle[1].color = bottomLeftData.color;

    bottomLeftTriangle[2].position = {bottomLeftData.radius, inputData.height};
    bottomLeftTriangle[2].color = bottomLeftData.color;

    sf::CircleShape bottomLeftCurve(bottomLeftData.radius,
                                    bottomLeftData.vertices);

    bottomLeftCurve.setFillColor(sf::Color::Blue);
    bottomLeftCurve.move(0.0F, inputData.height -
                                   bottomLeftCurve.getGlobalBounds().height);

    // BOTTOM RIGHT CURVE
    const CornerData &bottomRightData = inputData.corners.at("BottomRight");
    sf::VertexArray bottomRightTriangle(sf::PrimitiveType::Triangles, 3);

    bottomRightTriangle[0].position = {inputData.width - bottomRightData.radius,
                                       inputData.height};
    bottomRightTriangle[0].color = bottomRightData.color;

    bottomRightTriangle[1].position = {inputData.width, inputData.height};
    bottomRightTriangle[1].color = bottomRightData.color;

    bottomRightTriangle[2].position = {
        inputData.width, inputData.height - bottomRightData.radius};
    bottomRightTriangle[2].color = bottomRightData.color;

    sf::CircleShape bottomRightCurve(bottomRightData.radius,
                                     bottomRightData.vertices);

    bottomRightCurve.setFillColor(sf::Color::Blue);
    bottomRightCurve.move(
        inputData.width - bottomRightCurve.getGlobalBounds().width,
        inputData.height - bottomRightCurve.getGlobalBounds().height);

    // TOP RIGHT CURVE
    const CornerData &topRightData = inputData.corners.at("TopRight");
    sf::VertexArray topRightTriangle(sf::PrimitiveType::Triangles, 3);

    topRightTriangle[0].position = {inputData.width, topRightData.radius};
    topRightTriangle[0].color = topRightData.color;

    topRightTriangle[1].position = {inputData.width, 0.0F};
    topRightTriangle[1].color = topRightData.color;

    topRightTriangle[2].position = {inputData.width - topRightData.radius,
                                    0.0F};
    topRightTriangle[2].color = topRightData.color;

    sf::CircleShape topRightCurve(topRightData.radius, topRightData.vertices);

    topRightCurve.setFillColor(sf::Color::Blue);
    topRightCurve.move(inputData.width - topRightCurve.getGlobalBounds().width,
                       0.0F);

    // Draw the shapes
    renderTexture.clear(sf::Color::Transparent);

    renderTexture.draw(topLeftTriangle);
    renderTexture.draw(topLeftCurve);

    renderTexture.draw(bottomLeftTriangle);
    renderTexture.draw(bottomLeftCurve);

    renderTexture.draw(bottomRightTriangle);
    renderTexture.draw(bottomRightCurve);

    renderTexture.draw(topRightTriangle);
    renderTexture.draw(topRightCurve);

    // Save the image
    const sf::Texture &underlyingTexture = renderTexture.getTexture();
    sf::Image generatedImage = underlyingTexture.copyToImage();
    generatedImage.createMaskFromColor(sf::Color::Blue);

    if (!generatedImage.saveToFile(inputData.output)) {
      std::cout << "Failed to generate image" << std::endl;
    }

  } catch (const std::exception &e) {
    std::cout << e.what() << std::endl;
  } catch (...) {
    std::cout << "Non-std exception caught in main" << std::endl;
  }
}

// End of file