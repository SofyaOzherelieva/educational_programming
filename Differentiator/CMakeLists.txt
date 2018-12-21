cmake_minimum_required(VERSION 3.9)
project(Differentiator)

set(CMAKE_CXX_STANDARD 17)

add_executable(Differentiator main.cpp Differentiator.h ExpressionToGraph.h GraphVisualizer.h ExpressionNode.h GraphToExpression.h Operations.h SimplifyGraph.h)