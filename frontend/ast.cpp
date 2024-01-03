#include "lexer.h"



class Expression : public Statement {
  
};

class Program : public Statement {
  public:
    Program() { kind = NodeType::NODE_PROGRAM; }
    virtual ~Program() = default;
    void setKind(NodeType k) { kind = k; }
    NodeType getKind() { return kind; }
    std::vector<Statement*> body;

    friend std::ostream& operator<<(std::ostream& os, Program& e) {
      os << "{ kind: \"" << e.getKindName() << "\", body: \"";
      os << "\", }";
      return os;
    }
};

class VariableDeclaration : public Statement {
  public:
    VariableDeclaration() { kind = NodeType::NODE_VARIABLEDECLARATION; }
    VariableDeclaration(NodeType k, Expression* val, bool c) { 
      kind = NodeType::NODE_VARIABLEDECLARATION; 
      expressionValue = val;
      constant = c; 
    }
    VariableDeclaration(NodeType k, Expression* val, bool c, std::string id) {
      kind = NodeType::NODE_VARIABLEDECLARATION;
      expressionValue = val;
      constant = c;
      identifier = id;
    }
    VariableDeclaration(std::string id, bool c) : constant(c), identifier(id) { 
      kind = NodeType::NODE_VARIABLEDECLARATION; 
      constant = c;
    }
    virtual ~VariableDeclaration() = default;
    void setKind(NodeType k) { kind = k; }
    NodeType getKind() { return kind; }
    bool constant;
    std::string identifier = "";
    Expression* expressionValue = nullptr;

    void toString() {
      std::cout << "{\n";
      std::cout << "  kind: \"" << this->getKindName() << "\",\n";
      std::cout << "  constant: " << this->constant << ",\n";
      std::cout << "  identifier: \"" << this->identifier << "\",\n";  
      std::cout << "}\n";
    }
};

class FunctionDeclaration : public Statement {
  public:
    FunctionDeclaration() { kind = NodeType::NODE_FUNCTIONDECLARATION; }
    FunctionDeclaration(std::vector<std::string> p) {
      kind = NodeType::NODE_FUNCTIONDECLARATION;
      parameters = p;
    }
    FunctionDeclaration(std::vector<std::string> p, std::string n) {
      kind = NodeType::NODE_FUNCTIONDECLARATION;
      parameters = p;
      name = n;
    }
    FunctionDeclaration(std::vector<std::string> p, std::string n, std::vector<Statement*> b) {
      kind = NodeType::NODE_FUNCTIONDECLARATION;
      parameters = p;
      name = n;
      body = b;
    }
    virtual ~FunctionDeclaration() = default;
    std::vector<std::string> parameters = {};
    std::string name = "";
    std::vector<Statement*> body;

    void toString() {
      std::cout << "{\n";
      std::cout << "  kind: \"" << this->getKindName() << "\",\n";
      std::cout << "  parameters: [ ";
      for(auto param : this->parameters) {
        std::cout << "\"" << param << "\", ";
      }
      std::cout << " ],\n";
      std::cout << "  name: \"" << this->name << "\",\n";
      std::cout << "  body: [ ";
      for(auto statement : this->body) {
        if(statement->getKind() == NodeType::NODE_VARIABLEDECLARATION) {
          dynamic_cast<VariableDeclaration*>(statement)->toString();
        }
      }
    }
};

class IfStatement : public Statement {
  public:
    IfStatement() {
      kind = NODE_IFEXPRESSION;
    }
    IfStatement(Expression* t) {
      kind = NODE_IFEXPRESSION;
      test = t;
    }
    IfStatement(Expression* t, std::vector<Statement*> b) {
      kind = NODE_IFEXPRESSION;
      test = t;
      body = b;
    }
    IfStatement(Expression* t, std::vector<Statement*> b, std::vector<Statement*> a) {
      kind = NODE_IFEXPRESSION;
      test = t;
      body = b;
      alternate = a;
    }
    Expression* test = nullptr;
    std::vector<Statement*> body = {};
    std::vector<Statement*> alternate = {};
};

class Iden : public Expression {
  public:
    Iden(std::string val) : value(val) { kind = NodeType::NODE_IDENTIFIER; }
    std::string value = "";

    void toString() {
      std::cout << "{\n";
      std::cout << "  kind: \"" << this->getKindName() << "\",\n";
      std::cout << "  value: \"" << this->value << "\"\n";
      std::cout << "}";
    }
};

class NumericLiteral : public Expression {
  public:
    NumericLiteral(float val) : value(val) { kind = NodeType::NODE_NUMERICLITERAL; }
    float value;

    void toString() {
      std::cout << "{\n";
      std::cout << "  kind: \"" << this->getKindName() << "\",\n";
      std::cout << "  value: \"" << this->value << "\"\n";
      std::cout << "}";
    }
};

class StringLiteral : public Expression {
  public:
    StringLiteral(std::string val) : value(val) { kind = NodeType::NODE_STRINGLITERAL; }
    std::string value;
};

// class NullLiteral : public Expression {
//   public:
//     NullLiteral() { kind = NodeType::NODE_NULLLITERAL; }
//     std::string value = "null";

//     friend std::ostream& operator<<(std::ostream& os, NullLiteral& e) {
//       return os << "{ kind: \"" << e.getKindName() << "\", value: \"" << e.value << "\" }" <<std::endl;
//     }
// };

class AssignmentExpression : public Expression {
  private:
  public:
    AssignmentExpression() {
      kind = NodeType::NODE_ASSIGNMENTEXPRESSION;
    }
    AssignmentExpression(Expression* a, Expression* v) {
      kind = NodeType::NODE_ASSIGNMENTEXPRESSION;
      assigne = a;
      value = v;
    }
    Expression* assigne = nullptr;
    Expression* value = nullptr;

    friend std::ostream& operator<<(std::ostream& os, AssignmentExpression& e) {
      return os << "{ kind: \"" << e.getKindName() << "\", assigne: " << e.assigne << ", value: \"" << e.value << "\" }";
    }
};

class BinaryExpression : public Expression {
  private:
  public:
    BinaryExpression(Expression* l, Expression* r, const std::string& op) : left(l), right(r), oper(op) { kind = NodeType::NODE_BINARYEXPRESSION; }
    Expression* left = nullptr;
    Expression* right = nullptr;
    std::string oper = "";

    void toString() {
      std::cout << "{\n";
      std::cout << "  kind: \"" << this->getKindName() << "\",\n";
      if (dynamic_cast<Iden*>(this->left)) {
        std::cout << "  left:";
        dynamic_cast<Iden*>(this->left)->toString();
      } else if (dynamic_cast<NumericLiteral*>(this->left)) {
        std::cout << "  left:";
        dynamic_cast<NumericLiteral*>(this->left)->toString();
      } 
      // else if (dynamic_cast<MemberExpression*>(this->left)) {
      //   std::cout << "  left:";
      //   dynamic_cast<MemberExpression*>(this->left)->toString();
      // } else if (dynamic_cast<CallExpression*>(this->left)) {
      //   std::cout << "  left:";
      //   dynamic_cast<CallExpression*>(this->left)->toString();
      // }
      
      
      std::cout << "  oper: \"" << this->oper << "\"\n";
      std::cout << "}";
    }
};

class MemberExpression : public Expression {
  private:
  public:
    MemberExpression() {
      kind = NODE_MEMBEREXPRESSION;
    }
    MemberExpression(Expression* o) {
      kind = NODE_MEMBEREXPRESSION;
      object = o;
    }
    MemberExpression(Expression* o, Expression* p) {
      kind = NODE_MEMBEREXPRESSION;
      object = o;
      property = p;
    }
    MemberExpression(Expression* o, Expression* p, bool c) {
      kind = NODE_MEMBEREXPRESSION;
      object = o;
      property = p;
      computed = c;
    }
    Expression* object = nullptr;
    Expression* property = nullptr;
    bool computed;
};

class CallExpression : public Expression {
  private:
  public:
    CallExpression() {
      kind = NODE_CALLEXPRESSION;
    }
    CallExpression(std::vector<Expression*> a) {
      kind = NODE_CALLEXPRESSION;
      args = a;
    }
    CallExpression(std::vector<Expression*> a, Expression* c) {
      kind = NODE_CALLEXPRESSION;
      args = a;
      caller = c;
    }
    std::vector<Expression*> args = {};
    Expression* caller = nullptr;
    std::string oper = "";

    void toString() {
      std::cout << "{\n";
      std::cout << "  kind: \"" << this->getKindName() << "\",\n";
      for (auto a : this->args) {
        if(dynamic_cast<Iden*>(a)) {
          std::cout << "  args:";
          dynamic_cast<Iden*>(a)->toString();
        } else if(dynamic_cast<NumericLiteral*>(a)) {
          std::cout << "  args:";
          dynamic_cast<NumericLiteral*>(a)->toString();
        }
      }
      if (dynamic_cast<Iden*>(this->caller)) {
        std::cout << "  caller:";
        dynamic_cast<Iden*>(this->caller)->toString();
      } else if (dynamic_cast<NumericLiteral*>(this->caller)) {
        std::cout << "  caller:";
        dynamic_cast<NumericLiteral*>(this->caller)->toString();
      }
      std::cout << "  oper: \"" << this->oper << "\"\n";
      std::cout << "}";
    }
};

class Property : public Expression {
  public:
    Property() {
      this->kind = NODE_PROPERTY;
      this->key = "";
      this->value = nullptr;
    }
    Property(NodeType k, std::string key) {
      this->kind = k;
      this->key = key;
      this->value = nullptr;
    }
    Property(NodeType k, std::string key, Expression* val) {
      this->kind = k;
      this->key = key;
      this->value = val;
    }
    std::string key = "";
    Expression* value = nullptr;

    // friend std::ostream& operator<<(std::ostream& os, Property& e) {
    //   std::string k = e.key;
    //   os << "{ kind: \"" << e.getKindName() << "\", value: ";
    //   if(e.value->getKind() == NODE_NUMERICLITERAL) {
    //       os << *dynamic_cast<NumericLiteral*>(e.value) << ", key: " << k << " }," << std::endl;
    //   } else if(e.value->getKind() == NODE_IDENTIFIER) {
    //       os << *dynamic_cast<Iden*>(e.value) << ", key: " << k << " },";
    //   } else if(e.value->getKind() == NODE_ASSIGNMENTEXPRESSION){
    //       os << *dynamic_cast<AssignmentExpression*>(e.value) << ", key: " << k << " },";
    //   } else if(e.value->getKind() == NODE_BINARYEXPRESSION){
    //       os << *dynamic_cast<BinaryExpression*>(e.value) << ", key: " << k << " },";
    //   } else if(e.value->getKind() == NODE_PROGRAM){
    //       os << *dynamic_cast<Program*>(e.value) << ", key: " << k << " },";
    //   } else if(e.value->getKind() == NODE_VARIABLEDECLARATION){
    //       os << dynamic_cast<VariableDeclaration*>(e.value) << ", key: " << k << " },";
    //   } else if(e.value->getKind() == NODE_PROPERTY){
    //       os << e.value << ", key: " << k << " },";
    //   }
    //   return os;
    // }
};

class ObjectLiteral : public Expression {
  public:
    ObjectLiteral() {
      kind = NODE_OBJECTLITERAL;
      properties = {};
    }
    ObjectLiteral(std::vector<Property*> p) {
      kind = NODE_OBJECTLITERAL;
      value = 0.0;
      properties = p;
    }
    std::vector<Property*> properties = {};
};