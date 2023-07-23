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

// File: FileOpenException.hpp
// Purpose: Thrown upon failure to open a file at a specific file path

#ifndef INCLUDE_FILE_OPEN_EXCEPTION_HPP
#define INCLUDE_FILE_OPEN_EXCEPTION_HPP

#include <exception>
#include <stdexcept>

namespace Curvy {
class FileOpenException final : public std::exception {
public:
  explicit FileOpenException(const std::string &) noexcept;
  const char *what() const noexcept override;

private:
  const std::string m_message;
};
} // namespace Curvy

#endif