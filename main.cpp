#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <memory>
enum class TokenType
{
   Number,
   Plus,
   End,
};
struct Token {
   TokenType type;
   std::string text;
};
class Expr {
   public:
   virtual ~Expr() {}
   virtual int evaluate() const= 0;
};
class NumberExpr : public Expr {
   public:int value;
   int evaluate() const override {
      return value;
   };
};
class BinaryExpr : public Expr {
public:std::unique_ptr<Expr> left;
   TokenType op;
   std::unique_ptr<Expr> right;
   int evaluate() const override {
      if (op == TokenType::Plus) {
         return left->evaluate() + right->evaluate();
      }
   }

};
std::vector<Token> tokenize(const std::string& source) {
   std::vector<Token> tokens;
   int pos = 0;
   while (pos<source.size()) {
      char ch = source[pos];
      if (ch >= '0' && ch <= '9') {
         std::string text="";
         while (pos<source.size()&&source[pos]>='0'&&source[pos]<='9') {
            ch=source[pos++];
            text+=ch;
         }
         Token numberToken;
         numberToken.type = TokenType::Number;
         numberToken.text = text;
         tokens.push_back(numberToken);
         //数字分支
      }
      else if (ch == '+' ) {
         Token plusToken;
         plusToken.type = TokenType::Plus;
         plusToken.text = ch;
         tokens.push_back(plusToken);
         pos++;
      }
      else if (ch == ' ') {
         pos++;//忽略空格
      }
      else {
         throw std::runtime_error("Invalid token");
      }

   }
   Token EndToken;
   EndToken.type = TokenType::End;
   EndToken.text = "";
   tokens.push_back(EndToken);
   return tokens;
}
std::unique_ptr<Expr>  parseExpression(const std::vector<Token>& tokens) {
   std::size_t current =0;
      if (tokens[current].type != TokenType::Number) {
         throw std::runtime_error("表达式必须以数字开头");
      }
         auto firstNumber =std::make_unique<NumberExpr>();
         firstNumber->value =std::stoi(tokens[current].text);
   std::unique_ptr<Expr>expr=std::move(firstNumber);
   current++;
         while (tokens[current].type == TokenType::Plus) {
            TokenType opration = tokens[current].type;
            current++;
            if (tokens[current].type != TokenType::Number) {
               throw std::runtime_error("加号后面必须是数字");
            }
            auto rightNumber =std::make_unique<NumberExpr>();
            rightNumber->value =std::stoi(tokens[current].text);
            current++;
            auto binary =std::make_unique<BinaryExpr>();
            binary->left = std::move(expr);
            binary->op = opration;
            binary->right = std::move(rightNumber);

            expr = std::move(binary);
         }
   if (tokens[current].type == TokenType::End) {
      std::cout<<"通过"<<std::endl;
   }
   else {
      throw std::runtime_error("表达式中有多余的Token");
   }
   return expr;

}
int main() {
   std::string source="";
   std::cout<<"请输入表达式"<<std::endl;
   std::getline(std::cin,source);
   std::vector<Token> result ;
   try {
      result =tokenize(source);
      auto ast = parseExpression(result);
      std::cout << ast->evaluate() << std::endl;
   }catch (const std::runtime_error& error) {
      std::cerr << error.what() << std::endl;
   }
}








