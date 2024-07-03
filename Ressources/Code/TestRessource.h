#pragma once

#include "Define.h"
#include "Ressources/IRessource.h"

class TestRessource : public Ressources::IRessource
{
public:
    TestRessource(const int64_t p_UUID) : IRessource(p_UUID) {}
    ~TestRessource() {}

    REGISTER_RESSOURCE(TestRessource)

private:
};
class TestRessource2 : public Ressources::IRessource
{
public:
    TestRessource2(const int64_t p_UUID) : IRessource(p_UUID) {}
    ~TestRessource2() {}

    REGISTER_RESSOURCE(TestRessource2)

private:
};