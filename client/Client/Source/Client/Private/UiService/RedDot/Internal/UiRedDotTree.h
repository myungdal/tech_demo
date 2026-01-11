// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================


#pragma once

struct FUiRedDotNode;

class FUiRedDotTree
{
private:	
	using RedDotNodeArray = std::array<FUiRedDotNode*, static_cast<size_t>(RedDotType::MAX)>;

	RedDotNodeArray mRedDotNodeArray;
	
public:
	DISABLE_COPY(FUiRedDotTree);
	explicit FUiRedDotTree();
	~FUiRedDotTree();

private:
	void AttachChildToParent(RedDotType child, RedDotType parent);

public:
	void AddRedDot(RedDotType redDotType);
	void RemoveRedDot(RedDotType redDotType);
	bool HasRedDot(RedDotType redDotType) const;
};
