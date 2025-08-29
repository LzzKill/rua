/*
 * Code: value.cpp
 *
 * @Author LzzKill
 * @License GNU Public License v3.0
 *
 *
 * */

module;
#include <string>
#include <utility>
module moonlisp.runtime.value;

using moonlisp::runtime::Value_p;

bool moonlisp::runtime::Environment::exists(const std::string &name)
{
  if (table.find(name) != table.end())
    return true;
  if (!parent)
    return parent->exists(name);
  return false;
}

Value_p moonlisp::runtime::Environment::get(const std::string &name)
{
  if (this->exists(name))
    return table[name];
  return nullptr;
}

void moonlisp::runtime::Environment::setLocal(const std::string &name,
                                              Value_p v)
{
  table[name] = std::move(v);
}

void moonlisp::runtime::Environment::setGlobal(const std::string &name,
                                               Value_p v)
{
  if (this->parent) {
    parent->setGlobal(name, std::move(v));
  } else {
    setLocal(name, std::move(v));
  }
};