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

// File: Serializer.cpp
// Purpose: Self-contained file reader class definitions

#include "Serializer.hpp"
#include "FileOpenException.hpp"
#include "FileReadException.hpp"

#include <fstream>

namespace Curvy {
Serializer::Serializer(const ReadPath &readPath) noexcept
    : m_readPath(readPath), m_contents() {}

void Serializer::setReadPath(const ReadPath &readPath) noexcept {
  m_readPath = readPath;
  if (!m_contents.empty()) {
    m_contents.clear();
  }
}

const ReadPath &Serializer::getReadPath() const noexcept { return m_readPath; }

void Serializer::execute() {
  std::ifstream in(m_readPath, std::ios_base::in | std::ios_base::binary);
  if (!in) {
    throw FileOpenException("Could not open " + m_readPath.string());
  }

  m_contents = {std::istreambuf_iterator<char>{in}, {}};
  if (!in) {
    throw FileReadException("Could not read " + m_readPath.string());
  }
}

const std::string &Serializer::getContents() const noexcept {
  return m_contents;
}

} // namespace Curvy

// End of file