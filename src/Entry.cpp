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
// Purpose: Definition of main

#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>

#include "Entry.hpp"
#include "Serializer.hpp"

#include "cxxopts.hpp"
#include "json.hpp"

struct CornerData final {
  float radius;
  sf::Color color;
  std::size_t vertices;
};

struct InputData final {
  std::string output;
  float width;
  float height;
  sf::Color color;
  std::map<std::string, CornerData> corners;
};

void printInputData(const InputData &inputData) {
  std::cout << inputData.output << std::endl;
  std::cout << inputData.width << std::endl;
  std::cout << inputData.height << std::endl;
  std::cout << static_cast<unsigned int>(inputData.color.r) << "/"
            << static_cast<unsigned int>(inputData.color.g) << "/"
            << static_cast<unsigned int>(inputData.color.b) << "/"
            << static_cast<unsigned int>(inputData.color.a) << std::endl;
  for (const auto &corner : inputData.corners) {
    std::cout << corner.first << std::endl;
    std::cout << corner.second.radius << std::endl;
    std::cout << static_cast<unsigned int>(corner.second.color.r) << "/"
              << static_cast<unsigned int>(corner.second.color.g) << "/"
              << static_cast<unsigned int>(corner.second.color.b) << "/"
              << static_cast<unsigned int>(corner.second.color.a) << std::endl;
    std::cout << corner.second.vertices << std::endl;
  }
}

using Json = nlohmann::json;

void from_json(const Json &json, InputData &inputData) {
  inputData.output = json.at("Output");
  inputData.width = json.at("Width");
  inputData.height = json.at("Height");
  inputData.color.r = json.at("Color").at("R");
  inputData.color.g = json.at("Color").at("G");
  inputData.color.b = json.at("Color").at("B");
  inputData.color.a = json.at("Color").at("A");

  auto &topLeft = inputData.corners["TopLeft"];
  topLeft.radius = json.at("Corners").at("TopLeft").at("Radius");
  topLeft.color.r = json.at("Corners").at("TopLeft").at("Color").at("R");
  topLeft.color.g = json.at("Corners").at("TopLeft").at("Color").at("G");
  topLeft.color.b = json.at("Corners").at("TopLeft").at("Color").at("B");
  topLeft.color.a = json.at("Corners").at("TopLeft").at("Color").at("A");
  topLeft.vertices = json.at("Corners").at("TopLeft").at("Vertices");

  auto &bottomLeft = inputData.corners["BottomLeft"];
  bottomLeft.radius = json.at("Corners").at("BottomLeft").at("Radius");
  bottomLeft.color.r = json.at("Corners").at("BottomLeft").at("Color").at("R");
  bottomLeft.color.g = json.at("Corners").at("BottomLeft").at("Color").at("G");
  bottomLeft.color.b = json.at("Corners").at("BottomLeft").at("Color").at("B");
  bottomLeft.color.a = json.at("Corners").at("BottomLeft").at("Color").at("A");
  bottomLeft.vertices = json.at("Corners").at("BottomLeft").at("Vertices");

  auto &bottomRight = inputData.corners["BottomRight"];
  bottomRight.radius = json.at("Corners").at("BottomRight").at("Radius");
  bottomRight.color.r =
      json.at("Corners").at("BottomRight").at("Color").at("R");
  bottomRight.color.g =
      json.at("Corners").at("BottomRight").at("Color").at("G");
  bottomRight.color.b =
      json.at("Corners").at("BottomRight").at("Color").at("B");
  bottomRight.color.a =
      json.at("Corners").at("BottomRight").at("Color").at("A");
  bottomRight.vertices = json.at("Corners").at("BottomRight").at("Vertices");

  auto &topRight = inputData.corners["TopRight"];
  topRight.radius = json.at("Corners").at("TopRight").at("Radius");
  topRight.color.r = json.at("Corners").at("TopRight").at("Color").at("R");
  topRight.color.g = json.at("Corners").at("TopRight").at("Color").at("G");
  topRight.color.b = json.at("Corners").at("TopRight").at("Color").at("B");
  topRight.color.a = json.at("Corners").at("TopRight").at("Color").at("A");
  topRight.vertices = json.at("Corners").at("TopRight").at("Vertices");
}

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

namespace Input {
auto codes() { return "i,input"; }

auto code() { return "i"; }

auto info() { return "specify input json file path"; }

auto value() {
  return cxxopts::value<std::string>()->default_value("input.json");
}
} // namespace Input

} // namespace Option

int main(const int argc, const char *const *const argv) {

  try {
    using OptionParser = cxxopts::Options;

    using namespace Option;
    OptionParser optionParser(Program::name(), Program::info());

    optionParser.add_options()(Help::codes(), Help::info(), Help::value())(
        Input::codes(), Input::info(), Input::value());

    auto options = optionParser.parse(argc, argv);

    if (options[Help::code()].as<bool>()) {
      std::cout << optionParser.help() << std::endl;
      return 0;
    }

    Curvy::Serializer reader{options[Input::code()].as<std::string>()};
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
