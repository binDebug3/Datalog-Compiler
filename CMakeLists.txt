cmake_minimum_required(VERSION 3.16)
project(Project1_Starter_Code)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Project1_Starter_Code
        Automaton.h
        ColonAutomaton.cpp
        ColonAutomaton.h
        ColonDashAutomaton.cpp
        ColonDashAutomaton.h
        Lexer.cpp
        Lexer.h
        main.cpp
        Token.cpp
        Token.h
        CommaAutomaton.h
        CommaAutomaton.cpp
        PeriodAutomaton.h
        PeriodAutomaton.cpp
        Q_MarkAutomaton.h
        Q_MarkAutomaton.cpp
        Left_ParenAutomaton.h
        Left_ParenAutomaton.cpp
        Right_ParenAutomaton.h
        Right_ParenAutomaton.cpp
        MultiplyAutomaton.h
        MultiplyAutomaton.cpp
        AddAutomaton.h
        AddAutomaton.cpp
        EOF_TypeAutomaton.h
        EOF_TypeAutomaton.cpp FactsAutomaton.h FactsAutomaton.cpp SchemesAutomaton.h SchemesAutomaton.cpp RulesAutomaton.h RulesAutomaton.cpp QueriesAutomaton.h QueriesAutomaton.cpp IDAutomaton.h IDAutomaton.cpp StringAutomaton.h StringAutomaton.cpp CommentAutomaton.h CommentAutomaton.cpp Parser.h Parser.cpp DatalogProgram.h DatalogProgram.cpp Predicate.h Predicate.cpp Rule.h Rule.cpp Parameter.h Parameter.cpp)
