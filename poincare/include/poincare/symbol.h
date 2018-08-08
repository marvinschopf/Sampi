#ifndef POINCARE_SYMBOL_H
#define POINCARE_SYMBOL_H

#include <poincare/expression.h>

namespace Poincare {

class SymbolNode : public ExpressionNode {
  friend class Store;
public:
  static SymbolNode * FailedAllocationStaticNode();

  void setName(const char name) { m_name = name; }
  char name() const { return m_name; }

  // TreeNode
  size_t size() const override { return sizeof(SymbolNode); }
#if TREE_LOG
  const char * description() const override { return "Symbol";  }
#endif
  int numberOfChildren() const override { return 0; }

  // Expression Properties
  Type type() const override { return Type::Symbol; }
  Sign sign() const override;
  Expression replaceSymbolWithExpression(char symbol, Expression expression) override;
  int polynomialDegree(char symbolName) const override;
  int getPolynomialCoefficients(char symbolName, Expression coefficients[]) const override;
  int getVariables(isVariableTest isVariable, char * variables) const override;
  float characteristicXRange(Context & context, Preferences::AngleUnit angleUnit) const override;

  /* Comparison */
  int simplificationOrderSameType(const ExpressionNode * e, bool canBeInterrupted) const override;

/* Layout */
  LayoutRef createLayout(Preferences::PrintFloatMode floatDisplayMode, int numberOfSignificantDigits) const override;
  int serialize(char * buffer, int bufferSize, Preferences::PrintFloatMode floatDisplayMode, int numberOfSignificantDigits) const override;

  /* Simplification */
  Expression shallowReduce(Context& context, Preferences::AngleUnit angleUnit) override;

  /* Approximation */
  Evaluation<float> approximate(SinglePrecision p, Context& context, Preferences::AngleUnit angleUnit) const override { return templatedApproximate<float>(context, angleUnit); }
  Evaluation<double> approximate(DoublePrecision p, Context& context, Preferences::AngleUnit angleUnit) const override { return templatedApproximate<double>(context, angleUnit); }

private:
  bool hasAnExactRepresentation(Context & context) const;
  template<typename T> Evaluation<T> templatedApproximate(Context& context, Preferences::AngleUnit angleUnit) const;

  char m_name;
};

class SymbolReference : public Expression {
public:
  enum SpecialSymbols : char {
    /* We can use characters from 1 to 31 as they do not correspond to usual
     * characters but events as 'end of text', 'backspace'... */
    Ans = 1,
    un = 2,
    un1 = 3,
    un2 = 4,
    vn = 5,
    vn1 = 6,
    vn2 = 7,
    M0 = 8,
    M1 = 9,
    M2,
    M3,
    M4,
    M5,
    M6,
    M7,
    M8,
    M9 = 17,
    V1,
    N1,
    V2,
    N2,
    V3,
    N3 = 23,
    X1 = 24,
    Y1,
    X2,
    Y2,
    X3,
    Y3 = 29
  };
  SymbolReference(const char name) : Expression(TreePool::sharedPool()->createTreeNode<SymbolNode>()) {
    if (!node->isAllocationFailure()) {
      static_cast<SymbolNode *>(node)->setName(name);
    }
  }

  // Symbol properties
  static const char * textForSpecialSymbols(char name);
  static SpecialSymbols matrixSymbol(char index);
  static bool isMatrixSymbol(char c);
  static bool isScalarSymbol(char c);
  static bool isVariableSymbol(char c);
  static bool isSeriesSymbol(char c);
  static bool isRegressionSymbol(char c);
  static bool isApproximate(char c, Context & context);

  // Expression
  Expression shallowReduce(Context& context, Preferences::AngleUnit angleUnit) const;
  Expression replaceSymbolWithExpression(char symbol, Expression expression);
  int getPolynomialCoefficients(char symbolName, Expression coefficients[]) const;
private:
  SymbolNode * node() { return static_cast<SymbolNode *>(Expression::node()); }
  char name() { return node()->name(); }
};

}

#endif
