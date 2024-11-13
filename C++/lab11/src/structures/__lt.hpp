#pragma once
#include <memory>
#include <string>
#include <iostream>
#include <stack>
#include <set>

enum class NodeType {
  Identifier,
  And,
  Or
};

class __lt {
private:
  struct Node {
    NodeType type;
    std::string value;
    std::shared_ptr<Node> left; 
    std::shared_ptr<Node> right; 
    double score;
    Node(const NodeType type, const std::string& value = "") : type(type), value(value) {}
  };

  std::shared_ptr<Node> createNode(NodeType type, const std::string& value = "") {
    auto node = std::make_shared<Node>(type, value);
    return node;
  }

  std::shared_ptr<Node> _root;
  std::set<std::string> _postorder_view;
  size_t curr_did;
public:
  std::shared_ptr<Node> parseExpression(const std::string& input, size_t& pos) {
    auto left = parseTerm(input, pos);
    skipWhitespace(input, pos);

    if (pos < input.size() && input[pos] == 'O') {
        std::string token = parseToken(input, pos);
        if (token == "OR") {  
            auto right = parseExpression(input, pos);
            auto node = createNode(NodeType::Or);
            node->left = left;
            node->right = right;
            return node;
        }
    }

    return left;
}

std::shared_ptr<Node> parseTerm(const std::string& input, size_t& pos) {
    auto left = parseFactor(input, pos);
    skipWhitespace(input, pos);

    if (pos < input.size() && input[pos] == 'A') {
        std::string token = parseToken(input, pos);
        if (token == "AND") {
            auto right = parseTerm(input, pos);
            auto node = createNode(NodeType::And);
            node->left = left;
            node->right = right;
            return node;
        }
    }

    return left;
}

  std::shared_ptr<Node> parseFactor(const std::string& input, size_t& pos) {
    skipWhitespace(input, pos);
    if (input[pos] == '(') {
        pos++; 
        auto node = parseExpression(input, pos);
        skipWhitespace(input, pos);
        if (input[pos] == ')') {
            pos++;
            return node;
        }
        throw std::runtime_error("Expected ')'");
    } else {
        std::string token = parseToken(input, pos);
        return createNode(NodeType::Identifier, token);
    }
  }

  std::string parseToken(const std::string& input, size_t& pos) {
    skipWhitespace(input, pos);
    std::string token;
    while (pos < input.size() && input[pos] != ' ' && input[pos] != '(' && input[pos] != ')') {
        token += input[pos++];
    }
    return token;
  }

  void skipWhitespace(const std::string& input, size_t& pos) {
    while (pos < input.size() && input[pos] == ' ') {
        pos++;
    }
  }

  void createAST(const std::string& input) {
    size_t pos = 0;
    _root = parseExpression(input, pos);
  }

  void TermListUtil(const std::shared_ptr<Node>& root) {
    if (root) {
        
        TermListUtil(root->left);
        TermListUtil(root->right);
        if (root->type == NodeType::Identifier) {
          _postorder_view.insert(root->value);
        }
    }
  }
  
  std::vector<std::string> TermList() {
    TermListUtil(_root);
    std::vector<std::string> ret = {_postorder_view.begin(), _postorder_view.end()};
    return ret;
  }

  auto root() {
    return _root;
  }

  void refreshScores(const std::shared_ptr<Node>& root) {
    if (root) {
      refreshScores(root->left);
      refreshScores(root->right);
      root->score = 0;
    }
  }

  void setScores(const std::shared_ptr<Node>& root, std::unordered_map<std::string, double> scores) {
    if (root) {
      setScores(root->left, scores);
      setScores(root->right, scores);
      if (root->type == NodeType::Identifier) {
        root->score = scores[root->value];
      }
    }
  }

  void getResultScoreUtil(const std::shared_ptr<Node>& root) {
    if (root) {
      getResultScoreUtil(root->left);
      getResultScoreUtil(root->right);
      if (root->type == NodeType::Or) {
        root->score = root->left->score + root->right->score;
      }
      if (root->type == NodeType::And) {
        root->score = root->left->score * root->right->score;
      }
    }
  }

  double getResultScore() {
    getResultScoreUtil(_root);
    return _root->score;
  }

  void setCurrDid(size_t did) { curr_did = did; }
  size_t currDid() {return curr_did; } 

  void clear() {
    _root.reset();
    _postorder_view.clear();
    curr_did = 0;
  }
};