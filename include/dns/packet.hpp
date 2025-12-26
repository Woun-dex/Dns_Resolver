#pragma once
#include "header.hpp"
#include "question.hpp"
#include "record.hpp"
#include <vector>

namespace dns {

struct Packet {
    Header header;                       // DNS packet header
    std::vector<Question> questions;     // List of questions
    std::vector<Record> answers;         // List of answer records
    std::vector<Record> authorities;     // List of authority records
    std::vector<Record> additionals;     // List of additional records
};

}