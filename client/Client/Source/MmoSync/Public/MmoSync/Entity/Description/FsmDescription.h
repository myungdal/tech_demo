// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once


struct FsmDescriptionAction
{
	StringKey mState;
	StringKey mAction;
	std::wstring mArgs;
};

struct FsmDescriptionTransition
{
	StringKey mStateSrc;
	StringKey mStateDest;
	StringKey mCondition;
	std::wstring mArgs;
};

struct FsmDescription
{
	using ActionList = std::vector<FsmDescriptionAction>;
	using TransitionList = std::vector<FsmDescriptionTransition>;

	ActionList mActionList;
	TransitionList mTransitionList;
};
