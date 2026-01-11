// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

// ServerEngineCommon.ixx
// Common 프로젝트 모듈
// 클라이언트는 기존 #include 방식, 서버는 import ServerEngineCommon; 으로 사용
//
// 내용: Random, Stat, Math, FlowControlScript 등 Common 프로젝트 유틸리티

module;

// GMF - pch_common.h 필요
#include "Common/PCH/pch_common.h"

// Random
#include "Common/Random/Mt19937Random/Mt19937Random32.h"
#include "Common/Random/Mt19937Random/Mt19937Random64.h"
#include "Common/Random/DeterministicRandom/DeterministicRandom32.h"
#include "Common/Random/DeterministicRandom/DeterministicRandom64.h"
#include "Common/Random/RatioTable/RatioTable.h"
#include "Common/Random/NonReplacementSampler/NonReplaceSampler.h"
#include "Common/Random/AdaptiveRandom/AdaptiveRandom.h"
#include "Common/Random/RandMark40/RandMark40.h"
#include "Common/Random/RandMark40/CvcValidator.h"

// Stat
#include "Common/Stat/StatContainer/StatContainer.h"
#include "Common/Stat/StatNodeBase/StatNodeBase.h"

// Math
#include "Common/Math/MathUtil.h"

// FlowControlScript
#include "Common/FlowControlScript/Types.h"
#include "Common/FlowControlScript/Lexer.h"
#include "Common/FlowControlScript/Parser.h"
#include "Common/FlowControlScript/Compiler.h"
#include "Common/FlowControlScript/VirtualMachine.h"
#include "Common/FlowControlScript/GlobalEnvironment.h"

// MemPool
#include "Common/MemPool/DynamicPool.h"
#include "Common/MemPool/FixedPool.h"
#include "Common/MemPool/CompactionPool.h"

// Time
#include "Common/Time/ValuePerTime.h"
#include "Common/Time/Cron.h"

// UsageMeter
#include "Common/UsageMeter/UsageMeter.h"

// FileUtil
#include "Common/FileUtil/FileUtil.h"
#include "Common/FileUtil/TextFile.h"

// Invoker
#include "Common/Invoker/ArgList.h"
#include "Common/Invoker/Invoker.h"
#include "Common/Invoker/TimerInvoker.h"

export module ServerEngineCommon;

// ============= Random =============
export using ::Mt19937Random32;
export using ::Mt19937Random64;
export using ::DeterministicRandom32;
export using ::DeterministicRandom64;
export using ::RatioTable;
export using ::NonReplaceSampler;
export using ::AdaptiveRandom;
export using ::RandMark40;
export using ::CvcValidator;

// ============= Random TLS =============
export using ::tRandomEngine32;
export using ::tRandomEngine64;

// ============= Stat =============
export using ::StatContainer;
export using ::StatNodeBase;

// ============= FlowControlScript =============
export using FlowControlScript::Value;
export using FlowControlScript::INVALID_VALUE;
export using FlowControlScript::INVALID_OFFSET;
export using FlowControlScript::TokenType;
export using FlowControlScript::NodeType;
export using FlowControlScript::OpCode;
export using FlowControlScript::Node;
export using FlowControlScript::Node_statement;
export using FlowControlScript::Node_expression;
export using FlowControlScript::Node_root;
export using FlowControlScript::Node_function;
export using FlowControlScript::Node_ifStatement;
export using FlowControlScript::Node_returnStatement;
export using FlowControlScript::Node_expressionStatement;
export using FlowControlScript::Node_binaryOperation;
export using FlowControlScript::Node_unaryOperation;
export using FlowControlScript::Node_variable;
export using FlowControlScript::Node_literal;
export using FlowControlScript::Node_grouping;
export using FlowControlScript::Node_assignment;
export using FlowControlScript::Node_call;
export using FlowControlScript::Token;
export using FlowControlScript::NativeFunction;
export using FlowControlScript::Bytecode;
export using FlowControlScript::CallFrame;
export using FlowControlScript::Lexer;
export using FlowControlScript::Parser;
export using FlowControlScript::Compiler;
export using FlowControlScript::VirtualMachine;
export using FlowControlScript::GlobalEnvironment;
export using ::gGlobalEnvironment;

// ============= MemPool =============
export using ::DynamicPool;
export using ::FixedPool;
export using ::CompactionPool;

// ============= Time =============
export using ::ValuePerTime;
export using cron::INVALID_TIME;
export using cron::INVALID_INDEX;
export using cron::bad_cronexpr;
export using cron::cron_standard_traits;
export using cron::cron_oracle_traits;
export using cron::cron_quartz_traits;
export using cron::cronexpr;
export using cron::make_cron;
export using cron::cron_next;
export using cron::cron_cnt;

// ============= UsageMeter =============
export using ::UsageMeter;

// ============= FileUtil =============
export using FileUtil::EnsureDirectoryExists;
export using FileUtil::CreateOutputFileStream;
export using FileUtil::CreateTextFile;
export using ::TextFile;

// ============= Invoker =============
export using ::ArgList;
export using ::Invoker;
export using ::TimerInvoker;

