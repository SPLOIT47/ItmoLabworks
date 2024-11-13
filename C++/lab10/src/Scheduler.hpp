#pragma once
#include <vector>
#include <set>
#include <map>
#include "__vertex.hpp"


class Scheduler {
public:

  Scheduler() = default;

  ~Scheduler() = default;

  template<typename T>
  __vertex_base* getFutureResult(__vertex_base& vertex) {
    for (auto& it : vertex.adj) {
    }
    for (auto it : adj) {
      if (it->id() == vertex.id()) {
        return it;
      }
    }

    if (vertex.id() || vertex.id() < 1) {
      adj[vertex.id() - 1] = &vertex;
    } else {  
      vertex.SetId(adj.size() + 1);
      adj.push_back(&vertex);
    }

    return &vertex;
  }

  template<typename... Args>
  std::enable_if_t<sizeof...(Args) == 1, __vertex1*> creator(int i) {
    __vertex1* vert = new __vertex1(i);
    adj.push_back(vert);
    return vert;
  }

  template<typename... Args>
  std::enable_if_t<sizeof...(Args) == 2, __vertex2*> creator(int i) {
    __vertex2* vert = new __vertex2(i);
    adj.push_back(vert);
    return vert;
  }
  
  template<typename... Args>
  void parseArguments(__vertex_base* new_vert, Args... args) {
    std::vector<any> arguments = {args...};
    for (any it : arguments) {
      if (it.type() == typeid(__vertex_base*)) {
        __vertex_base* vert = any_cast<__vertex_base*>(it);
        if (adj[vert->id()] != nullptr) {
          adj[vert->id()]->insert(new_vert);
        }
        new_vert->InsertArg(vert);
      } else {
        new_vert->InsertArg(it);
      }
    }
  }

  template<class Func, typename... Args>
  __vertex_base add(Func func, Args... args) { 
    auto vert = creator<Args...>(adj.size());
    parseArguments(vert, args...);
    vert->SetFunc(func);
    return *vert;
  } 

  void dfs(size_t v, std::vector<bool>& visited, std::vector<size_t>& ans) {
    visited[v] = true;
    
    for (auto it : adj[v]->adj) {
      if (!visited[it.first->id()]) {
        dfs(it.first->id(), visited, ans);
      }
    }
    
    ans.push_back(v);
  }

  void executeAll() {
    std::vector<bool> visited(adj.size(), false);
    std::vector<size_t> ans;
    for (auto it : adj) {
      if (!visited[it->id()]) {
        dfs(it->id(), visited, ans);
      }
    }
    std::reverse(ans.begin(), ans.end());

    for (auto it : ans) {
      adj[it]->execute();
    }
  }

  template<typename T>
  T getResult(__vertex_base id) {
    for (auto it : adj) {
      if (id.id() == it->id()) {
        return any_cast<T>(it->res);
      }
    }
    return any_cast<T>(any());
  }
  std::vector<__vertex_base*> adj;
private:
  
};