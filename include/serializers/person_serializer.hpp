/*
Boost.JSON

Performance: Boost.JSON is designed with performance in mind. It aims to be fast and efficient, making it suitable for high-performance applications.
Memory Efficiency: It provides good memory efficiency, which can be crucial for applications with limited resources.
Integration: Being part of the Boost libraries, it integrates well with other Boost components and follows Boost’s conventions and practices.
Features: It offers a comprehensive set of features for parsing, serializing, and manipulating JSON data.

nlohmann::json

Ease of Use: nlohmann::json is known for its intuitive and user-friendly syntax. It uses modern C++ features to provide a natural and easy-to-use interface.
Single Header: The entire library is contained in a single header file (json.hpp), making it easy to integrate into projects without additional dependencies.
Feature-Rich: It supports a wide range of features, including JSON Pointer, JSON Patch, and various binary formats like CBOR and MessagePack.
Community and Documentation: nlohmann::json has extensive documentation and a large community, which can be helpful for troubleshooting and learning.
*/

#pragma once

#include "../models/person.hpp"

#include <boost/asio.hpp>
/*#include <boost/json.hpp>

#include "nlohmann/json.hpp"

class PersonSerializer {
public:
  static boost::json::object toJson(const Person &person);
  static Person fromJson(const boost::json::object &obj);
};
*/