#pragma once
#include <set>
#include "any.hpp"
#include "function.hpp"
#include <functional>

class __vertex_base {
public:
  virtual ~__vertex_base() {};
  __vertex_base() = default;
  __vertex_base(int i) : _id(i) {}
  
  virtual void execute() {};

  enum Type {BASE, VERTEX1, VERTEX2}; 
  virtual Type type() const { return BASE; }

  size_t id() const {
    return _id;
  }

  void SetId(size_t id) {
    _id = id;
  }

  bool operator==(const __vertex_base& other) const {
    return _id == other._id;
  }

  void insert(__vertex_base* neighbor) {
    adj.insert({neighbor, NULL});
  }

  void SetResult(any result, __vertex_base* vert) {
    auto it = find(vert);
    it.second = result;
  }

  void erase(__vertex_base* neighbor) {
    auto it = adj.begin();
    while (it != adj.end()) {
        if (it->first == neighbor) {
            adj.erase(it);
            break;
        }
        ++it;
    }
  }

  std::pair<__vertex_base*, any> find(__vertex_base* neighbor) {
    for (auto it = adj.begin(); it != adj.end(); ++it) {
      if (it->first == neighbor) {
        return *it;
      }
    }

    return std::make_pair(nullptr, NULL);
  }

  void InsertArg(any arg) {
    arguments.push_back({arg, nullptr});
  }

  void InsertArg(__vertex_base* arg) {
    arguments.push_back({NULL, arg});
  }
  
  struct CompareIdPtr {
    bool operator()(const std::pair<__vertex_base*, any>& lhs, const std::pair<__vertex_base*, any>& rhs) const {
      return lhs.first->id() < rhs.first->id();
    }
  };

  std::set<std::pair<__vertex_base*, any>, CompareIdPtr> adj;
  size_t _id;
  std::vector<std::pair<any, __vertex_base*>> arguments;
  any res;
};

class __vertex1 : public __vertex_base {
public:
  __vertex1(int i) : __vertex_base(i) {}

  Type type() const override { return VERTEX1; }

  template<class Func>
  void SetFunc(Func func) {
    this->func = func;
  }

  function<any(any)> Func() {
    return func;
  }

  any call(std::vector<std::pair<any, __vertex_base*>> args) {
    if (args[0].second == nullptr) {
      return func(args[0].first);
    } else if (args[0].second != nullptr) {
      return func(args[0].second);
    }
    return any();
  }

  void execute() override {
    bool start = true;
    for (auto it : arguments) {
      if (it.second != nullptr) {
        start = false;
      }
    }

    if (start) {  
      any result = call(arguments);
      res = result;
    } else {
      std::vector<any> res_args;
      for (auto it : arguments) {
        if (it.second != nullptr) {
          res_args.push_back(it.second->res);
        } else {
          res_args.push_back(it.first);
        }
      }
      any result = func(res_args[0]);
      res = result;
    }
  }

  function<any(any)> func;
};

class __vertex2 : public __vertex_base {
public:
  __vertex2(int i) : __vertex_base(i) {}

  Type type() const override { return VERTEX2; }

  template<class Func>
  void SetFunc(Func func) {
    this->func = func;
  }

  function<any(any, any)> Func() {
    return func;
  }

  any call(std::vector<std::pair<any, __vertex_base*>> args) {
    if (args[0].second == nullptr && args[1].second == nullptr) {
      return func(args[0].first, args[1].first);
    } else if (args[0].second != nullptr && args[1].second == nullptr) {
      return func(args[0].second, args[1].first);
    } else if (args[0].second == nullptr && args[1].second != nullptr) {
      return func(args[0].first, args[1].second);
    } else if (args[0].second != nullptr && args[1].second != nullptr) {
      return func(args[0].second, args[1].second);
    }
    return any();
  }

  void execute() override {
    bool start = true;
    for (auto it : arguments) {
      if (it.second != nullptr) {
        start = false;
      }
    }

    if (start) {  
      any result = call(arguments);
      res = result;
    } else {
      std::vector<any> res_args;
      for (auto it : arguments) {
        if (it.second != nullptr) {
          res_args.push_back(it.second->res);
        } else {
          res_args.push_back(it.first);
        }
      }
      any result = func(res_args[0], res_args[1]);
      res = result;
    }
  }

  function<any(any, any)> func;
};

