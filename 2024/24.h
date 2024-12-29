#pragma once

#include "helpers.h"

bool verify(int num, std::map<std::string, std::vector<std::string>> &map);
bool verify_z(std::string wire, std::map<std::string, std::vector<std::string>> &map, int num);
bool verify_carry_bit(std::string wire, std::map<std::string, std::vector<std::string>> &map, int num);
bool verify_recarry(std::string wire, std::map<std::string, std::vector<std::string>> &map, int num);
bool verify_direct_carry(std::string wire, std::map<std::string, std::vector<std::string>> &map, int num);
bool verify_intermediate_xor(std::string wire, std::map<std::string, std::vector<std::string>> &map, int num);

